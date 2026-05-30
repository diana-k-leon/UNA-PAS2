// ============================================================
// PluginProcessor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 2 · Plugin: Pitch Shift (SIMPLIFICADO)
// ============================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

PitchShiftAudioProcessor::PitchShiftAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    buffer_acumulador.resize(fft_size, 0.0f);
    fft_buffer.resize(2 * fft_size, 0.0f);

    window.resize(fft_size);
    for (int n = 0; n < fft_size; ++n)
        window[n] = 0.5f * (1.0f - std::cos(2.0f * 3.14159265f * n / (fft_size - 1)));
}

PitchShiftAudioProcessor::~PitchShiftAudioProcessor() {}

void PitchShiftAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {}

void PitchShiftAudioProcessor::releaseResources() {}

void PitchShiftAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                            juce::MidiBuffer& midiMessages)
{
    auto* input = buffer.getReadPointer(0);   // canal izquierdo de entrada (solo lectura)
    auto* output = buffer.getWritePointer(0); // canal izquierdo de salida (escritura)
    int numSamples = buffer.getNumSamples();

    // Factor de pitch: +12 semitonos = ×2 (octava arriba), -12 = ×0.5 (octava abajo)
    float pitch_mult = std::pow(2.0f, pitch_shift_semitones / 12.0f);

    for (int n = 0; n < numSamples; ++n)
    {
        // Acumulamos samples hasta llenar el buffer de fft_size (2048)
        buffer_acumulador[buffer_idx++] = input[n];

        if (buffer_idx >= fft_size)
        {
            // ─── PASO 1: aplicar ventana Hann y preparar buffer para FFT ───
            // La ventana suaviza los bordes del bloque para evitar artefactos espectrales
            for (int i = 0; i < fft_size; ++i)
            {
                fft_buffer[2 * i]     = buffer_acumulador[i] * window[i]; // parte real
                fft_buffer[2 * i + 1] = 0.0f;                             // parte imaginaria = 0
            }

            // ─── PASO 2: FFT forward ─────────────────────────────────────
            // Convierte la señal del dominio del tiempo al dominio de la frecuencia
            fft.performRealOnlyForwardTransform(fft_buffer.data());

            // ─── PASO 3: escalar bins (el pitch shift propiamente dicho) ──
            // Cada bin k se mueve a la posición k * pitch_mult
            // Si pitch_mult > 1: los bins se mueven hacia arriba → pitch sube
            // Si pitch_mult < 1: los bins se mueven hacia abajo → pitch baja
            std::vector<float> temp_buffer(2 * fft_size, 0.0f);
            for (int k = 1; k < fft_size / 2; ++k)
            {
                int new_k = (int)(k * pitch_mult);
                
                if (new_k < fft_size / 2)
                {
                    temp_buffer[2 * new_k]     = fft_buffer[2 * k];     // parte real
                    temp_buffer[2 * new_k + 1] = fft_buffer[2 * k + 1]; // parte imaginaria
                }
                // si new_k >= fft_size/2 el bin se descarta (fuera del rango)
            }
            
            // copiar el espectro modificado de vuelta al fft_buffer
            for (int i = 0; i < 2 * fft_size; ++i)
                fft_buffer[i] = temp_buffer[i];

            // ─── PASO 4: IFFT ─────────────────────────────────────────────
            // Reconstruye la señal en el dominio del tiempo con el pitch modificado
            fft.performRealOnlyInverseTransform(fft_buffer.data());

            // ─── PASO 5: aplicar ventana y normalizar ─────────────────────
            // norm compensa la escala que introduce la FFT/IFFT
            float norm = 1.0f / fft_size;
            for (int i = 0; i < fft_size; ++i)
                output[i] = fft_buffer[2 * i] * norm * window[i];

            // reiniciar el índice para el próximo bloque
            buffer_idx = 0;
        }
    }

    // copiar canal izquierdo al derecho (mono → estéreo)
    if (buffer.getNumChannels() > 1)
        buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples());
}

juce::AudioProcessorEditor* PitchShiftAudioProcessor::createEditor()
{
    return new PitchShiftAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PitchShiftAudioProcessor();
}