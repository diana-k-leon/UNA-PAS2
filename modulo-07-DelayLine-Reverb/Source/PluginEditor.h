#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include "PluginProcessor.h"

class ReverbAlgoritmicaAudioProcessorEditor : public juce::AudioProcessorEditor,
                                              private juce::Timer
{
public:
    ReverbAlgoritmicaAudioProcessorEditor(ReverbAlgoritmicaAudioProcessor&);
    ~ReverbAlgoritmicaAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override {}

    ReverbAlgoritmicaAudioProcessor& processor;

    juce::Slider roomSizeSlider, wetSlider, drySlider;
    juce::Label  roomSizeLabel, wetLabel, dryLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbAlgoritmicaAudioProcessorEditor)
};
