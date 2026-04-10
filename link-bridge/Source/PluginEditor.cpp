#include "PluginEditor.h"

LinkBridgeEditor::LinkBridgeEditor(LinkBridgeProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    setSize(280, 340);
    setOpaque(true);
    
    bpmFont = juce::Font(juce::FontOptions().withHeight(72.0f).withStyle("Bold"));
    labelFont = juce::Font(juce::FontOptions().withHeight(18.0f));
    smallFont = juce::Font(juce::FontOptions().withHeight(13.0f));
    
    startTimerHz(30);
}

LinkBridgeEditor::~LinkBridgeEditor()
{
    stopTimer();
}

void LinkBridgeEditor::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    
    g.fillAll(juce::Colour(0xff18181b));
    
    g.setColour(juce::Colour(0xff27272a));
    g.fillRoundedRectangle(bounds.reduced(12), 12.0f);
    
    auto contentBounds = bounds.reduced(24);
    
    g.setColour(juce::Colour(0xff71717a));
    g.setFont(smallFont);
    g.drawText("TEMPO", contentBounds.removeFromTop(24), juce::Justification::centred);
    
    g.setColour(juce::Colours::white);
    g.setFont(bpmFont);
    juce::String bpmText = juce::String(processorRef.getCurrentBpm(), 1);
    g.drawText(bpmText, contentBounds.removeFromTop(80), juce::Justification::centred);
    
    g.setColour(juce::Colour(0xff71717a));
    g.setFont(labelFont);
    g.drawText("BPM", contentBounds.removeFromTop(28), juce::Justification::centred);
    
    contentBounds.removeFromTop(20);
    
    bool playing = processorRef.getIsPlaying();
    auto statusBounds = contentBounds.removeFromTop(36);
    auto statusRect = statusBounds.withSizeKeepingCentre(140, 32);
    
    if (playing)
    {
        g.setColour(juce::Colour(0xff22c55e).withAlpha(0.2f));
        g.fillRoundedRectangle(statusRect, 16.0f);
        
        g.setColour(juce::Colour(0xff22c55e));
        auto indicatorBounds = statusRect.withWidth(12).withHeight(12)
            .withX(statusRect.getX() + 14)
            .withY(statusRect.getCentreY() - 6);
        g.fillEllipse(indicatorBounds);
        
        g.setFont(smallFont);
        g.drawText("PLAYING", statusRect.withTrimmedLeft(32), juce::Justification::centred);
    }
    else
    {
        g.setColour(juce::Colour(0xff3f3f46));
        g.fillRoundedRectangle(statusRect, 16.0f);
        
        g.setColour(juce::Colour(0xff52525b));
        auto indicatorBounds = statusRect.withWidth(12).withHeight(12)
            .withX(statusRect.getX() + 14)
            .withY(statusRect.getCentreY() - 6);
        g.fillEllipse(indicatorBounds);
        
        g.setFont(smallFont);
        g.drawText("STOPPED", statusRect.withTrimmedLeft(32), juce::Justification::centred);
    }
    
    contentBounds.removeFromTop(24);
    
    bool linkEnabled = processorRef.isLinkEnabled();
    auto numPeers = processorRef.getNumLinkPeers();
    
    g.setColour(linkEnabled ? juce::Colour(0xff22c55e) : juce::Colour(0xff52525b));
    auto linkIndicator = contentBounds.removeFromTop(20).withSizeKeepingCentre(8, 8);
    g.fillEllipse(linkIndicator);
    
    g.setColour(juce::Colour(0xff71717a));
    g.setFont(smallFont);
    juce::String linkText = linkEnabled ? "Link Enabled" : "Link Disabled";
    g.drawText(linkText, contentBounds.removeFromTop(20), juce::Justification::centred);
    
    g.setColour(juce::Colour(0xff52525b));
    juce::String peersText = juce::String(numPeers) + (numPeers == 1 ? " peer" : " peers") + " connected";
    g.drawText(peersText, contentBounds.removeFromTop(18), juce::Justification::centred);
    
    g.setColour(juce::Colour(0xff52525b));
    g.setFont(juce::Font(juce::FontOptions().withHeight(10.0f)));
    auto creditBounds = getLocalBounds().toFloat().reduced(12);
    g.drawText("by Facundo Moreyra", creditBounds.removeFromBottom(20), juce::Justification::centred);
}

void LinkBridgeEditor::resized()
{
}

void LinkBridgeEditor::timerCallback()
{
    repaint();
}
