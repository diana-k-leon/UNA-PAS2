#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

class ReverbAlgoritmicaAudioProcessor : public juce::AudioProcessor
{
public:
    ReverbAlgoritmicaAudioProcessor();
    ~ReverbAlgoritmicaAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "ReverbAlgoritmica"; }
    bool acceptsMidi() const override  { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 2.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock&) override {}
    void setStateInformation(const void*, int) override {}

    // parámetros — accedidos desde el editor
    std::atomic<float> roomSize { 0.84f };  // feedback g de los combs (0 a 0.98)
    std::atomic<float> wet      { 0.3f  };  // mezcla wet
    std::atomic<float> dry      { 1.0f  };  // mezcla dry

private:
    double sampleRate = 44100.0;

    // --- 4 COMBS en paralelo ---
    // delays de Freeverb (en samples a 44100 Hz)
    static constexpr int COMB_DELAYS[4] = { 1557, 1617, 1491, 1422 };
    static constexpr int COMB_MAX = 2048;

    float combBuffer[4][COMB_MAX] = {};
    int   combWritePtr[4]         = {};

    // --- 2 ALLPASS en serie ---
    static constexpr int AP_DELAYS[2] = { 556, 441 };
    static constexpr int AP_MAX = 1024;

    float apBuffer[2][AP_MAX] = {};
    int   apWritePtr[2]       = {};

    static constexpr float AP_G = 0.5f;

    float processComb   (int idx, float input, float g);
    float processAllpass(int idx, float input);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbAlgoritmicaAudioProcessor)
};
