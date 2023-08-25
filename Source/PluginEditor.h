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
class StateFullnessAudioProcessorEditor  : public juce::AudioProcessorEditor,
    public juce::Button::Listener, public juce::Timer

{
public:
    StateFullnessAudioProcessorEditor (StateFullnessAudioProcessor&);
    ~StateFullnessAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* buttonThatWasClicked) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    StateFullnessAudioProcessor& audioProcessor;
    std::unique_ptr<juce::ToggleButton> juce__toggleGRS;
    std::unique_ptr<juce::Label> juce__labelRSNVal;
    std::unique_ptr<juce::Label> juce__labelGRS;
    std::unique_ptr<juce::Label> juce__labelRSN;
    std::unique_ptr<juce::TextEditor> juce__labelExtError;
    juce::TooltipWindow tooltipWindow;

    void timerCallback() override;

    std::string tmpRSN;
    bool ext_avail = false;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StateFullnessAudioProcessorEditor)
};
