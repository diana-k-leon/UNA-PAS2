// ============================================================
// PluginProcessor.h
// Programación Aplicada al Sonido II — UNA
// Módulo 1 · Plugin: Gain RMS
// La declaración del procesador
// ============================================================
//
// Declara el procesador de audio del plugin.
// El procesador es el núcleo del plugin — acá vive el DSP.
//
// Variables miembro:
//   rmsValue  → float público, el Editor lo lee para mostrar en pantalla
//   gain      → float privado, el factor de ganancia aplicado al audio
//
// Funciones clave:
//   prepareToPlay() → Reaper avisa la frecuencia y tamaño de buffer
//   processBlock()  → se llama ~86 veces por segundo con audio real
// ============================================================

// que este archivo se incluya una sola vez aunque lo llamen desde varios
#pragma once
// trae todo lo de JUCE que necesita el procesador
#include <juce_audio_processors/juce_audio_processors.h>


// Hereda de hereda de juce::AudioProcessor. 
// Eso significa que JUCE ya tiene definido cómo hablar con el DAW — vos solo completás los métodos que te interesan.
class TemplatePluginAudioProcessor : public juce::AudioProcessor
{
public:
    TemplatePluginAudioProcessor();
    ~TemplatePluginAudioProcessor() override;

    // Las tres funciones del ciclo de vida
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    // Dos variables miembro. rmsValue es pública porque el Editor la necesita leer. 
    // el Editor lee esto para mostrar el RMS en pantalla
    float rmsValue = 0.0f;

private:
    // gain es privada porque nadie de afuera la toca — por ahora es fija en 0.5.
    float gain = 0.5f;

    // Un macro de JUCE que hace dos cosas: prohíbe copiar el objeto (un plugin no debería copiarse) 
    // y agrega un detector de memory leaks en modo debug. No lo tocás nunca.
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TemplatePluginAudioProcessor)
};