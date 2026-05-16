# ============================================================
# CMakeLists.txt
# Programación Aplicada al Sonido II — UNA
# Módulo 1 · Plugin: Gain RMS
# ============================================================
#
# Define el proyecto JUCE y cómo compilarlo como plugin VST3.
#
# Qué hace este plugin:
# - Aplica un gain fijo (0.5) a todos los canales del buffer
# - Calcula el RMS del canal 0 y lo muestra en la ventana
#
# Antes de compilar verificá que la ruta de JUCE sea correcta:
#   add_subdirectory(/tu/ruta/a/JUCE ...)
#
# Compilar:  cmake -B build
#            cmake --build build
# El .vst3 queda en: build/GainRMSPlugin_artefacts/VST3/
# Copiarlo a: ~/Library/Audio/Plug-Ins/VST3/
# ============================================================

# Versión mínima de CMake y nombre del proyecto.
cmake_minimum_required(VERSION 3.22)
project(GainRMSPlugin VERSION 0.1)

# ajustá esta ruta a donde tengas JUCE clonado
add_subdirectory(/Users/derivasur/Desktop/dev/JUCE ./JUCE)

# Define el plugin: formato VST3, nombre que aparece en Reaper, 
# y dos códigos de 4 caracteres que identifican el plugin de forma única. 
# Si algún día hacés otro plugin, cambiás Grm1 por otra cosa.
juce_add_plugin(GainRMSPlugin
    FORMATS VST3
    PLUGIN_NAME "Gain RMS Plugin"
    PLUGIN_MANUFACTURER_CODE Mfr1
    PLUGIN_CODE Grm1
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    VST3_CATEGORIES "Fx"
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST3_CAN_REPLACE_VST2 FALSE
)

juce_generate_juce_header(GainRMSPlugin)

# Los archivos .cpp que forman el plugin. 
# Los .h no van acá — CMake los encuentra solo.
target_sources(GainRMSPlugin PRIVATE
    PluginProcessor.cpp
    PluginEditor.cpp
)

# Los módulos de JUCE que usamos. 
# audio_processors es el núcleo del plugin, 
# audio_utils tiene el editor, 
# dsp lo vamos a necesitar para FFT más adelante.
target_link_libraries(GainRMSPlugin PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_dsp
)

target_compile_definitions(GainRMSPlugin PUBLIC
    JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING=1
)
