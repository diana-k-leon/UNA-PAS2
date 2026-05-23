// ============================================================
// PluginEditor.cpp — MiTemplatePlugin
// Programación Aplicada al Sonido II — UNA
// ============================================================
// La CARA del plugin — dibuja el espectro en pantalla.
//
// Cómo funciona:
//   1. startTimerHz(30) → arranca un reloj que suena 30 veces por segundo
//   2. Cada vez que suena → timerCallback() → repaint()
//   3. repaint() dispara paint() → dibuja todo de nuevo
//
// paint() lee spectrum[] del Processor y dibuja una barra
// de color por cada bin de frecuencia.
// ============================================================

#include "PluginEditor.h"
#include <cmath>

// ─── CONSTRUCTOR ──────────────────────────────────────────────────────────────
MiTemplatePluginAudioProcessorEditor::MiTemplatePluginAudioProcessorEditor(
    MiTemplatePluginAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(1000, 400);   // ventana de 1000 x 400 píxeles
    startTimerHz(30);     // redibujar 30 veces por segundo
}

// ─── DESTRUCTOR ───────────────────────────────────────────────────────────────
MiTemplatePluginAudioProcessorEditor::~MiTemplatePluginAudioProcessorEditor()
{
    stopTimer();          // parar el reloj al cerrar
}

// ─── TIMER ────────────────────────────────────────────────────────────────────
void MiTemplatePluginAudioProcessorEditor::timerCallback()
{
    repaint();            // "redibujá la pantalla" → dispara paint()
}

// ─── PAINT — DIBUJA TODO ──────────────────────────────────────────────────────
void MiTemplatePluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    // fondo negro
    g.fillAll(juce::Colours::black);

    // márgenes del gráfico
    auto bounds = getLocalBounds();
    float width = bounds.getWidth();
    float height = bounds.getHeight();
    float margin_left = 50.0f;
    float margin_bottom = 50.0f;
    float margin_top = 20.0f;
    float margin_right = 20.0f;
    float plot_width = width - margin_left - margin_right;
    float plot_height = height - margin_top - margin_bottom;

    // grilla de dB (líneas horizontales cada 20 dB)
    g.setColour(juce::Colours::grey.darker());
    g.setFont(12.0f);
    for (int i = 0; i <= 4; i++)
    {
        float db_val = -i * 20.0f;
        float y = margin_top + (float)i / 4.0f * plot_height;
        g.drawLine(margin_left, y, margin_left + plot_width, y);
        g.drawText(juce::String((int)db_val) + " dB", 0, (int)y - 10, 48, 20,
                   juce::Justification::centredRight, false);
    }

    // leer espectro del Processor
    const auto& spectrum = audioProcessor.getSpectrum();
    int num_bins = audioProcessor.getFFTSize() / 2;

    if (num_bins > 0 && (int)spectrum.size() >= num_bins)
    {
        // buscar máximo para normalizar
        float max_val = 0.0f;
        for (int k = 0; k < num_bins; ++k)
            max_val = std::max(max_val, spectrum[k]);
        if (max_val < 1e-10f) max_val = 1.0f;

        float bin_width = plot_width / (float)num_bins;

        // dibujar una barra por cada bin
        for (int k = 0; k < num_bins; ++k)
        {
            float mag = spectrum[k];
            if (mag < 1e-10f) continue;

            // dB relativo al máximo (el pico más alto = 0 dB)
            float db = 20.0f * std::log10(mag / max_val);
            if (db < -80.0f) db = -80.0f;

            // convertir a posición Y
            float normalized = (db + 80.0f) / 80.0f;
            float height_bar = normalized * plot_height;
            float y_top = margin_top + plot_height - height_bar;

            // color según magnitud
            juce::Colour bar_colour;
            if (normalized < 0.5f)
                bar_colour = juce::Colour(255, (int)(normalized * 2.0f * 255.0f), 0);
            else
                bar_colour = juce::Colour(
                    (int)((1.0f - (normalized - 0.5f) * 2.0f) * 255.0f),
                    255, (int)((normalized - 0.5f) * 2.0f * 255.0f));

            g.setColour(bar_colour);
            g.fillRect(margin_left + k * bin_width, y_top,
                       std::max(bin_width, 1.0f), height_bar);
        }
    }

    // ejes
    g.setColour(juce::Colours::white);
    g.drawLine(margin_left, margin_top, margin_left, margin_top + plot_height, 2.0f);
    g.drawLine(margin_left, margin_top + plot_height,
               margin_left + plot_width, margin_top + plot_height, 2.0f);

    // etiquetas de frecuencia
    g.setFont(12.0f);
    for (int khz = 0; khz <= 24; khz += 3)
    {
        float x = margin_left + (khz / 24.0f) * plot_width;
        g.drawLine(x, margin_top + plot_height, x, margin_top + plot_height + 5);
        g.drawText(juce::String(khz) + "k", (int)(x - 15),
                   (int)(margin_top + plot_height + 5), 30, 20,
                   juce::Justification::centred, false);
    }

    // título
    g.setFont(14.0f);
    g.drawText("Mi Template Plugin", 10, 5, width - 20, 20,
               juce::Justification::topLeft, false);
}

void MiTemplatePluginAudioProcessorEditor::resized() {}
