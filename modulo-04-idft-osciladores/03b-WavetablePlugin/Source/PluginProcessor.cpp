// ============================================================
// PluginProcessor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 2 · Plugin: Wavetable Oscillator
// ============================================================
//
// Sintetiza una onda senoidal usando wavetable:
//
//   Pre-calcula un ciclo de sinusoide en prepareToPlay
//   En processBlock recorre la tabla según la frecuencia MIDI
//
// ADSR: caja negra por ahora.
// ============================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

OscillatorAudioProcessor::OscillatorAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}

OscillatorAudioProcessor::~OscillatorAudioProcessor() {}

void OscillatorAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    sample_rate = sampleRate;

    // Llenar la tabla con un ciclo completo de sinusoide
    // sin() se llama 1024 veces acá y nunca más en el loop de audio
    wavetable.resize(wavetable_size);
    for (int i = 0; i < wavetable_size; i++)
        wavetable[i] = std::sin(2.0f * 3.14159265f * i / wavetable_size);
}

void OscillatorAudioProcessor::releaseResources() {}

void OscillatorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                            juce::MidiBuffer& midiMessages)
{
    // el buffer puede tener basura del frame anterior
    buffer.clear();

    // Procesa MIDI
    for (const auto& metadata : midiMessages)
    {
        const auto msg = metadata.getMessage();
        if (msg.isNoteOn())
        {
            frequency = juce::MidiMessage::getMidiNoteInHertz(msg.getNoteNumber());
            note_on = true;
            adsr_state = 1;  // Attack
            adsr_counter = 0;
        }
        else if (msg.isNoteOff())
        {
            note_on = false;
            adsr_state = 4;  // Release
            adsr_counter = 0;
        }
    }

    auto* left = buffer.getWritePointer(0);
    auto* right = buffer.getWritePointer(1);
    int numSamples = buffer.getNumSamples();

    // increment = cuánto avanza el índice por sample
    // misma fórmula que los ejercicios de C++: freq * tableSize / sampleRate
    float increment = (float)(frequency * wavetable_size / sample_rate);

    for (int n = 0; n < numSamples; ++n)
    {
        // ADSR — envolvente de amplitud.
        // Caja negra por ahora, lo vemos en módulos posteriores.
        updateADSR();

        // Lee la tabla en la posición actual
        int idx = (int)wt_phase % wavetable_size;
        float sample = wavetable[idx] * adsr_value;

        left[n] = sample;
        right[n] = sample;

        // Avanza la fase — cuando llega al final de la tabla vuelve al principio
        wt_phase += increment;
        if (wt_phase >= wavetable_size)
            wt_phase -= wavetable_size;
    }
}

void OscillatorAudioProcessor::updateADSR()
{
    if (adsr_state == 1)  // Attack
    {
        adsr_value = (float)adsr_counter / attack_time;
        adsr_counter++;
        if (adsr_counter >= attack_time) { adsr_state = 2; adsr_counter = 0; }
    }
    else if (adsr_state == 2)  // Decay
    {
        adsr_value = 1.0f - (1.0f - sustain_level) * ((float)adsr_counter / decay_time);
        adsr_counter++;
        if (adsr_counter >= decay_time) { adsr_state = 3; adsr_counter = 0; }
    }
    else if (adsr_state == 3)  // Sustain
    {
        adsr_value = sustain_level;
    }
    else if (adsr_state == 4)  // Release
    {
        adsr_value = sustain_level * (1.0f - (float)adsr_counter / release_time);
        adsr_counter++;
        if (adsr_counter >= release_time) { adsr_state = 0; adsr_value = 0.0f; }
    }
    else
    {
        adsr_value = 0.0f;
    }
}

juce::AudioProcessorEditor* OscillatorAudioProcessor::createEditor()
{
    return new OscillatorAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OscillatorAudioProcessor();
}