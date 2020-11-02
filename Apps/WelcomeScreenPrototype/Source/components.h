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
            "Loopcloud will automatically transpose sounds so that they're in the same key as your track.",
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
                "Or visit our support sidebar and find out more about Loopcloud's exciting features."
            }
        };


    Page page1{ pageContent1, [](Context /*c*/) { return true; } };
    Page page2{ pageContent2, [](Context c) { return c.x && c.z; } };
    Page page3{ pageContent3, [](Context c) { return c.y; } };
    Page page4{ pageContent4, [](Context /*c*/) { return true; } };
    Page page5{ pageContent5, [](Context /*c*/) { return true; } };
    Page page6{ pageContent6, [](Context /*c*/) { return true; } };
    Page page7{ pageContent7, [](Context /*c*/) { return true; } };
    Page page8{ pageContent8, [](Context /*c*/) { return true; } };

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
    explicit TitleView (String & title, Position * position)
        : m_title{ title }, m_position{ position } {}

    String getTitle() { return m_title; }

    void setTitle (const String& newTitle)
    {
        m_title = newTitle;
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
        g.drawText(m_title, titleArea, Justification::topLeft, true);

        auto numLessons = m_position->nlessons;
        auto currentLesson = m_position->lesson + 1;
        auto positionString = String(currentLesson) + "/" + String(numLessons);

        g.setFont (Font("Montserrat", 21.0f, 0));
        g.drawText(positionString, positionArea, Justification::topRight, true);
    }

    String m_title;
    Position * m_position;
};

class PageContentView : public Component
{
    const int pictureWidth = 245;
    const int middleGapWidth = 16;
    const int rigthGapWidth = 28;
    const int itemHeight = 61;

public:
    class TopParagraphView : public Component
    {
    public:
        explicit TopParagraphView (String  description)
            : m_description (std::move (description))
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
            s.append (m_description, font, colour);

            TextLayout tl;
            tl.createLayout (s, static_cast<float> (getWidth()));
            tl.draw (g, area);
        }

        String m_description;
    };

    // TODO: derived classes: LinkItemView, CommandItemView etc...
    class ItemView : public Component
    {
    public:
        explicit ItemView (String text)
            : m_text (std::move (text))
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
            s.append (m_text, font, colour);

            TextLayout tl;
            tl.createLayout (s, static_cast<float> (getWidth()));
            tl.draw (g, area);
        }

        String m_text;
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

    explicit PageContentView (PageContent content) : m_content{ std::move( content ) }
    {
        initialiseWithNewContent();
    }

    void initialiseWithNewContent()
    {
        const auto descripion = m_content.getDescription();
        m_topParagraphView = std::make_unique<TopParagraphView> (descripion);
        addAndMakeVisible (*m_topParagraphView);

        m_itemViews.clear();
        for (auto & b : m_content.getBulletpoints())
            m_itemViews.emplace_back (std::make_unique<ItemView> (b));
        for (auto & b : m_itemViews)
            addAndMakeVisible (*b);

        m_imageView = std::make_unique<ImageView> ();
        addAndMakeVisible (*m_imageView);

        resized();
    }

    void setContent (PageContent newContent)
    {
        m_content = std::move(newContent);
        initialiseWithNewContent();
    }

private:
    void paint (Graphics& /*g*/) override
    {
    }

    void resized() override
    {
        auto area = getLocalBounds();

        auto pictureArea = area.removeFromLeft (pictureWidth);
        area.removeFromLeft (middleGapWidth);
        area.removeFromRight (rigthGapWidth);

        auto topParagraphArea = area.removeFromTop (itemHeight);
        topParagraphArea.removeFromBottom (19);

        auto itemsArea = area;

        m_topParagraphView->setBounds (topParagraphArea);

        for (auto & itemView : m_itemViews)
        {
            auto bArea = itemsArea.removeFromTop (itemHeight);
            itemsArea.removeFromTop (5);
            itemView->setBounds (bArea);
        }

        m_imageView->setBounds (pictureArea);
    }

    PageContent m_content;

    std::vector<std::unique_ptr<ItemView>> m_itemViews;
    std::unique_ptr<TopParagraphView> m_topParagraphView;
    std::unique_ptr<ImageView> m_imageView;
};

