/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StateFullnessAudioProcessorEditor::StateFullnessAudioProcessorEditor(StateFullnessAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(350, 150);
    ext_avail = audioProcessor.refreshName();
    if (ext_avail)
    {
        // "Rackspace is Global"
        juce__labelGRS.reset(new juce::Label("RSN", "Rackspace is Global"));
        addAndMakeVisible(juce__labelGRS.get());

        juce__labelGRS->setHelpText("Select this option to use StateFullness in the Global Rackspace");
        juce__labelGRS->setTooltip(juce__labelGRS->getHelpText());
        juce__labelGRS->setFont(juce::Font(15.00f, juce::Font::plain)); // .withTypefaceStyle("Bold"));
        juce__labelGRS->setJustificationType(juce::Justification::left);
        juce__labelGRS->setEditable(false, false, false);
        juce__labelGRS->setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
        juce__labelGRS->setColour(juce::Label::textColourId, juce::Colours::antiquewhite);

        // On/Off
        juce__toggleGRS.reset(new juce::ToggleButton("Global"));
        addAndMakeVisible(juce__toggleGRS.get());

        juce__toggleGRS->setButtonText("");
        juce__toggleGRS->setHelpText("Select this option to use StateFullness in the Global Rackspace");
        juce__toggleGRS->setTooltip(juce__toggleGRS->getHelpText());;
        juce__toggleGRS->setExplicitFocusOrder(1);
        juce__toggleGRS->addListener(this);
        juce__toggleGRS->setColour(juce::ToggleButton::textColourId, juce::Colours::white);
        juce__toggleGRS->setToggleState((bool)*audioProcessor.grs, juce::dontSendNotification);

        // "Rackspace Name"
        juce__labelRSN.reset(new juce::Label("Rackspace Name", "Rackspace Name"));
        addAndMakeVisible(juce__labelRSN.get());

        juce__labelRSN->setHelpText("The name of the Rackspace of which StateFullness is persisting the state");
        juce__labelRSN->setTooltip(juce__labelRSN->getHelpText());
        juce__labelRSN->setFont(juce::Font(15.00f, juce::Font::plain));// .withTypefaceStyle("Bold"));
        juce__labelRSN->setJustificationType(juce::Justification::left);
        juce__labelRSN->setEditable(false, false, false);
        juce__labelRSN->setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
        juce__labelRSN->setColour(juce::Label::textColourId, juce::Colours::antiquewhite);

        // <Name>
        juce__labelRSNVal.reset(new juce::Label("RSNVal")); // , TRANS("50% width")));
        addAndMakeVisible(juce__labelRSNVal.get());

        juce__labelRSNVal->setHelpText("The name of the Rackspace of which StateFullness is persisting the state");
        juce__labelRSNVal->setTooltip(juce__labelRSNVal->getHelpText());
        juce__labelRSNVal->setFont(juce::Font(15.00f, juce::Font::plain));
        juce__labelRSNVal->setJustificationType(juce::Justification::left);
        juce__labelRSNVal->setEditable(false, false, false);
        juce__labelRSNVal->setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
        juce__labelRSNVal->setColour(juce::Label::textColourId, juce::Colours::white);

        juce__labelGRS->setBounds(20, 20, 120, 24);
        juce__toggleGRS->setBounds(150, 20, 24, 24);

        juce__labelRSN->setBounds(20, 50, 120, 24);
        juce__labelRSNVal->setBounds(150, 50, 200, 24);
        startTimerHz(2);
    }
    else
    {
        juce__labelExtError.reset(new juce::TextEditor("ExtError"));
        
        addAndMakeVisible(juce__labelExtError.get());
        juce__labelExtError->setText("This a Gig Performer only plugin.\r\n\r\nThe GP StateFullness extension is not loaded.\r\n");
        juce__labelExtError->setHelpText("The GP StateFullness extension is not loaded");
        juce__labelExtError->setTooltip(juce__labelExtError->getHelpText());
        juce__labelExtError->setFont(juce::Font(15.00f, juce::Font::bold));

        juce__labelExtError->setMultiLine(true);
        juce__labelExtError->setReadOnly(true);

        // if you don't want the text to even be selectable
        juce__labelExtError->setInterceptsMouseClicks(false, false);
        juce__labelExtError->setBorder(juce::BorderSize(3));
        juce__labelExtError->setJustification(juce::Justification::verticallyCentred);
        juce__labelExtError->setColour(juce::TextEditor::backgroundColourId, juce::Colours::slategrey);
        juce__labelExtError->setColour(juce::TextEditor::textColourId, juce::Colours::white);

        juce__labelExtError->setBounds(getBounds());
    }
}

/// <summary>
/// Poll for change of the rackspace name when the editor is visible
/// </summary>
void StateFullnessAudioProcessorEditor::timerCallback()
{
    if (ext_avail)
    {
        audioProcessor.refreshName();
        if (tmpRSN.compare(audioProcessor.myRSName) != 0)
        {
            tmpRSN = audioProcessor.myRSName;
            this->repaint();
        }
    }
}

void StateFullnessAudioProcessorEditor::buttonClicked(juce::Button* buttonThatWasClicked)
{
    if (ext_avail)
    {
        if (buttonThatWasClicked == juce__toggleGRS.get())
        {
            audioProcessor.grs->setValueNotifyingHost(juce__toggleGRS.get()->getToggleStateValue().getValue());
        }
    }
}

StateFullnessAudioProcessorEditor::~StateFullnessAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void StateFullnessAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::slategrey);
    if (ext_avail)
        juce__labelRSNVal->setText(audioProcessor.myRSName.c_str(), juce::dontSendNotification);
}

void StateFullnessAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
