// ============================================================
// PluginProcessor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: FIR Filter
// ============================================================
//
// Convolución directa:
//   y[n] = Σ h[k] * x[n-k]  (k=0 a M)
//
// Usa buffer circular para evitar copies.
// ============================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

FIRFilterAudioProcessor::FIRFilterAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    fir_kernel.resize(fir_order, 0.0f);
    fir_buffer.resize(fir_order, 0.0f);
    computeKernel();
}

FIRFilterAudioProcessor::~FIRFilterAudioProcessor() {}

//computeKernel() es la función que calcula coeficientes más sofisticados para un corte más preciso. 
void FIRFilterAudioProcessor::computeKernel()
{
    // Calcula la frecuencia de corte normalizada
    float wc = 2.0f * 3.14159265f * cutoff_freq / (float)sample_rate;
    
    for (int n = 0; n < fir_order; ++n)
    {   
        // El sinc — coeficientes ideales
        float sinc_val;
        if (n == fir_order / 2)
            sinc_val = 1.0f;
        else
            sinc_val = std::sin(wc * (n - fir_order / 2)) / (wc * (n - fir_order / 2));
        
        // El sinc puro tiene bordes abruptos → artefactos. 
        // La solución es multiplicarlo por una ventana Hamming
        float hamming = 0.54f - 0.46f * std::cos(2.0f * 3.14159265f * n / (fir_order - 1));
        
        fir_kernel[n] = sinc_val * hamming;
    }

    // Normaliza
    float sum = 0.0f;
    for (float k : fir_kernel)
        sum += k;
    if (sum != 0.0f)
        for (float& k : fir_kernel)
            k /= sum;
}

void FIRFilterAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    sample_rate = sampleRate;
    computeKernel();
}

void FIRFilterAudioProcessor::releaseResources() {}

void FIRFilterAudioProcessor::setCutoffFreq(float freq)
{
    cutoff_freq = freq;
    computeKernel();
}

void FIRFilterAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                           juce::MidiBuffer& midiMessages)
{
    auto* input = buffer.getReadPointer(0);
    auto* output = buffer.getWritePointer(0);
    int numSamples = buffer.getNumSamples();

    for (int n = 0; n < numSamples; ++n)
    {
        // Agrega nuevo sample al buffer circular
        fir_buffer[fir_idx] = input[n];

        // Convolución: suma ponderada
        float y = 0.0f;
        for (int k = 0; k < fir_order; ++k)
        {
            // da la vuelta cuando llegás al final del array
            int idx = (fir_idx - k + fir_order) % fir_order;
            y += fir_kernel[k] * fir_buffer[idx];
        }

        output[n] = y;

        // Avanza puntero circular
        fir_idx = (fir_idx + 1) % fir_order;
    }

    // Canal derecho igual
    if (buffer.getNumChannels() > 1)
        buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples());
}

juce::AudioProcessorEditor* FIRFilterAudioProcessor::createEditor()
{
    return new FIRFilterAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FIRFilterAudioProcessor();
}