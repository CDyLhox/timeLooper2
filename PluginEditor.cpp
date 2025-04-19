#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p)
		: AudioProcessorEditor(&p)
		, processorRef(p)
{
				setOpaque(true);
				juce::ignoreUnused(processorRef);
				// Make sure that before the constructor has finished, you've set the
				// editor's size to whatever you need it to be.
				setSize(1000, 500);

				//________ NUMCROSSING FADER STUFF ________
				addAndMakeVisible(numCrossingsDial);
				numCrossingsDial.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
				numCrossingsDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 32);
				numCrossingsDial.setRange(0, 512, 1);
				numCrossingsDial.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colour::fromRGBA(255, 255, 255, 200));
				numCrossingsDial.setDoubleClickReturnValue(true, 256);
				// _______ threshold FADER STUFF _________
				addAndMakeVisible(thresholdSlider);
				thresholdSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
				thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 32, 32);
				thresholdSlider.setRange(0, 1, 0.05);
				// thresholdSlider.setColour(juce::Slider::ColourIds::)
				thresholdSlider.setDoubleClickReturnValue(true, 0.3);

				//--------BUTTON STUFF--------
				addAndMakeVisible(freezeButton);
				freezeButton.setButtonText("activateLoopButton");
				freezeButton.setClickingTogglesState(true);

				// buttonColours
				freezeButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::lightgoldenrodyellow);
				freezeButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::crimson);
				freezeButton.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::blueviolet);
				freezeButton.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::black);

				// __________RMS LABEL ____________

				addAndMakeVisible(rmsLabel);
				rmsLabel.setText("RMS: 0.0", juce::dontSendNotification); // Default text
				rmsLabel.setFont(juce::Font(20.0f));											// Set font size
				rmsLabel.setJustificationType(juce::Justification::centred);


				//________ZERO CROSSING SLABEL
				addAndMakeVisible(zeroCrossingsLabel);
				zeroCrossingsLabel.setText("0 out of 256 crossings.", juce::dontSendNotification);
				zeroCrossingsLabel.setFont(juce::Font(20.0f));
				zeroCrossingsLabel.setJustificationType(juce::Justification::centred);

				// _________ PLUGIN LOGIC _______

				setResizable(true, true);

				getConstrainer()->setFixedAspectRatio(2.0);

				numCrossingsAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
						processorRef.Params,
						"umaxNumZeroCrossings",
						numCrossingsDial);

				thresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
						processorRef.Params, "uThreshold", thresholdSlider);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g)
{
				// (Our component is opaque, so we must completely fill the background with a solid colour)
				g.fillAll(juce::Colour::fromRGBA(255, 251, 235, 200));

				g.setColour(juce::Colours::black);
				g.setFont(15.0f);
				g.drawFittedText("jue! timestretchlooper {version testvisual 0.6.1}", getLocalBounds(), juce::Justification::centredTop, 1);

				//______ LABEL OF RMS ______
				float currentRMS = processorRef.rmsValues[0]; // Or iterate over all channels
				// Update the label text with the current RMS value
				rmsLabel.setText(juce::String("RMS: ") + juce::String(currentRMS), juce::dontSendNotification);

				// Draw the label
				g.setColour(juce::Colours::white);
				g.drawText(rmsLabel.getText(), 10, 10, getWidth() - 20, 30, juce::Justification::centred);

				//________ LABEL OF ZEROCRSOSING ________
				int currentMin = processorRef.numZeroCrossingsInfo[0];
				int currentMax = processorRef.numZeroCrossingsInfo[1];
				rmsLabel.setText(juce::String(currentMin) + juce::String("out of") + juce::String(currentMax) + juce::String("zeroCrossings"), juce::dontSendNotification);

				// Draw the label
				g.setColour(juce::Colours::white);
				g.drawText(rmsLabel.getText(), 30, 10, getWidth() - 20, 30, juce::Justification::centred);

}

void AudioPluginAudioProcessorEditor::resized()
{
				auto leftMargin = getWidth() * 0.02;
				auto topMargin = getHeight() * 0.02;
				auto dialSize = getWidth() * 0.25;

				numCrossingsDial.setBounds(leftMargin, topMargin, dialSize, dialSize);

				thresholdSlider.setBounds(leftMargin, topMargin + dialSize, dialSize * 2, dialSize);

				auto buttonWidth
						= getWidth() * 0.1;
				auto buttonHeight = getHeight() * 0.05;
				freezeButton.setBounds(numCrossingsDial.getX() + numCrossingsDial.getWidth(), topMargin, buttonWidth, buttonHeight);
				// This is generally where you'll want to lay out the positions of any
				// subcomponents in your editor..
}
