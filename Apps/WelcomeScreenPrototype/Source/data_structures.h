//
// Created by Daniele Filaretti on 28/10/2020.
//

#pragma once

class PageContent
{
public:
    PageContent (const String& description,
                 const std::vector<String>& bulletpoints)
        : description (description),
          bulletpoints (bulletpoints)
    {
    }

    const String& getDescription() const { return description; }
    const std::vector<String>& getBulletpoints() const { return bulletpoints; }

private:
    String description;
    std::vector<String> bulletpoints;
};

class Page
{
public:
    Page (const PageContent& content,
          const std::function<bool()>& isCompleted)
        : content (content),
          isCompleted (isCompleted) {}
    const PageContent& getContent() const { return content; }
private:
    PageContent content;
    std::function<bool()> isCompleted; // TODO: function should take SystemContext
};

class Lesson
{
public:
    Lesson (const String& title,
            const std::vector<Page>& pages)
        : title (title),
          pages (pages)
    {
    }

    String getTitle() const { return title; }
    Page & operator[](std::size_t idx) { return pages[idx]; }
    const Page & operator[](std::size_t idx) const { return pages[idx]; }
private:
    String title;
    std::vector<Page> pages;
};

struct Position
{
    Position() = default;

    Position (int lesson, int page, unsigned long nLessons)
        : nLessons { nLessons },
          lesson{ lesson },
          page{ page } {}

    explicit Position (std::vector<Lesson> lessons)
        : nLessons { lessons.size() },
          lesson {0},
          page {0}
    {
    }

    unsigned long nLessons;
    int lesson = 0;
    int page = 0;
};

class Tutorial
{
public:
    Tutorial (const std::vector<Lesson>& lessons)
        : lessons (lessons),
          position (lessons) {}
    Lesson& operator[] (std::size_t idx) { return lessons[idx]; }
    const Lesson& operator[] (std::size_t idx) const { return lessons[idx]; }
    Position getPosition() { return position; }

private:
    std::vector<Lesson> lessons;
    Position position;
};
