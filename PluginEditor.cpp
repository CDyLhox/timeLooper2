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

				//________ FADER STUFF ________
				addAndMakeVisible(numCrossingsDial);
				numCrossingsDial.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
				numCrossingsDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 32);
				numCrossingsDial.setRange(0, 512, 1);
				numCrossingsDial.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colour::fromRGBA(255, 255, 255, 200));
				numCrossingsDial.setDoubleClickReturnValue(true, 0.3);
				//--------BUTTON STUFF--------
				addAndMakeVisible(freezeButton);
				freezeButton.setButtonText("activateLoopButton");
				freezeButton.setClickingTogglesState(true);

				numCrossingsAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
						processorRef.Params,
						"uCutoff",
						numCrossingsDial);

				// buttonColours
				freezeButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::lightgoldenrodyellow);
				freezeButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::crimson);
				freezeButton.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::blueviolet);
				freezeButton.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::black);

				setResizable(true, true);

				getConstrainer()->setFixedAspectRatio(2.0);
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
				g.drawFittedText("jue! timestretchlooper", getLocalBounds(), juce::Justification::centredTop, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
				auto leftMargin = getWidth() * 0.02;
				auto topMargin = getHeight() * 0.02;
				auto dialSize = getWidth() * 0.25;

				numCrossingsDial.setBounds(leftMargin, topMargin, dialSize, dialSize);

				auto buttonWidth = getWidth() * 0.1;
				auto buttonHeight = getHeight() * 0.05;
				freezeButton.setBounds(numCrossingsDial.getX() + numCrossingsDial.getWidth(), topMargin, buttonWidth, buttonHeight);
				// This is generally where you'll want to lay out the positions of any
				// subcomponents in your editor..
}

