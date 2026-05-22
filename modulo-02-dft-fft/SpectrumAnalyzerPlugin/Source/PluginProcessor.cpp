// ============================================================
// PluginProcessor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 1 · Plugin: Spectrum Analyzer
// ============================================================
// Implementación del procesador de audio.
// Todo el DSP vive en processBlock():
//   1. Acumula samples del DAW hasta tener fft_size (2048)
//   2. Convierte al formato intercalado de JUCE
//   3. Calcula FFT con juce::dsp::FFT
//   4. Calcula magnitudes: sqrt(re² + im²)
//   5. Guarda en spectrum[] para que el Editor las dibuje
//
// Este plugin NO modifica el audio — solo lo analiza.
// ============================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"
// incluimos solo los .h — nunca los .cpp
// el compilador junta todo automáticamente

// ─── CONSTRUCTOR ──────────────────────────────────────────────────────────────
SpectrumAnalyzerAudioProcessor::SpectrumAnalyzerAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    // le dice a JUCE: este plugin tiene entrada estéreo y salida estéreo
{
    // reservar memoria ANTES de que llegue audio
    // si reservás memoria durante processBlock, el audio se corta

    buffer_acumulador.resize(fft_size, 0.0f);
    // 2048 posiciones, todas en 0 — acá van los samples del DAW

    spectrum.resize(fft_size / 2 + 1, 0.0f);
    // 1025 posiciones (2048/2 + 1) — solo hasta Nyquist
    // la segunda mitad del espectro es espejo, no la necesitamos

    fft_buffer.resize(2 * fft_size, 0.0f);
    // 4096 posiciones — formato intercalado: [re, im, re, im, ...]
    // JUCE necesita este formato, no usa complex<float>
}

SpectrumAnalyzerAudioProcessor::~SpectrumAnalyzerAudioProcessor() {}
// destructor vacío — los vectores se liberan solos

void SpectrumAnalyzerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {}
// el DAW avisa a qué sample rate va a correr — acá no necesitamos hacer nada

void SpectrumAnalyzerAudioProcessor::releaseResources() {}
// se llama al cerrar el plugin — nada que liberar

// ─── PROCESS BLOCK — EL CORAZÓN DEL PLUGIN ───────────────────────────────────
// Reaper llama a esta función ~86 veces por segundo
// Cada vez manda un buffer con ~512 samples de audio
void SpectrumAnalyzerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                   juce::MidiBuffer& midiMessages)
{
    auto* input = buffer.getReadPointer(0);
    // puntero al canal 0 (izquierdo) — es un float[] con los samples

    int numSamples = buffer.getNumSamples();
    // cuántos samples mandó el DAW en este bloque (~512)

    for (int n = 0; n < numSamples; ++n)
    {
        buffer_acumulador[buffer_idx++] = input[n];
        // guardar cada sample en el acumulador
        // buffer_idx avanza de a uno

        if (buffer_idx >= fft_size)
        // cuando el acumulador está lleno (2048 samples) → calcular FFT
        {
            // ── PASO 1: convertir al formato intercalado de JUCE ──────────
            // nuestra FFT usaba complex<float> — JUCE usa un array plano
            // donde las posiciones pares son la parte real
            // y las impares son la parte imaginaria (0 porque es señal real)
            for (int i = 0; i < fft_size; ++i)
            {
                fft_buffer[2 * i]     = buffer_acumulador[i]; // parte real = el sample
                fft_buffer[2 * i + 1] = 0.0f;                 // parte imaginaria = 0
            }

            // ── PASO 2: FFT ───────────────────────────────────────────────
            // una línea — toda la Cooley-Tukey optimizada con SIMD adentro
            // el resultado queda en fft_buffer, mismo formato intercalado
            fft.performRealOnlyForwardTransform(fft_buffer.data());

            // ── PASO 3: calcular magnitudes ───────────────────────────────
            // después de la FFT, fft_buffer tiene: [re0, im0, re1, im1, ...]
            // magnitud de cada bin = sqrt(re² + im²)
            // solo calculamos la primera mitad (hasta Nyquist)
            for (int k = 0; k < fft_size / 2 + 1; ++k)
            {
                float re = fft_buffer[2 * k];      // parte real del bin k
                float im = fft_buffer[2 * k + 1];  // parte imaginaria del bin k
                spectrum[k] = std::sqrt(re * re + im * im);
                // spectrum[k] = magnitud del bin k
                // el Editor lee este vector para dibujar las barras
            }

            buffer_idx = 0;
            // resetear — empezar a acumular de nuevo
            // próxima FFT cuando se llenen otros 2048 samples
        }
    }
    // NOTA: este plugin no modifica el audio — solo lo analiza
    // el buffer sale igual que entró, Reaper sigue reproduciéndolo normal
}

// ─── CREAR EDITOR ─────────────────────────────────────────────────────────────
juce::AudioProcessorEditor* SpectrumAnalyzerAudioProcessor::createEditor()
{
    return new SpectrumAnalyzerAudioProcessorEditor(*this);
    // *this = le pasa una referencia al Processor
    // el Editor la guarda para poder leer spectrum[]
}

// ─── PUNTO DE ENTRADA ─────────────────────────────────────────────────────────
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpectrumAnalyzerAudioProcessor();
    // JUCE llama esta función para crear el plugin — siempre es igual
}
