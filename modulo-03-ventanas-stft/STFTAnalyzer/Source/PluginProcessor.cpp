// ============================================================
// PluginProcessor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 1 · Plugin: STFT Analyzer
// ============================================================
//
// STFT = Short-Time Fourier Transform
//
// Proceso:
//   1. Acumula hop_size (512) samples en buffer_acumulador
//   2. Aplica ventana Hann al buffer completo (fft_size)
//   3. Realiza FFT
//   4. Calcula magnitudes
//   5. Guarda en spectrum_history (deque con max 1024 frames)
//   6. Resetea buffer para próximo hop
//
// Ventana Hann reduce leakage espectral.
// ============================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

STFTAnalyzerAudioProcessor::STFTAnalyzerAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    buffer_acumulador.resize(fft_size, 0.0f);
    // buffer de trabajo que le pasás a JUCE para calcular la FFT
    fft_buffer.resize(2 * fft_size, 0.0f);

    // Pre-computa ventana Hann: w[n] = 0.5 * (1 - cos(2*pi*n / (N-1)))
    window.resize(fft_size);
    for (int n = 0; n < fft_size; ++n)
    {
        float val = 0.5f * (1.0f - std::cos(2.0f * 3.14159265f * n / (fft_size - 1)));
        window[n] = val;
    }
    
    // Pre-aloca historial
    for (int i = 0; i < 512; ++i)
        spectrum_history.push_back(std::vector<float>(fft_size / 2 + 1, 0.0f));
}

STFTAnalyzerAudioProcessor::~STFTAnalyzerAudioProcessor() {}

void STFTAnalyzerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {}

void STFTAnalyzerAudioProcessor::releaseResources() {}

void STFTAnalyzerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                               juce::MidiBuffer& midiMessages)
{
    // Leer el audio del DAW
    auto* input = buffer.getReadPointer(0);
    int numSamples = buffer.getNumSamples();

    for (int n = 0; n < numSamples; ++n)
    {
        buffer_acumulador[buffer_idx++] = input[n];

        // Cuando tenemos fft_size samples, hacemos FFT
        if (buffer_idx >= fft_size)
        {
            for (int i = 0; i < fft_size; ++i)
                fft_buffer[i] = buffer_acumulador[i] * window[i];
            for (int i = fft_size; i < 2 * fft_size; ++i)
                fft_buffer[i] = 0.0f;

            // FFT
            fft.performRealOnlyForwardTransform(fft_buffer.data());

            // Magnitudes
            std::vector<float> spectrum(fft_size / 2 + 1);
            for (int k = 0; k < fft_size / 2 + 1; ++k)
            {
                float re = fft_buffer[2 * k];
                float im = fft_buffer[2 * k + 1];
                spectrum[k] = std::sqrt(re * re + im * i m);
            }

            // Agrega al historial — sin lock, simplificado
            if (spectrum_history.size() >= 512)
                spectrum_history.pop_front();
            spectrum_history.push_back(spectrum);

            /*
            Acá está la diferencia con el SpectrumAnalyzer.
            En vez de guardar un solo vector que se sobreescribe, 
            guardamos una cola de 512 vectores. Si ya hay 512, 
            sacamos el más viejo (pop_front = sacar del frente). 
            Agregamos el nuevo al final (push_back). 
            El Editor lee esta cola y dibuja cada vector como una línea horizontal del
            espectrograma.
            */

            // Resetea buffer
            buffer_idx = 0;
        }
    }
}

juce::AudioProcessorEditor* STFTAnalyzerAudioProcessor::createEditor()
{
    return new STFTAnalyzerAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new STFTAnalyzerAudioProcessor();
}