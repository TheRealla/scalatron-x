cmake_minimum_required(VERSION 3.22)
project(ScalatronX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── JUCE (add as git submodule: git submodule add https://github.com/juce-framework/JUCE.git JUCE)
add_subdirectory(JUCE)

juce_add_plugin(ScalatronX
    VERSION                     1.0.0
    COMPANY_NAME                "ScalatronLabs"
    PLUGIN_MANUFACTURER_CODE    Sclb
    PLUGIN_CODE                 Sclx
    FORMATS                     VST3
    PRODUCT_NAME                "Scalatron-X"
    IS_SYNTH                    TRUE
    NEEDS_MIDI_INPUT            TRUE
    NEEDS_MIDI_OUTPUT           FALSE
    IS_MIDI_EFFECT              FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD     TRUE
)

target_sources(ScalatronX PRIVATE
    Source/ScalatronXAudioProcessor.cpp
    Source/ScalatronXAudioProcessorEditor.cpp
)

target_include_directories(ScalatronX PRIVATE Source)

target_compile_definitions(ScalatronX PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
)

target_link_libraries(ScalatronX
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_dsp
        juce::juce_gui_basics
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
