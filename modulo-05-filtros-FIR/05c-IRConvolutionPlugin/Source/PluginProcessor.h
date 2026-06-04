// ============================================================
// PluginProcessor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: IR Convolution
// ============================================================
//
// Convolución con IR real usando juce::dsp::Convolution
//
// El usuario arrastra un archivo WAV de IR al plugin.
// La señal de entrada se convolucioná con esa IR en tiempo real.
//
// Variables:
//   convolution  = el motor de convolución de JUCE
//   ir_loaded    = si hay una IR cargada
//   ir_name      = nombre del archivo cargado (para la UI)
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

class IRConvolutionAudioProcessor : public juce::AudioProcessor
{
public:
    IRConvolutionAudioProcessor();
    ~IRConvolutionAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock&) override {}
    void setStateInformation(const void*, int) override {}

    // Carga una IR desde un archivo
    void loadIR(const juce::File& file);

    bool isIRLoaded() const { return ir_loaded; }
    juce::String getIRName() const { return ir_name; }

private:
    // El motor de convolución de JUCE. JUCE lo hace todo 
    // carga la IR, calcula la FFT, hace el overlap-add
    juce::dsp::Convolution convolution;

    // para saber si hay una IR cargada 
    bool ir_loaded = false;

    // El nombre del archivo cargado para mostrarlo en la UI
    juce::String ir_name = "Arrastrá un archivo WAV de IR acá";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRConvolutionAudioProcessor)
};
