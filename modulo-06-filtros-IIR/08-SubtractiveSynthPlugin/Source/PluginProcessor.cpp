// ============================================================
// PluginProcessor.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 6 · Plugin: Subtractive Synth
// ============================================================
//
// La cadena:
//   ruido blanco → pasa-bajos IIR → salida
//
// El LFO modula el cutoff del filtro en tiempo real:
//   cutoff = cutoff_base + lfo_depth * sin(2π * lfo_phase)
//
// Con lfo_freq lento (0.1-1 Hz) → viento, respiración
// Con lfo_freq rápido (5-20 Hz) → trémolo, vibrato de timbre
// ============================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

#ifndef M_PI
    constexpr float PI = 3.14159265358979323846f;
#else
    constexpr float PI = (float)M_PI;
#endif

SubtractiveSynthAudioProcessor::SubtractiveSynthAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input",   juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    , rng(std::random_device{}())
{
}

SubtractiveSynthAudioProcessor::~SubtractiveSynthAudioProcessor() {}

void SubtractiveSynthAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    sample_rate = sampleRate;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate       = sampleRate;
    spec.maximumBlockSize = (juce::uint32) samplesPerBlock;
    spec.numChannels      = 1;

    lpf.prepare(spec);
    updateFilter(cutoff_base);
}

void SubtractiveSynthAudioProcessor::releaseResources()
{
    lpf.reset();
}

void SubtractiveSynthAudioProcessor::updateFilter(float cutoff)
{
    // Clampear el cutoff a rango audible seguro
    cutoff = juce::jlimit(20.0f, (float)(sample_rate * 0.45), cutoff);
    *lpf.coefficients = *Coefficients::makeLowPass(sample_rate, cutoff);
}

void SubtractiveSynthAudioProcessor::setCutoffBase(float hz)
{
    cutoff_base = juce::jlimit(100.0f, 8000.0f, hz);
}

void SubtractiveSynthAudioProcessor::setLFOFreq(float hz)
{
    lfo_freq = juce::jlimit(0.05f, 20.0f, hz);
}

void SubtractiveSynthAudioProcessor::setLFODepth(float hz)
{
    lfo_depth = juce::jlimit(0.0f, 4000.0f, hz);
}

void SubtractiveSynthAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                   juce::MidiBuffer&)
{
    int numSamples  = buffer.getNumSamples();
    auto* output    = buffer.getWritePointer(0);

    for (int n = 0; n < numSamples; ++n)
    {
        // 1. Generar ruido blanco
        float noise = noise_dist(rng);

        // 2. Calcular cutoff con LFO
        float lfo_value = std::sin(2.0f * PI * lfo_phase);
        float cutoff    = cutoff_base + lfo_depth * lfo_value;

        // 3. Actualizar filtro con el cutoff del momento
        updateFilter(cutoff);

        // 4. Aplicar filtro al ruido
        output[n] = lpf.processSample(noise);

        // 5. Avanzar fase del LFO
        lfo_phase += lfo_freq / (float)sample_rate;
        if (lfo_phase >= 1.0f) lfo_phase -= 1.0f;
    }

    // Canal derecho igual
    if (buffer.getNumChannels() > 1)
        buffer.copyFrom(1, 0, buffer, 0, 0, numSamples);
}

juce::AudioProcessorEditor* SubtractiveSynthAudioProcessor::createEditor()
{
    return new SubtractiveSynthAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SubtractiveSynthAudioProcessor();
}
