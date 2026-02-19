namespace tremolo {
    
    void CustomLookAndFeel::drawToggleButton(juce::Graphics& g,
                                             juce::ToggleButton& button,
                                             bool shouldDrawButtonAsHighlighted, 
                                             bool shouldDrawButtonAsDown) {
      const auto bounds =  button.getLocalBounds().reduced(2);
      const auto opacity = shouldDrawButtonAsHighlighted ? 0.7f : 1.0f;

      // Draw gradient outline
      auto outlineGradient = juce::ColourGradient::vertical(juce::Colour{0xFF042D4E},
                                                             juce::Colour{0xFF1A417C},
                                                             bounds);
      outlineGradient.addColour(0.31, juce::Colour{0xFF00385A});
      g.setGradientFill(outlineGradient);
      g.fillRoundedRectangle(bounds.toFloat(), 6.f);

      // Draw button fill inside the outline
      const auto innerBounds = bounds.reduced(3);

      if (button.getToggleState()) {
        auto buttonGradient = juce::ColourGradient::vertical(juce::Colour{0xFFFF901A}.withAlpha(opacity), 
                                                           juce::Colour{0xFFFFC300}.withAlpha(opacity),innerBounds);
      g.setGradientFill(buttonGradient);

      g.fillRoundedRectangle(innerBounds.toFloat(), 4.f);

      g.setColour(juce::Colour{0xFF501A0B}.withAlpha(opacity));
      }
        else {

      auto buttonGradient = juce::ColourGradient::vertical(juce::Colour{0xFF4A7090}.withAlpha(opacity), 
                                                           juce::Colour{0xFF324358}.withAlpha(opacity),innerBounds);
      buttonGradient.addColour(0.73, juce::Colour{0xFF315160}.withAlpha(opacity));
      g.setGradientFill(buttonGradient);
      g.fillRoundedRectangle(innerBounds.toFloat(), 4.f);

      g.setColour(juce::Colour{0xFFDDECFF}.withAlpha(opacity));
        }
      g.setFont(interMeduim().withHeight(19.f));
      g.drawText(button.getButtonText(), bounds, juce::Justification::centred, false);
    }
    juce::FontOptions CustomLookAndFeel::interMedium() {
      static const auto result = juce::Typeface::createSystemTypefaceFor(assets::Inter_Medium_ttf, assets::Inter_Medium_ttfSize);

      return juce::FontOptions(result);
    }
    
}  // namespace tremolo
