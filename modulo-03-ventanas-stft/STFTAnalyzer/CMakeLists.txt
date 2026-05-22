# ============================================================
# CMakeLists.txt
# Programación Aplicada al Sonido II — UNA
# Unidad 1 · Plugin: STFT Analyzer
# ============================================================
#
# Compilar:
#   cd ~/UNA-PAS2--juce-plugins/02-STFTAnalyzer
#   mkdir -p build && cd build && cmake .. && make
#
# Copiar a VST3:
#   cp -r ~/Desktop/dev/UNA-PAS2/UNA-PAS2--juce-plugins/02-STFTAnalyzer/build/STFTAnalyzer_artefacts/VST3/* ~/Library/Audio/Plug-Ins/VST3/
#
# ============================================================

cmake_minimum_required(VERSION 3.22)
project(STFTAnalyzer VERSION 0.1)

# Ajustá esta ruta a donde tengas JUCE clonado
add_subdirectory(/Users/derivasur/Desktop/dev/JUCE ./JUCE)

juce_add_plugin(STFTAnalyzer
    FORMATS VST3
    PLUGIN_NAME "STFT Analyzer"
    PLUGIN_MANUFACTURER_CODE Mfr1
    PLUGIN_CODE Stft
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    VST3_CATEGORIES "Analyzer"
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_CAN_REPLACE_VST2 FALSE
)

juce_generate_juce_header(STFTAnalyzer)

target_sources(STFTAnalyzer PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)

target_link_libraries(STFTAnalyzer PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_dsp
)

target_compile_definitions(STFTAnalyzer PUBLIC
    JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING=1
)