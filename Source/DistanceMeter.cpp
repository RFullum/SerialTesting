/*
  ==============================================================================

    DistanceMeter.cpp
    Created: 27 Feb 2021 11:16:21pm
    Author:  Robert Fullum

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DistanceMeter.h"

//==============================================================================
DistanceMeter::DistanceMeter() : smoothingFactor(0.4f), smoothDistVal(0.0f)
{
    // List of Serial Ports: Friendly name & path
    portlist = SerialPort::getSerialPortPaths();
    
    // Open Arduino Port
    pSP = new SerialPort ( portlist.getAllValues()[4], SerialPortConfig( 115200, 8, SerialPortConfig::SERIALPORT_PARITY_NONE, SerialPortConfig::STOPBITS_1, SerialPortConfig::FLOWCONTROL_NONE ) );
    
    if (pSP->exists())
    {
        pOutputStream = new SerialPortOutputStream ( pSP );
        pInputStream  = new SerialPortInputStream  ( pSP );
    }
}

DistanceMeter::~DistanceMeter()
{
    if (pSP->exists())
        delete pSP;
    
    delete pOutputStream;
    delete pInputStream;
}

void DistanceMeter::paint (juce::Graphics& g) {}

void DistanceMeter::resized() {}


/// Called in timerCallback() to read Distance Sensor values
void DistanceMeter::setDistanceValue()
{
    float dist = 0.0f;
    
    // If the serial stream exists...
    if (pSP->exists())
    {
        // Until the stream is exhausted, read the stream and write it to the String
        while (!pInputStream->isExhausted())
        {
            distanceStr = pInputStream->readString();
        }
        
        dist           = distanceStr.getDoubleValue();
        smoothDistVal += ( dist - smoothDistVal ) * smoothingFactor;
    }
}


/**
Returns the distance value in millimeters. Sensor is rated at 50mm to 1200mm.
It will sense values less than 50mm and greater than 1200mm depending on light levels and reflectiveness.
When the distance goes out of range, either too close or too far, it returns 0.0f
*/
float DistanceMeter::getDistanceValue()
{
    return smoothDistVal;
}
