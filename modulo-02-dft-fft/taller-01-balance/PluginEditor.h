// ============================================================
// PluginEditor.h
// Programación Aplicada al Sonido II — UNA
// Módulo 1 · Plugin: Gain RMS
// ============================================================
//
// Declara la ventana del plugin (lo que ve el usuario en Reaper).
//
// Hereda de Timer para redibujar la pantalla a 30fps.
// timerCallback() llama a repaint() que dispara paint().
//
// El Editor tiene una referencia al Processor para leer rmsValue.
// El audio NUNCA pasa por el Editor — solo los datos para mostrar.
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class TemplatePluginAudioProcessorEditor : public juce::AudioProcessorEditor,
                                           public juce::Timer
{
public:
    TemplatePluginAudioProcessorEditor(TemplatePluginAudioProcessor&);
    ~TemplatePluginAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    TemplatePluginAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TemplatePluginAudioProcessorEditor)
};