// ============================================================
// PluginProcessor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: IR Convolution
// ============================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"

// no hay resize ni computeKernel en el constructor — 
// porque los coeficientes no se calculan al arrancar?
IRConvolutionAudioProcessor::IRConvolutionAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input",   juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}

IRConvolutionAudioProcessor::~IRConvolutionAudioProcessor() {}

void IRConvolutionAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Preparar el motor de convolución
    // le dice al módulo DSP cómo va a trabajar antes de arrancar
    juce::dsp::ProcessSpec spec;
    spec.sampleRate       = sampleRate;
    // cuántas muestras llegan por bloque en processBlock
    spec.maximumBlockSize = (juce::uint32) samplesPerBlock;
    spec.numChannels      = 2;

    // Le pasa esa información al motor de convolución para que esté listo cuando llegue audio.
    convolution.prepare(spec);
}

void IRConvolutionAudioProcessor::releaseResources()
{
    convolution.reset();
}
/* 
No hay que "sacar" los coeficientes — el archivo de audio ya los contiene. 
Cuando hacés loadImpulseResponse(file), JUCE lee las muestras del WAV y las usa como coeficientes. 
Nada más.
Lo que sí existe en producción profesional es un proceso más sofisticado usando un sweep sinusoidal 
— en vez de un globo mandás un sine sweep que recorre todas las frecuencias, grabás la respuesta, 
y después deconvolucionás para obtener una IR más limpia y precisa. 
Pero para el uso básico del plugin, 
cualquier grabación de un sonido impulsivo en el espacio ya funciona como IR.

*/
void IRConvolutionAudioProcessor::loadIR(const juce::File& file)
{
    // Cargar IR desde archivo WAV
    // JUCE maneja internamente la FFT y el overlap-add
    convolution.loadImpulseResponse(
        file,
        juce::dsp::Convolution::Stereo::yes,
        juce::dsp::Convolution::Trim::yes, // recortar el silencio al principio y al final del archivo
        0  // tamaño 0 = usar la IR completa sin truncar
    );

    ir_loaded = true; //activa el flag para que processBlock empiece a procesar
    ir_name   = file.getFileNameWithoutExtension(); // guarda el nombre del archivo sin extensión para mostrarlo en la UI.
}
/*
En el plugin FIR que hicimos antes, computeKernel() sí aplicaba la ventana Hamming a mano 
porque estabamos calculando los coeficientes sinc ellos mismos. 
Pero cuando usás los módulos de alto nivel de JUCE como dsp::Convolution o dsp::IIR, 
todo eso ya está adentro — vos solo llamás a prepare, load y process.
*/
void IRConvolutionAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                               juce::MidiBuffer&)
{
    // Si no hay IR cargada, pasar la señal sin cambios
    if (!ir_loaded)
        return;

    // envuelve el buffer
    juce::dsp::AudioBlock<float> block(buffer);
    // le dice al motor que procese en el mismo buffer
    juce::dsp::ProcessContextReplacing<float> context(block);
    // ¿Qué diferencia hay entre esto y el processBlock del FIR?
    convolution.process(context);
}

juce::AudioProcessorEditor* IRConvolutionAudioProcessor::createEditor()
{
    return new IRConvolutionAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new IRConvolutionAudioProcessor();
}
