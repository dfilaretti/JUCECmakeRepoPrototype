//
// Created by Daniele Filaretti on 28/10/2020.
//

#pragma once

#include <JuceHeader.h>
#include <utility>
#include "data_structures.h"

const int leftRightMargin = 27;
const Colour lightGrey = Colours::lightgrey;
const Colour darkGrey = Colours::darkgrey;
const Colour bulletColour = Colours::grey;

Tutorial getDummyTutorial()
{
    PageContent pageContent1
        {
            "Loopcloud can play audio through your music software in time and in key with your track.",
            {
                "Find the Loopcloud plugin in your DAW and insert it onto an instrument track.",
                "See a tutorial for Ableton, Logic Pro X,  GarageBand, FL Studio, Cubase)."
            }
        };

    PageContent pageContent2
        {
            "Analyse and auto-tag your existing sample library.",
            {
                "Add your first folder by clicking the Add Samples button"
            }
        };

    PageContent pageContent3
        {
            "The folder has been added to Loopcloud!",
            {
                "Loopcloud will automatically keep up to date with changes to these folders.",
                "You can add more folders later from the library tab."
            }
        };

    PageContent pageContent4
        {
            "Loopcloud will automatically transpose sounds so that they’re in the same key as your track.",
            {
                "Use select a key from the highlighted menu."
            }
        };

    PageContent pageContent5
        {
            "Sounds are tagged in Loopcloud to make them easy for you to search",
            {
                "Select a Genre tag to make your first search."
            }
        };

    PageContent pageContent6
        {
            "Select a sound, and press play in your DAW.",
            {
                "The sound will play though your instrument channel."
            }
        };

    PageContent pageContent7
        {
            "Now you can select a sound to listen to, edit it, apply effects and export it to your DAW.",
            {

            }
        };

    PageContent pageContent8
        {
            "Now you can select a file to listen to, edit it, apply effects and export it to your DAW.",
            {
                "Purchase sounds from the store with points.",
                "Narrow down your search by selecting a BPM, or other tags.",
                "Or visit our support sidebar and find out more about Loopcloud’s exciting features."
            }
        };


    Page page1{ pageContent1, [](Context c) { return true; } };
    Page page2{ pageContent2, [](Context c) { return c.x && c.z; } };
    Page page3{ pageContent3, [](Context c) { return c.y; } };
    Page page4{ pageContent4, [](Context c) { return true; } };
    Page page5{ pageContent5, [](Context c) { return true; } };
    Page page6{ pageContent6, [](Context c) { return true; } };
    Page page7{ pageContent7, [](Context c) { return true; } };
    Page page8{ pageContent8, [](Context c) { return true; } };

    Lesson lesson1 {"Syncing LoopCloud with your DAW", { page1 }};
    Lesson lesson2 {"Add Your Existing Samples", { page2, page3 }};
    Lesson lesson3 {"Setup Auto Key", { page4 }};
    Lesson lesson4 {"Make Your First Search", { page5, page6, page7 }};
    Lesson lesson5 {"Time to Explore", { page8 }};

    Tutorial tut {{lesson1, lesson2, lesson3, lesson4, lesson5}};

    return tut;
}

Tutorial getEmptyTutorial()
{
    return Tutorial {{}};
}

class TitleView : public Component
{
    const int topMargin = 26;

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
        area.removeFromLeft (leftRightMargin);
        area.removeFromRight (leftRightMargin);
        area.removeFromTop (topMargin);

        auto positionArea = area.removeFromRight(43);
        auto titleArea = area;

        //g.fillAll (Colours::pink);
        g.setColour (juce::Colours::darkblue);

        g.setFont (Font("Montserrat", 24.0f, Font::bold));
        g.drawText(_title, titleArea, Justification::topLeft, true);

        auto numLessons = _pos->nlessons;
        auto currentLesson = _pos->lesson + 1;
        auto positionString = String(currentLesson) + "/" + String(numLessons);

        g.setFont (Font("Montserrat", 21.0f, 0));
        g.drawText(positionString, positionArea, Justification::topRight, true);
    }

    String _title;
    Position * _pos;
};

