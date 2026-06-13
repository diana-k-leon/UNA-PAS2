cmake_minimum_required(VERSION 3.22)
project(ReverbAlgoritmica VERSION 1.0.0)

set(JUCE_PATH "/Users/derivasur/Desktop/dev/JUCE")
add_subdirectory(${JUCE_PATH} JUCE)

juce_add_plugin(ReverbAlgoritmica
    COMPANY_NAME "UNA-PAS2"
    PLUGIN_MANUFACTURER_CODE "UPAS"
    PLUGIN_CODE "RvAl"
    FORMATS VST3 Standalone
    PRODUCT_NAME "ReverbAlgoritmica"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
)

juce_generate_juce_header(ReverbAlgoritmica)

target_sources(ReverbAlgoritmica PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)

target_compile_definitions(ReverbAlgoritmica PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(ReverbAlgoritmica PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_plugin_client
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
