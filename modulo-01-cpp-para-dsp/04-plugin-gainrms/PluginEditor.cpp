// ============================================================
// PluginEditor.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 1 · Plugin: Gain RMS
// ============================================================
//
// Implementa la ventana del plugin.
//
// startTimerHz(30) arranca un timer que llama timerCallback() 30 veces
// por segundo. Cada llamada dispara repaint() que llama a paint().
//
// paint() dibuja el valor RMS en pantalla:
//   - fondo negro
//   - texto blanco centrado con el valor de rmsValue (4 decimales)
//
// Nota: repaint() no va adentro de paint() — eso causaría un loop infinito.
// El timer es la forma correcta de actualizar la pantalla periódicamente.
// ============================================================

#include "PluginEditor.h"

TemplatePluginAudioProcessorEditor::TemplatePluginAudioProcessorEditor(
    TemplatePluginAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(400, 300);
    startTimerHz(30); // redibujar 30 veces por segundo
}

TemplatePluginAudioProcessorEditor::~TemplatePluginAudioProcessorEditor()
{
    stopTimer();
}

void TemplatePluginAudioProcessorEditor::timerCallback()
{
    repaint();
    // El timer llega acá 30 veces por segundo y dice "volvé a dibujar". Eso dispara paint().
    // Por qué no ponemos repaint() directo adentro de paint() — 
    // causaría un loop infinito. paint() llama repaint() que llama paint() que llama repaint()... 
    // El timer corta ese ciclo.
}

void TemplatePluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawFittedText(
        "RMS: " + juce::String(audioProcessor.rmsValue, 4),
        getLocalBounds(),
        juce::Justification::centred, 1
    );
}

void TemplatePluginAudioProcessorEditor::resized() {}