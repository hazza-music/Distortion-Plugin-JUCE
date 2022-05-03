/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

//==============================================================================
Distortionpass1AudioProcessor::Distortionpass1AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
    
{
}

Distortionpass1AudioProcessor::~Distortionpass1AudioProcessor()
{
}

//==============================================================================
const juce::String Distortionpass1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Distortionpass1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Distortionpass1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Distortionpass1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Distortionpass1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Distortionpass1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Distortionpass1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Distortionpass1AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Distortionpass1AudioProcessor::getProgramName (int index)
{
    return {};
}

void Distortionpass1AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Distortionpass1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto gaina = apvts.getRawParameterValue("GAIN");
    previousGain = gaina->load();
}

void Distortionpass1AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Distortionpass1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
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
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Distortionpass1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);
        applyDistortion(buffer, numSamples, channelData);
    }

    for (int channel = totalNumInputChannels; channel < totalNumOutputChannels; ++channel)
       buffer.clear(channel, 0, numSamples);
    gainControl(buffer);
}

void Distortionpass1AudioProcessor::applyDistortion(juce::AudioBuffer<float>& buffer, const int numSamples, float* channelData) {
    for (int sample = 0; sample < numSamples; ++sample) {
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            float out, an, bn, previousGain;

            auto d1a = apvts.getRawParameterValue("DISTORTION1");
            float d1 = d1a->load();
            auto d2a = apvts.getRawParameterValue("DISTORTION2");
            float d2 = d2a->load();
            auto thresha = apvts.getRawParameterValue("THRESHOLD");
            float thresh = thresha->load();

            const float in = buffer.getSample(channel, sample);
            an = cubicDistortion(in, d1);
            an = cubicDistortion(an, d2);
            bn = hardClip(in, thresh);
            out = an + bn;
            
            buffer.setSample(channel, sample, out);
        }
    }
}

float Distortionpass1AudioProcessor::cubicDistortion(const float& sample, float d1) {
    float out;
    out = (sample - d1 * ((1.0f / 3.0f) * powf(sample, 3)));
    return out;
}

float Distortionpass1AudioProcessor::hardClip(const float& sample, float thresh) {
    float out;
    if (sample >= thresh) {
        out = thresh;
    }
    else if (sample <= -thresh) {
        out = -thresh;
    }
    else {
        out = sample;
    }
    return out;
}
void Distortionpass1AudioProcessor::gainControl(juce::AudioBuffer<float>& buffer) {
    auto gaina = apvts.getRawParameterValue("GAIN");
    float currentGain = gaina->load();
    if (currentGain == previousGain)
    {
        buffer.applyGain(currentGain);
    }
    else
    {
        //buffer.applyGain(currentGain);
        buffer.applyGainRamp(0, buffer.getNumSamples(), previousGain, currentGain);
        previousGain = currentGain;
    }
}


//==============================================================================
bool Distortionpass1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Distortionpass1AudioProcessor::createEditor()
{
    return new Distortionpass1AudioProcessorEditor (*this);
}

//==============================================================================
void Distortionpass1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Distortionpass1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Distortionpass1AudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout Distortionpass1AudioProcessor::createParameters() {
    std::vector < std::unique_ptr < juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", 0.0f, 0.9f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DISTORTION1", "Distortion 1", 0.0f, 1.0f, 0.7f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DISTORTION2", "Distortion 2", 0.0f, 1.0f, 0.7f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("THRESHOLD", "Threshold", 0.0f, 1.0f, 0.5f));

    return { params.begin(), params.end() };

}