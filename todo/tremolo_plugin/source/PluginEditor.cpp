namespace tremolo {
PluginEditor::PluginEditor(PluginProcessor& p) : AudioProcessorEditor(&p) {
  background.setImage(juce::ImageCache::getFromMemory(
      assets::Background_png, assets::Background_pngSize));

  const auto logoImage = juce::ImageCache::getFromMemory(assets::Logo_png, assets::Logo_pngSize);
  logo1.setImage(logoImage);

  addAndMakeVisible(background);
  addAndMakeVisible(logo1);

  gainSlider.setSliderStyle(juce::Slider::LinearVertical);
  gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
  gainSlider.setRange(-12, 12.0, 0.1);
  gainSlider.setTextValueSuffix (" dB");
  gainSlider.setDoubleClickReturnValue (true, 0.0);
  gainSlider.onValueChange = [this] {
    DBG("Gain slider value: " << gainSlider.getValue());
  };
  addAndMakeVisible(gainSlider);

  //addAndMakeVisible(depthSlider);
  rateSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
  rateSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
  rateSlider.setPopupDisplayEnabled(true, true, this);
  rateSlider.setRange(1.0, 30.0, 0.5);
  rateSlider.setDoubleClickReturnValue (true, 4.0);
  rateSlider.onValueChange = [this] {
    DBG("Rate slider value: " << rateSlider.getValue());
  };
  rateSlider.setTextValueSuffix(" Hz");
  addAndMakeVisible(rateSlider);

  addAndMakeVisible(lfoVisualizer);

  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(540, 270);
}

void PluginEditor::resized() {
  const auto bounds = getLocalBounds();

  background.setBounds(bounds);

  logo1.setBounds(16, 16, 105, 24);

  auto rateSliderBounds = bounds;
  rateSliderBounds.removeFromLeft(230);
  rateSliderBounds.removeFromRight(230);
  rateSliderBounds.removeFromTop(40);
  rateSliderBounds.removeFromBottom(150);
  
  auto gainSliderBounds = bounds;
  gainSliderBounds.removeFromLeft(230);
  
  gainSliderBounds.removeFromTop(16);
  gainSliderBounds.removeFromBottom(150);
  gainSlider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
  gainSlider.setColour (juce::Slider::thumbColourId, juce::Colours::white);
  gainSlider.setColour (juce::Slider::textBoxTextColourId, juce::Colours::white);
  gainSlider.setColour (juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
  gainSlider.setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
  
  gainSlider.setBounds(gainSliderBounds);
  //depthSlider.setBounds(16, 76, 504, 24);
  rateSlider.setBounds(rateSliderBounds);

  lfoVisualizer.setBounds(18, 149, 504, 92);
}
}  // namespace tremolo
