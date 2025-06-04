#pragma once

#include "PluginProcessor.h"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor , public juce::Timer {
		public:
				explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&);
				~AudioPluginAudioProcessorEditor() override;

				//==============================================================================
				void paint(juce::Graphics&) override;
				void resized() override;

				// update paint class on timer callback
				void timerCallback() override
				{
								// Update whatever info you want to display
							/*	std::cout << "honey im home (timercallback) " << std::endl;

								std::cout << processorRef.rmsValues[0] <<std::endl;*/
								// Trigger a repaint
								repaint();
				}

		private:
				// This reference is provided as a quick way for your editor to
				// access the processor object that created it.
				AudioPluginAudioProcessor& processorRef;
				juce::Slider numCrossingsDial;
				juce::Slider thresholdSlider;
				juce::TextButton monoButton;

				juce::Label rmsLabel; // Label to display the RMS value
				juce::Label zeroCrossingsLabel;

				std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> numCrossingsAttachment;
				std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment;

				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
