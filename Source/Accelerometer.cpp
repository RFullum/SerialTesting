/*
  ==============================================================================

    Accelerometer.cpp
    Created: 26 Feb 2021 2:02:01pm
    Author:  Robert Fullum

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Accelerometer.h"

//==============================================================================
Accelerometer::Accelerometer() : smoothingFactor(0.4f), smoothValX(0.0f), smoothValY(0.0f), smoothValZ(0.0f)
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

Accelerometer::~Accelerometer()
{
    if (pSP->exists())
        delete pSP;
    
    delete pOutputStream;
    delete pInputStream;
}


void Accelerometer::paint (juce::Graphics& g) {}

void Accelerometer::resized() {}



/// Called in timerCalback() to read accelerometer values from Arduino via Serial (USB)
void Accelerometer::setAccelValue()
{
    float accelX = 0.0f;
    float accelY = 0.0f;
    float accelZ = 0.0f;
    
    
    // If the serial stream exists...
    if (pSP->exists())
    {
        pOutputStream->writeBool(true);
        
        // Until the stream is exhausted, read the stream and write it to accelStr
        while (!pInputStream->isExhausted())
        {
            accelStr = pInputStream->readString();
        }
        
        // Substrings
        String accelXStr;
        String accelYStr;
        String accelZStr;
        //DBG(accelStr);
        // Route the incoming string and convert its numeric portion to the appropriate float
        accelXStr   = accelStr.fromFirstOccurrenceOf ( "ax", false, true ).upToFirstOccurrenceOf ( "\n", false, true );
        accelX      = accelXStr.getDoubleValue();
        smoothValX += ( accelX - smoothValX ) * smoothingFactor;
        
        accelYStr   = accelStr.fromFirstOccurrenceOf ( "ay", false, true ).upToFirstOccurrenceOf ( "\n", false, true );
        accelY      = accelYStr.getDoubleValue();
        smoothValY += ( accelY - smoothValY ) * smoothingFactor;
        
        accelZStr   = accelStr.fromFirstOccurrenceOf ( "az", false, true ).upToFirstOccurrenceOf ( "\n", false, true );
        accelZ      = accelZStr.getDoubleValue();
        smoothValZ += ( accelZ - smoothValZ ) * smoothingFactor;
        
    }
    
    pOutputStream->writeBool(false);
}



/// Returns the X value from the Accelerometer, -4.0f to 4.0f
float Accelerometer::getAccelX()
{
    return smoothValX;
}

/// Returns the Y value from the Accelerometer, -4.0f to 4.0f
float Accelerometer::getAccelY()
{
    return smoothValY;
}

/// Returns the Z value from the Accelerometer, -4.0f to 4.0f
float Accelerometer::getAccelZ()
{
    return smoothValZ;
}