class PageView : public Component
{
    const int titleViewHeight = 91;
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
        area.removeFromBottom (bottomGapHeight);
        titleView.setBounds (titleArea);
        pageContentView.setBounds (area);
    }

    TitleView titleView;
    PageContentView pageContentView;
};

class ContextView : public Component
{
public:
    explicit ContextView (Context & context) : m_context (context)
    {
        addAndMakeVisible (m_xToggleButton);
        addAndMakeVisible (m_yToggleButton);
        addAndMakeVisible (m_zToggleButton);

        m_xToggleButton.onClick = [this]() { this->m_context.x = m_xToggleButton.getToggleState(); };
        m_yToggleButton.onClick = [this]() { this->m_context.y = m_yToggleButton.getToggleState(); };
        m_zToggleButton.onClick = [this]() { this->m_context.z = m_zToggleButton.getToggleState(); };
    }
private:
    void paint (Graphics& g) override { g.fillAll (Colours::black); }

    void resized() override
    {
        auto area = getLocalBounds();

        auto xArea = area.removeFromTop(50);
        m_xToggleButton.setBounds(xArea);

        auto yArea = area.removeFromTop(50);
        m_yToggleButton.setBounds(yArea);

        auto zArea = area.removeFromTop(50);
        m_zToggleButton.setBounds(zArea);
    }

    Context & m_context;

    ToggleButton m_xToggleButton{ "x" };
    ToggleButton m_yToggleButton{ "y" };
    ToggleButton m_zToggleButton{ "z" };
};

class TutorialView : public Component
{
    const int navigationViewHeight = 59;
public:
    class NavigationView : public Component, public Timer
    {
    public:
        explicit NavigationView (TutorialView & owner) : m_owner (owner)
        {
            addAndMakeVisible (m_back);
            addAndMakeVisible (m_next);
            addAndMakeVisible (m_skip);
            addAndMakeVisible (m_finish);

            m_next.onClick = ([this]() { this->m_owner.next(); });
            m_back.onClick = ([this]() { this->m_owner.back(); });
            m_skip.onClick = ([this]() { this->m_owner.skip(); });

            updateNavigationState();
        }

        bool isFirstPage() const { return m_firstPage; }
        void setFirstPage (bool isNowFirstPage) {
            m_firstPage = isNowFirstPage; refreshView(); }

        bool isLastPage() const { return m_lastPage; }
        void setLastPage (bool isNowLastPage) { m_lastPage = isNowLastPage; }

        bool isNextAllowed() const { return m_nextAllowed; }
        void setNextAllowed (bool sholdNextBeAllowed) { m_nextAllowed = sholdNextBeAllowed; }

        void refreshIsNextAllowed()
        {
            const auto newNextAllowed = m_owner.currentCondition()(m_owner.getContext());
            if (newNextAllowed != m_nextAllowed)
            {
                m_nextAllowed = newNextAllowed;
                refreshView();

                if (m_nextAllowed)
                    stopTimer();
            }
        }

        void updateNavigationState()
        {
            m_firstPage = m_owner.isAtFirstPage();
            m_lastPage = m_owner.isAtLastPage();
            m_nextAllowed = m_owner.currentCondition()(m_owner.getContext());

            refreshView();

            if (! m_nextAllowed)
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

            m_back.setBounds (backButtonPos);
            m_next.setBounds (nextButtonPos);
            m_skip.setBounds (skipButtonPos);
            m_finish.setBounds (nextButtonPos);
        }

