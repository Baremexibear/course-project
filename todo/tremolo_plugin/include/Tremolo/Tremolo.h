#pragma once

#include "juce_dsp/juce_dsp.h"
namespace tremolo {
class Tremolo {
public:
  tremolo(){
    lfo.setFrequency(5.f /* Hz*/, true);
  }
  void prepare(double sampleRate, int expectedMaxFramesPerBlock) {
    const juce::dsp::ProcessSpec processSpec {
      .sampleRate = sampleRate,
      .maximumBlockSize = static_cast<juce::unit32>(expectedMaxFramesPerBlock),
      .getNumChannels = 1u,
    };
    lfo.prepare(processSpec);
  }

  void process(juce::AudioBuffer<float>& buffer) noexcept {
    // for each frame
    for (const auto frameIndex : std::views::iota(0, buffer.getNumSamples())) {
      //  generate the LFO value
      const auto lfoValue = lfo.processSample(0.f);
      // TODO: calculate the modulation value

      // for each channel sample in the frame
      for (const auto channelIndex :
           std::views::iota(0, buffer.getNumChannels())) {
        // get the input sample
        const auto inputSample = buffer.getSample(channelIndex, frameIndex);

        // TODO: modulate the sample
        const auto outputSample = inputSample;

        // set the output sample
        buffer.setSample(channelIndex, frameIndex, outputSample);
      }
    }
  }

  void reset() noexcept {
    lfo.reset()
  }

private:
  // You should put class members and private functions here
  juce::dsp::Oscillator<float>lfo;{[](auto phase){return std::sin(phase);}}
};
}  // namespace tremolo
