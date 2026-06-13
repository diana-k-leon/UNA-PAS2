#include "PluginEditor.h"

ReverbAlgoritmicaAudioProcessorEditor::ReverbAlgoritmicaAudioProcessorEditor(
    ReverbAlgoritmicaAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    // Room size
    addAndMakeVisible(roomSizeSlider);
    roomSizeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    roomSizeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    roomSizeSlider.setRange(0.0, 0.98, 0.01);
    roomSizeSlider.setValue(processor.roomSize.load());
    roomSizeSlider.onValueChange = [this] {
        processor.roomSize = (float)roomSizeSlider.getValue();
    };
    addAndMakeVisible(roomSizeLabel);
    roomSizeLabel.setText("Room Size", juce::dontSendNotification);
    roomSizeLabel.attachToComponent(&roomSizeSlider, true);

    // Wet
    addAndMakeVisible(wetSlider);
    wetSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    wetSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    wetSlider.setRange(0.0, 1.0, 0.01);
    wetSlider.setValue(processor.wet.load());
    wetSlider.onValueChange = [this] {
        processor.wet = (float)wetSlider.getValue();
    };
    addAndMakeVisible(wetLabel);
    wetLabel.setText("Wet", juce::dontSendNotification);
    wetLabel.attachToComponent(&wetSlider, true);

    // Dry
    addAndMakeVisible(drySlider);
    drySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    drySlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    drySlider.setRange(0.0, 1.0, 0.01);
    drySlider.setValue(processor.dry.load());
    drySlider.onValueChange = [this] {
        processor.dry = (float)drySlider.getValue();
    };
    addAndMakeVisible(dryLabel);
    dryLabel.setText("Dry", juce::dontSendNotification);
    dryLabel.attachToComponent(&drySlider, true);

    setSize(420, 180);
    startTimer(100);
}

ReverbAlgoritmicaAudioProcessorEditor::~ReverbAlgoritmicaAudioProcessorEditor() {}

void ReverbAlgoritmicaAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff0d0f14));
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(16.0f, juce::Font::bold));
    g.drawFittedText("Reverb Algorítmica — Schroeder",
                     getLocalBounds().removeFromTop(35),
                     juce::Justification::centred, 1);
}

void ReverbAlgoritmicaAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(10);
    area.removeFromTop(35);
    int labelW = 80;
    roomSizeSlider.setBounds(area.removeFromTop(38).withTrimmedLeft(labelW));
    area.removeFromTop(4);
    wetSlider.setBounds(area.removeFromTop(38).withTrimmedLeft(labelW));
    area.removeFromTop(4);
    drySlider.setBounds(area.removeFromTop(38).withTrimmedLeft(labelW));
}
