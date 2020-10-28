//
// Created by Daniele Filaretti on 28/10/2020.
//

#pragma once

#include <JuceHeader.h>
#include "data_structures.h"

class MyComponent : public Component
{
    void paint (Graphics& g) override { g.fillAll (Colours::red); }
};

class PageView : public Component
{
public:
    class TitleView : public Component
    {
    private:
        void paint (Graphics& g) override { g.fillAll (Colours::pink); }
    };

    class PageContentView : public Component
    {
    public:
//        explicit PageContentView (const PageContent& content) : content{ content }
//        {
//
//        }

    private:
        void paint (Graphics& g) override { g.fillAll (Colours::orange); }

        //const PageContent& content;
    };

    PageView()
    {
        addAndMakeVisible(title);
        addAndMakeVisible(content);
    }

private:
    void paint (Graphics& g) override { g.fillAll (Colours::green); }

    void resized() override
    {
        auto area = getLocalBounds();
        const auto titleArea = area.removeFromTop(35);
        title.setBounds(titleArea);
        content.setBounds(area);
    }

    TitleView title;
    PageContentView content;
};

class NavigationView : public Component
{
public:
    NavigationView()
    {
        addAndMakeVisible (back);
        addAndMakeVisible (next);
        addAndMakeVisible (skip);
        addAndMakeVisible (finish);
    }

    bool isFirstPage() const { return _firstPage; }
    void setFirstPage (bool firstPage) { _firstPage = firstPage; }
    bool isLastPage() const { return _lastPage; }
    void setLastPage (bool lastPage) { _lastPage = lastPage; }
    bool isNextAllowed() const { return _nextAllowed; }
    void setNextAllowed (bool nextAllowed) { _nextAllowed = nextAllowed; }

private:
    void paint (Graphics& g) override { g.fillAll (Colours::yellow); }

    void resized() override
    {
        const auto buttonWidth = getWidth() / 4;
        const auto buttonArea = Rectangle<int> (0, 0, buttonWidth, getHeight());

        back.setBounds (buttonArea);
        skip.setBounds (buttonArea.withX (buttonWidth));
        next.setBounds (buttonArea.withX (buttonWidth * 2));
        finish.setBounds (buttonArea.withX (buttonWidth * 3));
    }

    bool _firstPage{ false };
    bool _lastPage{ false };
    bool _nextAllowed{ false };

    TextButton back{ "back" };
    TextButton next{ "next" };
    TextButton skip{ "skip" };
    TextButton finish{ "finish" };
};

class TutorialView : public Component
{
public:
    TutorialView()
    {
        addAndMakeVisible (navigation);
        addAndMakeVisible (page);
    }

    void resized() override
    {
        auto area = getLocalBounds();
        const auto navigationArea = area.removeFromBottom (50);
        navigation.setBounds (navigationArea);
        page.setBounds (area);
    }

private:
    Tutorial tutorial; // TODO: init
    NavigationView navigation;
    PageView page;
};

class Wrapper : public TopLevelWindow
{
public:
    Wrapper() : TopLevelWindow ("Test", true)
    {
        setOpaque (false);
        Component::setVisible (true);
        centreWithSize (500, 500);
        addAndMakeVisible (content);
        setAlwaysOnTop (true);
    }

    void resized() override
    {
        TopLevelWindow::resized();
        auto area = getLocalBounds();
        content.setBounds (area);
    }

private:
    TutorialView content;
};