class PageContentView : public Component
{
    const int pictureWidth = 245;
    const int middleGapWidth = 16;
    const int rigthGapWidth = 28;
    const int itemHeight = 61;

public:
    class PageDescriptionView : public Component
    {
    public:
        explicit PageDescriptionView (String  description)
            : description (std::move (description))
        {
        }

    private:
        void paint (Graphics & g) override
        {
            const auto font   = Font ("Helvetica Neue Bold", 15.0f, 0);
            const auto area   = getLocalBounds().toFloat();
            const auto colour = Colours::black;

            AttributedString s;
            s.setWordWrap (AttributedString::WordWrap::byWord);
            s.append (description, font, colour);

            TextLayout tl;
            tl.createLayout (s, static_cast<float> (getWidth()));
            tl.draw (g, area);
        }

        String description;
    };

    class BulletpointView : public Component
    {
    public:
        explicit BulletpointView (String text)
            : text (std::move (text))
        {}

    private:
        void paint (Graphics & g) override
        {
            g.setColour (bulletColour);
            g.fillRoundedRectangle (getLocalBounds().toFloat(), 10);

            const auto font   = Font ("Helvetica Neue Bold", 15.0f, Font::bold);
            const auto area   = getLocalBounds().toFloat().reduced (5);
            const auto colour = Colours::black;

            AttributedString s;
            s.setWordWrap (AttributedString::WordWrap::byWord);
            s.append (text, font, colour);

            TextLayout tl;
            tl.createLayout (s, static_cast<float> (getWidth()));
            tl.draw (g, area);
        }

        String text;
    };

    class ImageView : public Component
    {
        // TODO

        void paint (Graphics & g) override
        {
            g.setColour (Colours::green);
            g.drawRect (getLocalBounds());

            g.drawText("Image Here!", getLocalBounds(), Justification::centred, true);
        }
    };

    explicit PageContentView (PageContent content) : _content{ std::move( content ) }
    {
        initialiseWithNewContent();
    }

    void initialiseWithNewContent()
    {
        const auto descripion = _content.getDescription();
        descriptionView = std::make_unique<PageDescriptionView> (descripion);
        addAndMakeVisible (*descriptionView);

        bulletpointViews.clear();
        for (auto & b : _content.getBulletpoints())
            bulletpointViews.emplace_back (std::make_unique<BulletpointView> (b));
        for (auto & b : bulletpointViews)
            addAndMakeVisible (*b);

        imageView = std::make_unique<ImageView> ();
        addAndMakeVisible (*imageView);

        resized();
    }

    void setContent (PageContent content)
    {
        _content = std::move (content);
        initialiseWithNewContent();
    }

private:
    void paint (Graphics& g) override
    {
        // set background & text colour
        //g.fillAll (Colours::orange);
        g.setColour (juce::Colours::darkblue);
    }

    void resized() override
    {
        auto area = getLocalBounds();

        auto pictureArea = area.removeFromLeft (pictureWidth);
        area.removeFromLeft (middleGapWidth);
        area.removeFromRight (rigthGapWidth);

        auto descriptionArea = area.removeFromTop (itemHeight);
        descriptionArea.removeFromBottom (19);

        auto bulletpointsArea = area;

        descriptionView->setBounds (descriptionArea);

        for (auto & b : bulletpointViews)
        {
            auto bArea = bulletpointsArea.removeFromTop (itemHeight);
            bulletpointsArea.removeFromTop (5);
            b->setBounds (bArea);
        }

        imageView->setBounds (pictureArea);
    }

    PageContent _content;

    std::vector<std::unique_ptr<BulletpointView>> bulletpointViews;
    std::unique_ptr<PageDescriptionView> descriptionView;
    std::unique_ptr<ImageView> imageView;
};

class PageView : public Component
{
    const int titleViewHeight = 91;
    const int contentViewHeight = 245   ;
    const int bottomGapHeight = 19;
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
    void paint (Graphics& g) override { g.fillAll (lightGrey); }

