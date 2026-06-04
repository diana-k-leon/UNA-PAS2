// ============================================================
// PluginEditor.h
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: IR Convolution
// ============================================================

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class IRConvolutionAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                           public juce::FileDragAndDropTarget
{
public:
    IRConvolutionAudioProcessorEditor(IRConvolutionAudioProcessor&);
    ~IRConvolutionAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    // FileDragAndDropTarget
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

private:
    IRConvolutionAudioProcessor& audioProcessor;
    bool dragging_over = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRConvolutionAudioProcessorEditor)
};
