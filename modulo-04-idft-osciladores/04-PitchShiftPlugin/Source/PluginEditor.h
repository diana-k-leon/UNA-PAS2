// ============================================================
// PluginEditor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 2 · Plugin: Pitch Shift
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class PitchShiftAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    PitchShiftAudioProcessorEditor(PitchShiftAudioProcessor&);
    ~PitchShiftAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;

private:
    PitchShiftAudioProcessor& audioProcessor;
    float pitch_shift = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PitchShiftAudioProcessorEditor)
};