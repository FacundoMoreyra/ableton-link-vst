#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <ableton/Link.hpp>
#include <atomic>

class LinkController
{
public:
    LinkController(double initialTempo = 120.0, double quantum = 4.0)
        : link(initialTempo), quantum(quantum)
    {
        link.enable(true);
        link.enableStartStopSync(true);
    }

    ~LinkController()
    {
        link.enable(false);
    }

    void setTempo(double bpm)
    {
        if (bpm > 0.0 && bpm < 999.0)
        {
            auto sessionState = link.captureAppSessionState();
            sessionState.setTempo(bpm, link.clock().micros());
            link.commitAppSessionState(sessionState);
        }
    }

    void setPlaying(bool playing, double beatPosition)
    {
        auto now = link.clock().micros();
        auto sessionState = link.captureAppSessionState();
        sessionState.setIsPlaying(playing, now);
        
        if (playing)
        {
            double beatPhase = std::fmod(beatPosition, quantum);
            if (beatPhase < 0) beatPhase += quantum;
            sessionState.forceBeatAtTime(beatPhase, now, quantum);
        }
        
        link.commitAppSessionState(sessionState);
    }

    void syncPhase(double beatPosition)
    {
        auto now = link.clock().micros();
        auto sessionState = link.captureAppSessionState();
        
        double beatPhase = std::fmod(beatPosition, quantum);
        if (beatPhase < 0) beatPhase += quantum;
        sessionState.forceBeatAtTime(beatPhase, now, quantum);
        
        link.commitAppSessionState(sessionState);
    }

    double getTempo() const
    {
        auto sessionState = link.captureAppSessionState();
        return sessionState.tempo();
    }

    bool isPlaying() const
    {
        auto sessionState = link.captureAppSessionState();
        return sessionState.isPlaying();
    }

    std::size_t getNumPeers() const
    {
        return link.numPeers();
    }

    bool isEnabled() const
    {
        return link.isEnabled();
    }

    void setEnabled(bool enabled)
    {
        link.enable(enabled);
    }
    
    void setQuantum(double q)
    {
        quantum = q;
    }

private:
    ableton::Link link;
    double quantum;
};

class LinkBridgeProcessor : public juce::AudioProcessor
{
public:
    LinkBridgeProcessor();
    ~LinkBridgeProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    double getCurrentBpm() const { return currentBpm.load(); }
    bool getIsPlaying() const { return isPlaying.load(); }
    bool isLinkEnabled() const { return linkController.isEnabled(); }
    std::size_t getNumLinkPeers() const { return linkController.getNumPeers(); }
    double getBeatPosition() const { return beatPosition.load(); }
    
    void setLinkEnabled(bool enabled) { linkController.setEnabled(enabled); }

private:
    LinkController linkController;
    
    std::atomic<double> currentBpm{120.0};
    std::atomic<double> beatPosition{0.0};
    std::atomic<bool> isPlaying{false};
    
    double lastBpm{0.0};
    bool lastIsPlaying{false};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LinkBridgeProcessor)
};
