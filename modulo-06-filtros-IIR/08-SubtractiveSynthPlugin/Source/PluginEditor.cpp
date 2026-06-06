// ============================================================
// PluginEditor.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 6 · Plugin: Subtractive Synth
// ============================================================
//
// Tres sliders:
//   Cutoff base: 100 Hz — 8000 Hz (escala log)
//   LFO Freq:   0.05 Hz — 20 Hz  (escala log)
//   LFO Depth:  0 Hz — 4000 Hz   (lineal)
//
// Preset rápido: mover LFO Freq muy lento → viento
//               mover LFO Freq rápido → timbre pulsante
// ============================================================

#include "PluginEditor.h"
#include <cmath>

SubtractiveSynthAudioProcessorEditor::SubtractiveSynthAudioProcessorEditor(
    SubtractiveSynthAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(440, 340);
    cutoff_base = p.getCutoffBase();
    lfo_freq    = p.getLFOFreq();
    lfo_depth   = p.getLFODepth();
}

SubtractiveSynthAudioProcessorEditor::~SubtractiveSynthAudioProcessorEditor() {}

void SubtractiveSynthAudioProcessorEditor::drawSlider(
    juce::Graphics& g, float x, float y, float w, float h,
    float value, float min, float max,
    const juce::String& label, const juce::String& unit,
    juce::Colour color, bool log_scale)
{
    // Label
    g.setColour(color);
    g.setFont(12.0f);
    juce::String val_str = (value < 10.0f)
        ? juce::String(value, 2)
        : juce::String((int)value);
    g.drawText(label + ": " + val_str + " " + unit, x, y - 20, w, 18,
               juce::Justification::left, true);

    // Track
    g.setColour(juce::Colour(0xff2a2f42));
    g.fillRect(x, y, w, h);
    g.setColour(juce::Colour(0xff3a3f55));
    g.drawRect(x, y, w, h, 1.5f);

    // Posición
    float norm;
    if (log_scale)
    {
        float log_min = std::log(min);
        float log_max = std::log(max);
        norm = (std::log(value) - log_min) / (log_max - log_min);
    }
    else
        norm = (value - min) / (max - min);

    norm = juce::jlimit(0.0f, 1.0f, norm);
    g.setColour(color);
    g.fillRect(x + norm * w - 5.0f, y + 3.0f, 10.0f, h - 6.0f);
}

void SubtractiveSynthAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff0d0f14));

    // Título
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(20.0f, juce::Font::bold));
    g.drawText("Subtractive Synth", getLocalBounds().withHeight(45),
               juce::Justification::centred, true);

    // Cadena visual
    g.setFont(11.0f);
    g.setColour(juce::Colour(0xff555a72));
    g.drawText("Ruido blanco  →  Pasa-bajos IIR  →  Salida",
               0, 48, getWidth(), 18, juce::Justification::centred, true);

    float sx = 50.0f, sw = 340.0f, sh = 22.0f;

    // Slider 1: Cutoff base
    drawSlider(g, sx, 95, sw, sh, cutoff_base, 100.0f, 8000.0f,
               "Cutoff base", "Hz", juce::Colour(0xff4fc9a4), true);

    // Slider 2: LFO Freq
    drawSlider(g, sx, 160, sw, sh, lfo_freq, 0.05f, 20.0f,
               "LFO velocidad", "Hz", juce::Colour(0xff7c6dfa), true);

    // Slider 3: LFO Depth
    drawSlider(g, sx, 225, sw, sh, lfo_depth, 0.0f, 4000.0f,
               "LFO profundidad", "Hz", juce::Colour(0xfff5a623), false);

    // Info de preset
    float iy = 270.0f;
    g.setColour(juce::Colour(0xff1c2030));
    g.fillRoundedRectangle(sx, iy, sw, 50, 8);
    g.setColour(juce::Colour(0xff3a3f55));
    g.drawRoundedRectangle(sx, iy, sw, 50, 8, 1.0f);

    g.setFont(10.0f);
    g.setColour(juce::Colour(0xff8b90a8));
    g.drawText("🌬 Viento: cutoff alto, LFO lento (0.1 Hz), depth alto",
               sx + 8, iy + 8, sw - 16, 16, juce::Justification::left, true);
    g.drawText("🌊 Océano: cutoff medio, LFO muy lento (0.05 Hz), depth medio",
               sx + 8, iy + 26, sw - 16, 16, juce::Justification::left, true);
}

void SubtractiveSynthAudioProcessorEditor::resized() {}

void SubtractiveSynthAudioProcessorEditor::mouseDown(const juce::MouseEvent& e)
{
    float sx = 50.0f, sw = 340.0f;
    if (e.x < sx || e.x > sx + sw) { active_slider = -1; return; }

    if      (e.y >= 93  && e.y <= 120) active_slider = 0;
    else if (e.y >= 158 && e.y <= 185) active_slider = 1;
    else if (e.y >= 223 && e.y <= 250) active_slider = 2;
    else active_slider = -1;

    mouseDrag(e);
}

void SubtractiveSynthAudioProcessorEditor::mouseDrag(const juce::MouseEvent& e)
{
    if (active_slider < 0) return;

    float sx = 50.0f, sw = 340.0f;
    float norm = juce::jlimit(0.0f, 1.0f, (e.x - sx) / sw);

    if (active_slider == 0)
    {
        float log_min = std::log(100.0f);
        float log_max = std::log(8000.0f);
        cutoff_base = std::exp(log_min + norm * (log_max - log_min));
        audioProcessor.setCutoffBase(cutoff_base);
    }
    else if (active_slider == 1)
    {
        float log_min = std::log(0.05f);
        float log_max = std::log(20.0f);
        lfo_freq = std::exp(log_min + norm * (log_max - log_min));
        audioProcessor.setLFOFreq(lfo_freq);
    }
    else if (active_slider == 2)
    {
        lfo_depth = norm * 4000.0f;
        audioProcessor.setLFODepth(lfo_depth);
    }

    repaint();
}
