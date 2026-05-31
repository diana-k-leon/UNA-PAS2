// ============================================================
// PluginProcessor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 2 · Plugin: Wavetable 
// ============================================================
//
// Sintetizador mínimo: oscilador IIR 2° orden + ADSR.
//
// Variables:
//   phase        = fase del oscilador (0 a 2pi)
//   frequency    = frecuencia en Hz (recibe por MIDI)
//   adsr_env     = envolvente (Attack, Decay, Sustain, Release)
//   note_on      = si hay nota MIDI activa
//
// MIDI: recibe Note On/Off, transforma a frecuencia, genera tono.
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

class OscillatorAudioProcessor : public juce::AudioProcessor
{
public:
    OscillatorAudioProcessor();
    ~OscillatorAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int index) override {}
    const juce::String getProgramName(int index) override { return {}; }
    void changeProgramName(int index, const juce::String& newName) override {}
    
    void getStateInformation(juce::MemoryBlock& destData) override {}
    void setStateInformation(const void* data, int sizeInBytes) override {}

    void updateADSR();

private:
    double sample_rate = 48000.0;
    double phase = 0.0;
    double frequency = 440.0;
    bool note_on = false;

    // ADSR
    float adsr_value = 0.0f;
    int adsr_state = 0;  // 0=Off, 1=Attack, 2=Decay, 3=Sustain, 4=Release
    int adsr_counter = 0;
    
    // ADSR times (samples)
    int attack_time = 4800;    // 0.1s @ 48kHz
    int decay_time = 9600;     // 0.2s
    float sustain_level = 0.7f;
    int release_time = 19200;  // 0.4s

    static constexpr int wavetable_size = 1024;
    std::vector<float> wavetable;
    float wt_phase = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscillatorAudioProcessor)
};