// ============================================================
// PluginProcessor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 1 · Plugin: Spectrum Analyzer
// ============================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"

SpectrumAnalyzerAudioProcessor::SpectrumAnalyzerAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    buffer_acumulador.resize(fft_size, 0.0f);
    spectrum.resize(fft_size / 2, 0.0f);
    fft_buffer.resize(2 * fft_size, 0.0f);
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

        //¿ya tenemos 2048?
        if (buffer_idx >= fft_size)
        {
            // ── copiar la señal al buffer de JUCE ──────────────────────────────
            for (int i = 0; i < fft_size; ++i)
                fft_buffer[i] = buffer_acumulador[i];
            for (int i = fft_size; i < 2 * fft_size; ++i)
                fft_buffer[i] = 0.0f;

            // ── Calcular la FFT ───────────────────────────────────────────────────────
            fft.performRealOnlyForwardTransform(fft_buffer.data());
            // Una línea. Toda la Cooley-Tukey optimizada adentro. 
            // Después de esto, fft_buffer tiene los resultados intercalados: 
            // [real₀, imag₀, real₁, imag₁, ...].

            // ── extraer magnitudes ────────────────────────────────────────────────
            for (int k = 0; k < fft_size / 2; ++k)
            {
                float re = fft_buffer[2 * k];
                float im = fft_buffer[2 * k + 1];
                spectrum[k] = std::sqrt(re * re + im * im) / (float)fft_size;
            }
            //resetear
            buffer_idx = 0;
        }
    }
}
/*
El processBlock recibe ~512 samples del DAW. 
Los acumula de a uno hasta tener 2048. 
Cuando llega a 2048: copia al buffer de JUCE, 
calcula FFT en una línea, 
extrae magnitudes con sqrt(re² + im²), 
guarda en spectrum[], y resetea. 
El Editor lee spectrum[] 30 veces por segundo y dibuja barras.

*/

juce::AudioProcessorEditor* SpectrumAnalyzerAudioProcessor::createEditor()
{
    return new SpectrumAnalyzerAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpectrumAnalyzerAudioProcessor();
}