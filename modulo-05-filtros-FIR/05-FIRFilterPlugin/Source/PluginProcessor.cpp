// ============================================================
// PluginProcessor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: FIR Filter
// ============================================================
//
// Convolución directa:
//   y[n] = Σ h[k] * x[n-k]  (k=0 a M)
//
// Usa buffer circular para evitar copies.
// ============================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

FIRFilterAudioProcessor::FIRFilterAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    fir_kernel.resize(fir_order, 0.0f);
    fir_buffer.resize(fir_order, 0.0f);
    computeKernel(); 
}

FIRFilterAudioProcessor::~FIRFilterAudioProcessor() {}

//computeKernel() = H(k) , es la función que calcula coeficientes más sofisticados para un corte más preciso. 
/* 
Calcula la frecuencia de corte normalizada
    float wc = 2.0f * PI * cutoff_freq / sample_rate;
    Convierte la frecuencia de corte de Hz a radianes por muestra. 
    Es el mismo wc que vieron en la teoría de filtros.	
Calcula el sinc para cada coeficiente
	sinc_val = sin(wc * (n - M/2)) / (wc * (n - M/2))
    La función sinc es la respuesta al impulso ideal de un pasa-bajos perfecto. 
    El caso especial n == fir_order/2 es porque en ese punto el denominador sería cero — por L'Hôpital vale 1.
Multiplica por la ventana Hamming
	float hamming = 0.54f - 0.46f * cos(2*PI*n / (M-1));
    Esto ya lo conocen de las ventanas del módulo anterior. Suaviza los bordes del sinc para reducir el ripple.
    Normaliza
for (float& k : fir_kernel) k /= sum;
    Divide todos los coeficientes por su suma para que la ganancia total sea 1 — sin esto el volumen saldría distorsionado.
*/
void FIRFilterAudioProcessor::computeKernel()
{
    #ifndef M_PI
        constexpr float PI = 3.14159265358979323846f;
    #else
        constexpr float PI = (float)M_PI;
    #endif

    // Calcula la frecuencia de corte normalizada
    float wc = 2.0f * PI * cutoff_freq / (float)sample_rate;
    
    for (int n = 0; n < fir_order; ++n)
    {   
        // Calcula el sinc para cada coeficiente
        float sinc_val;
        if (n == fir_order / 2)
            sinc_val = 1.0f;
        else
            sinc_val = std::sin(wc * (n - fir_order / 2)) / (wc * (n - fir_order / 2));
        
        // El sinc puro tiene bordes abruptos → artefactos. 
        // Multiplica por la ventana Hamming
        float hamming = 0.54f - 0.46f * std::cos(2.0f * 3.14159265f * n / (fir_order - 1));
        
        fir_kernel[n] = sinc_val * hamming;
    }

    // Normaliza
    float sum = 0.0f;
    for (float k : fir_kernel)
        sum += k;
    if (sum != 0.0f)
        for (float& k : fir_kernel)
            k /= sum;
}
/* 
juce::dsp::FilterDesign::designFIRLowpassWindowMethod() — 
le pasás la frecuencia de corte, el orden y el tipo de ventana, 
y te devuelve los coeficientes listos.
O más simple todavía, usar directamente 
juce::dsp::FIR::Filter con juce::dsp::FIR::Coefficients que hace todo junto.
*/

void FIRFilterAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    sample_rate = sampleRate;
    computeKernel();
}

void FIRFilterAudioProcessor::releaseResources() {}
// ¿Por qué se llama a computeKernel() cada vez que cambia el cutoff?
void FIRFilterAudioProcessor::setCutoffFreq(float freq)
{
    cutoff_freq = freq;
    computeKernel();
}

void FIRFilterAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                           juce::MidiBuffer& midiMessages)
{
    auto* input = buffer.getReadPointer(0);
    auto* output = buffer.getWritePointer(0);
    int numSamples = buffer.getNumSamples();

    for (int n = 0; n < numSamples; ++n)
    {
        // Agrega nuevo sample al buffer circular
        fir_buffer[fir_idx] = input[n];

        // Convolución: suma ponderada
        float y = 0.0f;
        for (int k = 0; k < fir_order; ++k)
        {
            // dar la vuelta del buffer circular
            int idx = (fir_idx - k + fir_order) % fir_order;
            y += fir_kernel[k] * fir_buffer[idx];
        }

        output[n] = y;

        // Avanza puntero circular
        fir_idx = (fir_idx + 1) % fir_order;
    }

    // Canal derecho igual
    if (buffer.getNumChannels() > 1)
        buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples());
}

juce::AudioProcessorEditor* FIRFilterAudioProcessor::createEditor()
{
    return new FIRFilterAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FIRFilterAudioProcessor();
}