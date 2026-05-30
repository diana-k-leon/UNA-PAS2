// ============================================================
// PluginEditor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 2 · Plugin: Pitch Shift
// ============================================================
//
// Editor simple: slider vertical para pitch shift (-12 a +12 semitonos).
// Drag arriba/abajo para cambiar el pitch en tiempo real.
// ============================================================

#include "PluginEditor.h"

PitchShiftAudioProcessorEditor::PitchShiftAudioProcessorEditor(
    PitchShiftAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(300, 400);
}

PitchShiftAudioProcessorEditor::~PitchShiftAudioProcessorEditor() {}

void PitchShiftAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Pitch Shift", getLocalBounds().withHeight(50), juce::Justification::centred, true);

    g.setFont(16.0f);
    g.drawText(juce::String(pitch_shift, 1) + " semitones", 
        getLocalBounds().withTop(60).withHeight(50), 
        juce::Justification::centred, true);

    // Dibuja slider visual
    g.setColour(juce::Colours::grey);
    float slider_x = 100.0f;
    float slider_y = 150.0f;
    float slider_height = 200.0f;
    g.drawRect(slider_x, slider_y, 100.0f, slider_height, 2.0f);

    // Posición actual
    float norm_pitch = (pitch_shift + 12.0f) / 24.0f;  // -12 a +12 → 0 a 1
    float handle_y = slider_y + (1.0f - norm_pitch) * slider_height;
    g.setColour(juce::Colours::cyan);
    g.fillRect(slider_x, handle_y - 5.0f, 100.0f, 10.0f);

    g.setColour(juce::Colours::white);
    g.setFont(12.0f);
    g.drawText("-12", slider_x + 110.0f, slider_y + slider_height - 15.0f, 50, 20, juce::Justification::left, true);
    g.drawText("+12", slider_x + 110.0f, slider_y - 10.0f, 50, 20, juce::Justification::left, true);
}

void PitchShiftAudioProcessorEditor::resized() {}

void PitchShiftAudioProcessorEditor::mouseDown(const juce::MouseEvent& event)
{
    mouseDrag(event);
}

void PitchShiftAudioProcessorEditor::mouseDrag(const juce::MouseEvent& event)
{
    float slider_y = 150.0f;
    float slider_height = 200.0f;

    float norm = 1.0f - (event.y - slider_y) / slider_height;
    norm = std::max(0.0f, std::min(1.0f, norm));

    pitch_shift = norm * 24.0f - 12.0f;  // 0 a 1 → -12 a +12
    audioProcessor.setPitchShift(pitch_shift);

    repaint();
}