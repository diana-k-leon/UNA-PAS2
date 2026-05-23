// ============================================================
// PluginEditor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 1 · Plugin: Spectrum Analyzer
// ============================================================

#include "PluginEditor.h"
#include <cmath>

SpectrumAnalyzerAudioProcessorEditor::SpectrumAnalyzerAudioProcessorEditor(
    SpectrumAnalyzerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(1000, 400);
    startTimerHz(30);
}

SpectrumAnalyzerAudioProcessorEditor::~SpectrumAnalyzerAudioProcessorEditor()
{
    stopTimer();
}

void SpectrumAnalyzerAudioProcessorEditor::timerCallback()
{
    repaint();
}

void SpectrumAnalyzerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    auto bounds = getLocalBounds();
    float width = bounds.getWidth();
    float height = bounds.getHeight();
    float margin_left = 50.0f;
    float margin_bottom = 50.0f;
    float margin_top = 20.0f;
    float margin_right = 20.0f;
    float plot_width = width - margin_left - margin_right;
    float plot_height = height - margin_top - margin_bottom;

    // ── grilla de dB ──────────────────────────────────────────────────────
    g.setColour(juce::Colours::grey.darker());
    g.setFont(12.0f);
    // rango: 0 dB (arriba) a -80 dB (abajo)
    for (int i = 0; i <= 4; i++)
    {
        float db_val = -i * 20.0f;  // 0, -20, -40, -60, -80
        float y = margin_top + (float)i / 4.0f * plot_height;
        g.drawLine(margin_left, y, margin_left + plot_width, y);

        juce::String label = juce::String((int)db_val) + " dB";
        g.drawText(label, 0, (int)y - 10, 48, 20,
                   juce::Justification::centredRight, false);
    }

    // ── leer espectro ─────────────────────────────────────────────────────
    const auto& spectrum = audioProcessor.getSpectrum();
    int num_bins = audioProcessor.getFFTSize() / 2;

    if (num_bins > 0 && (int)spectrum.size() >= num_bins)
    {
        // buscar máximo para normalización relativa
        float max_val = 0.0f;
        for (int k = 0; k < num_bins; ++k)
            max_val = std::max(max_val, spectrum[k]);
        if (max_val < 1e-10f) max_val = 1.0f;

        float bin_width = plot_width / (float)num_bins;

        for (int k = 0; k < num_bins; ++k)
        {
            float mag = spectrum[k];
            if (mag < 1e-10f) continue;  // no dibujar silencio

            // dB relativo al máximo: el pico más alto = 0 dB
            float db = 20.0f * std::log10(mag / max_val);

            // clamp a rango -80 a 0
            if (db < -80.0f) db = -80.0f;

            // convertir a posición Y: 0 dB = arriba, -80 dB = abajo
            float normalized = (db + 80.0f) / 80.0f;  // 0 a 1

            float height_bar = normalized * plot_height;
            float y_bottom = margin_top + plot_height;
            float y_top = y_bottom - height_bar;

            // color: rojo (poco) → amarillo → verde → cyan (mucho)
            juce::Colour bar_colour;
            if (normalized < 0.5f)
            {
                bar_colour = juce::Colour(255, (int)(normalized * 2.0f * 255.0f), 0);
            }
            else
            {
                bar_colour = juce::Colour(
                    (int)((1.0f - (normalized - 0.5f) * 2.0f) * 255.0f),
                    255,
                    (int)((normalized - 0.5f) * 2.0f * 255.0f)
                );
            }

            g.setColour(bar_colour);
            float x = margin_left + k * bin_width;
            g.fillRect(x, y_top, std::max(bin_width, 1.0f), height_bar);
        }
    }

    // ── ejes ──────────────────────────────────────────────────────────────
    g.setColour(juce::Colours::white);
    g.drawLine(margin_left, margin_top, margin_left, margin_top + plot_height, 2.0f);
    g.drawLine(margin_left, margin_top + plot_height,
               margin_left + plot_width, margin_top + plot_height, 2.0f);

    // ── etiquetas Hz ──────────────────────────────────────────────────────
    g.setFont(12.0f);
    g.setColour(juce::Colours::white);
    for (int khz = 0; khz <= 24; khz += 3)
    {
        float x = margin_left + (khz / 24.0f) * plot_width;
        g.drawLine(x, margin_top + plot_height, x, margin_top + plot_height + 5);
        g.drawText(juce::String(khz) + "k",
                   (int)(x - 15), (int)(margin_top + plot_height + 5),
                   30, 20, juce::Justification::centred, false);
    }

    // ── título ────────────────────────────────────────────────────────────
    g.setFont(14.0f);
    g.drawText("Spectrum Analyzer", 10, 5, width - 20, 20,
               juce::Justification::topLeft, false);
}

void SpectrumAnalyzerAudioProcessorEditor::resized()
{
}
