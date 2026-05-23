// ============================================================
// PluginEditor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 1 · Plugin: Spectrum Analyzer
// ============================================================
// Declaraciones de la ventana del plugin (lo que ve el usuario en Reaper).
//
// Hereda de Timer para redibujar la pantalla a 30fps.
// timerCallback() llama a repaint() que dispara paint().
//
// El Editor tiene una referencia al Processor para leer spectrum[].
// El audio NUNCA pasa por el Editor — solo los datos para mostrar.
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class SpectrumAnalyzerAudioProcessorEditor : public juce::AudioProcessorEditor,
                                              public juce::Timer
// hereda de dos clases:
// AudioProcessorEditor = es una ventana de plugin
// Timer = puede hacer cosas periódicamente (redibujar a 30fps)
{
public:
    SpectrumAnalyzerAudioProcessorEditor(SpectrumAnalyzerAudioProcessor&);
    // recibe referencia al Processor para leer spectrum[]
    ~SpectrumAnalyzerAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;    // dibuja todo — se llama cada vez que hay repaint
    void resized() override;                  // se llama cuando cambia el tamaño de la ventana
    void timerCallback() override;            // se llama 30 veces por segundo

private:
    SpectrumAnalyzerAudioProcessor& audioProcessor;
    // referencia al Processor — para leer spectrum[]

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumAnalyzerAudioProcessorEditor)
};
