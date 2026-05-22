// ============================================================
// PluginEditor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 1 · Plugin: Spectrum Analyzer
// ============================================================
// Implementación de la ventana del plugin.
//
// startTimerHz(30) arranca un timer que llama timerCallback() 30 veces
// por segundo. Cada llamada dispara repaint() que llama a paint().
//
// paint() lee spectrum[] del Processor y dibuja:
//   - Una barra por cada bin de frecuencia
//   - Color según magnitud: rojo (poco) → amarillo → verde → cyan (mucho)
//   - Eje Y en dB, eje X en kHz
// ============================================================

#include "PluginEditor.h"

// ─── CONSTRUCTOR ──────────────────────────────────────────────────────────────
SpectrumAnalyzerAudioProcessorEditor::SpectrumAnalyzerAudioProcessorEditor(
    SpectrumAnalyzerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
    // guarda la referencia al Processor en audioProcessor
{
    setSize(1000, 400);    // ventana de 1000x400 píxeles
    startTimerHz(30);      // arrancar timer a 30fps
}

SpectrumAnalyzerAudioProcessorEditor::~SpectrumAnalyzerAudioProcessorEditor()
{
    stopTimer();           // parar el timer al cerrar
}

// ─── TIMER ────────────────────────────────────────────────────────────────────
void SpectrumAnalyzerAudioProcessorEditor::timerCallback()
{
    repaint();
    // cada 33ms (30fps) dice "redibujá la pantalla"
    // eso dispara paint() abajo
}

// ─── PAINT — DIBUJA TODO ──────────────────────────────────────────────────────
void SpectrumAnalyzerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    // fondo negro

    // ── márgenes del gráfico ──────────────────────────────────────────────
    auto bounds = getLocalBounds();
    float width = bounds.getWidth();
    float height = bounds.getHeight();
    float margin_left = 50.0f;
    float margin_bottom = 50.0f;
    float margin_top = 20.0f;
    float margin_right = 20.0f;
    float plot_width = width - margin_left - margin_right;
    float plot_height = height - margin_top - margin_bottom;

    // ── grilla de dB (líneas horizontales) ────────────────────────────────
    g.setColour(juce::Colours::grey.darker());
    g.setFont(12.0f);
    for (int db = 0; db <= 60; db += 12)
    {
        float y = margin_top + (1.0f - db / 60.0f) * plot_height;
        g.drawLine(margin_left, y, margin_left + plot_width, y);
        // línea horizontal cada 12 dB

        juce::String label = juce::String(db) + " dB";
        g.drawText(label, 5, (int)y - 10, 40, 20,
                   juce::Justification::centredRight, false);
        // etiqueta "0 dB", "12 dB", etc a la izquierda
    }

    // ── leer el espectro del Processor ────────────────────────────────────
    const auto& spectrum = audioProcessor.getSpectrum();
    int num_bins = spectrum.size();

    if (num_bins > 0)
    {
        // buscar el valor máximo para normalizar
        float max_val = 0.0f;
        for (float val : spectrum)
            max_val = std::max(max_val, val);
        if (max_val < 1e-6f) max_val = 1.0f;
        // si todo es silencio, evitar dividir por cero

        float bin_width = plot_width / num_bins;
        // ancho de cada barra en píxeles

        // ── dibujar una barra por cada bin ────────────────────────────────
        for (int k = 0; k < num_bins; ++k)
        {
            float mag = spectrum[k] / max_val;
            // normalizar: 0.0 a 1.0

            // convertir a dB: 20 * log10(magnitud)
            // rango: -120 dB (silencio) a 0 dB (máximo)
            // después lo desplazamos a 0-60 para dibujar
            float db = mag > 1e-6f ? 20.0f * std::log10(mag) : -120.0f;
            db = std::max(0.0f, std::min(60.0f, db + 60.0f));

            float height_bar = (db / 60.0f) * plot_height;
            // altura de la barra proporcional a los dB
            float y_bottom = margin_top + plot_height;
            float y_top = y_bottom - height_bar;

            // ── color según magnitud ──────────────────────────────────────
            // mag < 0.5: rojo → amarillo (poca energía)
            // mag > 0.5: amarillo → verde → cyan (mucha energía)
            juce::Colour bar_colour;
            if (mag < 0.5f)
            {
                bar_colour = juce::Colour(255, (int)(mag * 2.0f * 255.0f), 0);
                // rojo fijo, verde sube
            }
            else
            {
                bar_colour = juce::Colour(
                    (int)((1.0f - (mag - 0.5f) * 2.0f) * 255.0f),
                    255,
                    (int)((mag - 0.5f) * 2.0f * 255.0f)
                );
                // rojo baja, verde fijo, azul sube
            }

            g.setColour(bar_colour);
            float x = margin_left + k * bin_width;
            g.fillRect(x, y_top, bin_width, height_bar);
            // dibujar la barra
        }
    }

    // ── ejes ──────────────────────────────────────────────────────────────
    g.setColour(juce::Colours::white);
    g.drawLine(margin_left, margin_top, margin_left, margin_top + plot_height, 2.0f);
    // eje Y (vertical)
    g.drawLine(margin_left, margin_top + plot_height,
               margin_left + plot_width, margin_top + plot_height, 2.0f);
    // eje X (horizontal)

    // ── etiquetas de frecuencia (eje X) ───────────────────────────────────
    g.setFont(12.0f);
    g.setColour(juce::Colours::white);
    for (int khz = 0; khz <= 24; khz += 3)
    {
        float x = margin_left + (khz / 24.0f) * plot_width;
        g.drawLine(x, margin_top + plot_height, x, margin_top + plot_height + 5);
        g.drawText(juce::String(khz) + "k",
                   (int)(x - 15), (int)(margin_top + plot_height + 5),
                   30, 20, juce::Justification::centred, false);
        // "0k", "3k", "6k", ... "24k" Hz
    }

    // ── título ────────────────────────────────────────────────────────────
    g.setFont(14.0f);
    g.drawText("Spectrum Analyzer", 10, 5, width - 20, 20,
               juce::Justification::topLeft, false);
}

void SpectrumAnalyzerAudioProcessorEditor::resized()
{
    // no hacemos nada — tamaño fijo
}
