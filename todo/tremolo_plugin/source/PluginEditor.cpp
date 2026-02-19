namespace tremolo {
PluginEditor::PluginEditor(PluginProcessor& p) : 
  AudioProcessorEditor(&p), 
  waveformAttachment{p.getParameterRefs().waveform, waveformComboBox},
  gainAttachment{p.getParameterRefs().gain, gainSlider},
  depthAttachment{p.getParameterRefs().depth, depthSlider},
  rateAttachment{p.getParameterRefs().rate, rateSlider},
  bypassAttachment{p.getParameterRefs().bypassed, bypassButton} {
  background.setImage(juce::ImageCache::getFromMemory(
      assets::Background_png, assets::Background_pngSize));

  const auto logoImage = juce::ImageCache::getFromMemory(assets::Logo_png, assets::Logo_pngSize);
  logo1.setImage(logoImage);

  addAndMakeVisible(background);
  addAndMakeVisible(logo1);

  waveformComboBox.addItemList(p.getParameterRefs().waveform.choices, 1);
  waveformAttachment.sendInitialUpdate();
  addAndMakeVisible(waveformComboBox);

  addAndMakeVisible(lookAndFellComboBox);

  lfoCurveWidthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
  lfoCurveWidthSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 60, 20);
  lfoCurveWidthSlider.setRange(0, 10, 1);
  lfoCurveWidthSlider.setDoubleClickReturnValue (true, 4);
  lfoCurveWidthSlider.onValueChange = [this] {
  lfoVisualizer.setStrokeWidth(static_cast<float>(lfoCurveWidthSlider.getValue()));
  };
  addAndMakeVisible(lfoCurveWidthSlider);

  gainSlider.setSliderStyle(juce::Slider::LinearVertical);
  gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
  gainSlider.setTextValueSuffix (" dB");
  gainSlider.setDoubleClickReturnValue (true, 0.0);
  addAndMakeVisible(gainSlider);

  bypassButton.onClick = [this] {
    bypassButton.setButtonText(bypassButton.getToggleState() ? "Bypass" : "Off");
  };
  bypassButton.onClick();
  addAndMakeVisible(bypassButton);

  depthSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
  depthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
  depthSlider.setTextValueSuffix (" %");
  depthSlider.setDoubleClickReturnValue (true, 0.4);
  addAndMakeVisible(depthSlider);

  rateSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
  rateSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
  rateSlider.setPopupDisplayEnabled(true, true, this);
  rateSlider.setDoubleClickReturnValue (true, 5.0);
  rateSlider.setTextValueSuffix(" Hz");
  addAndMakeVisible(rateSlider);

  addAndMakeVisible(lfoVisualizer);

  setLookAndFeel(&lookAndFeel);

  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(540, 300);
}

PluginEditor::~PluginEditor() {
  setLookAndFeel(nullptr);
}

void PluginEditor::resized() {
  const auto bounds = getLocalBounds();

  auto backgroundBounds = bounds;
  backgroundBounds.removeFromBottom(30);
  background.setBounds(backgroundBounds);

  auto waveformComboBoxBounds = bounds;
  waveformComboBoxBounds.removeFromTop(40);
  waveformComboBoxBounds.removeFromLeft(16);
  waveformComboBoxBounds.removeFromRight(392);
  waveformComboBoxBounds.removeFromBottom(176);
  waveformComboBox.setBounds(waveformComboBoxBounds);
  
  auto lookAndFellComboBoxBounds = bounds;
  lookAndFellComboBoxBounds.removeFromTop(270);
  lookAndFellComboBoxBounds.removeFromLeft(420);
  lookAndFellComboBoxBounds.removeFromRight(5);
  lookAndFellComboBoxBounds.removeFromBottom(1);
  lookAndFellComboBox.setBounds(lookAndFellComboBoxBounds);

  auto buttonBounds = bounds;
  buttonBounds.removeFromTop(270);
  buttonBounds.removeFromLeft(420);
  buttonBounds.removeFromRight(5);
  bypassButton.setBounds(buttonBounds);
  
  logo1.setBounds(16, 16, 105, 24);

  auto depthSliderBounds = bounds;
  depthSliderBounds.removeFromLeft(275);
  depthSliderBounds.removeFromRight(60);
  depthSliderBounds.removeFromTop(50);
  depthSliderBounds.removeFromBottom(180);
  depthSlider.setBounds(depthSliderBounds);

  auto rateSliderBounds = bounds;
  rateSliderBounds.removeFromLeft(230);
  rateSliderBounds.removeFromRight(230);
  rateSliderBounds.removeFromTop(40);
  rateSliderBounds.removeFromBottom(210);
  rateSlider.setBounds(rateSliderBounds);
  
  auto gainSliderBounds = bounds;
  gainSliderBounds.removeFromLeft(390);
  gainSliderBounds.removeFromTop(16);
  gainSliderBounds.removeFromBottom(180);
  gainSlider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
  gainSlider.setColour (juce::Slider::thumbColourId, juce::Colours::white);
  gainSlider.setColour (juce::Slider::textBoxTextColourId, juce::Colours::white);
  gainSlider.setColour (juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
  gainSlider.setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
  gainSlider.setBounds(gainSliderBounds);

  auto lfoCurveWidthSliderBounds = bounds;
  lfoCurveWidthSliderBounds.removeFromLeft(0);
  lfoCurveWidthSliderBounds.removeFromRight(270);
  lfoCurveWidthSliderBounds.removeFromTop(273);
  lfoCurveWidthSlider.setBounds(lfoCurveWidthSliderBounds); 

  lfoVisualizer.setBounds(18, 149, 504, 92);
}
}  // namespace tremolo
