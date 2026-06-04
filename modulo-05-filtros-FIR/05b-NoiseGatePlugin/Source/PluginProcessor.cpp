// ============================================================
// PluginProcessor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: Noise Gate
// ============================================================
//
// El gate más simple posible:
//   if (abs(sample) < threshold) → silencio
//
// Para producción real se agregarían attack y release
// para suavizar la transición y evitar clicks.
// ============================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

NoiseGateAudioProcessor::NoiseGateAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input",   juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}

NoiseGateAudioProcessor::~NoiseGateAudioProcessor() {}

// convertir tiempo en humano → velocidad por sample
void NoiseGateAudioProcessor::updateCoefficients()
{
    // fórmula estándar de DSP para envelopes
    // cuánto se mueve gain por cada sample
    attack_coeff  = 1.0f - std::exp(-1.0f / (sample_rate * attack_ms  * 0.001f));
    release_coeff = 1.0f - std::exp(-1.0f / (sample_rate * release_ms * 0.001f));
}

void NoiseGateAudioProcessor::prepareToPlay(double sampleRate, int) {
    sample_rate = sampleRate;
    // arranca con el gate cerrado
    gain = 0.0f;
    updateCoefficients();
}
void NoiseGateAudioProcessor::releaseResources() {}

void NoiseGateAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                           juce::MidiBuffer&)
{

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* audio = buffer.getWritePointer(ch);
        int numSamples = buffer.getNumSamples();

        for (int n = 0; n < numSamples; ++n)
        {
            
            bool gate_open = std::abs(audio[n]) >= threshold;

            if (gate_open)
                // la distancia que le falta a gain para llegar a 1.0
                gain += attack_coeff  * (1.0f - gain);
            else
                // Lo mismo pero hacia 0.0 — baja suavemente
                gain += release_coeff * (0.0f - gain);

            audio[n] *= gain;
        }
    }
}

juce::AudioProcessorEditor* NoiseGateAudioProcessor::createEditor()
{
    return new NoiseGateAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NoiseGateAudioProcessor();
}
