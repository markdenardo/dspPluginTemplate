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
                       ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
    apvts.state.addListener(this);
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
    auto numSamples = buffer.getNumSamples();
    auto numChannels = jmin(totalNumInputChannels, totalNumOutputChannels);

    auto sumMaxVal = 0.0f;
    auto currentMaxVal = meterGlobalMaxVal.load();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, numSamples);
    
//    outputVolume.applyGain(buffer, buffer.getNumSamples());

    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        auto channelMaxVal = 0.0f;
            
        iirFilter[channel].processSamples(channelData, numSamples);
        
        outputVolume[channel].applyGain(channelData,numSamples);
        
        //absolute value of all samples in a buffer
        //is the current sample larger than our current max?
            //if yes -- channelaxVal = new max
        
        for (int sample = 0; sample < numSamples; ++sample)
        {
            auto rectifiedVal = std::abs(channelData[sample]);
            
            if (channelMaxVal < rectifiedVal)
                channelMaxVal = rectifiedVal;
            
            if (currentMaxVal< rectifiedVal)
                currentMaxVal=rectifiedVal;
        }
        
            sumMaxVal += channelMaxVal;//sum of ch 0 and  ch 1  max vals
        
            meterGlobalMaxVal.store(currentMaxVal);
//        ignoreUnused(channelData);
        
        
        for (int sample = 0; sample < numSamples; ++sample)
        {
//            channelData[sample] *=  outputVolume;
            
            //iterate hard clipper values
            channelData[sample] = jlimit(-1.0f, 1.0f, channelData[sample]);
            
        }
    }
    
    meterLocalMaxVal.store (sumMaxVal/(float)numChannels) ; //numChannels
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
    ValueTree copyState = apvts.copyState();
    std::unique_ptr<XmlElement> xml = copyState.createXml();
    copyXmlToBinary(*xml.get(), destData);
}

void PluginTemplateAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xml = getXmlFromBinary(data, sizeInBytes);
    ValueTree copyState = ValueTree::fromXml(*xml.get());
    apvts.replaceState(copyState);
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
    auto frequency = apvts.getRawParameterValue("LPF");
    auto volume = apvts.getRawParameterValue("VOL");
//    outputVolume = Decibels::decibelsToGain(volume->load())
    
    for (int channel = 0; channel < 2; ++channel)
    {
        iirFilter[channel].setCoefficients(IIRCoefficients::makeLowPass(getSampleRate(), frequency->load()));
        outputVolume[channel].setTargetValue( Decibels::decibelsToGain(volume->load()));
    }
    
}

void PluginTemplateAudioProcessor::reset()
{
  //Reset DSP parameters
    for (int channel = 0; channel < 2; ++channel)
        
    {
        iirFilter[channel].reset();
        outputVolume[channel].reset(getSampleRate(), 0.050);
    }
    
    meterLocalMaxVal.store(0.0f);
    meterGlobalMaxVal.store(0.0f);
}

//void PluginTemplateAudioProcessor::userChangedParameter()
//{
//    mustUpdateProcessing = true;
//}

AudioProcessorValueTreeState::ParameterLayout PluginTemplateAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> parameters;
    
    //float value returns as a string w a mx length of 4 characters
    std::function<String(float, int)> valueToTextFunction = [](float x, int l) { return String(x,4);};
    
    //value to text function
    std::function<float(const String&)> textToValueFunction = [](const String& str) {return str.getFloatValue(); };
    
    //Filter
    parameters.push_back(std::make_unique<AudioParameterFloat >("LPF", "Low Pass Filter", NormalisableRange<float>(20.0f, 20000.0f, 20.0f, 2.0f), 800.0f, "Hz", AudioProcessorParameter::genericParameter, valueToTextFunction, textToValueFunction));
    
    //long way to declare parameter, same application in code
    //create our parameters for VOL
    parameters.push_back(std::make_unique<AudioParameterFloat >("VOL", "Volume",NormalisableRange<float>(-40.0f, 40.0f),0.0f,"db",AudioProcessorParameter::genericParameter,valueToTextFunction,textToValueFunction ));
    
//    auto gainParam = ;
//    //add them to the vector
    
//    parameters.push_back(std::move(gainParam));
    
 
    return { parameters.begin(), parameters.end() };
}
