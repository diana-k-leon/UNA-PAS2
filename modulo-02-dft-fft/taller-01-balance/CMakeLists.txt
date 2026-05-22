# ============================================================
# CMakeLists.txt
# Programación Aplicada al Sonido II — UNA
# Taller 01 · Balance L/R
# ============================================================
#
# Compilar:
#   mkdir -p build && cd build && cmake .. && make
#
# Copiar (Mac):
#   cp -r build/Taller01Balance_artefacts/VST3/Taller01Balance.vst3 \
#         ~/Library/Audio/Plug-Ins/VST3/
# ============================================================

cmake_minimum_required(VERSION 3.22)
project(Taller01Balance VERSION 0.1)

# ajustá esta ruta a donde tengas JUCE clonado
add_subdirectory(/Users/derivasur/Desktop/dev/JUCE ./JUCE)

juce_add_plugin(Taller01Balance
    FORMATS VST3
    PLUGIN_NAME "Taller 01 · Balance"
    PLUGIN_MANUFACTURER_CODE Mfr1
    PLUGIN_CODE Tb01
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    VST3_CATEGORIES "Fx"
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_CAN_REPLACE_VST2 FALSE
)

juce_generate_juce_header(Taller01Balance)

target_sources(Taller01Balance PRIVATE
    PluginProcessor.cpp
    PluginEditor.cpp
)

target_link_libraries(Taller01Balance PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_dsp
)

target_compile_definitions(Taller01Balance PUBLIC
    JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING=1
)