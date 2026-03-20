#pragma once
#include <JuceHeader.h>
#include <array>

constexpr int MAX_VOICES = 240;
constexpr uint64_t MASTER_CLOCK = 3579545ULL; // Exact 3.579545 MHz NTSC crystal

struct Voice {
    uint32_t divider = 1;
    uint64_t counter = 0;
    bool ffState     = false;
    float env        = 0.0f;
    float envInc     = 0.0f;
    bool gate        = false;
};

class ScalatronCore
{
public:
    ScalatronCore() { initDividers(); }

    void initDividers (double a4 = 440.0, int midiBase = 12)
    {
        for (int k = 0; k < MAX_VOICES; ++k)
        {
            double semi     = double (k + midiBase - 69);
            double freq     = a4 * std::pow (2.0, semi / 12.0);
            uint64_t d      = uint64_t (std::round (double (MASTER_CLOCK) / freq));
            dividerTable[k] = uint32_t (juce::jlimit (1ULL, 1023ULL, d));
        }
    }

    void noteOn (int midiNote, double sr, float attackSec)
    {
        int k = midiNote - 12;
        if (k < 0 || k >= MAX_VOICES) return;
        auto& v   = voices[k];
        v.gate    = true;
        v.divider = dividerTable[k];
        v.envInc  = attackSec > 0.001f ? 1.0f / float (attackSec * sr) : 1.0f;
        v.env     = 0.0f;
        v.counter = 0;
    }

    void noteOff (int midiNote, double sr, float releaseSec)
    {
        int k = midiNote - 12;
        if (k < 0 || k >= MAX_VOICES) return;
        auto& v  = voices[k];
        v.gate   = false;
        v.envInc = releaseSec > 0.001f ? -1.0f / float (releaseSec * sr) : -1.0f;
    }

    void processBlock (juce::AudioBuffer<float>& buf, double sr)
    {
        const int n            = buf.getNumSamples();
        float* left            = buf.getWritePointer (0);
        const float gain       = 0.18f / float (MAX_VOICES);
        const double clocksPS  = double (MASTER_CLOCK) / sr;

        for (int i = 0; i < n; ++i)
        {
            float sum = 0.0f;

            for (int k = 0; k < MAX_VOICES; ++k)
            {
                auto& v = voices[k];
                if (!v.gate && v.env <= 0.0001f) continue;

                const uint64_t whole = uint64_t (clocksPS);
                const double   frac  = clocksPS - double (whole);
                v.counter += whole + uint64_t (frac);

                while (v.counter >= v.divider)
                {
                    v.counter -= v.divider;
                    v.ffState  = !v.ffState;
                }

                float s = v.ffState ? 1.0f : -1.0f;
                v.env   = juce::jlimit (0.0f, 1.0f, v.env + v.envInc);
                sum    += s * v.env;
            }

            float out = sum * gain;
            out = out / (1.0f + 0.25f * std::abs (out)); // vintage soft-clip
            left[i] = out;
            if (buf.getNumChannels() > 1)
                buf.getWritePointer (1)[i] = out;
        }
    }

private:
    std::array<Voice,    MAX_VOICES> voices       {};
    std::array<uint32_t, MAX_VOICES> dividerTable {};
};
