# ============================================================
# CMakeLists.txt
# Programación Aplicada al Sonido II — UNA
# Unidad 3 · Plugin: FIR Filter
# ============================================================
#
# Compilar:
#   cd ~/UNA-PAS2--juce-plugins/05-FIRFilterPlugin
#   mkdir -p build && cd build && cmake .. && make
#
# Copiar a VST3:
#   cp -r ~/Desktop/dev/UNA-PAS2/UNA-PAS2--juce-plugins/05-FIRFilterPlugin/build/FIRFilterPlugin_artefacts/VST3/* ~/Library/Audio/Plug-Ins/VST3/
#
# ============================================================

cmake_minimum_required(VERSION 3.22)
project(FIRFilterPlugin VERSION 0.1)

add_subdirectory(/Users/derivasur/Desktop/dev/JUCE ./JUCE)

juce_add_plugin(FIRFilterPlugin
    FORMATS VST3
    PLUGIN_NAME "FIR Filter"
    PLUGIN_MANUFACTURER_CODE Mfr1
    PLUGIN_CODE Fir1
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    VST3_CATEGORIES "Fx"
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST3_CAN_REPLACE_VST2 FALSE
)

juce_generate_juce_header(FIRFilterPlugin)

target_sources(FIRFilterPlugin PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)

target_link_libraries(FIRFilterPlugin PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_dsp
)

target_compile_definitions(FIRFilterPlugin PUBLIC
    JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING=1
)