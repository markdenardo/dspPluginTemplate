/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginTemplateAudioProcessor::PluginTemplateAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    init();
}

PluginTemplateAudioProcessor::~PluginTemplateAudioProcessor()
{
}

//==============================================================================
const juce::String PluginTemplateAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginTemplateAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginTemplateAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PluginTemplateAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PluginTemplateAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginTemplateAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PluginTemplateAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PluginTemplateAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PluginTemplateAudioProcessor::getProgramName (int index)
{
    return {};
}

void PluginTemplateAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PluginTemplateAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    prepare(sampleRate, samplesPerBlock);
    update();
    reset();
    isActive = true;
}

void PluginTemplateAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PluginTemplateAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void PluginTemplateAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if(!isActive)
    {
        return;
    }
    
    if (mustUpdateProcessing)
    {
        update();
    }
    
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

   
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        ignoreUnused(channelData);
    }
}

//==============================================================================
bool PluginTemplateAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginTemplateAudioProcessor::createEditor()
{
    return new PluginTemplateAudioProcessorEditor (*this);
}

//==============================================================================
void PluginTemplateAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PluginTemplateAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginTemplateAudioProcessor();
}

//==============================================================================

void PluginTemplateAudioProcessor::init()
{
    //Called Once; Give Initial Values to DSP
    
}
    
void PluginTemplateAudioProcessor::prepare(double sampleRate, int samplesPerBlock)
{
  //Pass Sample Rate and Buffer Size to DSP
}
void PluginTemplateAudioProcessor::update()
{
    mustUpdateProcessing = false;
    //Update DSP when a user changes parameters
}

void PluginTemplateAudioProcessor::reset()
{
  //Reset DSP parameters
}

void PluginTemplateAudioProcessor::userChangedParameter()
{
    mustUpdateProcessing = true;
}
