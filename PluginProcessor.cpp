#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <memory>

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
		: AudioProcessor(BusesProperties()
							.withInput("Input", juce::AudioChannelSet::stereo(), true)
							.withOutput("Output", juce::AudioChannelSet::stereo(), true))
		, Params(*this, nullptr, "Parameters", {
																							 std::make_unique<juce::AudioParameterInt>(juce::ParameterID { "uMaxNumZeroCrossings", 1 }, "MaxNumZeroCrossings", 2, 512, 256),
																							 std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "uThreshold", 1 }, "Threshold", 0.1, 1.0, 0.3),
																					 })
{
				// Use the parameter ID to return a pointer to our parameter data
				MaxNumZeroCrossings = Params.getRawParameterValue("uMaxNumZeroCrossings");
				threshold = Params.getRawParameterValue("uThreshold");

				// for each input channel emplace one filter
				/*	for (auto i = 0; i < getBusesLayout().getNumChannels(true, 0); ++i) {
									timestretchers.emplace_back();
									// Giving the filter some hardcoded start up params
					}*/
}
// w

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
				return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
				return true;
#else
				return false;
#endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
				return true;
#else
				return false;
#endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
				return true;
#else
				return false;
#endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
				return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
				return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
									// so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
				return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram(int index)
{
				juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index)
{
				juce::ignoreUnused(index);
				return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
				juce::ignoreUnused(index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
				timestretchers.resize(getTotalNumInputChannels());
				rmsValues.resize(getTotalNumInputChannels(), 0.0f); // Resize to match the number of input channels

				for (auto& ts : timestretchers)
								ts.prepare();
}

void AudioPluginAudioProcessor::releaseResources()
{
				// When playback stops, you can use this as an opportunity to free up any
				// spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
				juce::ignoreUnused(layouts);
				return true;
#else
				// This is the place where you check if the layout is supported.
				// In this template code we only support mono or stereo.
				// Some plugin hosts, such as certain GarageBand versions, will only
				// load plugins that support stereo bus layouts.
				if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
						&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
								return false;

				// This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
				if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
								return false;
#endif

				return true;
#endif
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
		juce::MidiBuffer& midiMessages)
{

				for (auto& stretcher : timestretchers) {
								stretcher.setMaxNumZeroCrossings(*MaxNumZeroCrossings);
								stretcher.setThreshold(*threshold);
				}

				juce::ignoreUnused(midiMessages);

				juce::ScopedNoDenormals noDenormals;
				auto totalNumInputChannels = getTotalNumInputChannels();
				auto totalNumOutputChannels = getTotalNumOutputChannels();

				for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
								buffer.clear(i, 0, buffer.getNumSamples());

				// the equivalent of your old audio callback :- )
				for (int channel = 0; channel < totalNumInputChannels; ++channel) { // Note: writePointer and readPointer point to the same data (in-place processing)
								auto* writePointer = buffer.getWritePointer(channel);				// input
								auto* readPointer = buffer.getReadPointer(channel);					// output
								for (auto sample = 0; sample < buffer.getNumSamples(); ++sample) {
												timestretchers[channel].processFrame(readPointer[sample], writePointer[sample]);
												rmsValues[channel] = timestretchers[channel].getRmsSignal();
												numZeroCrossingsInfo = timestretchers[channel].getNumZeroCrossings();
												//std::cout << "RMSVALUES IN PROCESSBLOCK" << rmsValues[channel] << " CHANNEL " << channel << std::endl;
								}
				}
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
				return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
				return new AudioPluginAudioProcessorEditor(*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
				// You should use this method to store your parameters in the memory block.
				// You could do that either as raw data, or use the XML or ValueTree classes
				// as intermediaries to make it easy to save and load complex data.
				// juce::ignoreUnused(destData);
				auto state = Params.copyState();
				std::unique_ptr<juce::XmlElement> xml(state.createXml());
				copyXmlToBinary(*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
				// You should use this method to restore your parameters from this memory block,
				// whose contents will have been created by the getStateInformation() call.
				// juce::ignoreUnused(data, sizeInBytes);
				std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
				if (xmlState.get() != nullptr) {
								if (xmlState->hasTagName(Params.state.getType())) {
												Params.replaceState(juce::ValueTree::fromXml(*xmlState));
								}
				}
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
				return new AudioPluginAudioProcessor();
}
