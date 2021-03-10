/*
  ==============================================================================

    Accelerometer.h
    Created: 26 Feb 2021 2:02:01pm
    Author:  Robert Fullum

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "juce_serialport/juce_serialport.h"

//==============================================================================
/*
*/
class Accelerometer  : public juce::Component
{
public:
    Accelerometer();
    ~Accelerometer() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /// Called in timerCalback() to read accelerometer values from Arduino via Serial (USB)
    void setAccelValue();
    
    /// Returns the X value from the Accelerometer, -4.0f to 4.0f 
    float getAccelX();
    
    /// Returns the Y value from the Accelerometer, -4.0f to 4.0f
    float getAccelY();
    
    /// Returns the Z value from the Accelerometer, -4.0f to 4.0f
    float getAccelZ();
    

private:
    
    
    // Serial instances
    StringPairArray portlist;
    
    SerialPort* pSP;
    
    SerialPortOutputStream* pOutputStream;
    SerialPortInputStream*  pInputStream;
    
    // Accelerometer Variables
    String accelStr;
    
    float smoothingFactor;
    float smoothValX;
    float smoothValY;
    float smoothValZ;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Accelerometer)
};
