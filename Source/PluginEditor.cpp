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
    
    lookAndFeelButton = std::make_unique<TextButton>("LookAndFeel");
    addAndMakeVisible(lookAndFeelButton.get());
    
    lookAndFeelButton->addListener(this);
    
    theLFDark.setColourScheme(LookAndFeel_V4::getDarkColourScheme());
    theLFMid.setColourScheme(LookAndFeel_V4::getMidnightColourScheme());
    theLFGrey.setColourScheme(LookAndFeel_V4::getGreyColourScheme());
    theLFLight.setColourScheme(LookAndFeel_V4::getLightColourScheme());
    
    LookAndFeel::setDefaultLookAndFeel(&theLFDark);
   
    
    setSize (400, 300);
}

PluginTemplateAudioProcessorEditor::~PluginTemplateAudioProcessorEditor()
{
    LookAndFeel::setDefaultLookAndFeel(nullptr); 
}

//==============================================================================
void PluginTemplateAudioProcessorEditor::paint (juce::Graphics& g)
{
    
    auto bounds = getLocalBounds();
    auto textBounds = bounds.removeFromTop(40);
    
    g.setColour (getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    g.fillRect(bounds);
    
    g.setColour(Colours::blueviolet);
    g.fillRect(textBounds);
    
    g.setColour(Colours::white);
    g.setFont(Font(20.0).italicised().withExtraKerningFactor(0.1f));
    g.drawFittedText ("DSP Lesson 1", textBounds, Justification::centredLeft, 1);
}

void PluginTemplateAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto recTop = bounds.removeFromTop(40);
    bounds.reduce(40, 40);
    
    recTop.reduce(10, 0);
    lookAndFeelButton->setBounds(recTop.removeFromRight(120).withSizeKeepingCentre(120, 24));
    
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

void PluginTemplateAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == lookAndFeelButton.get())
    {
        PopupMenu m;
        
        m.addItem(1,"Dark Look and Feel", true,currentLF==1);
        m.addItem(2,"Midnight Look and Feel", true,currentLF==2);
        m.addItem(3,"Grey Look and Feel", true,currentLF==3);
        m.addItem(4,"Light Look and Feel", true,currentLF==4);
        
        m.addSeparator();
        m.addItem(5,"JUCE 4 Look and Feel", true,currentLF==5);
        m.addItem(5,"JUCE 3 Look and Feel", true,currentLF==6);
        
        auto result = m.showAt(lookAndFeelButton.get());
        
        if (result == 1)
        LookAndFeel::setDefaultLookAndFeel(&theLFDark);
        
        else if (result == 2)
        LookAndFeel::setDefaultLookAndFeel(&theLFMid);
        
        else if (result == 3)
        LookAndFeel::setDefaultLookAndFeel(&theLFGrey);
        
        else if (result == 4)
        LookAndFeel::setDefaultLookAndFeel(&theLFLight);
        
        else if (result == 5)
        LookAndFeel::setDefaultLookAndFeel(&theLFV2);
        
        else if (result == 6)
        LookAndFeel::setDefaultLookAndFeel(&theLFV3);
            
        if(result != 0)
        currentLF = result;

        
        
    }
}
