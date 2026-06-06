// ============================================================
// PluginProcessor.h
// Programación Aplicada al Sonido II — UNA
// Módulo 6 · Plugin: Subtractive Synth
// ============================================================
//
// Síntesis sustractiva básica:
//   fuente: ruido blanco
//   filtro: biquad pasa-bajos IIR (JUCE dsp::IIR)
//   modulación: LFO seno que mueve el cutoff
//
// Variables:
//   cutoff_base  = frecuencia de corte base (Hz)
//   lfo_freq     = velocidad del LFO (Hz)
//   lfo_depth    = profundidad del LFO (Hz)
//   lfo_phase    = fase actual del LFO (0.0 a 1.0)
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <random>

class SubtractiveSynthAudioProcessor : public juce::AudioProcessor
{
public:
    SubtractiveSynthAudioProcessor();
    ~SubtractiveSynthAudioProcessor() override;

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

    float getCutoffBase() const { return cutoff_base; }
    float getLFOFreq()    const { return lfo_freq; }
    float getLFODepth()   const { return lfo_depth; }

    void setCutoffBase(float hz);
    void setLFOFreq(float hz);
    void setLFODepth(float hz);

private:
    double sample_rate = 48000.0;

    // Parámetros
    float cutoff_base = 1000.0f;  // Hz
    float lfo_freq    = 0.5f;     // Hz — velocidad del LFO
    float lfo_depth   = 500.0f;   // Hz — cuánto varía el cutoff
    float lfo_phase   = 0.0f;     // 0.0 a 1.0

    // Filtro pasa-bajos IIR de JUCE
    using Filter = juce::dsp::IIR::Filter<float>;
    using Coefficients = juce::dsp::IIR::Coefficients<float>;
    Filter lpf;

    // Generador de ruido blanco
    std::mt19937 rng;
    std::uniform_real_distribution<float> noise_dist{ -1.0f, 1.0f };

    void updateFilter(float cutoff);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SubtractiveSynthAudioProcessor)
};
