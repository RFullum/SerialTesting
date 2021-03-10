/*
  ==============================================================================

    TouchScreen.cpp
    Created: 28 Feb 2021 12:58:22am
    Author:  Robert Fullum

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TouchScreen.h"

//==============================================================================
TouchScreen::TouchScreen() : smoothingFactor(0.4f), smoothXVal(0.0f), smoothYVal(0.0f), smoothZVal(0.0f)
{
    // list of serial ports: friendly name & port path
    portlist = SerialPort::getSerialPortPaths();
    
    // Open Arduino Port
    pSP = new SerialPort( portlist.getAllValues()[4], SerialPortConfig( 9600, 8, SerialPortConfig::SERIALPORT_PARITY_NONE, SerialPortConfig::STOPBITS_1, SerialPortConfig::FLOWCONTROL_NONE ) );
    
    if (pSP->exists())
    {
        pOutputStream = new SerialPortOutputStream ( pSP );
        pInputStream  = new SerialPortInputStream  ( pSP );
    }
}

TouchScreen::~TouchScreen()
{
    if (pSP->exists())
        delete pSP;
    
    delete pOutputStream;
    delete pInputStream;
}

void TouchScreen::paint (juce::Graphics& g) {}

void TouchScreen::resized() {}


void TouchScreen::setTouchScreenCoords()
{
    float xCoord = 0.0f;
    float yCoord = 0.0f;
    float zCoord = 0.0f;
    
    if (pSP->exists())
    {
        while (!pInputStream->isExhausted())
        {
            touchStr = pInputStream->readString();
        }
        
        // Substrings
        String touchXStr;
        String touchYStr;
        String touchZStr;
        
        touchXStr = touchStr.fromFirstOccurrenceOf("tsx", false, true).upToFirstOccurrenceOf("\n", false, true);
        touchYStr = touchStr.fromFirstOccurrenceOf("tsy", false, true).upToFirstOccurrenceOf("\n", false, true);
        touchZStr = touchStr.fromFirstOccurrenceOf("tsz", false, true).upToFirstOccurrenceOf("\n", false, true);
        
        xCoord = touchXStr.getDoubleValue();
        yCoord = touchYStr.getDoubleValue();
        zCoord = touchZStr.getDoubleValue();
        
        smoothXVal += ( xCoord - smoothXVal ) * smoothingFactor;
        smoothYVal += ( yCoord - smoothYVal ) * smoothingFactor;
        smoothZVal += ( zCoord - smoothZVal ) * smoothingFactor;
        
    }
}


float TouchScreen::getTouchScreenX()
{
    return smoothXVal;
}

float TouchScreen::getTouchScreenY()
{
    return smoothYVal;
}

float TouchScreen::getTouchScreenZ()
{
    if (smoothZVal > 1024.0f)
        smoothZVal = 1024.0f;
    
    return  smoothZVal;
}
