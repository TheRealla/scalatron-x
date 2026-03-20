#include "ScalatronXAudioProcessor.h"
#include "ScalatronXAudioProcessorEditor.h"

//==============================================================================
ScalatronXAudioProcessor::ScalatronXAudioProcessor()
    : AudioProcessor (BusesProperties()
          .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
    addParameter (toneTabParam = new juce::AudioParameterChoice (
        "tone", "Tone Tab", {"Pure", "Flute", "Reed", "String"}, 0));
    addParameter (attackParam  = new juce::AudioParameterFloat (
        "attack",  "Attack",  0.001f, 2.0f, 0.02f));
    addParameter (releaseParam = new juce::AudioParameterFloat (
        "release", "Release", 0.01f,  8.0f, 0.8f));

    // ── Manual Historical Presets (1-10) ────────────────────────────────────
    factoryPresets = {
        { "1974 Factory Pure",     0, 0.02f,  0.8f  },
        { "Secor 31 Baseline",     0, 0.05f,  1.2f  },
        { "Blackwood 19 Stab",     0, 0.01f,  0.4f  },
        { "Motorola Lab Clean",    0, 0.03f,  1.0f  },
        { "Early Prototype",       1, 0.04f,  0.9f  },
        { "1974 Reed Echo",        2, 0.06f,  1.5f  },
        { "Vintage Flute Drift",   1, 0.08f,  2.0f  },
        { "Church Full",           0, 0.01f,  4.0f  },
        { "31-EDO Stab",           0, 0.005f, 0.1f  },
        { "Line Under Attack",     2, 0.001f, 0.05f },
    };

    // ── Algorithmic generation to reach 250 ─────────────────────────────────
    auto& rng = juce::Random::getSystemRandom();
    while (int (factoryPresets.size()) < 250)
    {
        const int idx = int (factoryPresets.size());
        const int cat = (idx < 50)  ? 0
                      : (idx < 120) ? 1
                      : (idx < 170) ? 2
                      : (idx < 210) ? 3
                      :               4;

        juce::String name;
        if      (cat == 0) name = "Historical "  + juce::String (idx + 1);
        else if (cat == 1) name = "EDO Assault "  + juce::String (idx - 49);
        else if (cat == 2) name = "JI Blade "     + juce::String (idx - 119);
        else if (cat == 3) name = "Secor Grid "   + juce::String (idx - 169);
        else               name = "Exp Crack "    + juce::String (idx - 209);

        factoryPresets.emplace_back (
            name,
            rng.nextInt (4),
            rng.nextFloat() * 0.1f  + 0.001f,
            rng.nextFloat() * 5.0f  + 0.1f);
    }
}

//==============================================================================
void ScalatronXAudioProcessor::prepareToPlay (double sr, int)
{
    core.initDividers();
    // sampleRate is available via AudioProcessor::getSampleRate() in processBlock
    juce::ignoreUnused (sr);
}

//==============================================================================
void ScalatronXAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    const double sr = getSampleRate();

    for (const auto meta : midi)
    {
        const auto msg = meta.getMessage();
        if (msg.isNoteOn())
            core.noteOn  (msg.getNoteNumber(), sr, *attackParam);
        if (msg.isNoteOff())
            core.noteOff (msg.getNoteNumber(), sr, *releaseParam);
    }

    core.processBlock (buffer, sr);

    // Tone-tab gain tilt (applied after core mixing)
    const int   tab  = toneTabParam->getIndex();
    const float tilt = (tab == 0) ? 1.0f
                     : (tab == 1) ? 0.85f
                     : (tab == 2) ? 1.15f
                     :              1.05f;
    buffer.applyGain (tilt);
}

//==============================================================================
void ScalatronXAudioProcessor::setCurrentProgram (int index)
{
    if (index < 0 || index >= int (factoryPresets.size())) return;
    currentProgram = index;

    const auto& [name, tab, att, rel] = factoryPresets[index];
    *toneTabParam  = tab;
    *attackParam   = att;
    *releaseParam  = rel;
}

const juce::String ScalatronXAudioProcessor::getProgramName (int index)
{
    if (index < 0 || index >= int (factoryPresets.size()))
        return {};
    return std::get<0> (factoryPresets[index]);
}

//==============================================================================
void ScalatronXAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream (destData, true);
    stream.writeInt   (currentProgram);
    stream.writeInt   (toneTabParam->getIndex());
    stream.writeFloat (*attackParam);
    stream.writeFloat (*releaseParam);
}

void ScalatronXAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::MemoryInputStream stream (data, size_t (sizeInBytes), false);
    currentProgram  = stream.readInt();
    *toneTabParam   = stream.readInt();
    *attackParam    = stream.readFloat();
    *releaseParam   = stream.readFloat();
}

//==============================================================================
juce::AudioProcessorEditor* ScalatronXAudioProcessor::createEditor()
{
    return new ScalatronXAudioProcessorEditor (*this);
}

//==============================================================================
// Required JUCE factory function
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ScalatronXAudioProcessor();
}
