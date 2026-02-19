#pragma once

namespace tremolo {
class PluginEditor : public juce::AudioProcessorEditor {
public:
  explicit PluginEditor(PluginProcessor&);
  ~PluginEditor() override;

  void resized() override;

  juce::ImageComponent background;
  juce::ImageComponent logo1;

  juce::ComboBox waveformComboBox;
  juce::ComboBoxParameterAttachment waveformAttachment;
  
  juce::Slider gainSlider;
  juce::SliderParameterAttachment gainAttachment;

  juce::Slider depthSlider;
  juce::SliderParameterAttachment depthAttachment;

  RateSlider rateSlider;
  juce::SliderParameterAttachment rateAttachment;

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
