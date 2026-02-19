#pragma once

namespace tremolo {
class PluginEditor : public juce::AudioProcessorEditor {
public:
  explicit PluginEditor(PluginProcessor&);
  ~PluginEditor() override;

  void resized() override;

  juce::ImageComponent background;
  juce::ImageComponent logo1;

  juce::Label waveformLabel{"waveform label", "WAVEFORM"};
  juce::ComboBox waveformComboBox;
  juce::ComboBoxParameterAttachment waveformAttachment;
  
  juce::Label gainLabel{"gain label", "GAIN"};
  juce::Slider gainSlider;
  juce::SliderParameterAttachment gainAttachment;

  juce::Label depthLabel{"depth label", "DEPTH"};
  juce::Slider depthSlider;
  juce::SliderParameterAttachment depthAttachment;

  juce::Label rateLabel{"rate label", "RATE"};
  RateSlider rateSlider;
  juce::SliderParameterAttachment rateAttachment;

  juce::Label bypassLabel{"bypass label", "BYPASS"};
  juce::ToggleButton bypassButton;
  juce::ButtonParameterAttachment bypassAttachment;

  juce::ComboBox lookAndFellComboBox;

  juce::Slider lfoCurveWidthSlider;

  LfoVisualizer lfoVisualizer;

  juce::LookAndFeel_V1 lookAndFeelV1;
  juce::LookAndFeel_V2 lookAndFeelV2;
  juce::LookAndFeel_V3 lookAndFeelV3;
  juce::LookAndFeel_V4 lookAndFeelV4;
  CustomLookAndFeel lookAndFeel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
}  // namespace tremolo
