/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PluginTemplateAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                            public Button::Listener
{
public:
    PluginTemplateAudioProcessorEditor (PluginTemplateAudioProcessor&);
    ~PluginTemplateAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void buttonClicked(Button* button) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    std::unique_ptr<Slider> volumeSlider, lpfSlider;
    std::unique_ptr<Label> volumeLabel, lpfLabel;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> volumeAttachment, lpfAttachment;
    std::unique_ptr<TextButton> lookAndFeelButton;
    
    LookAndFeel_V4 theLFDark, theLFMid, theLFGrey, theLFLight;
    LookAndFeel_V3 theLFV3;
    LookAndFeel_V2 theLFV2;
    int currentLF = 1;
    
     
    PluginTemplateAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginTemplateAudioProcessorEditor)
};
