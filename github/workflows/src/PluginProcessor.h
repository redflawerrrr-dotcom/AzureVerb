#pragma once
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_dsp/juce_dsp.h>
#include "ReverbEngine.h"

class AzureVerbProcessor : public juce::AudioProcessor
{
public:
    AzureVerbProcessor();
    ~AzureVerbProcessor() override = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Azure Verb"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 9.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    static juce::AudioProcessorValueTreeState::ParameterLayout createLayout();
    juce::AudioProcessorValueTreeState apvts;

private:
    using Duplicator = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                                      juce::dsp::IIR::Coefficients<float>>;
    ReverbEngine engine;
    Duplicator hp, lowShelf, midPeak, highShelf;
    juce::AudioBuffer<float> wet;
    juce::SmoothedValue<float> mixSm;

    double sr = 44100.0;
    float env = 0.0f, atk = 0.01f, rel = 0.001f;
    float lastHp = -1.0f, lastLow = -999.0f, lastMid = -999.0f, lastHigh = -999.0f;
    std::atomic<float> *pMode = nullptr, *pMix = nullptr, *pDecay = nullptr, *pPre = nullptr,
                       *pLow = nullptr, *pMid = nullptr, *pHigh = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AzureVerbProcessor)
};
