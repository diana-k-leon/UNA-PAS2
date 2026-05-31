// ============================================================
// PluginEditor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 2 · Plugin: Oscillator
// ============================================================

#include "PluginEditor.h"

OscillatorAudioProcessorEditor::OscillatorAudioProcessorEditor(
    OscillatorAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(400, 300);
}

OscillatorAudioProcessorEditor::~OscillatorAudioProcessorEditor() {}

void OscillatorAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Oscillator", getLocalBounds(), juce::Justification::centred, true);
    
    g.setFont(14.0f);
    g.drawText("Usa MIDI para tocar notas", getLocalBounds().withTop(100), juce::Justification::centred, true);
}

void OscillatorAudioProcessorEditor::resized() {}