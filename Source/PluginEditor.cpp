/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SerialTestingAudioProcessorEditor::SerialTestingAudioProcessorEditor (SerialTestingAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 300);
    
    Timer::startTimerHz(60);
    
    // Slider Setup
    sliderSetup ( accelXSlider, Slider::SliderStyle::LinearVertical, true );
    sliderSetup ( accelYSlider, Slider::SliderStyle::LinearVertical, true );
    sliderSetup ( accelZSlider, Slider::SliderStyle::LinearVertical, true );
    
    sliderSetup ( distanceSlider, Slider::SliderStyle::LinearVertical, true );
    
    sliderSetup ( tsXSlider, Slider::SliderStyle::LinearVertical, true );
    sliderSetup ( tsYSlider, Slider::SliderStyle::LinearVertical, true );
    sliderSetup ( tsZSlider, Slider::SliderStyle::LinearVertical, true );
    
    
    // Label Setup
    sliderLabelSetup ( accelXLabel, "Accel X" );
    sliderLabelSetup ( accelYLabel, "Accel Y" );
    sliderLabelSetup ( accelZLabel, "Accel Z" );
    
    sliderLabelSetup ( distanceLabel, "Distance" );
    
    sliderLabelSetup ( tsXLabel, "Touch X" );
    sliderLabelSetup ( tsYLabel, "Touch Y" );
    sliderLabelSetup ( tsZLabel, "Touch Pressure" );

    // Attachment Setup
    accelXSliderAttachment  = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "accelX", accelXSlider );
    accelYSliderAttachment  = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "accelY", accelYSlider );
    accelZSliderAttachment  = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "accelZ", accelZSlider );
    
    distanceSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "distance", distanceSlider );
    
    tsXSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "tsX", tsXSlider );
    tsYSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "tsY", tsYSlider );
    tsZSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> ( audioProcessor.parameters, "tsZ", tsZSlider );
    
}

SerialTestingAudioProcessorEditor::~SerialTestingAudioProcessorEditor()
{
    Timer::stopTimer();
}

//==============================================================================
void SerialTestingAudioProcessorEditor::paint (juce::Graphics& g)
{
    
}

void SerialTestingAudioProcessorEditor::resized()
{
    Rectangle<int> totalArea = getLocalBounds();
    
    //float thirdArea = totalArea.getWidth() * 0.33f;
    float sliderDivisions = totalArea.getWidth() / 7.0f;
    float labelHeight = 50.0f;
    
    Rectangle<int> accelXArea      = totalArea.removeFromLeft ( sliderDivisions );
    Rectangle<int> accelXLabelArea = accelXArea.removeFromTop ( labelHeight );
    
    accelXLabel.setBounds  ( accelXLabelArea );
    accelXSlider.setBounds ( accelXArea );
    
    Rectangle<int> accelYArea      = totalArea.removeFromLeft ( sliderDivisions );
    Rectangle<int> accelYLabelArea = accelYArea.removeFromTop ( labelHeight );
    
    accelYLabel.setBounds  ( accelYLabelArea );
    accelYSlider.setBounds ( accelYArea );
    
    Rectangle<int> accelZArea      = totalArea.removeFromLeft ( sliderDivisions );
    Rectangle<int> accelZLabelArea = accelZArea.removeFromTop ( labelHeight );
    
    accelZLabel.setBounds  ( accelZLabelArea );
    accelZSlider.setBounds ( accelZArea );
    
    Rectangle<int> distArea      = totalArea.removeFromLeft ( sliderDivisions );
    Rectangle<int> distLabelArea = distArea.removeFromTop   ( labelHeight );
    
    distanceLabel.setBounds  ( distLabelArea );
    distanceSlider.setBounds ( distArea );
    
    Rectangle<int> tsXArea      = totalArea.removeFromLeft ( sliderDivisions );
    Rectangle<int> tsXLabelArea = tsXArea.removeFromTop    ( labelHeight );
    
    tsXLabel.setBounds  ( tsXLabelArea );
    tsXSlider.setBounds ( tsXArea );
    
    Rectangle<int> tsYArea      = totalArea.removeFromLeft ( sliderDivisions );
    Rectangle<int> tsYLabelArea = tsYArea.removeFromTop    ( labelHeight );
    
    tsYLabel.setBounds  ( tsYLabelArea );
    tsYSlider.setBounds ( tsYArea );
    
    Rectangle<int> tsZArea      = totalArea;
    Rectangle<int> tsZLabelArea = tsZArea.removeFromTop ( labelHeight );
    
    tsZLabel.setBounds  ( tsZLabelArea );
    tsZSlider.setBounds ( tsZArea );
    
    //accelXSlider.setBounds ( totalArea.removeFromLeft( quarterArea ) );
    //accelYSlider.setBounds ( totalArea.removeFromLeft( quarterArea ) );
    //accelZSlider.setBounds ( totalArea.removeFromLeft(<#int amountToRemove#>) );
}


void SerialTestingAudioProcessorEditor::timerCallback()
{
    accelerometer.setAccelValue();
    
    accelXSlider.setValue ( accelerometer.getAccelX() );
    accelYSlider.setValue ( accelerometer.getAccelY() );
    accelZSlider.setValue ( accelerometer.getAccelZ() );
    
    
    /*
    distanceMeter.setDistanceValue();
    
    distanceSlider.setValue ( distanceMeter.getDistanceValue() );
    */
    
    /*
    touchScreen.setTouchScreenCoords();
    
    tsXSlider.setValue ( touchScreen.getTouchScreenX() );
    tsYSlider.setValue ( touchScreen.getTouchScreenY() );
    tsZSlider.setValue ( touchScreen.getTouchScreenZ() );
    */
}



void SerialTestingAudioProcessorEditor::sliderSetup(Slider &sliderInstance, Slider::SliderStyle style, bool showTextBox)
{
    sliderInstance.setSliderStyle(style);
        
    // If slider has a textbox, draw it, otherwise, don't
    if (showTextBox)
    {
        sliderInstance.setTextBoxStyle ( Slider::TextBoxBelow, false, 50, 20 );
        sliderInstance.setColour       ( Slider::textBoxOutlineColourId, Colour( (uint8)0, (uint8)0, (uint8)0, (uint8)0 ) );
        sliderInstance.setColour       ( Slider::textBoxTextColourId, Colours::darkblue );
    }
    else
    {
        sliderInstance.setTextBoxStyle( Slider::NoTextBox, false, 0, 0 );
    }
    
    addAndMakeVisible(sliderInstance);
}


void SerialTestingAudioProcessorEditor::sliderLabelSetup(Label& labelInstance, String labelText)
{
    labelInstance.setText              ( labelText, dontSendNotification );
    labelInstance.setJustificationType ( Justification::centred );
    addAndMakeVisible                  ( labelInstance );
}
