#pragma comment(lib, "DelayImp")

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <Windows.h>
#define UNREFERENCED_PARAMETER(P) (P)

extern "C" void __declspec(dllexport) _stdcall ApplyState(std::string, std::string);
extern "C" void __declspec(dllexport) _stdcall getAllState(std::string, std::string&);
extern "C" void __declspec(dllexport) _stdcall getRSUuid(std::string&);
extern "C" void __declspec(dllexport) _stdcall getGRSUuid(std::string&);
extern "C" void __declspec(dllexport) _stdcall getRSName(std::string, std::string&);

HMODULE hmod = NULL;
bool ext_avail = false;
//void juce::AudioProcessorParameter::Listener lgain()
//{
//}
//==============================================================================
StateFullnessAudioProcessor::StateFullnessAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
    auto attributes = juce::AudioParameterBoolAttributes().withAutomatable(true);
    juce::ParameterID id("Global Rackspace", 0);
    addParameter(grs = new juce::AudioParameterBool(id, "Global Rackspace", false, attributes));

    grs->addListener(this);
    
    hmod = LoadLibrary("StateFullness-Ext.dll");
    if (hmod != NULL)
    {
        ext_avail = true;
        FreeLibrary(hmod);
        std::cout << "Found lib" << std::endl;
    }

    if (ext_avail)
    {
        getRSUuid(myRSUuid);
        getGRSUuid(myGRSUuid);
        getRSName(myRSUuid, myRSName);
        std::cout << "Found lib and got names" << std::endl;
    }
    else
    {
        myRSName = "GP Extension not loaded";
    }

}

StateFullnessAudioProcessor::~StateFullnessAudioProcessor()
{
}

//==============================================================================
const juce::String StateFullnessAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StateFullnessAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool StateFullnessAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool StateFullnessAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double StateFullnessAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int StateFullnessAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int StateFullnessAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StateFullnessAudioProcessor::setCurrentProgram(int index)
{
    UNREFERENCED_PARAMETER(index);
}

const juce::String StateFullnessAudioProcessor::getProgramName(int index)
{
    UNREFERENCED_PARAMETER(index);
    return "Default";
}

void StateFullnessAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    UNREFERENCED_PARAMETER(index);
    UNREFERENCED_PARAMETER(newName);
}

//==============================================================================
void StateFullnessAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    UNREFERENCED_PARAMETER(sampleRate);
    UNREFERENCED_PARAMETER(samplesPerBlock);
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void StateFullnessAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StateFullnessAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
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


void StateFullnessAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    UNREFERENCED_PARAMETER(midiMessages);
    UNREFERENCED_PARAMETER(buffer);
    /*
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    */
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    /*
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    */
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    /*
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        // ..do something to the data...
    }
    */
}

//==============================================================================
bool StateFullnessAudioProcessor::hasEditor() const
{
    return true; // false; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* StateFullnessAudioProcessor::createEditor()
{
    return new StateFullnessAudioProcessorEditor(*this); //  
    //return new juce::GenericAudioProcessorEditor(this);
}

//==============================================================================
void StateFullnessAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // This exports data from the plugin to the host

    if (ext_avail)
    {
        if (*grs)
        {
            getAllState(myGRSUuid, myState);
            std::string FullState = "Global;";

            FullState.append(myState);
            destData.replaceAll(FullState.c_str(), FullState.size());
        }
        else
        {
            getAllState(myRSUuid, myState);
            std::string FullState = "Local;";

            FullState.append(myState);
            destData.replaceAll(FullState.c_str(), FullState.size());
        }
    }
}

void StateFullnessAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // This imports data from the host to the plugin
    if (ext_avail)
    {
        if (sizeInBytes > 0)
        {
            std::string FullState = std::string((const char*)data);
            if (FullState._Starts_with("Global;"))
            {
                *grs = true;
                myState.assign(FullState.substr(7));
                ApplyState(myGRSUuid, myState);
            }
            if (FullState._Starts_with("Local;"))
            {
                *grs = false;
                myState.assign(FullState.substr(6));
                ApplyState(myRSUuid, myState);
            }
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StateFullnessAudioProcessor();
}

void StateFullnessAudioProcessor::parameterValueChanged(int parameterIndex, float newValue)
{
    if (ext_avail)
    {
        if (parameterIndex == 0)
        {
            if (newValue == 1.0)
                getRSName(myGRSUuid, myRSName);
            if (newValue == 0.0)
                getRSName(myRSUuid, myRSName);
        }

        juce::AudioProcessorEditor* ae = getActiveEditor();
        if (ae != nullptr)
            ae->repaint();
    }
}

void StateFullnessAudioProcessor::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)
{
    UNREFERENCED_PARAMETER(parameterIndex);
    UNREFERENCED_PARAMETER(gestureIsStarting);
}

bool StateFullnessAudioProcessor::refreshName()
{
    if (ext_avail)
    {
        if (*grs)
            getRSName(myGRSUuid, myRSName);
        else
            getRSName(myRSUuid, myRSName);
    }
    return ext_avail;
}
