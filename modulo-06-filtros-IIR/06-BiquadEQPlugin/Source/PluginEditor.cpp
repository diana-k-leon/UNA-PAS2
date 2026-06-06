// ============================================================
// PluginEditor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: Biquad EQ
// ============================================================
//
// Dos sliders verticales: Banda 1 (500 Hz) y Banda 2 (5 kHz).
// Drag arriba/abajo para boost/cut (-12 dB a +12 dB).
// ============================================================

#include "PluginEditor.h"

BiquadEQAudioProcessorEditor::BiquadEQAudioProcessorEditor(
    BiquadEQAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(400, 300);
}

BiquadEQAudioProcessorEditor::~BiquadEQAudioProcessorEditor() {}

void BiquadEQAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawText("Biquad EQ", getLocalBounds().withHeight(40), juce::Justification::centred, true);

    // Banda 1
    g.setFont(14.0f);
    g.drawText("500 Hz", 50, 50, 100, 20, juce::Justification::centred, true);
    g.drawText(juce::String(gain1, 1) + " dB", 50, 70, 100, 20, juce::Justification::centred, true);

    g.setColour(juce::Colours::grey);
    g.drawRect(70.0f, 120.0f, 60.0f, 150.0f, 2.0f);
    
    float norm1 = (gain1 + 12.0f) / 24.0f;
    float y1 = 120.0f + (1.0f - norm1) * 150.0f;
    g.setColour(juce::Colours::cyan);
    g.fillRect(70.0f, y1 - 5.0f, 60.0f, 10.0f);

    // Banda 2
    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText("5 kHz", 250, 50, 100, 20, juce::Justification::centred, true);
    g.drawText(juce::String(gain2, 1) + " dB", 250, 70, 100, 20, juce::Justification::centred, true);

    g.setColour(juce::Colours::grey);
    g.drawRect(270.0f, 120.0f, 60.0f, 150.0f, 2.0f);
    
    float norm2 = (gain2 + 12.0f) / 24.0f;
    float y2 = 120.0f + (1.0f - norm2) * 150.0f;
    g.setColour(juce::Colours::cyan);
    g.fillRect(270.0f, y2 - 5.0f, 60.0f, 10.0f);

    // Labels
    g.setColour(juce::Colours::white);
    g.setFont(10.0f);
    g.drawText("-3", 30, 260, 40, 15, juce::Justification::centred, true);
    g.drawText("+3", 30, 115, 40, 15, juce::Justification::centred, true);
}

void BiquadEQAudioProcessorEditor::resized() {}

void BiquadEQAudioProcessorEditor::mouseDown(const juce::MouseEvent& event)
{
    if (event.x >= 70 && event.x < 130)
        active_slider = 0;
    else if (event.x >= 270 && event.x < 330)
        active_slider = 1;
    
    mouseDrag(event);
}

void BiquadEQAudioProcessorEditor::mouseDrag(const juce::MouseEvent& event)
{
    if (active_slider == 0)
    {
        float norm = 1.0f - (event.y - 120.0f) / 150.0f;
        norm = std::max(0.0f, std::min(1.0f, norm));
        gain1 = norm * 6.0f - 3.0f;  // -3 a +3
        audioProcessor.setBiquad(0, 500.0f, gain1, 1.0f);
    }
    else if (active_slider == 1)
    {
        float norm = 1.0f - (event.y - 120.0f) / 150.0f;
        norm = std::max(0.0f, std::min(1.0f, norm));
        gain2 = norm * 6.0f - 3.0f;  // -3 a +3
        audioProcessor.setBiquad(1, 5000.0f, gain2, 1.0f);
    }
    
    repaint();
}