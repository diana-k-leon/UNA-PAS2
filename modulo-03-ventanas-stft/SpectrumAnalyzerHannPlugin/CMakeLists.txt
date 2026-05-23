# ============================================================
# CMakeLists.txt
# Programación Aplicada al Sonido II — UNA
# Unidad 1 · Plugin: Spectrum Analyzer con ventana Hann
# ============================================================
#
# Compilar:
#   cd ~/Desktop/dev/UNA-PAS2/modulo-02-dft-fft/SpectrumAnalyzerHannPlugin
#   rm -rf build && mkdir build && cd build && cmake .. && make
#
# ============================================================

cmake_minimum_required(VERSION 3.22)
project(SpectrumAnalyzerHann VERSION 0.1)

add_subdirectory(/Users/derivasur/Desktop/dev/JUCE ./JUCE)

juce_add_plugin(SpectrumAnalyzerHann
    FORMATS VST3
    PLUGIN_NAME "Spectrum Analyzer Hann"
    PLUGIN_MANUFACTURER_CODE Mfr1
    PLUGIN_CODE SpHn
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    VST3_CATEGORIES "Analyzer"
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST3_CAN_REPLACE_VST2 FALSE
)

juce_generate_juce_header(SpectrumAnalyzerHann)

target_sources(SpectrumAnalyzerHann PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)

target_link_libraries(SpectrumAnalyzerHann PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_dsp
)

target_compile_definitions(SpectrumAnalyzerHann PUBLIC
    JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING=1
)
