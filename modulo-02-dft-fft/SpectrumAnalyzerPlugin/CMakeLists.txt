# ============================================================
# CMakeLists.txt
# Programación Aplicada al Sonido II — UNA
# Unidad 1 · Plugin: Spectrum Analyzer
# ============================================================
#
# Compilar:
#   cd ~/Desktop/dev/UNA-PAS2/modulo-02-dft-fft/SpectrumAnalyzerPlugin
#   mkdir -p build && cd build && cmake .. && make
#
# Copiar a VST3:
#   cp -r build/SpectrumAnalyzer_artefacts/VST3/SpectrumAnalyzer.vst3 ~/Library/Audio/Plug-Ins/VST3/
# ============================================================
# mkdir -p build — crea la carpeta build (si ya existe no hace nada)
# cd build — entra a la carpeta
# cmake .. — configura el proyecto (lee CMakeLists.txt del directorio padre)
# make — compila
#

# ============================================================

cmake_minimum_required(VERSION 3.22)
project(SpectrumAnalyzer VERSION 0.1)

# Ajustá esta ruta a donde tengas JUCE clonado
add_subdirectory(/Users/derivasur/Desktop/dev/JUCE ./JUCE)

juce_add_plugin(SpectrumAnalyzerV3
    FORMATS VST3
    PLUGIN_NAME "Spectrum Analyzer V3"
    PLUGIN_MANUFACTURER_CODE Mfr1
    PLUGIN_CODE Spec
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    VST3_CATEGORIES "Analyzer"
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST3_CAN_REPLACE_VST2 FALSE
)

juce_generate_juce_header(SpectrumAnalyzerV3)

target_sources(SpectrumAnalyzerV3 PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)

target_link_libraries(SpectrumAnalyzerV3 PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_dsp
)

target_compile_definitions(SpectrumAnalyzerV3 PUBLIC
    JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING=1
)