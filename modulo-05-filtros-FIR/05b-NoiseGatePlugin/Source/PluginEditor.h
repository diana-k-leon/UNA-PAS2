// ============================================================
// PluginEditor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: Noise Gate
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class NoiseGateAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    NoiseGateAudioProcessorEditor(NoiseGateAudioProcessor&);
    ~NoiseGateAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent&) override;
    void mouseDrag(const juce::MouseEvent&) override;

private:
    NoiseGateAudioProcessor& audioProcessor;
    float threshold = 0.01f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoiseGateAudioProcessorEditor)
};
