// ============================================================
// PluginEditor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: FIR Filter
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class FIRFilterAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    FIRFilterAudioProcessorEditor(FIRFilterAudioProcessor&);
    ~FIRFilterAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;

private:
    FIRFilterAudioProcessor& audioProcessor;
    float cutoff_freq = 5000.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FIRFilterAudioProcessorEditor)
};