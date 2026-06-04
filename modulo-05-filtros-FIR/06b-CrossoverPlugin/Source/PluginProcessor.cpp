// ============================================================
// PluginProcessor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: Crossover FIR
// ============================================================
//
// Crossover Linkwitz-Riley 4to orden:
//   - Canal izquierdo  → graves  (pasa-bajos)
//   - Canal derecho    → agudos  (pasa-altos)
//
// Linkwitz-Riley = dos biquad Butterworth en cascada
// Garantiza que LPF + HPF sumen perfectamente (flat response)
// ============================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"

CrossoverAudioProcessor::CrossoverAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input",   juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}

CrossoverAudioProcessor::~CrossoverAudioProcessor() {}

void CrossoverAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    sample_rate = sampleRate;

    // Preparar contexto DSP para cada filtro
    juce::dsp::ProcessSpec spec;
    spec.sampleRate       = sampleRate;
    spec.maximumBlockSize = (juce::uint32) samplesPerBlock;
    spec.numChannels      = 1;

    lpf1.prepare(spec);
    lpf2.prepare(spec);
    hpf1.prepare(spec);
    hpf2.prepare(spec);

    updateFilters();
}

void CrossoverAudioProcessor::releaseResources()
{
    lpf1.reset();
    lpf2.reset();
    hpf1.reset();
    hpf2.reset();
}

void CrossoverAudioProcessor::updateFilters()
{
    // Linkwitz-Riley 4to orden = dos Butterworth 2do orden en cascada
    // JUCE calcula los coeficientes automáticamente
    auto lpf_coeffs = Coefficients::makeLowPass(sample_rate, crossover_freq);
    auto hpf_coeffs = Coefficients::makeHighPass(sample_rate, crossover_freq);

    *lpf1.coefficients = *lpf_coeffs;
    *lpf2.coefficients = *lpf_coeffs;
    *hpf1.coefficients = *hpf_coeffs;
    *hpf2.coefficients = *hpf_coeffs;
}

void CrossoverAudioProcessor::setCrossoverFreq(float freq)
{
    crossover_freq = freq;
    updateFilters();
}

void CrossoverAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                           juce::MidiBuffer&)
{
    int numSamples = buffer.getNumSamples();

    // Leer señal mono de entrada (canal 0)
    auto* input = buffer.getReadPointer(0);

    // Buffers temporales para graves y agudos
    juce::AudioBuffer<float> graves(1, numSamples);
    juce::AudioBuffer<float> agudos(1, numSamples);

    graves.copyFrom(0, 0, buffer, 0, 0, numSamples);
    agudos.copyFrom(0, 0, buffer, 0, 0, numSamples);

    // Aplicar pasa-bajos en cascada → graves
    {
        juce::dsp::AudioBlock<float> block(graves);
        juce::dsp::ProcessContextReplacing<float> ctx(block);
        lpf1.process(ctx);
        lpf2.process(ctx);
    }

    // Aplicar pasa-altos en cascada → agudos
    {
        juce::dsp::AudioBlock<float> block(agudos);
        juce::dsp::ProcessContextReplacing<float> ctx(block);
        hpf1.process(ctx);
        hpf2.process(ctx);
    }

    // Canal izquierdo  → graves
    // Canal derecho    → agudos
    buffer.copyFrom(0, 0, graves, 0, 0, numSamples);

    if (buffer.getNumChannels() > 1)
        buffer.copyFrom(1, 0, agudos, 0, 0, numSamples);
}

juce::AudioProcessorEditor* CrossoverAudioProcessor::createEditor()
{
    return new CrossoverAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CrossoverAudioProcessor();
}
