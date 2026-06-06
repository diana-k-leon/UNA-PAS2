// ============================================================
// PluginProcessor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: FIR Filter v2 (orden variable)
// ============================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

#ifndef M_PI
    constexpr float PI = 3.14159265358979323846f;
#else
    constexpr float PI = (float)M_PI;
#endif

FIRFilterV2AudioProcessor::FIRFilterV2AudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input",   juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    resizeBuffers();
    computeKernel();
}

FIRFilterV2AudioProcessor::~FIRFilterV2AudioProcessor() {}

void FIRFilterV2AudioProcessor::resizeBuffers()
{
    fir_kernel.assign(fir_order, 0.0f);
    fir_buffer.assign(fir_order, 0.0f);
    fir_idx = 0;
}

void FIRFilterV2AudioProcessor::computeKernel()
{
    float wc = 2.0f * PI * cutoff_freq / (float)sample_rate;

    for (int n = 0; n < fir_order; ++n)
    {
        float sinc_val;
        int centro = fir_order / 2;
        if (n == centro)
            sinc_val = 1.0f;
        else
            sinc_val = std::sin(wc * (n - centro)) / (wc * (n - centro));

        float hamming = 0.54f - 0.46f * std::cos(2.0f * PI * n / (fir_order - 1));
        fir_kernel[n] = sinc_val * hamming;
    }

    // Normalizar
    float sum = 0.0f;
    for (float k : fir_kernel) sum += k;
    if (sum != 0.0f)
        for (float& k : fir_kernel) k /= sum;
}

void FIRFilterV2AudioProcessor::prepareToPlay(double sampleRate, int)
{
    sample_rate = sampleRate;
    resizeBuffers();
    computeKernel();
}

void FIRFilterV2AudioProcessor::releaseResources() {}

void FIRFilterV2AudioProcessor::setCutoffFreq(float freq)
{
    cutoff_freq = freq;
    computeKernel();
}

void FIRFilterV2AudioProcessor::setFIROrder(int order)
{
    // Orden siempre par para que el centro esté bien definido
    fir_order = (order % 2 == 0) ? order : order + 1;
    fir_order = juce::jlimit(4, 256, fir_order);
    resizeBuffers();
    computeKernel();
}

void FIRFilterV2AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer&)
{
    auto* input  = buffer.getReadPointer(0);
    auto* output = buffer.getWritePointer(0);
    int numSamples = buffer.getNumSamples();

    for (int n = 0; n < numSamples; ++n)
    {
        // Push al buffer circular
        fir_buffer[fir_idx] = input[n];

        // Convolución
        float y = 0.0f;
        for (int k = 0; k < fir_order; ++k)
        {
            int idx = (fir_idx - k + fir_order) % fir_order;
            y += fir_kernel[k] * fir_buffer[idx];
        }

        output[n] = y;

        // Avanzar puntero circular
        fir_idx = (fir_idx + 1) % fir_order;
    }

    // Canal derecho igual
    if (buffer.getNumChannels() > 1)
        buffer.copyFrom(1, 0, buffer, 0, 0, numSamples);
}

juce::AudioProcessorEditor* FIRFilterV2AudioProcessor::createEditor()
{
    return new FIRFilterV2AudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FIRFilterV2AudioProcessor();
}