        void refreshView()
        {
            m_back.setVisible (!isFirstPage());
            m_next.setVisible (!isLastPage());
            m_next.setEnabled (isNextAllowed());
            m_skip.setVisible (!isLastPage());
            m_finish.setVisible (isLastPage());
        }

        bool m_firstPage{ false };
        bool m_lastPage{ false };
        bool m_nextAllowed{ false };

        TextButton m_back{ "back" };
        TextButton m_next{ "next" };
        TextButton m_skip{ "skip" };
        TextButton m_finish{ "finish" };

        TutorialView & m_owner;
    };

    explicit TutorialView (Tutorial tutorial, Context & context)
        : m_context (context), m_tutorial{std::move(tutorial)}, m_navigationView (*this), m_pageView (getCurrentTitle(), currentPageContent(), &currentPosition())
    {
        addAndMakeVisible (m_navigationView);
        addAndMakeVisible (m_pageView);
    }

    void setTitle (const String& newTitle) { m_pageView.setTitle (newTitle); }
    void setContent (const PageContent& newContent) { m_pageView.setContent (newContent); }
    void refreshNavigation() { m_navigationView.updateNavigationState(); }

    void next()
    {
        auto currentLesson = currentLessonNumber();
        auto currentPage = currentPageNumber();
        auto maxPage = nPagesInCurrentLesson() - 1;
        auto maxLesson = nLessons() - 1;

        if (currentPage < maxPage)
        {
            currentPosition().setNextPage();
        }
        else
            if (currentLesson < maxLesson)
            {
                currentPosition().setNextLesson();
                currentPosition().setFirstPage();
            }
            else
            {
                DBG ("NEXT: reached end of last lesson");
            }

        refreshContent();
    }

    void back()
    {
        auto currentLesson = currentLessonNumber();
        auto currentPage = currentPageNumber();

        if (currentPage > 0)
        {
            currentPosition().setPreviousPage();
        }
        else
            if (currentLesson > 0)
            {
                currentPosition().setPreviousLesson();
                currentPosition().setPage(nPagesInCurrentLesson() - 1);
            }
            else
            {
                DBG ("NEXT: reached beginning of first lesson");
            }

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
        {
            DBG ("SKIP: already in last lesson!!");
        }

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
    Context& getContext() { return m_context; }

private:
    void resized() override
    {
        auto area = getLocalBounds();
        const auto navigationArea = area.removeFromBottom (navigationViewHeight);
        m_navigationView.setBounds (navigationArea);
        m_pageView.setBounds (area);
    }

    Position& currentPosition() { return m_tutorial.getPosition(); }
    std::size_t currentLessonNumber() { return currentPosition().lesson; }
    std::size_t currentPageNumber() { return currentPosition().page; }
    String getCurrentTitle() { return m_tutorial[currentLessonNumber()].getTitle(); }
    Page currentPage() { return m_tutorial[currentLessonNumber()][currentPageNumber()]; }
    PageContent currentPageContent() { return currentPage().getContent(); }
    std::size_t nPagesInLesson (std::size_t lesson) { return m_tutorial[lesson].numberOfPages(); }
    std::size_t nPagesInCurrentLesson() { return nPagesInLesson (currentLessonNumber()); }
    std::size_t nLessons() { return m_tutorial.numberOfLessons(); }

    Context & m_context;
    Tutorial m_tutorial;

    NavigationView m_navigationView;
    PageView m_pageView;
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
        centreWithSize (totalWidth, height);
        addAndMakeVisible (m_contextView);
        addAndMakeVisible (m_tutorialView);
        setAlwaysOnTop (true);
    }

    void resized() override
    {
        TopLevelWindow::resized();
        auto area = getLocalBounds();
        auto contextArea = area.removeFromRight (contextViewWidth);
        m_contextView.setBounds (contextArea);
        m_tutorialView.setBounds (area);
    }

private:
    Context m_context;
    ContextView m_contextView{ m_context };
    TutorialView m_tutorialView{getDummyTutorial(), m_context };
};
