/*
  ==============================================================================

    TouchScreen.h
    Created: 28 Feb 2021 12:58:22am
    Author:  Robert Fullum

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "juce_serialport/juce_serialport.h"

//==============================================================================
/*
*/
class TouchScreen  : public juce::Component
{
public:
    TouchScreen();
    ~TouchScreen() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void setTouchScreenCoords();
    
    float getTouchScreenX();
    
    float getTouchScreenY();
    
    float getTouchScreenZ();

private:
    // Serial instances
    StringPairArray portlist;
    
    SerialPort* pSP;
    
    SerialPortOutputStream* pOutputStream;
    SerialPortInputStream*  pInputStream;
    
    // Sensor Variables
    String touchStr;
    
    float smoothingFactor;
    float smoothXVal;
    float smoothYVal;
    float smoothZVal;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TouchScreen)
};
