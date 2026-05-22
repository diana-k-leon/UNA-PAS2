// ============================================================
// PluginProcessor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 1 · Plugin: STFT Analyzer
// ============================================================
//
// FFT con ventana + overlap + espectrograma.
//
// Variables:
//   fft_size         = 1024 (en vez de 2048, más resolución temporal)
//   hop_size         = 512 (50% overlap)
//   window           = Hann window
//   buffer_acumulador = acumula hop_size samples
//   spectrum_history = historial de espectros para espectrograma
//   spectrum_idx     = índice en el historial (scrolling)
//
// El espectrograma es una matriz:
//   - Filas: bins de frecuencia (0 a 512)
//   - Columnas: tiempo (1024 frames históricos)
//   - Color: magnitud (dB)
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <vector>
#include <deque>

class STFTAnalyzerAudioProcessor : public juce::AudioProcessor
{
public:
    STFTAnalyzerAudioProcessor();
    ~STFTAnalyzerAudioProcessor() override;

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

    // El Editor lee esto
    const std::deque<std::vector<float>>& getSpectrogramHistory() const { return spectrum_history; }
    int getFFTSize() const { return fft_size; }

private:
    static constexpr int fft_size = 1024;
    static constexpr int hop_size = 512;  // 50% overlap
    
    juce::dsp::FFT fft{static_cast<int>(std::log2(fft_size))};
    
    std::vector<float> window;              // Ventana Hann
    std::vector<float> buffer_acumulador;
    int buffer_idx = 0;

    std::vector<float> fft_buffer;          // Para FFT
    std::deque<std::vector<float>> spectrum_history;  // Historial (max 1024 frames)

    void applyHannWindow(std::vector<float>& buffer);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(STFTAnalyzerAudioProcessor)
};