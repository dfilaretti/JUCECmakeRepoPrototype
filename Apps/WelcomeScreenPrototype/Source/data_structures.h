//
// Created by Daniele Filaretti on 28/10/2020.
//

#pragma once

class PageContent
{
    String description;
    std::vector<String> bulletpoints;
    Image image;
};

class Page
{
    PageContent content;
    std::function<bool()> isCompleted; // TODO: function should take SystemContext
};

class Lesson
{
    String title;
    std::vector<Page> pages;
};

class Position
{
    int lesson;
    int page;
};

class Tutorial
{
    std::vector<Lesson> lessons;
    Position currentPage;
};