    void resized() override
    {
        auto area = getLocalBounds();
        const auto titleArea = area.removeFromTop (titleViewHeight);
        const auto bottomGapArea = area.removeFromBottom (bottomGapHeight);
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

        xToggleButton.onClick = [this]() { this->context.x = xToggleButton.getToggleState(); };
        yToggleButton.onClick = [this]() { this->context.y = yToggleButton.getToggleState(); };
        zToggleButton.onClick = [this]() { this->context.z = zToggleButton.getToggleState(); };
    }
private:
    void paint (Graphics& g) override { g.fillAll (Colours::black); }

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
    const int navigationViewHeight = 59;
public:
    class NavigationView : public Component, public Timer
    {
    public:
        explicit NavigationView (TutorialView & owner) : _owner (owner)
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
            else
                stopTimer();
        }

    private:

        void timerCallback() override
        {
            DBG ("Tick: waiting for condition to come true...");
            refreshIsNextAllowed();
        }

        void paint (Graphics& g) override { g.fillAll (darkGrey); }

        void resized() override
        {
            const auto buttonWidth = 75;
            const auto buttonHeight = 31;
            const auto buttonArea = Rectangle<int> (0, 0, buttonWidth, buttonHeight);

            auto area = getLocalBounds();
            area.removeFromTop (13);
            area.removeFromBottom (15);
            area.removeFromLeft (27);
            area.removeFromRight (27);

            const auto topLeft = area.getTopLeft();
            const auto topRight = area.getTopRight();

            const auto backButtonPos = buttonArea.withPosition (topLeft);
            const auto nextButtonPos = buttonArea.withPosition (topRight.withX (topRight.getX() - buttonWidth));
            const auto skipButtonPos = buttonArea.withPosition (topRight.withX(topRight.getX() - buttonWidth * 2));

            back.setBounds (backButtonPos);
            next.setBounds (nextButtonPos);
            skip.setBounds (skipButtonPos);
            finish.setBounds (nextButtonPos);
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

    explicit TutorialView (Tutorial tutorial, Context & context)
        : _context (context),
          _tutorial {std::move(tutorial)},
          _navigationView (*this),
          _pageView (getCurrentTitle(), currentPageContent(), &currentPosition())
    {
        addAndMakeVisible (_navigationView);
        addAndMakeVisible (_pageView);
    }

    void setTitle (const String& newTitle) { _pageView.setTitle (newTitle); }
    void setContent (const PageContent& newContent) { _pageView.setContent (newContent); }
    void refreshNavigation() { _navigationView.updateNavigationState(); }

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
        refreshNavigation();
    }

    bool isAtFirstPage() { return currentLessonNumber() == 0 && currentPageNumber() == 0; }
    bool isAtLastPage() { return currentLessonNumber() == nLessons() - 1 && currentPageNumber() == nPagesInCurrentLesson() - 1; }
    std::function<bool(Context)> currentCondition() { return currentPage().getCondition(); }
    Context& getContext() { return _context; }

private:
    void resized() override
    {
        auto area = getLocalBounds();
        const auto navigationArea = area.removeFromBottom (navigationViewHeight);
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
    const int mainContentWidth = 710;
    const int contextViewWidth = 100;
    const int totalWidth = mainContentWidth + contextViewWidth;
    const int height = 414;

public:
    Wrapper() : TopLevelWindow ("Test", true)
    {
        setOpaque (false);
        Component::setVisible (true);
        centreWithSize (totalWidth, 414);
        addAndMakeVisible (_contextView);
        addAndMakeVisible (_tutorialView);
        setAlwaysOnTop (true);
    }

    void resized() override
    {
        TopLevelWindow::resized();
        auto area = getLocalBounds();
        auto contextArea = area.removeFromRight (contextViewWidth);
        _contextView.setBounds (contextArea);
        _tutorialView.setBounds (area);
    }

private:
    Context _context;
    ContextView _contextView{ _context };
    TutorialView _tutorialView{getDummyTutorial(), _context};
};
