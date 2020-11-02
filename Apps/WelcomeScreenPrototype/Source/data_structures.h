#include <utility>

//
// Created by Daniele Filaretti on 28/10/2020.
//

#pragma once

struct Context
{
    bool x = false;
    bool y = false;
    bool z = false;

    String toString() const
    {
        return String ("[ x: ") +
               std::to_string (x) +
               String(",y: ") +
               std::to_string (y) +
               String(",z: ") +
               std::to_string (z) +
               String ("]");
    }
};

class PageContent
{
public:
    PageContent (String description,
                 std::vector<String> bulletpoints)
        : m_description (std::move (description)),
          m_bulletpoints (std::move (bulletpoints))
    {
    }

    const String& getDescription() const { return m_description; }
    const std::vector<String>& getBulletpoints() const { return m_bulletpoints; }
    std::size_t getNumberOfBulletpoints() const { return m_bulletpoints.size(); }
    String bulletpoint (std::size_t n) const { return m_bulletpoints[n]; }

private:
    String m_description;
    std::vector<String> m_bulletpoints;
};

class Page
{
public:
    Page (PageContent content,
          std::function<bool(Context)> isCompleted)
        : m_content (std::move (content)),
          m_isCompleted (std::move (isCompleted)) {}
    const PageContent& getContent() const { return m_content; }
    const std::function<bool(Context)>& getCondition() const { return m_isCompleted; }
private:
    PageContent m_content;
    std::function<bool(Context)> m_isCompleted; // TODO: function should take SystemContext
};

class Lesson
{
public:
    Lesson (String title,
            std::vector<Page> pages)
        : m_title (std::move(title)),
          m_pages (std::move(pages))
    {
    }

    String getTitle() const { return m_title; }
    std::size_t numberOfPages() const { return m_pages.size(); }
    Page & operator[](std::size_t idx) { return m_pages[idx]; }
    const Page & operator[](std::size_t idx) const { return m_pages[idx]; }
private:
    String m_title;
    std::vector<Page> m_pages;
};

struct Position
{
    explicit Position (const std::vector<Lesson>& lessons)
        : nlessons (lessons.size())
    {
        jassert (!lessons.empty());
    }

    void setNextPage() { page++; }
    void setPreviousPage() { page--; }
    void setNextLesson() { lesson++; }
    void setPreviousLesson() { lesson--; }
    void setFirstPage() { page = 0; }
    void setPage (std::size_t newPage) { page = newPage; }

    const std::size_t nlessons = 0;
    std::size_t lesson = 0;
    std::size_t page = 0;
};

class Tutorial
{
public:
    explicit Tutorial (const std::vector<Lesson>& lessons)
        : m_lessons (lessons),
          m_position (lessons) {}

    Lesson& operator[] (std::size_t idx) { return m_lessons[idx]; }
    const Lesson& operator[] (std::size_t idx) const { return m_lessons[idx]; }

    Position & getPosition() { return m_position; }

    std::size_t numberOfLessons() { return m_lessons.size(); }

private:
    std::vector<Lesson> m_lessons;
    Position m_position;
};


