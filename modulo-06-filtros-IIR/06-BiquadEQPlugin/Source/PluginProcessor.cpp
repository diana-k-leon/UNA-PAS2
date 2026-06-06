// ============================================================
// PluginProcessor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: Biquad EQ
// ============================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

BiquadEQAudioProcessor::BiquadEQAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    computeCoefficients(0);
    computeCoefficients(1);
}

BiquadEQAudioProcessor::~BiquadEQAudioProcessor() {}

void BiquadEQAudioProcessor::computeCoefficients(int idx)
{
    #ifndef M_PI
        constexpr float PI = 3.14159265358979323846f;
    #else
        constexpr float PI = (float)M_PI;
    #endif

    float w0 = 2.0f * PI * freq[idx] / (float)sample_rate;
    float sin_w0 = std::sin(w0);
    float cos_w0 = std::cos(w0);
    
    float Q_safe = std::max(0.5f, Q[idx]);
    float A = std::pow(10.0f, gain_db[idx] / 40.0f);
    float alpha = sin_w0 / (2.0f * Q_safe);

    // Coeficientes numerador (peaking)
    b0[idx] = 1.0f + alpha * A;
    b1[idx] = -2.0f * cos_w0;
    b2[idx] = 1.0f - alpha * A;
    
    // Coeficientes denominador
    float a0 = 1.0f + alpha / A;
    float a1_temp = -2.0f * cos_w0;
    float a2_temp = 1.0f - alpha / A;

    // Normaliza
    if (a0 != 0.0f)
    {
        b0[idx] /= a0;
        b1[idx] /= a0;
        b2[idx] /= a0;
        a1[idx] = a1_temp / a0;
        a2[idx] = a2_temp / a0;
    }
    else
    {
        b0[idx] = 1.0f;
        b1[idx] = 0.0f;
        b2[idx] = 0.0f;
        a1[idx] = 0.0f;
        a2[idx] = 0.0f;
    }

    // Clampea a valores seguros
    b0[idx] = std::max(-10.0f, std::min(10.0f, b0[idx]));
    b1[idx] = std::max(-10.0f, std::min(10.0f, b1[idx]));
    b2[idx] = std::max(-10.0f, std::min(10.0f, b2[idx]));
    a1[idx] = std::max(-10.0f, std::min(10.0f, a1[idx]));
    a2[idx] = std::max(-10.0f, std::min(10.0f, a2[idx]));
}

void BiquadEQAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    sample_rate = sampleRate;
    computeCoefficients(0);
    computeCoefficients(1);
}

void BiquadEQAudioProcessor::releaseResources() {}

void BiquadEQAudioProcessor::setBiquad(int idx, float f, float g, float q)
{
    freq[idx] = std::max(20.0f, std::min(20000.0f, f));
    gain_db[idx] = std::max(-3.0f, std::min(3.0f, g));  // Limitamos a ±3 dB
    Q[idx] = std::max(0.5f, q);
    computeCoefficients(idx);
}

void BiquadEQAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                          juce::MidiBuffer& midiMessages)
{
    auto* data = buffer.getWritePointer(0);
    int numSamples = buffer.getNumSamples();

    for (int n = 0; n < numSamples; ++n)
    {
        // Biquad 1
        float y = b0[0] * data[n] + b1[0] * x1[0] + b2[0] * x2[0] - 
                  a1[0] * y1[0] - a2[0] * y2[0];
        
        // Evita blow-up
        if (std::isnan(y) || std::isinf(y)) y = 0.0f;
        
        x2[0] = x1[0];
        x1[0] = data[n];
        y2[0] = y1[0];
        y1[0] = y;

        // Biquad 2
        float y2_out = b0[1] * y + b1[1] * x1[1] + b2[1] * x2[1] - a1[1] * y1[1] - a2[1] * y2[1];
        
        if (std::isnan(y2_out) || std::isinf(y2_out)) y2_out = 0.0f;
        
        x2[1] = x1[1];
        x1[1] = y;
        y2[1] = y1[1];
        y1[1] = y2_out;

        // Hard limit para evitar blow-up
        data[n] = std::max(-1.0f, std::min(1.0f, y2_out));
    }

    // Canal derecho
    if (buffer.getNumChannels() > 1)
        buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples());
}

juce::AudioProcessorEditor* BiquadEQAudioProcessor::createEditor()
{
    return new BiquadEQAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BiquadEQAudioProcessor();
}