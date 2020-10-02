/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginTemplateAudioProcessorEditor::PluginTemplateAudioProcessorEditor (PluginTemplateAudioProcessor& p)
    : AudioProcessorEditor (&p), processor(p)
{
    
    
    
    volumeSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow);
    //other way to do this ...
    //  volumeSlider.reset(new Slider(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow));
    addAndMakeVisible(volumeSlider.get());
    volumeAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts,"VOL",*volumeSlider );
    
    volumeLabel = std::make_unique<Label>("","Volume");
    addAndMakeVisible(volumeLabel.get());
    
    volumeLabel->attachToComponent(volumeSlider.get(), false);
    volumeLabel->setJustificationType(Justification::centred);
    
    //LPF
    lpfSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow);
    addAndMakeVisible(lpfSlider.get());
    lpfAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts,"LPF",*lpfSlider );
    
    lpfLabel = std::make_unique<Label>("","Low-Pass");
    addAndMakeVisible(lpfLabel.get());
    
    lpfLabel->attachToComponent(lpfSlider.get(), false);
    lpfLabel->setJustificationType(Justification::centred);
    
    setSize (400, 300);
}

PluginTemplateAudioProcessorEditor::~PluginTemplateAudioProcessorEditor()
{
}

//==============================================================================
void PluginTemplateAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void PluginTemplateAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
