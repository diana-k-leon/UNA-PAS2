// ============================================================
// PluginProcessor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 1 · Plugin: Spectrum Analyzer
// ============================================================
// Declaraciones del procesador de audio.
// El Processor es el cerebro del plugin — recibe audio del DAW,
// calcula la FFT, y guarda las magnitudes en spectrum[].
// El Editor lee spectrum[] para dibujar las barras.
// ============================================================

// que este archivo se incluya una sola vez
#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>  // acá vive juce::dsp::FFT
#include <vector>

class SpectrumAnalyzerAudioProcessor : public juce::AudioProcessor
{
public:
    // --- funciones que JUCE obliga a implementar ---
    SpectrumAnalyzerAudioProcessor();
    ~SpectrumAnalyzerAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    // processBlock es donde vive el DSP — se llama ~86 veces por segundo

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    // --- boilerplate que JUCE necesita pero no tocamos ---
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

    // --- lo que el Editor necesita leer ---
    const std::vector<float>& getSpectrum() const { return spectrum; }
    // devuelve una referencia al vector de magnitudes — el Editor lo lee para dibujar
    int getFFTSize() const { return fft_size; }

private:
    static constexpr int fft_size = 2048;
    // tamaño de la FFT — potencia de 2
    // 2048 samples a 44100 Hz = ~46ms de audio por análisis
    // resolución: 44100/2048 = ~21.5 Hz por bin

    juce::dsp::FFT fft{static_cast<int>(std::log2(fft_size))};
    // la FFT de JUCE se inicializa con el "orden" = log₂(N)
    // log₂(2048) = 11, así que internamente es: juce::dsp::FFT fft(11)
    
    std::vector<float> buffer_acumulador;
    // acumula samples que llegan del DAW hasta tener fft_size
    int buffer_idx = 0;
    // índice: cuántos samples llevamos acumulados

    std::vector<float> spectrum;
    // las magnitudes calculadas — esto es lo que lee el Editor para dibujar

    std::vector<float> fft_buffer;
    // buffer en formato intercalado (real, imag, real, imag...) que JUCE espera
    // tamaño: 2 * fft_size (cada sample ocupa 2 posiciones)

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumAnalyzerAudioProcessor)
};
