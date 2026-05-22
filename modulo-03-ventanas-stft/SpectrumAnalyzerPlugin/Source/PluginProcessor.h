// ============================================================
// PluginProcessor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 1 · Plugin: Spectrum Analyzer
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <vector>

class SpectrumAnalyzerAudioProcessor : public juce::AudioProcessor
{
public:
    SpectrumAnalyzerAudioProcessor();
    ~SpectrumAnalyzerAudioProcessor() override;

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

    const std::vector<float>& getSpectrum() const { return spectrum; }
    int getFFTSize() const { return fft_size; }

private:
    static constexpr int fft_size = 2048;
    juce::dsp::FFT fft{static_cast<int>(std::log2(fft_size))};
    
    std::vector<float> buffer_acumulador;
    int buffer_idx = 0;

    std::vector<float> spectrum;
    std::vector<float> fft_buffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumAnalyzerAudioProcessor)
};