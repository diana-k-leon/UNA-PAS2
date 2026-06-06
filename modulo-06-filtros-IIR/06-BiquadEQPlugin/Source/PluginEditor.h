// ============================================================
// PluginEditor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: Biquad EQ
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class BiquadEQAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    BiquadEQAudioProcessorEditor(BiquadEQAudioProcessor&);
    ~BiquadEQAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;

private:
    BiquadEQAudioProcessor& audioProcessor;
    float gain1 = 0.0f, gain2 = 0.0f;
    int active_slider = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BiquadEQAudioProcessorEditor)
};