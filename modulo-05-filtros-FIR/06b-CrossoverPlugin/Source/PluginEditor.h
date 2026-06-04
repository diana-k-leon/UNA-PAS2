// ============================================================
// PluginEditor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: Crossover FIR
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class CrossoverAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    CrossoverAudioProcessorEditor(CrossoverAudioProcessor&);
    ~CrossoverAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent&) override;
    void mouseDrag(const juce::MouseEvent&) override;

private:
    CrossoverAudioProcessor& audioProcessor;
    float crossover_freq = 2000.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CrossoverAudioProcessorEditor)
};
