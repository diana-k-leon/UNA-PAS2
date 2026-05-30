// ============================================================
// PluginProcessor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 2 · Plugin: Pitch Shift
// ============================================================
//
// Pitch shifting por escalado espectral:
//
//   1. FFT del audio de entrada
//   2. Escala los bins: si shift=+2 semitonos, multiplicador ≈ 1.122
//   3. IFFT para reconstruir
//   4. Overlap-add para suavidad
//
// Variables:
//   pitch_shift_semitones = -12 a +12 semitonos
//   fft_size = 2048
//   hop_size = 512
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <vector>

class PitchShiftAudioProcessor : public juce::AudioProcessor
{
public:
    PitchShiftAudioProcessor();
    ~PitchShiftAudioProcessor() override;

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

    float getPitchShift() const { return pitch_shift_semitones; }
    void setPitchShift(float semitones) { pitch_shift_semitones = semitones; }

private:
    static constexpr int fft_size = 2048;
    static constexpr int hop_size = 512;
    
    juce::dsp::FFT fft{static_cast<int>(std::log2(fft_size))};
    
    std::vector<float> buffer_acumulador;
    int buffer_idx = 0;
    std::vector<float> fft_buffer;
    std::vector<float> window;

    float pitch_shift_semitones = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PitchShiftAudioProcessor)
};