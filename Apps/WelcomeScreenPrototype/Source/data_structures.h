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
    PageContent (const String& description,
                 const std::vector<String>& bulletpoints)
        : description (description),
          bulletpoints (bulletpoints)
    {
    }

    const String& getDescription() const { return description; }
    const std::vector<String>& getBulletpoints() const { return bulletpoints; }
    int getNumberOfBulletpoints() const { return bulletpoints.size(); }
    String bulletpoint (int n) const { return bulletpoints[n]; }

private:
    String description;
    std::vector<String> bulletpoints;
};

class Page
{
public:
    Page (const PageContent& content,
          const std::function<bool(Context)>& isCompleted)
        : content (content),
          isCompleted (isCompleted) {}
    const PageContent& getContent() const { return content; }
    const std::function<bool(Context)>& getCondition() const { return isCompleted; }
private:
    PageContent content;
    std::function<bool(Context)> isCompleted; // TODO: function should take SystemContext
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
    int numberOfPages() const { return pages.size(); }
    Page & operator[](std::size_t idx) { return pages[idx]; }
    const Page & operator[](std::size_t idx) const { return pages[idx]; }
private:
    String title;
    std::vector<Page> pages;
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
    void setPage (int newPage) { page = newPage; }

    const unsigned long nlessons = 0;
    int lesson = 0;
    int page = 0;
};

class Tutorial
{
public:
    explicit Tutorial (const std::vector<Lesson>& lessons)
        : lessons (lessons),
          position (lessons) {}

    Lesson& operator[] (std::size_t idx) { return lessons[idx]; }
    const Lesson& operator[] (std::size_t idx) const { return lessons[idx]; }

    Position & getPosition() { return position; }

    int numberOfLessons() { return lessons.size(); }

private:
    std::vector<Lesson> lessons;
    Position position;
};


