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

    const String& getTitle() const { return title; }
    Page & operator[](std::size_t idx) { return pages[idx]; }
    const Page & operator[](std::size_t idx) const { return pages[idx]; }
private:
    String title;
    std::vector<Page> pages;
};

struct Position
{
    Position() = default;
    Position (int lesson, int page) : lesson{ lesson }, page{ page } {}

    int lesson = 0;
    int page = 0;
};

class Tutorial
{
public:
    Tutorial (const std::vector<Lesson>& lessons,
              const Position& currentPage)
        : lessons (lessons),
          currentPage (currentPage) {}
    Lesson & operator[](std::size_t idx) { return lessons[idx]; }
    const Lesson & operator[](std::size_t idx) const { return lessons[idx]; }
private:
    std::vector<Lesson> lessons;
    Position currentPage;
};
