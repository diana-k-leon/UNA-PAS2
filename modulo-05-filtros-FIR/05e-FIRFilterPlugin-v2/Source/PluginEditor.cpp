// ============================================================
// PluginEditor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: FIR Filter v2 (orden variable)
// ============================================================
//
// Slider 1 (horizontal): Cutoff frequency 200 Hz — 20 kHz
// Slider 2 (horizontal): Orden FIR 4 — 256
//
// Escuchar la diferencia entre orden bajo y orden alto:
//   orden 4  → corte muy bruto
//   orden 256 → corte muy preciso
// ============================================================

#include "PluginEditor.h"

FIRFilterV2AudioProcessorEditor::FIRFilterV2AudioProcessorEditor(
    FIRFilterV2AudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(420, 320);
    cutoff_freq = p.getCutoffFreq();
    fir_order   = p.getFIROrder();
}

FIRFilterV2AudioProcessorEditor::~FIRFilterV2AudioProcessorEditor() {}

void FIRFilterV2AudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff0d0f14));

    // Título
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(20.0f, juce::Font::bold));
    g.drawText("FIR Filter — orden variable", getLocalBounds().withHeight(45),
               juce::Justification::centred, true);

    float sx = 50.0f, sw = 320.0f, sh = 24.0f;

    // ── Slider 1: Cutoff ──
    g.setColour(juce::Colour(0xff4fc9a4));
    g.setFont(12.0f);
    g.drawText("Cutoff: " + juce::String((int)cutoff_freq) + " Hz",
               sx, 68, sw, 18, juce::Justification::left, true);

    g.setColour(juce::Colour(0xff2a2f42));
    g.fillRect(sx, 90.0f, sw, sh);
    g.setColour(juce::Colour(0xff3a3f55));
    g.drawRect(sx, 90.0f, sw, sh, 1.5f);

    float log_min = std::log(200.0f);
    float log_max = std::log(20000.0f);
    float norm1 = (std::log(cutoff_freq) - log_min) / (log_max - log_min);
    norm1 = juce::jlimit(0.0f, 1.0f, norm1);

    g.setColour(juce::Colour(0xff4fc9a4));
    g.fillRect(sx + norm1 * sw - 5.0f, 93.0f, 10.0f, sh - 6.0f);

    g.setColour(juce::Colour(0xff555a72));
    g.setFont(10.0f);
    g.drawText("200 Hz", sx, 116, 60, 16, juce::Justification::left, true);
    g.drawText("20 kHz", sx + sw - 40, 116, 50, 16, juce::Justification::right, true);

    // ── Slider 2: Orden FIR ──
    g.setColour(juce::Colour(0xff7c6dfa));
    g.setFont(12.0f);
    g.drawText("Orden FIR: " + juce::String(fir_order),
               sx, 155, sw, 18, juce::Justification::left, true);

    g.setColour(juce::Colour(0xff2a2f42));
    g.fillRect(sx, 177.0f, sw, sh);
    g.setColour(juce::Colour(0xff3a3f55));
    g.drawRect(sx, 177.0f, sw, sh, 1.5f);

    float norm2 = (float)(fir_order - 4) / (256.0f - 4.0f);
    norm2 = juce::jlimit(0.0f, 1.0f, norm2);

    g.setColour(juce::Colour(0xff7c6dfa));
    g.fillRect(sx + norm2 * sw - 5.0f, 180.0f, 10.0f, sh - 6.0f);

    g.setColour(juce::Colour(0xff555a72));
    g.setFont(10.0f);
    g.drawText("4 (bruto)", sx, 203, 70, 16, juce::Justification::left, true);
    g.drawText("256 (preciso)", sx + sw - 80, 203, 80, 16, juce::Justification::right, true);

    // ── Info ──
    float iy = 240.0f;
    juce::Colour boxColor = (fir_order < 32)
        ? juce::Colour(0xffe05c5c).withAlpha(0.1f)
        : juce::Colour(0xff4fc9a4).withAlpha(0.1f);
    juce::Colour borderColor = (fir_order < 32)
        ? juce::Colour(0xffe05c5c)
        : juce::Colour(0xff4fc9a4);

    g.setColour(boxColor);
    g.fillRoundedRectangle(sx, iy, sw, 50, 8);
    g.setColour(borderColor);
    g.drawRoundedRectangle(sx, iy, sw, 50, 8, 1.5f);

    g.setFont(11.0f);
    juce::String msg = (fir_order < 32)
        ? "Orden bajo — corte bruto, pocos recursos"
        : (fir_order < 128)
            ? "Orden medio — buen balance entre calidad y costo"
            : "Orden alto — corte preciso, más multiplicaciones";
    g.drawText(msg, sx, iy, sw, 50, juce::Justification::centred, true);
}

void FIRFilterV2AudioProcessorEditor::resized() {}

void FIRFilterV2AudioProcessorEditor::mouseDown(const juce::MouseEvent& e)
{
    if (e.y >= 88 && e.y <= 116)
        active_slider = 0;
    else if (e.y >= 175 && e.y <= 203)
        active_slider = 1;
    else
        active_slider = -1;

    mouseDrag(e);
}

void FIRFilterV2AudioProcessorEditor::mouseDrag(const juce::MouseEvent& e)
{
    float sx = 50.0f, sw = 320.0f;
    float norm = juce::jlimit(0.0f, 1.0f, (e.x - sx) / sw);
    // ACA!
    if (active_slider == 0)
    {
        float log_min = std::log(200.0f);
        float log_max = std::log(20000.0f);
        cutoff_freq = std::exp(log_min + norm * (log_max - log_min));
        audioProcessor.setCutoffFreq(cutoff_freq);
    }
    else if (active_slider == 1)
    {
        fir_order = (int)(4 + norm * (256 - 4));
        audioProcessor.setFIROrder(fir_order);
    }

    repaint();
}
