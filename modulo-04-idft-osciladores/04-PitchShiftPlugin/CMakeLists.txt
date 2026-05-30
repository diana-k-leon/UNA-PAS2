# ============================================================
# CMakeLists.txt
# Programación Aplicada al Sonido II — UNA
# Unidad 2 · Plugin: Pitch Shift
# ============================================================
#
# Compilar:
#   cd ~/UNA-PAS2--juce-plugins/04-PitchShiftPlugin
#   mkdir -p build && cd build && cmake .. && make
#
# Copiar a VST3:
#   cp -r ~/Desktop/dev/UNA-PAS2/UNA-PAS2--juce-plugins/04-PitchShiftPlugin/build/PitchShiftPlugin_artefacts/VST3/* ~/Library/Audio/Plug-Ins/VST3/
#
# ============================================================

cmake_minimum_required(VERSION 3.22)
project(PitchShiftPlugin VERSION 0.1)

add_subdirectory(/Users/derivasur/Desktop/dev/JUCE ./JUCE)

juce_add_plugin(PitchShiftPlugin
    FORMATS VST3
    PLUGIN_NAME "Pitch Shift"
    PLUGIN_MANUFACTURER_CODE Mfr1
    PLUGIN_CODE Ptch
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    VST3_CATEGORIES "Fx"
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_CAN_REPLACE_VST2 FALSE
)

juce_generate_juce_header(PitchShiftPlugin)

target_sources(PitchShiftPlugin PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)

target_link_libraries(PitchShiftPlugin PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_dsp
)

target_compile_definitions(PitchShiftPlugin PUBLIC
    JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING=1
)