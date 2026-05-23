// ============================================================
// PluginProcessor.h — MiTemplatePlugin
// Programación Aplicada al Sonido II — UNA
// ============================================================
// El Processor es el CEREBRO del plugin.
// Recibe audio del DAW, lo procesa, y guarda los resultados.
// El Editor lee esos resultados para dibujar en pantalla.
//
// Este archivo es la DECLARACIÓN: dice qué funciones y
// variables existen, pero no cómo funcionan.
// La implementación está en PluginProcessor.cpp.
// ============================================================

#pragma once  // que este archivo se incluya una sola vez

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>  // acá vive juce::dsp::FFT
#include <vector>

class MiTemplatePluginAudioProcessor : public juce::AudioProcessor
{
public:
    MiTemplatePluginAudioProcessor();
    ~MiTemplatePluginAudioProcessor() override;

    // --- las 3 funciones del ciclo de vida ---
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    // processBlock = el corazón. El DAW la llama ~86 veces por segundo.

    // --- crear la ventana del plugin ---
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    // --- boilerplate (JUCE lo necesita, no lo tocamos) ---
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

    // --- lo que el Editor necesita leer ---
    const std::vector<float>& getSpectrum() const { return spectrum; }
    int getFFTSize() const { return fft_size; }

private:
    // --- parámetros de la FFT ---
    // potencia de 2. A 44100 Hz → resolución de ~21.5 Hz por bin
    // Es una constante de la clase — se define una vez y no cambia nunca. 
    // static constexpr significa que el valor se conoce en tiempo de compilación. 
    // Como es miembro de la clase, cualquier método del Processor puede usarla 
    // directamente: fft_size sin más.
    static constexpr int fft_size = 2048;
   
    
    
    juce::dsp::FFT fft { static_cast<int>(std::log2(fft_size)) };
    // la FFT de JUCE. Se inicializa con log₂(2048) = 11

    // --- buffers ---
    std::vector<float> buffer_acumulador;  // acumula samples del DAW
    int buffer_idx = 0;                     // cuántos samples llevamos

    std::vector<float> spectrum;            // magnitudes — el Editor lee esto
    std::vector<float> fft_buffer;          // buffer para JUCE (2 * fft_size)

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MiTemplatePluginAudioProcessor)
};
