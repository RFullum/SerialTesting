/*
  ==============================================================================

    DistanceMeter.h
    Created: 27 Feb 2021 11:16:21pm
    Author:  Robert Fullum

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "juce_serialport/juce_serialport.h"


//==============================================================================
/*
*/
class DistanceMeter  : public juce::Component
{
public:
    DistanceMeter();
    ~DistanceMeter() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /// Called in timerCallback() to read Distance Sensor values
    void setDistanceValue();
    
    /**
     Returns the distance value in millimeters. Sensor is rated at 50mm to 1200mm.
     It will sense values less than 50mm and greater than 1200mm depending on light levels and reflectiveness.
     When the distance goes out of range, either too close or too far, it returns 0.0f
     */
    float getDistanceValue();

private:
    // Serial instances
    StringPairArray portlist;
    
    SerialPort* pSP;
    
    SerialPortOutputStream* pOutputStream;
    SerialPortInputStream*  pInputStream;
    
    // Sensor Variables
    String distanceStr;
    
    float smoothingFactor;
    float smoothDistVal;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistanceMeter)
};
