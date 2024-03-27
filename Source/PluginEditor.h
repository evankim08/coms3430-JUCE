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
class Lab4AudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener // Add inheritance here

{
public:
    Lab4AudioProcessorEditor (Lab4AudioProcessor&);
    ~Lab4AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    // Override sliderValueChanged to handle multiple sliders
    void sliderValueChanged(juce::Slider* slider) override;

private:
    Lab4AudioProcessor& audioProcessor;
    
    juce::Slider mySlider; // for gain
    juce::Slider pitchSlider; // add this for pitch shifting
    
    juce::Label gainLabel; // label for the gain slider
    juce::Label pitchLabel; // label for the pitch slider

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Lab4AudioProcessorEditor)
};

