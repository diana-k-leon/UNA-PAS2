// ============================================================
// PluginEditor.h
// Programación Aplicada al Sonido II — UNA
// Módulo 6 · Plugin: Subtractive Synth
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class SubtractiveSynthAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    SubtractiveSynthAudioProcessorEditor(SubtractiveSynthAudioProcessor&);
    ~SubtractiveSynthAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent&) override;
    void mouseDrag(const juce::MouseEvent&) override;

private:
    SubtractiveSynthAudioProcessor& audioProcessor;

    float cutoff_base = 1000.0f;
    float lfo_freq    = 0.5f;
    float lfo_depth   = 500.0f;
    int   active_slider = -1;

    void drawSlider(juce::Graphics& g, float x, float y, float w, float h,
                    float value, float min, float max,
                    const juce::String& label, const juce::String& unit,
                    juce::Colour color, bool log_scale = false);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SubtractiveSynthAudioProcessorEditor)
};
