/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Distortionpass1AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Distortionpass1AudioProcessorEditor (Distortionpass1AudioProcessor&);
    ~Distortionpass1AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;


private:
    juce::Slider gainSlider;
    juce::Slider dist1Slider;
    juce::Slider dist2Slider;
    juce::Slider threshSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dist1SliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dist2SliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> threshSliderAttachment;


    Distortionpass1AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Distortionpass1AudioProcessorEditor)
};
