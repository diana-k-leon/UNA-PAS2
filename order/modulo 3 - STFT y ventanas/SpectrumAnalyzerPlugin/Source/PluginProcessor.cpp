// ============================================================
// PluginProcessor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 1 · Plugin: Spectrum Analyzer
// ============================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"
// Incluimos solo los .h — nunca los .cpp
// El .h es la declaración (qué existe), el .cpp es la implementación (cómo funciona)

SpectrumAnalyzerAudioProcessor::SpectrumAnalyzerAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    // Boilerplate de JUCE
    // tiene entrada y salida estéreo.
{
    // Constructor — reservar memoria antes de que llegue cualquier audio
    // Los vectores arrancan en cero — resize(tamaño, valor_inicial)

    buffer_acumulador.resize(fft_size, 0.0f);  // acumula samples del DAW
    spectrum.resize(fft_size / 2 + 1, 0.0f);   // magnitudes hasta Nyquist (primera mitad)
    fft_buffer.resize(2 * fft_size, 0.0f);      // formato intercalado real/imag para JUCE
}

/* Por eso existe la FFT — para que sea rápido suficiente para tiempo real.
Con N=1024 la FFT tarda menos de 1ms. El DAW llama a processBlock 86 veces
por segundo — o sea cada 11ms. El plugin tiene 11ms para hacer todo el trabajo.
Con la DFT serían ~30ms — más que el tiempo disponible. El audio se cortaría.
*/

SpectrumAnalyzerAudioProcessor::~SpectrumAnalyzerAudioProcessor() {}
// Destructor vacío — JUCE libera la memoria automáticamente

void SpectrumAnalyzerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {}
// El DAW avisa a qué sr va a correr y cuántos samples por bloque
// En este plugin no necesitamos hacer nada acá

void SpectrumAnalyzerAudioProcessor::releaseResources() {}
// Se llama cuando el plugin se cierra — liberar recursos si reservaste algo en prepareToPlay

// Se llama ~86 veces por segundo con un buffer de audio del DAW
// Todo lo que hagamos acá se escucha en tiempo real
void SpectrumAnalyzerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                   juce::MidiBuffer& midiMessages)
{
    auto* input = buffer.getReadPointer(0);  // leer canal 0 (izquierdo)
    int numSamples = buffer.getNumSamples(); // cuántos samples hay en este bloque

    // Cada vez que el DAW llama a processBlock llegan numSamples samples —
    // típicamente 512. El acumulador espera hasta tener fft_size (1024).
    for (int n = 0; n < numSamples; ++n)
    {
        // acumular samples de a uno hasta tener fft_size
        buffer_acumulador[buffer_idx++] = input[n];

        // cuando llenamos el acumulador — disparar la FFT
        if (buffer_idx >= fft_size)
        {
            // --------------------------------------------------
            // 1. convertir al formato intercalado que espera JUCE
            //    posición par   → parte real (el sample)
            //    posición impar → parte imaginaria (siempre 0 — señal real)
            for (int i = 0; i < fft_size; ++i)
            {
                fft_buffer[2 * i]     = buffer_acumulador[i]; // real
                fft_buffer[2 * i + 1] = 0.0f;                 // imaginario = 0
            }

            // --------------------------------------------------
            // 2. calcular la FFT — toda la Cooley-Tukey encapsulada en una línea
            fft.performRealOnlyForwardTransform(fft_buffer.data());

            // --------------------------------------------------
            // 3. calcular magnitudes — el módulo de cada bin
            //    después de la FFT fft_buffer sigue intercalado: re, im, re, im...
            //    sqrt(re² + im²) es el mismo std::abs() que vimos en complex
            for (int k = 0; k < fft_size / 2 + 1; ++k)
            {
                float re = fft_buffer[2 * k];
                float im = fft_buffer[2 * k + 1];
                // spectrum[k] = magnitud del bin k — lo que el Editor va a dibujar
                spectrum[k] = std::sqrt(re * re + im * im);
                
            }

            buffer_idx = 0; // resetear — empezar a acumular de nuevo
        }
    }
}

juce::AudioProcessorEditor* SpectrumAnalyzerAudioProcessor::createEditor()
{
    // crear y devolver la ventana del plugin
    // *this pasa una referencia al Processor — el Editor la necesita para leer spectrum[]
    return new SpectrumAnalyzerAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    // punto de entrada del plugin — JUCE llama esta función para crear el plugin
    // siempre es igual, no se toca
    return new SpectrumAnalyzerAudioProcessor();
}