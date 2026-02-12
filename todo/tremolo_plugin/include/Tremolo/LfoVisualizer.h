namespace tremolo {
class LfoVisualizer : public juce::Component {
public:
  void setWaveform(Tremolo::LfoWaveform newWaveform) {
    waveform = newWaveform;
    resized();
  }

  void resized() override {
    wavePath.clear();
    
    const auto halfHeight = getHeight() / 2;
    const auto amplitude = halfHeight - strokeWidth / 2.f;
    const auto width = getWidth();
    const auto extension = 2;

    wavePath.startNewSubPath(-extension, halfHeight + amplitude * getWaveformValue(-extension));

    for (const auto x : std::views::iota(-extension + 1, width + extension)) {
      wavePath.lineTo(x, halfHeight + amplitude * getWaveformValue(x));
    }
  }

  void paint(juce::Graphics& g) override {
    g.setColour(juce::Colours::orange);
    g.strokePath(wavePath, juce::PathStrokeType{strokeWidth});
  }

private:
  float getWaveformValue(int x) const {
    const auto phase = 0.1f * x;
    
    using LfoWaveform = Tremolo::LfoWaveform;
    
    switch (waveform) {
      case LfoWaveform::sine:     return std::sin(phase);
      case LfoWaveform::triangle: return Tremolo::triangle(phase);
      case LfoWaveform::square:   return Tremolo::square(phase);
      case LfoWaveform::sawtooth: return Tremolo::sawtooth(phase);
      default:                    return std::sin(phase);
    }
  }

  juce::Path wavePath;
  float strokeWidth = 4.f;
  Tremolo::LfoWaveform waveform = Tremolo::LfoWaveform::sine;
};
} // namespace tremolo