// ============================================================
// PluginEditor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 1 · Plugin: Spectrum Analyzer
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class SpectrumAnalyzerAudioProcessorEditor : public juce::AudioProcessorEditor,
                                              public juce::Timer
{
public:
    SpectrumAnalyzerAudioProcessorEditor(SpectrumAnalyzerAudioProcessor&);
    ~SpectrumAnalyzerAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    SpectrumAnalyzerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumAnalyzerAudioProcessorEditor)
};