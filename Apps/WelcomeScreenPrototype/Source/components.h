//
// Created by Daniele Filaretti on 28/10/2020.
//

#pragma once

#include <JuceHeader.h>

#include <utility>
#include "data_structures.h"

Tutorial getDummyTutorial()
{
    PageContent pageContent1 {"dummy things you should know...", {"item 1", "item 2", "helloooo"}};
    PageContent pageContent2 {"MORE things you should know...", {"item 3", "item 4"}};
    PageContent pageContent3 {"A dummy page", {"item x", "item y", "item z"}};
    PageContent pageContent4 {"Hello World Page", {"foo", "bar", "baz", "wux"}};

    Page page1{ pageContent1, []() { return true; } };
    Page page2{ pageContent2, []() { return true; } };
    Page page3{ pageContent3, []() { return false; } };
    Page page4{ pageContent4, []() { return true; } };

    Lesson lesson1 {"Things you Should Know", {page1, page2, page4}};
    Lesson lesson2 {"Dummy lesson", {page3}};
    Lesson lesson3 {"THE FINAL LESSON", {page3, page2, page1, page4}};

    Tutorial tut {{lesson1, lesson2, lesson3}};

    return tut;
}

Tutorial getEmptyTutorial()
{
    return Tutorial {{}};
}

class TitleView : public Component
{
public:
    explicit TitleView (String & title, Position * pos)
        : _title{ title },
          _pos{ pos } {}

    String getTitle() { return _title; }

    void setTitle (const String& title)
    {
        _title = title;
        repaint();
    }

    Position * getPosition() { return _pos; }

//    void setPosition (Position & newPosition)
//    {
//        _pos = newPosition;
//        repaint();
//    }

private:
    void paint (Graphics & g) override
    {
        auto area = getLocalBounds();
        auto positionArea = area.removeFromRight(50);
        auto titleArea = area;

        g.fillAll (Colours::pink);
        g.setColour (juce::Colours::darkblue);
        g.setFont (32.0f);

        g.drawText(_title, titleArea, Justification::centred, true);

        auto numLessons = _pos->nlessons;
        auto currentLesson = _pos->lesson + 1;
        auto positionString = String(currentLesson) + "/" + String(numLessons);

        g.drawText(_title, titleArea, Justification::centred, true);
        g.drawText(positionString, positionArea, Justification::centred, true);
    }

    String _title;
    Position * _pos;
};

class PageContentView : public Component
{
public:
    explicit PageContentView (PageContent content) : _content{std::move( content )} {}

    const PageContent& getContent() const { return _content; }

    void setContent (PageContent content)
    {
        _content = std::move(content);
        repaint();
    }

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
    PageView (String title, PageContent content, Position* pos)
        : titleView{ title, pos },
          pageContentView{ std::move(content) }
    {
        addAndMakeVisible (titleView);
        addAndMakeVisible (pageContentView);
    }

    void setTitle (const String& newTitle) { titleView.setTitle (newTitle); }
    //void setPosition (Position& newPosition) { titleView.setPosition (newPosition); }
    void setContent (const PageContent& newContent) { pageContentView.setContent (newContent); }

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

class TutorialView : public Component
{
public:

    class NavigationView : public Component
    {

    public:
        explicit NavigationView (TutorialView & owner)
            : _owner (owner)
        {
            addAndMakeVisible (back);
            addAndMakeVisible (next);
            addAndMakeVisible (skip);
            addAndMakeVisible (finish);

            next.onClick = ([this]() { _owner.next(); });
            back.onClick = ([this]() { _owner.back(); });
            skip.onClick = ([this]() { _owner.skip(); });
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

        TutorialView & _owner;
    };

    explicit TutorialView(Tutorial tutorial)
        : _tutorial {std::move(tutorial)},
          navigationView (* this),
          pageView (getCurrentTitle(),
                    currentPageContent(),
                    &currentPosition())
    {
        addAndMakeVisible (navigationView);
        addAndMakeVisible (pageView);
    }

    void setTitle (const String& newTitle) { pageView.setTitle (newTitle); }
//    void setPosition (Position& newPosition) { pageView.setPosition (newPosition); }
    void setContent (const PageContent& newContent) { pageView.setContent (newContent); }

    void next()
    {
        auto currentLesson = currentLessonNumber();
        auto currentPage = currentPageNumber();
        auto maxPage = nPagesInCurrentLesson() - 1;
        auto maxLesson = nLessons() - 1;

        if (currentPage < maxPage)
            currentPosition().setNextPage();
        else
            if (currentLesson < maxLesson)
            {
                currentPosition().setNextLesson();
                currentPosition().setFirstPage();
            }
            else
                DBG ("NEXT: reached end of last lesson");

        refreshContent();
    }

    void back()
    {
        auto currentLesson = currentLessonNumber();
        auto currentPage = currentPageNumber();

        if (currentPage > 0)
            currentPosition().setPreviousPage();
        else
            if (currentLesson > 0)
            {
                currentPosition().setPreviousLesson();
                currentPosition().setPage(nPagesInCurrentLesson() - 1);
            }
            else
                DBG ("NEXT: reached beginning of first lesson");

        refreshContent();
    }

    void skip()
    {
        auto currentLesson = currentLessonNumber();
        auto maxLesson = nLessons() - 1;

        if (currentLesson < maxLesson)
        {
            currentPosition().setNextLesson();
            currentPosition().setFirstPage();
        }
        else
            DBG ("SKIP: already in last lesson!!");

        refreshContent();
    }

    void refreshContent()
    {
        setTitle (getCurrentTitle());
        setContent (currentPageContent());
    }

private:
    void resized() override
    {
        auto area = getLocalBounds();
        const auto navigationArea = area.removeFromBottom (50);
        navigationView.setBounds (navigationArea);
        pageView.setBounds (area);
    }

    // getters
    Position& currentPosition() { return _tutorial.getPosition(); }
    int currentLessonNumber() { return currentPosition().lesson; }
    int currentPageNumber() { return currentPosition().page; }
    String getCurrentTitle() { return _tutorial[currentLessonNumber()].getTitle(); }
    Page currentPage() { return _tutorial[currentLessonNumber()][currentPageNumber()]; }
    PageContent currentPageContent() { return currentPage().getContent(); }
    std::function<bool()> currentCondition() { return currentPage().getCondition(); }
    int nPagesInLesson (int lesson) { return _tutorial[lesson].numberOfPages(); }
    int nPagesInCurrentLesson() { return nPagesInLesson (currentLessonNumber()); }
    int nLessons() { return _tutorial.numberOfLessons(); }

    Tutorial _tutorial;
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
