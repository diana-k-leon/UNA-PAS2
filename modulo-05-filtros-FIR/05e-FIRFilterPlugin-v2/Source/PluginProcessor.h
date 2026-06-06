// ============================================================
// PluginProcessor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: FIR Filter v2 (orden variable)
// ============================================================
//
// Filtro FIR con dos parámetros controlables:
//   cutoff_freq  = frecuencia de corte (Hz)
//   fir_order    = orden del filtro (4 a 256)
//
// Kernel: sinc(n) * ventana Hamming
// Buffer circular para tiempo real
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <vector>

class FIRFilterV2AudioProcessor : public juce::AudioProcessor
{
public:
    FIRFilterV2AudioProcessor();
    ~FIRFilterV2AudioProcessor() override;

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

    float getCutoffFreq() const { return cutoff_freq; }
    int   getFIROrder()   const { return fir_order; }

    void setCutoffFreq(float freq);
    void setFIROrder(int order);

private:
    float  cutoff_freq = 5000.0f;
    int    fir_order   = 64;
    double sample_rate = 48000.0;

    std::vector<float> fir_kernel;
    std::vector<float> fir_buffer;
    int fir_idx = 0;

    void computeKernel();
    void resizeBuffers();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FIRFilterV2AudioProcessor)
};
