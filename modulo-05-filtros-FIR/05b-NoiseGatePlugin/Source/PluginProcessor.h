// ============================================================
// PluginProcessor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: Noise Gate
// ============================================================
//
// Noise gate simple:
//   si abs(sample) < threshold → silencio
//   si no → pasa sin cambios
//
// Variables:
//   threshold    = umbral de amplitud (0.0 a 1.0)
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

class NoiseGateAudioProcessor : public juce::AudioProcessor
{
public:
    NoiseGateAudioProcessor();
    ~NoiseGateAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock&) override {}
    void setStateInformation(const void*, int) override {}

    float getThreshold() const { return threshold; }
    void setThreshold(float t) { threshold = t; }
    
    void updateCoefficients();

private:
    float threshold = 0.01f;
    
    float attack_ms  = 10.0f;
    float release_ms = 100.0f;
    double sample_rate = 48000.0;
    float gain = 0.0f;
    float attack_coeff  = 0.0f;
    float release_coeff = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoiseGateAudioProcessor)
};
