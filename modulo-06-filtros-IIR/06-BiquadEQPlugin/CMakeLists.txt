# ============================================================
# CMakeLists.txt
# Programación Aplicada al Sonido II — UNA
# Unidad 3 · Plugin: Biquad EQ
# ============================================================
#
# Compilar:
#   cd ~/UNA-PAS2--juce-plugins/06-BiquadEQPlugin
#   mkdir -p build && cd build && cmake .. && make 
#   cmake .. && make
#
# Copiar a VST3:
#   cp -r ~/Desktop/dev/UNA-PAS2/UNA-PAS2--juce-plugins/06-BiquadEQPlugin/build/BiquadEQPlugin_artefacts/VST3/* ~/Library/Audio/Plug-Ins/VST3/
#
# ============================================================

cmake_minimum_required(VERSION 3.22)
project(BiquadEQPlugin VERSION 0.1)

add_subdirectory(/Users/derivasur/Desktop/dev/JUCE ./JUCE)

juce_add_plugin(BiquadEQPlugin
    FORMATS VST3
    PLUGIN_NAME "Biquad EQ"
    PLUGIN_MANUFACTURER_CODE Mfr1
    PLUGIN_CODE Biq1
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    VST3_CATEGORIES "Fx"
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_CAN_REPLACE_VST2 FALSE
)

juce_generate_juce_header(BiquadEQPlugin)

target_sources(BiquadEQPlugin PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)

target_link_libraries(BiquadEQPlugin PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_dsp
)

target_compile_definitions(BiquadEQPlugin PUBLIC
    JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING=1
)