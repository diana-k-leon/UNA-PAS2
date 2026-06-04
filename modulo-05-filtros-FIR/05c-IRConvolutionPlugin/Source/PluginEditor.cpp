// ============================================================
// PluginEditor.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 3 · Plugin: IR Convolution
// ============================================================
//
// Interfaz simple: zona de drag & drop para cargar IR
// ============================================================

#include "PluginEditor.h"

IRConvolutionAudioProcessorEditor::IRConvolutionAudioProcessorEditor(
    IRConvolutionAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(420, 280);
}

IRConvolutionAudioProcessorEditor::~IRConvolutionAudioProcessorEditor() {}

void IRConvolutionAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Fondo
    g.fillAll(juce::Colour(0xff0d0f14));

    // Título
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(20.0f, juce::Font::bold));
    g.drawText("IR Convolution", getLocalBounds().withHeight(50),
               juce::Justification::centred, true);

    // Zona de drop
    float zx = 40, zy = 70, zw = getWidth() - 80, zh = 130;

    if (dragging_over)
    {
        g.setColour(juce::Colour(0xff4fc9a4).withAlpha(0.15f));
        g.fillRoundedRectangle(zx, zy, zw, zh, 10);
        g.setColour(juce::Colour(0xff4fc9a4));
    }
    else if (audioProcessor.isIRLoaded())
    {
        g.setColour(juce::Colour(0xff7c6dfa).withAlpha(0.1f));
        g.fillRoundedRectangle(zx, zy, zw, zh, 10);
        g.setColour(juce::Colour(0xff7c6dfa));
    }
    else
    {
        g.setColour(juce::Colour(0xff2a2f42));
        g.fillRoundedRectangle(zx, zy, zw, zh, 10);
        g.setColour(juce::Colour(0xff3a3f55));
    }

    g.drawRoundedRectangle(zx, zy, zw, zh, 10, 1.5f);

    // Ícono y texto
    g.setFont(32.0f);
    g.drawText(audioProcessor.isIRLoaded() ? "✓" : "🎈", 
               zx, zy + 15, zw, 40, juce::Justification::centred, true);

    g.setFont(13.0f);
    if (audioProcessor.isIRLoaded())
    {
        g.setColour(juce::Colour(0xff7c6dfa));
        g.drawText(audioProcessor.getIRName(),
                   zx, zy + 60, zw, 24, juce::Justification::centred, true);
        g.setColour(juce::Colour(0xff8b90a8));
        g.setFont(11.0f);
        g.drawText("Arrastrá otro archivo para cambiar la IR",
                   zx, zy + 88, zw, 20, juce::Justification::centred, true);
    }
    else
    {
        g.setColour(juce::Colour(0xff8b90a8));
        g.drawText("Arrastrá un archivo WAV de IR acá",
                   zx, zy + 60, zw, 24, juce::Justification::centred, true);
        g.setFont(11.0f);
        g.drawText("freesound.org → buscar \"impulse response\"",
                   zx, zy + 85, zw, 20, juce::Justification::centred, true);
    }

    // Estado abajo
    g.setFont(11.0f);
    g.setColour(juce::Colour(0xff8b90a8));
    g.drawText(audioProcessor.isIRLoaded()
               ? "Convolución activa — señal procesada con IR real"
               : "Sin IR cargada — señal sin procesar",
               0, getHeight() - 35, getWidth(), 25,
               juce::Justification::centred, true);
}

void IRConvolutionAudioProcessorEditor::resized() {}

bool IRConvolutionAudioProcessorEditor::isInterestedInFileDrag(
    const juce::StringArray& files)
{
    // Solo acepta archivos WAV o AIFF
    for (auto& f : files)
    {
        juce::File file(f);
        if (file.hasFileExtension("wav") || file.hasFileExtension("aiff") ||
            file.hasFileExtension("aif"))
            return true;
    }
    return false;
}

void IRConvolutionAudioProcessorEditor::filesDropped(
    const juce::StringArray& files, int, int)
{
    dragging_over = false;

    if (files.isEmpty()) return;

    juce::File file(files[0]);
    if (file.existsAsFile())
    {
        audioProcessor.loadIR(file);
        repaint();
    }
}
