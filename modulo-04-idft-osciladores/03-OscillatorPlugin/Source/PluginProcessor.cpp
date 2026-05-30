// ============================================================
// PluginProcessor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 2 · Plugin: Oscillator
// ============================================================
//
// Sintetiza una onda senoidal pura:
//
//   sample = sin(phase) * adsr_env
//   phase += 2*pi*freq / sample_rate
//
// ADSR: ataca rápido, decae, sostiene, y libera cuando nota termina.
// ============================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

// por que no tiene .withInput() ?
OscillatorAudioProcessor::OscillatorAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}

OscillatorAudioProcessor::~OscillatorAudioProcessor() {}

// el DAW te dice cuál es el sample rate real del sistema (48000, 44100, etc.) y el tamaño del buffer.
// En este caso solo guardás el sample rate porque el oscilador lo necesita para calcular cuánto avanza
// la fase por sample. samplesPerBlock se ignora porque el oscilador no necesita saber el tamaño del buffer.
void OscillatorAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    //¿por qué no guardamos el sample rate directo en el constructor?
    sample_rate = sampleRate;
}

void OscillatorAudioProcessor::releaseResources() {}

void OscillatorAudioProcessor::updateADSR()
{
    // ADSR
        if (adsr_state == 1)  // Attack
        {
            adsr_value = (float)adsr_counter / attack_time;
            adsr_counter++;
            if (adsr_counter >= attack_time)
            {
                adsr_state = 2;
                adsr_counter = 0;
            }
        }
        else if (adsr_state == 2)  // Decay
        {
            adsr_value = 1.0f - (1.0f - sustain_level) * ((float)adsr_counter / decay_time);
            adsr_counter++;
            if (adsr_counter >= decay_time)
            {
                adsr_state = 3;
                adsr_counter = 0;
            }
        }
        else if (adsr_state == 3)  // Sustain
        {
            adsr_value = sustain_level;
        }
        else if (adsr_state == 4)  // Release
        {
            adsr_value = sustain_level * (1.0f - (float)adsr_counter / release_time);
            adsr_counter++;
            if (adsr_counter >= release_time)
            {
                adsr_state = 0;
                adsr_value = 0.0f;
            }
        }
        else  // Off
        {
            adsr_value = 0.0f;
        }
}

// Qué relación hay entre acceptsMidi() y el MidiBuffer que recibe processBlock? 
void OscillatorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                            juce::MidiBuffer& midiMessages)
{
    //el buffer puede tener basura del frame anterior
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

    for (int n = 0; n < numSamples; ++n)
    {
        // ADSR — envolvente de amplitud. 
        // Caja negra por ahora, lo vemos en módulos posteriores.
        updateADSR();

        // *** Genera sinusoide ****
        // sin(phase) genera el sample de la sinusoide, 
        // Multiplicarlos es simplemente escalar la amplitud de la onda. 
        // Si adsr_value = 0.5 y sin(phase) = 0.8, el sample que sale es 0.4. La forma de la onda no cambia, solo su tamaño.
        float sample = (float)std::sin(phase) * adsr_value;
        left[n] = sample;
        right[n] = sample;

        // Avanza fase
        phase += 2.0 * 3.14159265 * frequency / sample_rate;
        if (phase > 2.0 * 3.14159265)
            phase -= 2.0 * 3.14159265;
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