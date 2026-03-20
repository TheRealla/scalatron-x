#pragma once
#include <JuceHeader.h>
#include "ScalatronCore.h"

class ScalatronXAudioProcessor : public juce::AudioProcessor
{
public:
    ScalatronXAudioProcessor();
    ~ScalatronXAudioProcessor() override = default;

    //==============================================================================
    void prepareToPlay (double sr, int samplesPerBlock) override;
    void releaseResources() override {}
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    //==============================================================================
    const juce::String getName() const override { return "Scalatron-X"; }
    bool acceptsMidi()  const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    //==============================================================================
    int  getNumPrograms() override { return int (factoryPresets.size()); }
    int  getCurrentProgram() override { return currentProgram; }
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int, const juce::String&) override {}

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // Public for Editor access
    using PresetTuple = std::tuple<juce::String, int, float, float>;
    std::vector<PresetTuple> factoryPresets;

    juce::AudioParameterChoice* toneTabParam = nullptr;
    juce::AudioParameterFloat*  attackParam  = nullptr;
    juce::AudioParameterFloat*  releaseParam = nullptr;

private:
    ScalatronCore core;
    int currentProgram = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScalatronXAudioProcessor)
};
