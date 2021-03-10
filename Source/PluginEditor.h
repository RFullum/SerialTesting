/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Accelerometer.h"
#include "DistanceMeter.h"
#include "TouchScreen.h"

//==============================================================================
/**
*/
class SerialTestingAudioProcessorEditor  : public juce::AudioProcessorEditor, public Timer
{
public:
    SerialTestingAudioProcessorEditor (SerialTestingAudioProcessor&);
    ~SerialTestingAudioProcessorEditor() override;
    

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    /// Sets up Slider object instances in constructor. sliderInstance is the slider to set up, suffix is textValueSuffix, sliderFillColor is the slider color below the thumb
    void sliderSetup(Slider& sliderInstance, Slider::SliderStyle style, bool showTextBox);
    
    /// Sets up Label for the Slider instances. Takes the labelInstance and the text for setText
    void sliderLabelSetup(Label& labelInstance, String labelText);
    
    // Sliders
    Slider accelXSlider;
    Slider accelYSlider;
    Slider accelZSlider;
    
    Slider distanceSlider;
    
    Slider tsXSlider;
    Slider tsYSlider;
    Slider tsZSlider;
    
    // Labels
    Label accelXLabel;
    Label accelYLabel;
    Label accelZLabel;
    
    Label distanceLabel;
    
    Label tsXLabel;
    Label tsYLabel;
    Label tsZLabel;
    
    // Attachments
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> accelXSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> accelYSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> accelZSliderAttachment;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> distanceSliderAttachment;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> tsXSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> tsYSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> tsZSliderAttachment;
    
    
    
    
    // Sensor Components
    Accelerometer accelerometer;
    //DistanceMeter distanceMeter;
    //TouchScreen touchScreen;
    
    
    SerialTestingAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SerialTestingAudioProcessorEditor)
};
