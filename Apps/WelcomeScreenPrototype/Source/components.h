//
// Created by Daniele Filaretti on 28/10/2020.
//

#pragma once

#include <JuceHeader.h>
#include <utility>
#include "data_structures.h"

Tutorial getDummyTutorial()
{
    PageContent pageContent1 {"This is a sample page. Read the following items then click next...", {"item 1", "item 2", "item 3", "..."}};
    PageContent pageContent2 {"You need to take action now!", {"select x", "select z", "... then you can click next (or skip!)"}};
    PageContent pageContent3 {"A dummy page with some action needed", {"y needs to be active!!", "blah blah blah"}};
    PageContent pageContent4 {"Hello World Page", {"foo", "bar", "baz", "wux"}};

    Page page1{ pageContent1, [](Context c) { return true; } };
    Page page2{ pageContent2, [](Context c) { return c.x && c.z; } };
    Page page3{ pageContent3, [](Context c) { return c.y; } };
    Page page4{ pageContent4, [](Context c) { return true; } };

    Lesson lesson1 {"First lesson", { page1, page2, page4 }};
    Lesson lesson2 {"Second lesson", { page3 }};
    Lesson lesson3 {"THE FINAL LESSON", { page2, page1, page4 }};

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

class ContextView : public Component
{
public:
    explicit ContextView (Context & context) : context (context)
    {
        addAndMakeVisible (xToggleButton);
        addAndMakeVisible (yToggleButton);
        addAndMakeVisible (zToggleButton);

        xToggleButton.onClick = [this]()
        {
            this->context.x = xToggleButton.getToggleState();
            DBG (this->context.toString());
        };

        yToggleButton.onClick = [this]()
        {
          this->context.y = yToggleButton.getToggleState();
          DBG (this->context.toString());
        };

        zToggleButton.onClick = [this]()
        {
          this->context.z = zToggleButton.getToggleState();
          DBG (this->context.toString());
        };
    }
private:

    void paint (Graphics & g) override
    {
        g.fillAll (Colours::black);
    }

    void resized() override
    {
        auto area = getLocalBounds();

        auto xArea = area.removeFromTop(50);
        xToggleButton.setBounds(xArea);

        auto yArea = area.removeFromTop(50);
        yToggleButton.setBounds(yArea);

        auto zArea = area.removeFromTop(50);
        zToggleButton.setBounds(zArea);
    }

    Context & context;

    ToggleButton xToggleButton{ "x" };
    ToggleButton yToggleButton{ "y" };
    ToggleButton zToggleButton{ "z" };
};

class TutorialView : public Component
{
public:
    class NavigationView : public Component, public Timer
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

            updateNavigationState();
        }

        bool isFirstPage() const { return _firstPage; }
        void setFirstPage (bool firstPage) { _firstPage = firstPage; refreshView(); }
        bool isLastPage() const { return _lastPage; }
        void setLastPage (bool lastPage) { _lastPage = lastPage; }
        bool isNextAllowed() const { return _nextAllowed; }
        void setNextAllowed (bool nextAllowed) { _nextAllowed = nextAllowed; }

        void refreshIsNextAllowed()
        {
            const auto newNextAllowed = _owner.currentCondition()(_owner.getContext());
            if (newNextAllowed != _nextAllowed)
            {
                _nextAllowed = newNextAllowed;
                refreshView();

                if (_nextAllowed)
                    stopTimer();
            }
        }

        void updateNavigationState()
        {
            _firstPage = _owner.isAtFirstPage();
            _lastPage = _owner.isAtLastPage();
            _nextAllowed = _owner.currentCondition()(_owner.getContext());

            refreshView();

            if (!_nextAllowed)
                startTimer (500);
        }

    private:

        void timerCallback() override
        {
            DBG ("Tick: waiting for condition to come true...");
            refreshIsNextAllowed();
        }

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

        void refreshView()
        {
            back.setVisible (!isFirstPage());
            next.setVisible (!isLastPage());
            next.setEnabled (isNextAllowed());
            skip.setVisible (!isLastPage());
            finish.setVisible (isLastPage());
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

    explicit TutorialView(Tutorial tutorial, Context & context)
        : _context (context),
          _tutorial {std::move(tutorial)},
          _navigationView (* this),
          _pageView (getCurrentTitle(),
                    currentPageContent(),
                    &currentPosition())
    {
        addAndMakeVisible (_navigationView);
        addAndMakeVisible (_pageView);
    }

    void setTitle (const String& newTitle) { _pageView.setTitle (newTitle); }
    void setContent (const PageContent& newContent) { _pageView.setContent (newContent); }

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
        _navigationView.updateNavigationState();
    }

    bool isAtFirstPage() { return currentLessonNumber() == 0 && currentPageNumber() == 0; }
    bool isAtLastPage()
    {
        return currentLessonNumber() == nLessons() - 1 &&
               currentPageNumber() == nPagesInCurrentLesson() - 1;
    }

    std::function<bool(Context)> currentCondition() { return currentPage().getCondition(); }

    Context & getContext()
    {
        return _context;
    }

private:
    void resized() override
    {
        auto area = getLocalBounds();
        const auto navigationArea = area.removeFromBottom (50);
        _navigationView.setBounds (navigationArea);
        _pageView.setBounds (area);
    }

    // getters
    Position& currentPosition() { return _tutorial.getPosition(); }
    int currentLessonNumber() { return currentPosition().lesson; }
    int currentPageNumber() { return currentPosition().page; }
    String getCurrentTitle() { return _tutorial[currentLessonNumber()].getTitle(); }
    Page currentPage() { return _tutorial[currentLessonNumber()][currentPageNumber()]; }
    PageContent currentPageContent() { return currentPage().getContent(); }
    int nPagesInLesson (int lesson) { return _tutorial[lesson].numberOfPages(); }
    int nPagesInCurrentLesson() { return nPagesInLesson (currentLessonNumber()); }
    int nLessons() { return _tutorial.numberOfLessons(); }

    Context & _context;
    Tutorial _tutorial;
    NavigationView _navigationView;
    PageView _pageView;
};

class Wrapper : public TopLevelWindow
{
public:
    Wrapper() : TopLevelWindow ("Test", true)
    {
        setOpaque (false);
        Component::setVisible (true);
        centreWithSize (600, 500);
        addAndMakeVisible (_contextView);
        addAndMakeVisible (_tutorialView);
        setAlwaysOnTop (true);
    }

    void resized() override
    {
        TopLevelWindow::resized();
        auto area = getLocalBounds();
        auto contextArea = area.removeFromRight(100);
        _contextView.setBounds (contextArea);
        _tutorialView.setBounds (area);
    }

private:
    Context _context;
    ContextView _contextView{ _context };
    TutorialView _tutorialView{getDummyTutorial(), _context};
};
