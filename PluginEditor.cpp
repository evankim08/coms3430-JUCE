/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Lab4AudioProcessorEditor::Lab4AudioProcessorEditor (Lab4AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    mySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    mySlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 50, 20);
    mySlider.setRange(0.01f, 1.0f, 0.01f);
    mySlider.setValue(0.5f);
    mySlider.addListener(this);
    addAndMakeVisible(mySlider);
    
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.attachToComponent(&mySlider, false);
    addAndMakeVisible(gainLabel);
    
    // Set up pitch shift slider
    pitchSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    pitchSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 100, 20);
    pitchSlider.setRange(0.01f, 1.0f, 0.01f); // Example range, can be adjusted
    pitchSlider.setValue(1.0); // Default no pitch shift
    pitchSlider.addListener(this);
    addAndMakeVisible(pitchSlider);
    
    pitchLabel.setText("Pitch Shift", juce::dontSendNotification);
    pitchLabel.attachToComponent(&pitchSlider, false);
    addAndMakeVisible(pitchLabel);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

Lab4AudioProcessorEditor::~Lab4AudioProcessorEditor()
{
}

//==============================================================================
void Lab4AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (18.0f);
    g.drawFittedText ("My First Plug-in!", getLocalBounds(), juce::Justification::centred, 2);
    mySlider.setBounds(50, getHeight() / 2 - 100, 100, 200);
}

void Lab4AudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
    if (slider == &mySlider) {
        audioProcessor.gain = mySlider.getValue();
    } else if (slider == &pitchSlider) {
        // pitchSlider's range ensures pitchShiftFactor won't exceed 1.0
        audioProcessor.pitchShiftFactor = pitchSlider.getValue();
    }
}

void Lab4AudioProcessorEditor::resized() {
    // Set bounds for your sliders
    mySlider.setBounds(40, 30, 20, getHeight() - 60);
    pitchSlider.setBounds(100, 30, getWidth() - 110, 20);
}

