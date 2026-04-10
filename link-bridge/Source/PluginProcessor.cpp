#include "PluginProcessor.h"
#include "PluginEditor.h"

LinkBridgeProcessor::LinkBridgeProcessor()
    : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}

LinkBridgeProcessor::~LinkBridgeProcessor()
{
}

const juce::String LinkBridgeProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LinkBridgeProcessor::acceptsMidi() const
{
    return false;
}

bool LinkBridgeProcessor::producesMidi() const
{
    return false;
}

bool LinkBridgeProcessor::isMidiEffect() const
{
    return false;
}

double LinkBridgeProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LinkBridgeProcessor::getNumPrograms()
{
    return 1;
}

int LinkBridgeProcessor::getCurrentProgram()
{
    return 0;
}

void LinkBridgeProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String LinkBridgeProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void LinkBridgeProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

void LinkBridgeProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void LinkBridgeProcessor::releaseResources()
{
}

bool LinkBridgeProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void LinkBridgeProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                        juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    if (auto* playHead = getPlayHead())
    {
        if (auto posInfo = playHead->getPosition())
        {
            double newBpm = 120.0;
            double newBeatPos = 0.0;
            bool newIsPlaying = false;
            
            if (auto bpm = posInfo->getBpm())
                newBpm = *bpm;
            
            if (auto ppq = posInfo->getPpqPosition())
                newBeatPos = *ppq;
            
            newIsPlaying = posInfo->getIsPlaying();
            
            currentBpm.store(newBpm);
            beatPosition.store(newBeatPos);
            isPlaying.store(newIsPlaying);
            
            bool tempoChanged = std::abs(newBpm - lastBpm) > 0.01;
            bool playStateChanged = newIsPlaying != lastIsPlaying;
            
            if (tempoChanged && newIsPlaying)
            {
                // Pause -> Update tempo -> Sync phase -> Play
                linkController.setPlaying(false, newBeatPos);
                linkController.setTempo(newBpm);
                linkController.setPlaying(true, newBeatPos);
                
                lastBpm = newBpm;
            }
            else if (tempoChanged)
            {
                linkController.setTempo(newBpm);
                lastBpm = newBpm;
            }
            
            if (playStateChanged)
            {
                linkController.setPlaying(newIsPlaying, newBeatPos);
                lastIsPlaying = newIsPlaying;
            }
        }
    }
}

bool LinkBridgeProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* LinkBridgeProcessor::createEditor()
{
    return new LinkBridgeEditor(*this);
}

void LinkBridgeProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::ignoreUnused(destData);
}

void LinkBridgeProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    juce::ignoreUnused(data, sizeInBytes);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LinkBridgeProcessor();
}
