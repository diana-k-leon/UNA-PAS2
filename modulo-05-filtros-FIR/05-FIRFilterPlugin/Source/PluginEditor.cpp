// ============================================================
// PluginEditor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: FIR Filter
// ============================================================
//
// Slider para cutoff frequency (200 Hz — 20 kHz).
// Drag horizontalmente para cambiar.
// ============================================================

#include "PluginEditor.h"

FIRFilterAudioProcessorEditor::FIRFilterAudioProcessorEditor(
    FIRFilterAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(400, 300);
    cutoff_freq = p.getCutoffFreq();
}

FIRFilterAudioProcessorEditor::~FIRFilterAudioProcessorEditor() {}

void FIRFilterAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("FIR Lowpass Filter", getLocalBounds().withHeight(50), juce::Justification::centred, true);

    g.setFont(16.0f);
    g.drawText("Cutoff: " + juce::String((int)cutoff_freq) + " Hz", 
        getLocalBounds().withTop(60).withHeight(50), 
        juce::Justification::centred, true);

    // Slider visual
    g.setColour(juce::Colours::grey);
    float slider_x = 50.0f;
    float slider_y = 150.0f;
    float slider_width = 300.0f;
    g.drawRect(slider_x, slider_y, slider_width, 30.0f, 2.0f);

    // Posición actual (200 Hz — 20 kHz en escala log)
    float log_min = std::log(200.0f);
    float log_max = std::log(20000.0f);
    float log_val = std::log(cutoff_freq);
    float norm = (log_val - log_min) / (log_max - log_min);
    norm = std::max(0.0f, std::min(1.0f, norm));

    g.setColour(juce::Colours::cyan);
    g.fillRect(slider_x + norm * slider_width - 5.0f, slider_y + 5.0f, 10.0f, 20.0f);

    g.setColour(juce::Colours::white);
    g.setFont(12.0f);
    g.drawText("200 Hz", slider_x - 30.0f, slider_y + 30.0f, 80, 20, juce::Justification::left, true);
    g.drawText("20 kHz", slider_x + slider_width - 30.0f, slider_y + 30.0f, 80, 20, juce::Justification::left, true);
}

void FIRFilterAudioProcessorEditor::resized() {}

void FIRFilterAudioProcessorEditor::mouseDown(const juce::MouseEvent& event)
{
    mouseDrag(event);
}

void FIRFilterAudioProcessorEditor::mouseDrag(const juce::MouseEvent& event)
{
    float slider_x = 50.0f;
    float slider_width = 300.0f;

    float norm = (event.x - slider_x) / slider_width;
    norm = std::max(0.0f, std::min(1.0f, norm));

    // Escala log: 200 Hz — 20 kHz
    float log_min = std::log(200.0f);
    float log_max = std::log(20000.0f);
    cutoff_freq = std::exp(log_min + norm * (log_max - log_min));

    audioProcessor.setCutoffFreq(cutoff_freq);
    repaint();
}