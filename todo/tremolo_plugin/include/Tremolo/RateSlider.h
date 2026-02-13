#pragma once

namespace tremolo {
class RateSlider : public juce::Slider{
public:
    void paint(juce::Graphics& g) override{
    auto sliderPos = (float) valueToProportionOfLength (getValue());
    jassert (sliderPos >= 0 && sliderPos <= 1);

      const auto rotaryParams = getRotaryParameters();
      const auto rotaryStartAngle = rotaryParams.startAngleRadians;
      const auto rotaryEndAngle = rotaryParams.endAngleRadians;

   
      auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
     


      if (isEnabled()) {
        juce::Path valueArc;
        valueArc.addCentredArc(getLocalBounds().getCentreX(), 
                               getLocalBounds().getCentreY(),
                               (getWidth() / 2.0f) - 2.0f,
                               (getHeight() / 2.0f) - 2.0f,
                               0.0f,
                               rotaryStartAngle,
                               toAngle,
                               true);
        
        g.setColour(juce::Colours::orange);
        g.strokePath(valueArc, juce::PathStrokeType(4.0f));
      }
    }
};
}