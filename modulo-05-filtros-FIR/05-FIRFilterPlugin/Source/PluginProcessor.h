// ============================================================
// PluginProcessor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: FIR Filter
// ============================================================
//
// Filtro FIR pasa-bajos por convolución directa.
//
// FIR = Finite Impulse Response
// Kernel: h[n] = sinc(n) * ventana (Hamming)
//
// Variables:
//   cutoff_freq  = frecuencia de corte (Hz)
//   fir_kernel   = coeficientes del filtro
//   fir_buffer   = buffer circular para estados pasados
//   fir_idx      = índice en buffer circular
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <vector>

class FIRFilterAudioProcessor : public juce::AudioProcessor
{
public:
    FIRFilterAudioProcessor();
    ~FIRFilterAudioProcessor() override;

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

    float getCutoffFreq() const { return cutoff_freq; }
    void setCutoffFreq(float freq);

private:
    // el orden del filtro (M)
    // static constexpr > no cambia nunca en tiempo de ejecución.
    static constexpr int fir_order = 128;
    
    //se controla con el slider
    float cutoff_freq = 5000.0f;
    double sample_rate = 48000.0;
    
    std::vector<float> fir_kernel; // h[k] 
    std::vector<float> fir_buffer; // historial
    int fir_idx = 0; // índice del buffer circular

    void computeKernel();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FIRFilterAudioProcessor)
};