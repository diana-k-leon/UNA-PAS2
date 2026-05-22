// ============================================================
// PluginEditor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 1 · Plugin: STFT Analyzer
// ============================================================
//
// Espectrograma "waterfall" (scrolling vertical).
//
// Visualización:
//   - Eje X: frecuencia (Hz)
//   - Eje Y: tiempo (arriba = más antiguo, abajo = más reciente)
//   - Color: magnitud (azul oscuro = silencio, rojo/blanco = energía)
//
// Cada línea horizontal es un frame de STFT.
// Scrollea hacia abajo conforme se agregan frames nuevos.
// ============================================================

#include "PluginEditor.h"
#include <cmath>

STFTAnalyzerAudioProcessorEditor::STFTAnalyzerAudioProcessorEditor(
    STFTAnalyzerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(1000, 500);
    startTimerHz(30);
}

STFTAnalyzerAudioProcessorEditor::~STFTAnalyzerAudioProcessorEditor()
{
    stopTimer();
}

void STFTAnalyzerAudioProcessorEditor::timerCallback()
{
    repaint();
}

void STFTAnalyzerAudioProcessorEditor::paint(juce::Graphics& g)
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

    // Copia el historial (evita race conditions)
    const auto& history = audioProcessor.getSpectrogramHistory();
    int history_size = history.size();

    if (history_size > 0)
    {
        const int num_bins = history[0].size();
        float bin_width = plot_width / num_bins;

        // Normalización global
        float max_val = 1e-6f;
        for (int frame = 0; frame < history_size; ++frame)
        {
            for (int k = 0; k < num_bins; ++k)
            {
                max_val = std::max(max_val, history[frame][k]);
            }
        }

        // Dibuja espectrograma
        for (int frame = 0; frame < history_size; ++frame)
        {
            float y_pos = margin_top + (frame / (float)history_size) * plot_height;
            float row_height = plot_height / history_size;

            for (int k = 0; k < num_bins; ++k)
            {
                float mag = history[frame][k] / max_val;
                float db = mag > 1e-6f ? 20.0f * std::log10(mag) : -120.0f;
                db = std::max(0.0f, std::min(60.0f, db + 60.0f));

                // Color gradient: azul → verde → rojo → blanco
                juce::Colour pixel_colour;
                float norm_db = db / 60.0f;  // 0 a 1
                
                if (norm_db < 0.25f)
                {
                    // Azul oscuro a cian
                    float t = norm_db / 0.25f;
                    pixel_colour = juce::Colour(0, (int)(t * 150), (int)(50 + t * 150));
                }
                else if (norm_db < 0.5f)
                {
                    // Cian a verde
                    float t = (norm_db - 0.25f) / 0.25f;
                    pixel_colour = juce::Colour(0, (int)(150 + t * 105), (int)(200 - t * 100));
                }
                else if (norm_db < 0.75f)
                {
                    // Verde a amarillo/naranja
                    float t = (norm_db - 0.5f) / 0.25f;
                    pixel_colour = juce::Colour((int)(t * 200), (int)(255 - t * 150), 0);
                }
                else
                {
                    // Naranja a rojo/blanco
                    float t = (norm_db - 0.75f) / 0.25f;
                    pixel_colour = juce::Colour((int)(200 + t * 55), (int)(105 + t * 150), 0);
                }

                g.setColour(pixel_colour);
                float x = margin_left + k * bin_width;
                g.fillRect(x, y_pos, bin_width, row_height);
            }
        }
    }

    // Ejes
    g.setColour(juce::Colours::white);
    g.drawLine(margin_left, margin_top, margin_left, margin_top + plot_height, 2.0f);
    g.drawLine(margin_left, margin_top + plot_height, margin_left + plot_width, margin_top + plot_height, 2.0f);

    // Etiquetas X (Hz)
    g.setFont(12.0f);
    g.setColour(juce::Colours::white);
    for (int khz = 0; khz <= 24; khz += 3)
    {
        float x = margin_left + (khz / 24.0f) * plot_width;
        g.drawLine(x, margin_top + plot_height, x, margin_top + plot_height + 5);
        g.drawText(juce::String(khz) + "k", (int)(x - 15), (int)(margin_top + plot_height + 5), 30, 20, juce::Justification::centred, false);
    }

    // Título
    g.setFont(14.0f);
    g.drawText("STFT Analyzer (Waterfall)", 10, 5, width - 20, 20, juce::Justification::topLeft, false);
}

void STFTAnalyzerAudioProcessorEditor::resized()
{
}