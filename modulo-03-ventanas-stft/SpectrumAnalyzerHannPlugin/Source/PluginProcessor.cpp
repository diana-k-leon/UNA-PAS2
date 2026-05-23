// ============================================================
// PluginProcessor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 1 · Plugin: Spectrum Analyzer con ventana Hann
// ============================================================
// Igual que el SpectrumAnalyzer pero aplica ventana Hann
// antes de la FFT. Comparar lado a lado para ver la diferencia:
// sin ventana = leakage por todos lados
// con Hann = espectro limpio
// ============================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

SpectrumAnalyzerAudioProcessor::SpectrumAnalyzerAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    buffer_acumulador.resize(fft_size, 0.0f);
    spectrum.resize(fft_size / 2, 0.0f);
    fft_buffer.resize(2 * fft_size, 0.0f);

    // pre-calcular ventana Hann
    // w[n] = 0.5 * (1 - cos(2π * n / (N-1)))
    // Pre-calcula la ventana Hann una sola vez en el constructor, no en cada processBlock
    window.resize(fft_size);
    for (int n = 0; n < fft_size; ++n)
        window[n] = 0.5f * (1.0f - std::cos(2.0f * 3.14159265f * n / (fft_size - 1)));
}

SpectrumAnalyzerAudioProcessor::~SpectrumAnalyzerAudioProcessor() {}
void SpectrumAnalyzerAudioProcessor::prepareToPlay(double, int) {}
void SpectrumAnalyzerAudioProcessor::releaseResources() {}

void SpectrumAnalyzerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                   juce::MidiBuffer&)
{
    auto* input = buffer.getReadPointer(0);
    int numSamples = buffer.getNumSamples();

    for (int n = 0; n < numSamples; ++n)
    {
        buffer_acumulador[buffer_idx++] = input[n];

        if (buffer_idx >= fft_size)
        {
            // ── aplicar ventana Hann y copiar al buffer FFT ───────────────
            // cada sample se multiplica por el valor correspondiente de la ventana
            // los bordes se atenúan a cero → sin discontinuidad → sin leakage
            for (int i = 0; i < fft_size; ++i)
                fft_buffer[i] = buffer_acumulador[i] * window[i];
            for (int i = fft_size; i < 2 * fft_size; ++i)
                fft_buffer[i] = 0.0f;

            // ── FFT ───────────────────────────────────────────────────────
            fft.performRealOnlyForwardTransform(fft_buffer.data());

            // ── magnitudes ────────────────────────────────────────────────
            for (int k = 0; k < fft_size / 2; ++k)
            {
                float re = fft_buffer[2 * k];
                float im = fft_buffer[2 * k + 1];
                spectrum[k] = std::sqrt(re * re + im * im) / (float)fft_size;
            }

            buffer_idx = 0;
        }
    }
}

juce::AudioProcessorEditor* SpectrumAnalyzerAudioProcessor::createEditor()
{
    return new SpectrumAnalyzerAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpectrumAnalyzerAudioProcessor();
}
