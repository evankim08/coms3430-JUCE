/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class Lab4AudioProcessor  : public juce::AudioProcessor
                                    #if JucePlugin_Enable_ARA
                                     , public juce::AudioProcessorARAExtension
                                    #endif
{
public:
    //==============================================================================
    Lab4AudioProcessor();
    ~Lab4AudioProcessor() override;

    //==============================================================================
   
    float pitchShiftFactor = 1.0f; // Pitch shift multiplier, 1.0 means no shift
    float gain = 1.0f; // Gain, 1.0 means no change
   
    void setPitchShiftFactor(float value); // Add this line
    
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    double sliderVal = 0;

private:
    
    juce::dsp::LadderFilter <float> filter;
    juce::dsp::Chorus <float> chorusEffect;
    juce::dsp::StateVariableTPTFilter<float> lowPassFilter;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Lab4AudioProcessor)
};
