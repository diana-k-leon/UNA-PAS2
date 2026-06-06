// ============================================================
// PluginProcessor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: Biquad EQ
// ============================================================
//
// Dos secciones biquad independientes (peaking EQ).
//
// Ecuación de diferencias:
//   y[n] = b0*x[n] + b1*x[n-1] + b2*x[n-2] - a1*y[n-1] - a2*y[n-2]
//
// Variables por biquad:
//   freq, gain (dB), Q (ancho de banda)
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

class BiquadEQAudioProcessor : public juce::AudioProcessor
{
public:
    BiquadEQAudioProcessor();
    ~BiquadEQAudioProcessor() override;

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
    void setCurrentProgram(int index) override {}
    const juce::String getProgramName(int index) override { return {}; }
    void changeProgramName(int index, const juce::String& newName) override {}

    void getStateInformation(juce::MemoryBlock& destData) override {}
    void setStateInformation(const void* data, int sizeInBytes) override {}

    void setBiquad(int idx, float freq, float gain_db, float Q);
    float getFreq(int idx) const { return freq[idx]; }
    float getGain(int idx) const { return gain_db[idx]; }

private:
    double sample_rate = 48000.0;

    // Dos biquads independientes
    float freq[2] = {500.0f, 5000.0f};
    float gain_db[2] = {0.0f, 0.0f};
    float Q[2] = {1.0f, 1.0f};

    // Coeficientes
    float b0[2], b1[2], b2[2], a1[2], a2[2];

    // Estados (x pasado, y pasado)
    float x1[2] = {0.0f, 0.0f}, x2[2] = {0.0f, 0.0f};
    float y1[2] = {0.0f, 0.0f}, y2[2] = {0.0f, 0.0f};

    void computeCoefficients(int idx);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BiquadEQAudioProcessor)
};