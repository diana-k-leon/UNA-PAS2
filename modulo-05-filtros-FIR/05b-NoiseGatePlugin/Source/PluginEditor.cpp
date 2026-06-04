// ============================================================
// PluginEditor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: Noise Gate
// ============================================================
//
// Slider de threshold: 0.0 a 1.0
// ============================================================

#include "PluginEditor.h"

NoiseGateAudioProcessorEditor::NoiseGateAudioProcessorEditor(NoiseGateAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(400, 260);
    threshold = p.getThreshold();
}

NoiseGateAudioProcessorEditor::~NoiseGateAudioProcessorEditor() {}

void NoiseGateAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff0d0f14));

    // Título
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(20.0f, juce::Font::bold));
    g.drawText("Noise Gate", getLocalBounds().withHeight(50),
               juce::Justification::centred, true);

    // Threshold valor
    g.setFont(16.0f);
    g.setColour(juce::Colour(0xff4fc9a4));
    g.drawText("Threshold: " + juce::String(threshold, 4),
        getLocalBounds().withTop(55).withHeight(30),
        juce::Justification::centred, true);

    // Slider
    float sx = 50.0f, sy = 110.0f, sw = 300.0f, sh = 30.0f;
    g.setColour(juce::Colour(0xff2a2f42));
    g.fillRect(sx, sy, sw, sh);
    g.setColour(juce::Colour(0xff3a3f55));
    g.drawRect(sx, sy, sw, sh, 1.5f);

    // Posición slider (lineal 0.0 a 1.0)
    float norm = juce::jlimit(0.0f, 1.0f, threshold);
    g.setColour(juce::Colour(0xff4fc9a4));
    g.fillRect(sx + norm * sw - 5.0f, sy + 3.0f, 10.0f, sh - 6.0f);

    // Labels
    g.setColour(juce::Colour(0xff8b90a8));
    g.setFont(11.0f);
    g.drawText("0.0", sx, sy + sh + 5, 40, 20, juce::Justification::left, true);
    g.drawText("1.0", sx + sw - 20, sy + sh + 5, 40, 20, juce::Justification::right, true);

    // Estado del gate
    float sy2 = 185.0f;
    bool gate_open = threshold < 0.5f;

    g.setColour(gate_open
        ? juce::Colour(0xff4fc9a4).withAlpha(0.15f)
        : juce::Colour(0xffe05c5c).withAlpha(0.15f));
    g.fillRoundedRectangle(50, sy2, 300, 45, 8);

    g.setColour(gate_open ? juce::Colour(0xff4fc9a4) : juce::Colour(0xffe05c5c));
    g.drawRoundedRectangle(50, sy2, 300, 45, 8, 1.5f);
    g.setFont(13.0f);
    g.drawText(gate_open ? "Gate activo — eliminando ruido de fondo"
                         : "⚠ Threshold muy alto — puede cortar señal útil",
               50, sy2, 300, 45, juce::Justification::centred, true);
}

void NoiseGateAudioProcessorEditor::resized() {}

void NoiseGateAudioProcessorEditor::mouseDown(const juce::MouseEvent& e) { mouseDrag(e); }

void NoiseGateAudioProcessorEditor::mouseDrag(const juce::MouseEvent& e)
{
    float sx = 50.0f, sw = 300.0f;
    threshold = juce::jlimit(0.0f, 1.0f, (e.x - sx) / sw);
    audioProcessor.setThreshold(threshold);
    repaint();
}
