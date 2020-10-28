//
// Created by Daniele Filaretti on 28/10/2020.
//

#pragma once

#include <JuceHeader.h>
#include "data_structures.h"

Tutorial getDummyTutorial()
{
    PageContent pageContent1 {"dummy things you should know...", {"item 1", "item 2", "helloooo"}};
    PageContent pageContent2 {"MORE things you should know...", {"item 3", "item 4"}};
    PageContent pageContent3 {"A dummy page", {"item x", "item y", "item z"}};

    Page page1{ pageContent1, []() { return true; } };
    Page page2{ pageContent2, []() { return true; } };
    Page page3{ pageContent3, []() { return false; } };

    Lesson lesson1 {"Things you Should Know", {page1, page2}};
    Lesson lesson2 {"Dummy lesson", {page3}};

    Position pos;
    Tutorial tut {{lesson1, lesson2}, pos};

    return tut;
}

class TitleView : public Component
{
public:
    explicit TitleView (String & title) : _title { title } {}

    String& getTitle() { return _title; }
    void setTitle (String& title) { _title = title; }

private:
    void paint (Graphics & g) override
    {
        g.fillAll (Colours::pink);
        g.setColour (juce::Colours::darkblue);

        g.setFont (32.0f);
        g.drawText(_title, getLocalBounds(), Justification::centred, true);
    }

    String _title;
};

class PageContentView : public Component
{
public:
    explicit PageContentView (PageContent content) : _content{ content } {}
    const PageContent& getContent() const { return _content; }
    void setContent (PageContent content) { _content = content; }

private:
    void paint (Graphics& g) override
    {
        auto area = getLocalBounds();
        auto descriptionArea = area.removeFromTop(40);
        auto bulletpointsArea = area;

        // set background & text colour
        g.fillAll (Colours::orange);
        g.setColour (juce::Colours::darkblue);

        // description
        g.setFont (24.0f);
        g.drawText(_content.getDescription(), descriptionArea, Justification::centred, true);

        // bulletpoints
        g.setFont (18.0f);
        for (auto & b : _content.getBulletpoints())
        {
            auto bArea = bulletpointsArea.removeFromTop(30);
            g.drawText(b, bArea, Justification::centred, true);
        }
    }

    PageContent _content;

};

class PageView : public Component
{
public:
    PageView (String title, PageContent content)
        : titleView{ title },
          pageContentView{ content }
    {
        addAndMakeVisible (titleView);
        addAndMakeVisible (pageContentView);
    }

private:
    void paint (Graphics& g) override { g.fillAll (Colours::green); }

    void resized() override
    {
        auto area = getLocalBounds();
        const auto titleArea = area.removeFromTop (50);
        titleView.setBounds (titleArea);
        pageContentView.setBounds (area);
    }

    TitleView titleView;
    PageContentView pageContentView;
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
    explicit TutorialView(Tutorial tutorial)
        : _tutorial {tutorial},
          pageView (_tutorial[0].getTitle(),
                    _tutorial[0][0].getContent())
    {
        addAndMakeVisible (navigationView);
        addAndMakeVisible (pageView);
    }

private:
    void resized() override
    {
        auto area = getLocalBounds();
        const auto navigationArea = area.removeFromBottom (50);
        navigationView.setBounds (navigationArea);
        pageView.setBounds (area);
    }

    // data
    Tutorial _tutorial;

    // children
    NavigationView navigationView;
    PageView pageView;
};

class Wrapper : public TopLevelWindow
{
public:
    Wrapper()
        : TopLevelWindow ("Test", true),
          _tutorialView (getDummyTutorial())
    {
        setOpaque (false);
        Component::setVisible (true);
        centreWithSize (500, 500);
        addAndMakeVisible (_tutorialView);
        setAlwaysOnTop (true);
    }

    void resized() override
    {
        TopLevelWindow::resized();
        auto area = getLocalBounds();
        _tutorialView.setBounds (area);
    }

private:
    TutorialView _tutorialView;
};
