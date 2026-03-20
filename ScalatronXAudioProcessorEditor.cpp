#include "ScalatronXAudioProcessorEditor.h"

//==============================================================================
ScalatronXAudioProcessorEditor::ScalatronXAudioProcessorEditor (ScalatronXAudioProcessor& p)
    : AudioProcessorEditor (p), processor (p)
{
    setLookAndFeel (&vintageLF);
    setSize (900, 520);

    // ── Preset ComboBox ──────────────────────────────────────────────────────
    addAndMakeVisible (presetBox);
    for (int i = 0; i < int (processor.factoryPresets.size()); ++i)
        presetBox.addItem (std::get<0> (processor.factoryPresets[i]), i + 1);

    presetBox.setSelectedItemIndex (processor.getCurrentProgram(),
                                    juce::dontSendNotification);
    presetBox.onChange = [this]()
    {
        processor.setCurrentProgram (presetBox.getSelectedItemIndex());
    };

    // ── Attack slider ────────────────────────────────────────────────────────
    attackSlider.setSliderStyle (juce::Slider::Rotary);
    attackSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible (attackSlider);

    attackLabel.setText ("Attack", juce::dontSendNotification);
    attackLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (attackLabel);

    // SliderParameterAttachment is the correct JUCE 6+ API (no attachToParameter())
    attackAttachment = std::make_unique<juce::SliderParameterAttachment> (
        *processor.attackParam, attackSlider);

    // ── Release slider ───────────────────────────────────────────────────────
    releaseSlider.setSliderStyle (juce::Slider::Rotary);
    releaseSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible (releaseSlider);

    releaseLabel.setText ("Release", juce::dontSendNotification);
    releaseLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (releaseLabel);

    releaseAttachment = std::make_unique<juce::SliderParameterAttachment> (
        *processor.releaseParam, releaseSlider);
}

ScalatronXAudioProcessorEditor::~ScalatronXAudioProcessorEditor()
{
    // Detach attachments before destroying sliders
    attackAttachment.reset();
    releaseAttachment.reset();
    setLookAndFeel (nullptr);
}

//==============================================================================
void ScalatronXAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Background - Dark Walnut
    g.fillAll (juce::Colour (0xFF1C1C1C));

    // Title
    g.setColour (juce::Colours::orange);
    g.setFont (juce::Font (28.0f, juce::Font::bold));
    g.drawText ("SCALATRON-X 1974", 20, 20, 500, 40, juce::Justification::left);

    // Subtitle
    g.setFont (juce::Font (13.0f));
    g.setColour (juce::Colour (0xFF888888));
    g.drawText ("NTSC 3.579545 MHz  |  240-Voice Polyphony  |  Just Intonation Engine",
                20, 58, 600, 20, juce::Justification::left);

    // Divider line
    g.setColour (juce::Colour (0xFF444444));
    g.drawHorizontalLine (84, 20.0f, 880.0f);

    // CRT Monitor frame
    g.setColour (juce::Colour (0xFF333333));
    g.fillRoundedRectangle (642.0f, 88.0f, 238.0f, 140.0f, 6.0f);
    g.setColour (juce::Colours::green);
    g.drawRoundedRectangle (642.0f, 88.0f, 238.0f, 140.0f, 6.0f, 3.0f);

    // CRT header
    g.setFont (juce::Font (12.0f, juce::Font::bold));
    g.setColour (juce::Colours::green);
    g.drawText ("TUNING MONITOR", 652, 98, 218, 24, juce::Justification::centred);

    // CRT scan-line aesthetic
    g.setColour (juce::Colour (0x22000000));
    for (int y = 122; y < 228; y += 3)
        g.drawHorizontalLine (y, 643.0f, 879.0f);

    // CRT readout text
    g.setColour (juce::Colour (0xFF44FF44));
    g.setFont (juce::Font (juce::Font::getDefaultMonospacedFontName(), 11.0f, juce::Font::plain));
    g.drawText ("A4 = 440.000 Hz",      662, 126, 198, 18, juce::Justification::left);
    g.drawText ("CLOCK = 3579545 Hz",   662, 146, 198, 18, juce::Justification::left);
    g.drawText ("VOICES = 240",         662, 166, 198, 18, juce::Justification::left);
    g.drawText ("MODE  = NTSC CRYSTAL", 662, 186, 198, 18, juce::Justification::left);

    // Preset area label
    g.setFont (juce::Font (11.0f));
    g.setColour (juce::Colour (0xFF888888));
    g.drawText ("PRESET", 20, 98, 60, 16, juce::Justification::left);

    // Knob section label
    g.setColour (juce::Colour (0xFF888888));
    g.drawText ("ENVELOPE", 20, 195, 100, 16, juce::Justification::left);
}

//==============================================================================
void ScalatronXAudioProcessorEditor::resized()
{
    // Preset box - top strip
    presetBox.setBounds (20, 116, 610, 30);

    // Knobs
    const int knobSize = 110;
    const int knobY    = 210;

    attackSlider.setBounds  (20,  knobY, knobSize, knobSize);
    attackLabel.setBounds   (20,  knobY + knobSize, knobSize, 20);

    releaseSlider.setBounds (150, knobY, knobSize, knobSize);
    releaseLabel.setBounds  (150, knobY + knobSize, knobSize, 20);
}
