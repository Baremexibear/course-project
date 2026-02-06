#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_dsp/juce_dsp.h"
namespace tremolo {
enum class ApplySmoothing { no, yes };
class Tremolo {
public:
enum class LfoWaveform : size_t {
  sine = 0,
  triangle = 1,
  square = 2,
  sawtooth = 3,

};
  Tremolo() {
    setModulationRateHz(5.f, ApplySmoothing::no);
  }
  void prepare(double sampleRate, int expectedMaxFramesPerBlock) {
    const juce::dsp::ProcessSpec processSpec {
      .sampleRate = sampleRate,
      .maximumBlockSize = static_cast<juce::uint32>(expectedMaxFramesPerBlock),
      .numChannels = 1u,
    };

    lfoTransitionSmoother.reset(sampleRate, 0.025 /* 25 milliseconds */);
    gainSmoother.reset(sampleRate, 0.025 /* 25 milliseconds */);

    // allocate defensively
    lfoSamples.resize(4u * static_cast<size_t>(expectedMaxFramesPerBlock));

    for(auto& lfo: lfos){
      lfo.prepare(processSpec);
    }
  }
  void setModulationRateHz(
      float rateHz,
      ApplySmoothing applySmoothing = ApplySmoothing::yes) noexcept {
    const auto force = applySmoothing == ApplySmoothing::no;
    for (auto& lfo : lfos) {
      lfo.setFrequency(rateHz, force);
    }
  }
  void setGain(float gainDb) noexcept {
    const auto gainLinear = juce::Decibels::decibelsToGain(gainDb);
    gainSmoother.setTargetValue(gainLinear);
  }
  void setLfoWaveform(LfoWaveform waveform,
                      ApplySmoothing applySmoothing = ApplySmoothing::yes) {
    jassert(waveform == LfoWaveform::sine || waveform == LfoWaveform::triangle || waveform == LfoWaveform::square || waveform == LfoWaveform::sawtooth);

    lfoToSet = waveform;

    if (applySmoothing == ApplySmoothing::no) {
      currentLfo = waveform;
    }
  }

  void process(juce::AudioBuffer<float>& buffer) noexcept {
    updateLfoWaveform();
    // for each frame
    for (const auto frameIndex : std::views::iota(0, buffer.getNumSamples())) {
      //  generate the LFO value
      const auto lfoValue = getNextLfoValue();
     
      //  calculate the modulation value
      constexpr auto modulationDepth = 0.4f;
      const auto modulationValue = modulationDepth * lfoValue + 1.f;

      // get the smoothed gain value for this frame
      const auto gainValue = gainSmoother.getNextValue();

      // for each channel sample in the frame
      for (const auto channelIndex :
           std::views::iota(0, buffer.getNumChannels())) {
        // get the input sample
        const auto inputSample = buffer.getSample(channelIndex, frameIndex);

        // modulate the sample and apply gain
        const auto outputSample = inputSample * modulationValue * gainValue;

        // set the output sample
        buffer.setSample(channelIndex, frameIndex, outputSample);
      }
    }
  }

  void reset() noexcept {
    for(auto& lfo: lfos){
      lfo.reset();
    }
  }

private:
  static float triangle(float phase) {
    const auto ft = phase / juce::MathConstants<float>::twoPi;
    return 4.f * std::abs(ft - std::floor(ft + 0.5f)) - 1.f;
  }

  static float square(float phase) {
    return std::sin(phase) < 0.f ? 1.f : -1.f;
  }

  static float sawtooth(float phase) {
    const auto offsetPhase = phase + juce::MathConstants<float>::pi;
    const auto ft = offsetPhase / juce::MathConstants<float>::twoPi;
    return 2.f * (ft - std::floor(ft)) - 1.f;
  }

  float getNextLfoValue() noexcept {
    if (lfoTransitionSmoother.isSmoothing()) {
      return lfoTransitionSmoother.getNextValue();
    }
    // the argument is added to the generated sample, thus, we pass in 0
    // to get just the generated sample
    return lfos[juce::toUnderlyingType(currentLfo)].processSample(0.f);
  }
  
 
  void updateLfoWaveform() {
    if (currentLfo != lfoToSet) {
      lfoTransitionSmoother.setCurrentAndTargetValue(getNextLfoValue());
      
      currentLfo = lfoToSet;

      // initiate smoothing
      lfoTransitionSmoother.setTargetValue(getNextLfoValue());
    }
  }
  std::array<juce::dsp::Oscillator<float>, 4u> lfos{
    juce::dsp::Oscillator<float>{[](auto phase){return std::sin(phase);}},
    juce::dsp::Oscillator<float>{triangle},
    juce::dsp::Oscillator<float>{square},
    juce::dsp::Oscillator<float>{sawtooth},
  };
  LfoWaveform currentLfo = LfoWaveform::sine;
  LfoWaveform lfoToSet = currentLfo;

  juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear>
      lfoTransitionSmoother{0.f};
  juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear>
      gainSmoother{1.f};
  std::vector<float> lfoSamples;
};
}  // namespace tremolo
