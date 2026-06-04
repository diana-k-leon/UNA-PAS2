# ============================================================
# CMakeLists.txt
# Programación Aplicada al Sonido II — UNA
# Unidad 3 · Plugin: IR Convolution
# ============================================================
#
# Compilar:
#   cd ~/UNA-PAS2--juce-plugins/05b-IRConvolutionPlugin
#   mkdir -p build && cd build && cmake .. && make
#
# ============================================================

cmake_minimum_required(VERSION 3.22)
project(IRConvolutionPlugin VERSION 0.1)

add_subdirectory(/Users/derivasur/Desktop/dev/JUCE ./JUCE)

juce_add_plugin(IRConvolutionPlugin
    FORMATS VST3
    PLUGIN_NAME "IR Convolution"
    PLUGIN_MANUFACTURER_CODE Mfr1
    PLUGIN_CODE IRc1
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    VST3_CATEGORIES "Fx"
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST3_CAN_REPLACE_VST2 FALSE
)

juce_generate_juce_header(IRConvolutionPlugin)

target_sources(IRConvolutionPlugin PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)

target_link_libraries(IRConvolutionPlugin PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_dsp
)

target_compile_definitions(IRConvolutionPlugin PUBLIC
    JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING=1
)
