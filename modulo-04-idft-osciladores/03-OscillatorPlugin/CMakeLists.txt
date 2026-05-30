# ============================================================
# CMakeLists.txt
# Programación Aplicada al Sonido II — UNA
# Unidad 2 · Plugin: Oscillator
# ============================================================
#
# Compilar:
#   cd ~/carpeta/03-OscillatorPlugin
#   mkdir -p build && cd build && cmake .. && make
#
# Copiar a VST3:
#   cp -r ~/Desktop/dev/UNA-PAS2/modulo-04-idft-osciladores/03-OscillatorPlugin/build/OscillatorPlugin_artefacts/VST3/* ~/Library/Audio/Plug-Ins/VST3/
#
# ============================================================

cmake_minimum_required(VERSION 3.22)
project(OscillatorPlugin VERSION 0.1)

add_subdirectory(/Users/derivasur/Desktop/dev/JUCE ./JUCE)

juce_add_plugin(OscillatorPlugin
    FORMATS VST3
    PLUGIN_NAME "Oscillator"
    PLUGIN_MANUFACTURER_CODE Mfr1
    PLUGIN_CODE Osci
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    VST3_CATEGORIES "Synth"
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_CAN_REPLACE_VST2 FALSE
)

juce_generate_juce_header(OscillatorPlugin)

target_sources(OscillatorPlugin PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)

target_link_libraries(OscillatorPlugin PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_dsp
)

target_compile_definitions(OscillatorPlugin PUBLIC
    JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING=1
)