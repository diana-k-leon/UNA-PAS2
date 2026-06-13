#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cstring>

ReverbAlgoritmicaAudioProcessor::ReverbAlgoritmicaAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    for (int i = 0; i < 4; ++i) { memset(combBuffer[i], 0, sizeof(float) * COMB_MAX); combWritePtr[i] = 0; }
    for (int i = 0; i < 2; ++i) { memset(apBuffer[i],   0, sizeof(float) * AP_MAX);   apWritePtr[i]   = 0; }
}

ReverbAlgoritmicaAudioProcessor::~ReverbAlgoritmicaAudioProcessor() {}

void ReverbAlgoritmicaAudioProcessor::prepareToPlay(double sr, int)
{
    sampleRate = sr;
    for (int i = 0; i < 4; ++i) { memset(combBuffer[i], 0, sizeof(float) * COMB_MAX); combWritePtr[i] = 0; }
    for (int i = 0; i < 2; ++i) { memset(apBuffer[i],   0, sizeof(float) * AP_MAX);   apWritePtr[i]   = 0; }
}

void ReverbAlgoritmicaAudioProcessor::releaseResources() {}

// ============================================================
// COMB FILTER: y[n] = x[n-D] + g * y[n-D]
// ============================================================
float ReverbAlgoritmicaAudioProcessor::processComb(int idx, float input, float g)
{
    int D = COMB_DELAYS[idx];
    int w = combWritePtr[idx];
    int N = COMB_MAX;

    int   readPtr = (w - D + N) % N;
    float delayed = combBuffer[idx][readPtr];

    combBuffer[idx][w] = input + g * delayed;
    combWritePtr[idx]  = (w + 1) % N;

    return delayed;
}

// ============================================================
// ALLPASS FILTER
// ============================================================
float ReverbAlgoritmicaAudioProcessor::processAllpass(int idx, float input)
{
    int D = AP_DELAYS[idx];
    int w = apWritePtr[idx];
    int N = AP_MAX;

    int   readPtr = (w - D + N) % N;
    float buf_out = apBuffer[idx][readPtr];
    float buf_in  = input + AP_G * buf_out;

    apBuffer[idx][w] = buf_in;
    apWritePtr[idx]  = (w + 1) % N;

    return buf_out - AP_G * buf_in;
}

// ============================================================
// PROCESSBLOCK
// ============================================================
void ReverbAlgoritmicaAudioProcessor::processBlock(
    juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    // El roomSize del slider convertido a g — el feedback de los combs. 
    float g    = juce::jlimit(0.0f, 0.98f, roomSize.load());
    //cuánto de reverb y cuánto de señal original
    float wetG = wet.load();
    float dryG = dry.load();

    int numSamples   = buffer.getNumSamples();
    int numChannels  = buffer.getNumChannels();

    // Procesamos en mono (canal 0) y copiamos al resto al final
    auto* data = buffer.getWritePointer(0);

    for (int n = 0; n < numSamples; ++n)
    {
        float input = data[n];

        // 4 combs en paralelo
        float combSum = 0.0f;
        for (int i = 0; i < 4; ++i)
            combSum += processComb(i, input, g);
        // ?
        combSum *= 0.25f;

        // 2 allpass en serie
        float rev = processAllpass(0, combSum);
        rev        = processAllpass(1, rev);

        // Mezcla final 
        data[n] = dryG * input + wetG * rev;
    }

    // Copiar canal 0 al resto
    for (int ch = 1; ch < numChannels; ++ch)
        buffer.copyFrom(ch, 0, buffer, 0, 0, numSamples);
}

juce::AudioProcessorEditor* ReverbAlgoritmicaAudioProcessor::createEditor()
{
    return new ReverbAlgoritmicaAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbAlgoritmicaAudioProcessor();
}
