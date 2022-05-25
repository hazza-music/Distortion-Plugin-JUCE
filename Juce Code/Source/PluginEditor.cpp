/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Distortionpass1AudioProcessorEditor::Distortionpass1AudioProcessorEditor (Distortionpass1AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    getLookAndFeel().setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    dist1Slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    dist1Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    addAndMakeVisible(dist1Slider);

    dist2Slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    dist2Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    addAndMakeVisible(dist2Slider);

    threshSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    threshSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    addAndMakeVisible(threshSlider);

    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    addAndMakeVisible(gainSlider);


    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);
    dist1SliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DISTORTION1", dist1Slider);
    dist2SliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DISTORTION2", dist2Slider);
    threshSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "THRESHOLD", threshSlider);
    setSize (600, 400);
}

Distortionpass1AudioProcessorEditor::~Distortionpass1AudioProcessorEditor()
{
}

//==============================================================================
void Distortionpass1AudioProcessorEditor::paint (juce::Graphics& g)
{

    juce::Image background = juce::ImageCache::getFromMemory(BinaryData::BG2_png, BinaryData::BG2_pngSize);
    g.drawImageAt(background, 0, 0);
    //g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));


}

void Distortionpass1AudioProcessorEditor::resized()
{
    dist1Slider.setBounds(getWidth() / 4-150, getHeight()/4-45, 200, 100);
    dist2Slider.setBounds(getWidth() / 2 - 75, getHeight() / 4 - 45, 200, 100);
    threshSlider.setBounds(getWidth() / 4 - 35, getHeight() / 2 + 30, 200, 100);
    gainSlider.setBounds(getWidth() / 4 + 250, getHeight() / 6 - 20 , 200, 300);
}
