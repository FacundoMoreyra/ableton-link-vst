#pragma once

#include "PluginProcessor.h"
#include <juce_gui_basics/juce_gui_basics.h>

class LinkBridgeEditor : public juce::AudioProcessorEditor,
                         private juce::Timer
{
public:
    explicit LinkBridgeEditor(LinkBridgeProcessor&);
    ~LinkBridgeEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    LinkBridgeProcessor& processorRef;
    
    juce::Font bpmFont;
    juce::Font labelFont;
    juce::Font smallFont;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LinkBridgeEditor)
};
