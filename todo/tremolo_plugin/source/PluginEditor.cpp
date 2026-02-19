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

  lookAndFellComboBox.addItem("LookAndFeel_V1", 1);
  lookAndFellComboBox.addItem("LookAndFeel_V2", 2);
  lookAndFellComboBox.addItem("LookAndFeel_V3", 3);
  lookAndFellComboBox.addItem("LookAndFeel_V4", 4);
  lookAndFellComboBox.addItem("CustomLookAndFeel", 5);
  lookAndFellComboBox.setSelectedId(5);
  lookAndFellComboBox.onChange = [this] {
    switch (lookAndFellComboBox.getSelectedId()) {
      case 1: setLookAndFeel(&lookAndFeelV1); break;
      case 2: setLookAndFeel(&lookAndFeelV2); break;
      case 3: setLookAndFeel(&lookAndFeelV3); break;
      case 4: setLookAndFeel(&lookAndFeelV4); break;
      case 5: setLookAndFeel(&lookAndFeel); break;
      default: break;
    }
  };
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
  depthSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
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

  addAndMakeVisible(waveformLabel);
  addAndMakeVisible(bypassLabel);
  addAndMakeVisible(gainLabel);
  depthLabel.setInterceptsMouseClicks(false, false);
  addAndMakeVisible(depthLabel);
  rateLabel.setInterceptsMouseClicks(false, false);
  addAndMakeVisible(rateLabel);

  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(570, 300);
}

PluginEditor::~PluginEditor() {
  setLookAndFeel(nullptr);
}

void PluginEditor::resized() {
  const auto bounds = getLocalBounds();

  auto backgroundBounds = bounds;
  backgroundBounds.removeFromBottom(30);
  backgroundBounds.removeFromRight(30);
  background.setBounds(backgroundBounds);

  auto waveformComboBoxBounds = bounds;
  waveformComboBoxBounds.removeFromTop(60);
  waveformComboBoxBounds.removeFromLeft(16);
  waveformComboBoxBounds.removeFromRight(400);
  waveformComboBoxBounds.removeFromBottom(200);
  waveformComboBox.setBounds(waveformComboBoxBounds);
  
  auto lookAndFellComboBoxBounds = bounds;
  lookAndFellComboBoxBounds.removeFromTop(270);
  lookAndFellComboBoxBounds.removeFromLeft(270);
  lookAndFellComboBoxBounds.removeFromRight(30);
  lookAndFellComboBoxBounds.removeFromBottom(1);
  lookAndFellComboBox.setBounds(lookAndFellComboBoxBounds);

  auto buttonBounds = bounds;
  buttonBounds.removeFromTop(20);
  buttonBounds.removeFromLeft(345);
  buttonBounds.removeFromRight(65);
  buttonBounds.removeFromBottom(240);
  bypassButton.setBounds(buttonBounds);
  
  logo1.setBounds(16, 16, 105, 24);

  auto depthSliderBounds = bounds;
  depthSliderBounds.removeFromLeft(330);
  depthSliderBounds.removeFromRight(80);
  depthSliderBounds.removeFromTop(70);
  depthSliderBounds.removeFromBottom(168);
  depthSlider.setBounds(depthSliderBounds);

  auto rateSliderBounds = bounds;
  rateSliderBounds.removeFromLeft(230);
  rateSliderBounds.removeFromRight(260);
  rateSliderBounds.removeFromTop(40);
  rateSliderBounds.removeFromBottom(210);
  rateSlider.setBounds(rateSliderBounds);
  
  auto gainSliderBounds = bounds;
  gainSliderBounds.removeFromLeft(540);
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
  lfoCurveWidthSliderBounds.removeFromRight(300);
  lfoCurveWidthSliderBounds.removeFromTop(273);
  lfoCurveWidthSlider.setBounds(lfoCurveWidthSliderBounds); 

  const auto waveformLabelBounds = waveformComboBoxBounds.translated(0, -waveformComboBoxBounds.getHeight());
  waveformLabel.setBounds(waveformLabelBounds);

  const auto bypassLabelBounds = buttonBounds.translated(0, -buttonBounds.getHeight());
  bypassLabel.setBounds(bypassLabelBounds);

  const auto gainLabelBounds = gainSliderBounds.translated(0, -gainSliderBounds.getHeight());
  gainLabel.setBounds(gainLabelBounds);

  depthLabel.setJustificationType(juce::Justification::centred);
  depthLabel.setBounds(depthSliderBounds);

  rateLabel.setJustificationType(juce::Justification::centred);
  rateLabel.setBounds(rateSliderBounds);

  lfoVisualizer.setBounds(18, 149, 504, 92);
}
}  // namespace tremolo
