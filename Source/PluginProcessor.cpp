/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.
 
    Arduino Sketch that sends Acceleromter Values from Arduino Nano IoT 33
    via Serial is IMU_Serial_test

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SerialTestingAudioProcessor::SerialTestingAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif

//
// ParameterFloats:
// id, description, min, max, default
// ~OR~
// id, description, normalisableRange(min, max, increment, skew, symmetric),
//                 default, param label, param category, string from value, string to value
//
// ParameterChoices:
// id, description, StringArray( {"choice1", "choice2", ... } ), default index of StringArray
//
parameters(*this, nullptr, "ParameterTree",
{
    // Accelerometer
    std::make_unique<AudioParameterFloat>( "accelX", "Accelerometer X",
                                           NormalisableRange<float>( -4.0f, 4.0f, 0.01f, 1.0f, true ), 0.0f, "" ),
    std::make_unique<AudioParameterFloat>( "accelY", "Accelerometer Y",
                                           NormalisableRange<float>( -4.0f, 4.0f, 0.01f, 1.0f, true ), 0.0f, "" ),
    std::make_unique<AudioParameterFloat>( "accelZ", "Accelerometer Z",
                                           NormalisableRange<float>( -4.0f, 4.0f, 0.01f, 1.0f, true ), 0.0f, "" ),
    
    // Distance
    std::make_unique<AudioParameterFloat>( "distance", "Distance",
                                           NormalisableRange<float>( 0.0f, 1500.0f, 0.01f, 1.0f, true), 50.0f, "" ),
    
    // Touchscreen
    std::make_unique<AudioParameterFloat>( "tsX", "Touch X",
                                           NormalisableRange<float>( 0.0f, 1024.0f, 0.1f, 1.0f, true), 0.0f, "" ),
    std::make_unique<AudioParameterFloat>( "tsY", "Touch Y",
                                           NormalisableRange<float>( 0.0f, 1024.0f, 0.1f, 1.0f, true), 0.0f, "" ),
    std::make_unique<AudioParameterFloat>( "tsZ", "Touch Pressure",
                                           NormalisableRange<float>( 0.0f, 1024.0f, 0.1f, 1.0f, true), 0.0f, "" )
})

// Constructor
{
    // Parameter Setup
    accelXParam = parameters.getRawParameterValue ( "accelX" );
    accelYParam = parameters.getRawParameterValue ( "accelY" );
    accelZParam = parameters.getRawParameterValue ( "accelZ" );
    
    distanceParam = parameters.getRawParameterValue ( "distance" );
    
    tsXParam = parameters.getRawParameterValue ( "tsX" );
    tsYParam = parameters.getRawParameterValue ( "tsY" );
    tsZParam = parameters.getRawParameterValue ( "tsZ" );
    
    
    // SmoothedValue setup
    //accelXSmoothed.reset          ( getSampleRate(), 0.01f );
    //accelXSmoothed.setTargetValue ( 0.0f );
}

SerialTestingAudioProcessor::~SerialTestingAudioProcessor()
{
}

//==============================================================================
const juce::String SerialTestingAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SerialTestingAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SerialTestingAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SerialTestingAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SerialTestingAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SerialTestingAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SerialTestingAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SerialTestingAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SerialTestingAudioProcessor::getProgramName (int index)
{
    return {};
}

void SerialTestingAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SerialTestingAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    
}

void SerialTestingAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SerialTestingAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SerialTestingAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    int numSamples     = buffer.getNumSamples();
    float sampleRate   = getSampleRate();
    auto* leftChannel  = buffer.getWritePointer ( 0 );
    auto* rightChannel = buffer.getWritePointer ( 1 );
    
    // DSP!!!
    for (int i = 0; i < numSamples; i++)
    {
        float sampleL = leftChannel[i];
        float sampleR = rightChannel[i];
        
        
        
        leftChannel[i]  = 0.0f;
        rightChannel[i] = 0.0f;
    }
    
}

//==============================================================================
bool SerialTestingAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SerialTestingAudioProcessor::createEditor()
{
    return new SerialTestingAudioProcessorEditor (*this);
}

//==============================================================================
void SerialTestingAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SerialTestingAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SerialTestingAudioProcessor();
}
