// ============================================================
// PluginProcessor.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 1 · Plugin: Gain RMS
// ============================================================
//
// Implementa el procesador de audio del plugin.
// Todo el DSP vive en processBlock() — el resto es boilerplate de JUCE.
//
// processBlock() hace dos cosas:
//   1. Gain — multiplica cada sample por 0.5 (mitad de volumen)
//              mismo loop que 02-gain.cpp pero sobre el buffer del DAW
//   2. RMS  — calcula la raíz cuadrada de la media de los cuadrados
//              misma fórmula que 03-rms.cpp, guarda el resultado en rmsValue
//
// El Editor lee rmsValue y lo muestra en pantalla ~30 veces por segundo.
// ============================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"

TemplatePluginAudioProcessor::TemplatePluginAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}


// La firma de la función. TemplatePluginAudioProcessor:: — pertenece a tu clase. 
// buffer es el audio que mandó Reaper. 
// midiMessages existe pero no lo usamos — este plugin no hace MIDI.
void TemplatePluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                juce::MidiBuffer& midiMessages)
{
    // Iteramos los canales — izquierdo y derecho. Hay que aplicar el gain a cada uno por separado.
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {   
        // auto* es un puntero — un float[] que apunta directamente al array de samples del canal ch
        //Write porque vamos a modificarlo
        auto* data = buffer.getWritePointer(ch);
        for (int n = 0; n < buffer.getNumSamples(); ++n)
            data[n] *= gain;
    }

    // calcular RMS sobre canal 0
    auto* data = buffer.getReadPointer(0);
    float sum = 0.0f;
    for (int n = 0; n < buffer.getNumSamples(); ++n)
        sum += data[n] * data[n];
    // Suma de cuadrados, dividida por N, raíz cuadrada. El resultado se guarda en rmsValue para que el Editor lo muestre
    rmsValue = std::sqrt(sum / buffer.getNumSamples());
    // Exactamente 03-rms.cpp. 
   
}

TemplatePluginAudioProcessor::~TemplatePluginAudioProcessor() {}

const juce::String TemplatePluginAudioProcessor::getName() const { return JucePlugin_Name; }
bool TemplatePluginAudioProcessor::acceptsMidi() const { return false; }
bool TemplatePluginAudioProcessor::producesMidi() const { return false; }
double TemplatePluginAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int TemplatePluginAudioProcessor::getNumPrograms() { return 1; }
int TemplatePluginAudioProcessor::getCurrentProgram() { return 0; }
void TemplatePluginAudioProcessor::setCurrentProgram(int) {}
const juce::String TemplatePluginAudioProcessor::getProgramName(int) { return {}; }
void TemplatePluginAudioProcessor::changeProgramName(int, const juce::String&) {}

void TemplatePluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {}

void TemplatePluginAudioProcessor::releaseResources() {}



void TemplatePluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {}
void TemplatePluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {}

bool TemplatePluginAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* TemplatePluginAudioProcessor::createEditor()
{
    return new TemplatePluginAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TemplatePluginAudioProcessor();
}