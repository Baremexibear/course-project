namespace tremolo {
  namespace {
    struct SerializableParameters {
      float rate;
      float gain;
      juce::String waveform;
      bool bypassed;
      float depth;

      static constexpr auto marshallingVersion = 1;

      template <typename Archive, typename T>
      static void serialise(Archive& archive, T& t){
        using namespace juce;

        if (archive.getVersion() != 1) {
          return;
        }

        std::string pluginName = TREMOLO_PLUGIN_NAME;

        archive(named("pluginName", pluginName));

        if(pluginName != TREMOLO_PLUGIN_NAME) {
          return;
        }
        
        archive(named("modulationRateHz", t.rate), named("bypassed", t.bypassed), named("modulationWaveform", t.waveform), named("gain", t.gain), named("modulationDepth", t.depth));
      }
    };
    SerializableParameters from(const tremolo::Parameters& parameters) {
      return {
        .rate = parameters.rate.get(),
        .gain = parameters.gain.get(),
        .waveform = parameters.waveform.getCurrentChoiceName(),
        .bypassed = parameters.bypassed.get(),
        .depth = parameters.depth.get(),
      };
    }
  }
void JsonSerializer::serialize(const Parameters& parameters,
                               juce::OutputStream& output) {
 const auto parametersToSerialize = from(parameters);

 const auto json = juce::ToVar::convert(parametersToSerialize);

 if (!json.has_value()) {
   return;
 }

 juce::JSON::writeToStream(output, *json, juce::JSON::FormatOptions{}.withSpacing(juce::JSON::Spacing::multiLine).withMaxDecimalPlaces(2));
}

juce::Result JsonSerializer::deserialize(juce::InputStream& input,
                                         Parameters& parameters) {
      juce::var parsedResult;
      const auto result = juce::JSON::parse(input.readEntireStreamAsString(), parsedResult);     
      
      if (result.failed()) {
        return result;
      }
      
      const auto parsedParameters = juce::FromVar::convert<SerializableParameters>(parsedResult);

      if(!parsedParameters.has_value()) {
        return juce::Result::fail("failed to parse parameters from JSON representation");
      }
      const auto modulationWaveformIndex = parameters.waveform.choices.indexOf(
        parsedParameters->waveform);
      
      if (modulationWaveformIndex < 0) {
    // don't update parameters if modulation waveform name is invalid
    return juce::Result::fail(
        "invalid modulation waveform name; supported values are: " +
        parameters.waveform.choices.joinIntoString(", "));
      }
      
      parameters.waveform = modulationWaveformIndex;
      parameters.rate = parsedParameters->rate;
      parameters.gain = parsedParameters->gain;
      parameters.bypassed = parsedParameters->bypassed;
      parameters.depth = parsedParameters->depth;
      
      return juce::Result::ok();
}
}  // namespace tremolo
