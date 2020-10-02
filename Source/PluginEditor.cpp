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
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void PluginTemplateAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(40);
    bounds.reduce(40, 40);
    
    Grid grid;
    using Track = Grid::TrackInfo;
    using Fr = Grid::Fr;
    
    grid.items.add(GridItem(volumeSlider.get()));
    grid.items.add(GridItem(lpfSlider.get()));
    
    grid.templateColumns = { Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1)) };
    grid.templateRows = {Track (Fr (1)), Track (Fr (1)) };
    grid.columnGap = Grid::Px (10);
    grid.rowGap = Grid::Px (10);
    
    grid.performLayout(bounds);
    
}
