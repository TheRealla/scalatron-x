#pragma once
#include <JuceHeader.h>
#include "ScalatronXAudioProcessor.h"

class ScalatronXAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit ScalatronXAudioProcessorEditor (ScalatronXAudioProcessor&);
    ~ScalatronXAudioProcessorEditor() override;

    void paint   (juce::Graphics&) override;
    void resized ()                override;

private:
    ScalatronXAudioProcessor& processor;

    juce::ComboBox presetBox;
    juce::Slider   attackSlider, releaseSlider;
    juce::Label    attackLabel,  releaseLabel;

    // Parameter attachments (manage lifetime, thread-safe automation)
    std::unique_ptr<juce::SliderParameterAttachment> attackAttachment;
    std::unique_ptr<juce::SliderParameterAttachment> releaseAttachment;

    struct VintageLookAndFeel : public juce::LookAndFeel_V4
    {
        VintageLookAndFeel()
        {
            setColour (juce::Slider::thumbColourId,          juce::Colours::orange);
            setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xFFCC6600));
            setColour (juce::ComboBox::backgroundColourId,   juce::Colour (0xFF2A2A2A));
            setColour (juce::ComboBox::textColourId,         juce::Colours::orange);
            setColour (juce::ComboBox::arrowColourId,        juce::Colours::orange);
            setColour (juce::Label::textColourId,            juce::Colours::orange);
            setColour (juce::PopupMenu::backgroundColourId,  juce::Colour (0xFF2A2A2A));
            setColour (juce::PopupMenu::textColourId,        juce::Colours::orange);
            setColour (juce::PopupMenu::highlightedBackgroundColourId, juce::Colour (0xFF3A3A3A));
        }
    } vintageLF;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScalatronXAudioProcessorEditor)
};
