// ============================================================
// PluginEditor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: Crossover FIR
// ============================================================
//
// Slider para frecuencia de cruce (200 Hz — 10 kHz)
// Canal izquierdo → graves / Canal derecho → agudos
// ============================================================

#include "PluginEditor.h"

CrossoverAudioProcessorEditor::CrossoverAudioProcessorEditor(CrossoverAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(420, 280);
    crossover_freq = p.getCrossoverFreq();
}

CrossoverAudioProcessorEditor::~CrossoverAudioProcessorEditor() {}

void CrossoverAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff0d0f14));

    // Título
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(20.0f, juce::Font::bold));
    g.drawText("Crossover FIR", getLocalBounds().withHeight(50), juce::Justification::centred, true);

    // Frecuencia de cruce
    g.setFont(16.0f);
    g.setColour(juce::Colour(0xfff5a623));
    g.drawText("Crossover: " + juce::String((int)crossover_freq) + " Hz",
        getLocalBounds().withTop(55).withHeight(30),
        juce::Justification::centred, true);

    // Slider
    float sx = 60.0f, sy = 110.0f, sw = 300.0f, sh = 30.0f;
    g.setColour(juce::Colour(0xff2a2f42));
    g.fillRect(sx, sy, sw, sh);
    g.setColour(juce::Colour(0xff3a3f55));
    g.drawRect(sx, sy, sw, sh, 1.5f);

    // Posición del slider (escala log 200-10000 Hz)
    float log_min = std::log(200.0f);
    float log_max = std::log(10000.0f);
    float norm = (std::log(crossover_freq) - log_min) / (log_max - log_min);
    norm = juce::jlimit(0.0f, 1.0f, norm);

    g.setColour(juce::Colour(0xfff5a623));
    g.fillRect(sx + norm * sw - 5.0f, sy + 3.0f, 10.0f, sh - 6.0f);

    // Labels slider
    g.setColour(juce::Colour(0xff8b90a8));
    g.setFont(11.0f);
    g.drawText("200 Hz", sx, sy + sh + 5, 60, 20, juce::Justification::left, true);
    g.drawText("10 kHz", sx + sw - 40, sy + sh + 5, 50, 20, juce::Justification::right, true);

    // Diagrama de canales
    float dy = 175.0f;
    float boxW = 140.0f, boxH = 50.0f;

    // Canal izquierdo — graves
    g.setColour(juce::Colour(0xff4fc9a4).withAlpha(0.15f));
    g.fillRoundedRectangle(30, dy, boxW, boxH, 6);
    g.setColour(juce::Colour(0xff4fc9a4));
    g.drawRoundedRectangle(30, dy, boxW, boxH, 6, 1.5f);
    g.setFont(juce::Font(12.0f, juce::Font::bold));
    g.drawText("Canal L", 30, dy + 6, boxW, 20, juce::Justification::centred, true);
    g.setFont(10.0f);
    g.setColour(juce::Colour(0xff8b90a8));
    g.drawText("graves · 0 — " + juce::String((int)crossover_freq) + " Hz", 30, dy + 26, boxW, 18, juce::Justification::centred, true);

    // Canal derecho — agudos
    g.setColour(juce::Colour(0xff7c6dfa).withAlpha(0.15f));
    g.fillRoundedRectangle(250, dy, boxW, boxH, 6);
    g.setColour(juce::Colour(0xff7c6dfa));
    g.drawRoundedRectangle(250, dy, boxW, boxH, 6, 1.5f);
    g.setFont(juce::Font(12.0f, juce::Font::bold));
    g.drawText("Canal R", 250, dy + 6, boxW, 20, juce::Justification::centred, true);
    g.setFont(10.0f);
    g.setColour(juce::Colour(0xff8b90a8));
    g.drawText(juce::String((int)crossover_freq) + " Hz — 20 kHz", 250, dy + 26, boxW, 18, juce::Justification::centred, true);
}

void CrossoverAudioProcessorEditor::resized() {}

void CrossoverAudioProcessorEditor::mouseDown(const juce::MouseEvent& e) { mouseDrag(e); }

void CrossoverAudioProcessorEditor::mouseDrag(const juce::MouseEvent& e)
{
    float sx = 60.0f, sw = 300.0f;
    float norm = juce::jlimit(0.0f, 1.0f, (e.x - sx) / sw);

    float log_min = std::log(200.0f);
    float log_max = std::log(10000.0f);
    crossover_freq = std::exp(log_min + norm * (log_max - log_min));

    audioProcessor.setCrossoverFreq(crossover_freq);
    repaint();
}
