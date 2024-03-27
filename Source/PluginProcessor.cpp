/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Lab4AudioProcessor::Lab4AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

Lab4AudioProcessor::~Lab4AudioProcessor()
{
}

//==============================================================================
const juce::String Lab4AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Lab4AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Lab4AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Lab4AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Lab4AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Lab4AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Lab4AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Lab4AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Lab4AudioProcessor::getProgramName (int index)
{
    return {};
}

void Lab4AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Lab4AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    juce::ignoreUnused(samplesPerBlock);
    prevsampleRate = sampleRate;

}

void Lab4AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Lab4AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Lab4AudioProcessor::addSynthData(float* channelData, const float* synthData, int numSamples) {
    for (int sample = 0; sample < numSamples; ++sample) {
        channelData[sample] += synthData[sample];
    }
}
void Lab4AudioProcessor::processGain(juce::AudioBuffer<float>& buffer) {
    buffer.applyGain(gain);
}

void Lab4AudioProcessor::processPitch(float* channelData, int numSamples) {
    
    if (pitch > 0) {
        const int jump = static_cast<int>(std::pow(2, pitch));
        const int buff_size = numSamples / jump;
        std::vector<float> buff(buff_size);
        
        for (int sample = 0; sample < numSamples; ++sample) {
            if (sample % jump == 0) {
                buff[sample / jump] = channelData[sample];
            }
        }
        
        for (int sample = 0; sample < numSamples; ++sample) {
            if (sample % jump == 0) {
                channelData[sample] = buff[sample % buff_size];
            }
        }
    }
}

void Lab4AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear unused channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    // Generate buffer
    juce::AudioBuffer<float> synthBuffer(buffer.getNumChannels(), buffer.getNumSamples());
    synthBuffer.clear();

    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto* channelData = buffer.getWritePointer(channel);
        
        processPitch(channelData, buffer.getNumSamples());
        // Now directly applying gain to the whole buffer
    }

    // Apply gain to the entire buffer
    processGain(buffer);

    // Add synthesizer data
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto* channelData = buffer.getWritePointer(channel);
        auto* synthData = synthBuffer.getReadPointer(channel);
        addSynthData(channelData, synthData, buffer.getNumSamples());
    }
}

//==============================================================================
bool Lab4AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Lab4AudioProcessor::createEditor()
{
    return new Lab4AudioProcessorEditor (*this);
}

//==============================================================================
void Lab4AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Lab4AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Lab4AudioProcessor();
}

