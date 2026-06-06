# ============================================================
# CMakeLists.txt
# Programación Aplicada al Sonido II — UNA
# Módulo 6 · Plugin: Subtractive Synth
# ============================================================
#
# Compilar:
#   cd ~/UNA-PAS2--juce-plugins/07-SubtractiveSynthPlugin
#   mkdir -p build && cd build && cmake .. && make
#
# ============================================================

cmake_minimum_required(VERSION 3.22)
project(SubtractiveSynthPlugin VERSION 0.1)

add_subdirectory(/Users/derivasur/Desktop/dev/JUCE ./JUCE)

juce_add_plugin(SubtractiveSynthPlugin
    FORMATS VST3
    PLUGIN_NAME "Subtractive Synth"
    PLUGIN_MANUFACTURER_CODE Mfr1
    PLUGIN_CODE Sub1
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    VST3_CATEGORIES "Fx"
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST3_CAN_REPLACE_VST2 FALSE
)

juce_generate_juce_header(SubtractiveSynthPlugin)

target_sources(SubtractiveSynthPlugin PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)

target_link_libraries(SubtractiveSynthPlugin PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_dsp
)

target_compile_definitions(SubtractiveSynthPlugin PUBLIC
    JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING=1
)
