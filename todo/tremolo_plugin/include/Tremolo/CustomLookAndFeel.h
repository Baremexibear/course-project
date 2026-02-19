#pragma once

namespace tremolo {
class CustomLookAndFeel : public juce::LookAndFeel_V4 {
public:
    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    static juce::FontOptions getSideLabelFont() {
        return interMedium().withHeight(12.f);
    }

private:
   static juce::FontOptions interMedium();
};
}  // namespace tremolo
