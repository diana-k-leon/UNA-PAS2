// ============================================================
// PluginEditor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 1 · Plugin: STFT Analyzer
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class STFTAnalyzerAudioProcessorEditor : public juce::AudioProcessorEditor,
                                         public juce::Timer
{
public:
    STFTAnalyzerAudioProcessorEditor(STFTAnalyzerAudioProcessor&);
    ~STFTAnalyzerAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    STFTAnalyzerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(STFTAnalyzerAudioProcessorEditor)
};