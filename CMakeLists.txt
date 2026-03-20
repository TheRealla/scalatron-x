cmake_minimum_required(VERSION 3.15)

# 1. Define Project Name
project(ScalatronX VERSION 1.0.0)

# 2. Link JUCE (Ensure JUCE is in a subdirectory or installed)
# If JUCE is not in the project root, change 'JUCE' to the correct path
add_subdirectory(JUCE) 

# 3. Create the Plugin Target
juce_add_plugin(ScalatronX
    COMPANY_NAME "Motorola-Lab-Recreations"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE "Moto"
    PLUGIN_CODE "ScaX"
    FORMATS VST3 AU Standalone
    PRODUCT_NAME "Scalatron-X")

# 4. Include Source Files
target_sources(ScalatronX PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    Source/ScalatronCore.h)

# 5. Link Libraries and Define Settings
target_compile_definitions(ScalatronX
    PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(ScalatronX
    PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_audio_formats
    juce::juce_audio_devices
    juce::juce_audio_basics
    juce::juce_graphics
    juce::juce_data_structures
    juce::juce_events
    juce::juce_core)

# 6. Optimization for Production Build
if(MSVC)
    target_compile_options(ScalatronX PRIVATE /W4)
else()
    target_compile_options(ScalatronX PRIVATE -Wall -Wextra -Wpedantic)
endif()
