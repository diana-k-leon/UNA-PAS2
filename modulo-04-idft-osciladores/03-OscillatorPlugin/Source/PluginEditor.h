// ============================================================
// PluginEditor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 2 · Plugin: Oscillator
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class OscillatorAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    OscillatorAudioProcessorEditor(OscillatorAudioProcessor&);
    ~OscillatorAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    OscillatorAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscillatorAudioProcessorEditor)
};