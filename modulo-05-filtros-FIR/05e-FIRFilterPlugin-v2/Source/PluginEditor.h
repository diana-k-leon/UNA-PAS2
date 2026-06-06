// ============================================================
// PluginEditor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: FIR Filter v2 (orden variable)
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class FIRFilterV2AudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    FIRFilterV2AudioProcessorEditor(FIRFilterV2AudioProcessor&);
    ~FIRFilterV2AudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent&) override;
    void mouseDrag(const juce::MouseEvent&) override;

private:
    FIRFilterV2AudioProcessor& audioProcessor;
    float cutoff_freq = 5000.0f;
    int   fir_order   = 64;
    int   active_slider = -1;  // 0 = cutoff, 1 = order

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FIRFilterV2AudioProcessorEditor)
};
