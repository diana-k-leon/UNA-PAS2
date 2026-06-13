// ============================================================
// PluginProcessor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: Crossover FIR
// ============================================================
//
// Crossover: divide la señal en dos bandas usando JUCE dsp::IIR
//
//   señal → pasa-bajos → canal izquierdo  (graves)
//   señal → pasa-altos → canal derecho    (agudos)
//
// Variables:
//   crossover_freq  = frecuencia de cruce (Hz)
//   lpf             = filtro pasa-bajos (Linkwitz-Riley 4to orden)
//   hpf             = filtro pasa-altos (Linkwitz-Riley 4to orden)
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

class CrossoverAudioProcessor : public juce::AudioProcessor
{
public:
    CrossoverAudioProcessor();
    ~CrossoverAudioProcessor() override;

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

    float getCrossoverFreq() const { return crossover_freq; }
    void setCrossoverFreq(float freq);

private:
    float crossover_freq = 2000.0f;
    double sample_rate = 48000.0;

    // Dos filtros IIR de segundo orden en cascada = Linkwitz-Riley 4to orden
    using Filter = juce::dsp::IIR::Filter<float>;
    using Coefficients = juce::dsp::IIR::Coefficients<float>;

    // Pasa-bajos: dos biquad en serie
    Filter lpf1, lpf2; // JUCE maneja coeficientes + historia + proceso

    // Pasa-altos: dos biquad en serie
    Filter hpf1, hpf2;
    /* 
    juce::dsp::IIR::Filter que es el objeto filtro completo de JUCE — 
    ya incluye la historia (x1, x2, y1, y2) internamente y 
    tiene su propio process().*/
    void updateFilters();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CrossoverAudioProcessor)
};


