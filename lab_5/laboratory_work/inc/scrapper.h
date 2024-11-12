#ifndef __SCRAPPER_H__
#define __SCRAPPER_H__

#include <algorithm>
#include <chrono>
#include <compare>
#include <vector>
#include <string>


//from start of the unix
struct ProcessTime
{
    std::chrono::system_clock::time_point start, starttask;
    std::chrono::system_clock::time_point end, endtask;
};

struct Scrapped
{
    Scrapped() : task_id(9156), unique_id(unique_id_counter++)
    {
        this->lifetime.start = std::chrono::system_clock::now();
    }
    Scrapped(const std::string& url)
    : Scrapped()
    {
        this->url = url;
    }

    Scrapped(Scrapped&& scrap)
    : times(scrap.times),
    url(std::move(scrap.url)),
    name(std::move(scrap.name)),
    html(std::move(scrap.html)),
    text(std::move(scrap.text)),
    images(std::move(scrap.images)),
    task_id(std::move(scrap.task_id)),
    unique_id(std::move(scrap.unique_id))
    {
        lifetime = scrap.lifetime;
    }
    Scrapped(const Scrapped& scrap)
    : times(scrap.times), url(scrap.url), name(scrap.name), html(scrap.html), text(scrap.text),
      images(scrap.images), task_id(scrap.task_id), unique_id(scrap.unique_id)
    {
        lifetime = scrap.lifetime;
    }

    Scrapped& operator=(Scrapped&& scrap)
    {
        url = std::move(scrap.url);
        name = std::move(scrap.name);
        html = std::move(scrap.html);
        text = std::move(scrap.text);
        images = std::move(scrap.images);
        task_id = std::move(scrap.task_id);
        unique_id = std::move(scrap.unique_id);
        lifetime = scrap.lifetime;
        return *this;
    }

    Scrapped& operator=(const Scrapped& scrap)
    {
        url = scrap.url;
        name = scrap.name;
        html = scrap.html;
        text = scrap.text;
        images = scrap.images;
        lifetime = scrap.lifetime;
        task_id = scrap.task_id;
        unique_id = scrap.unique_id;
        return *this;
    }

    bool operator==(const Scrapped& scrap) const
    {
        return this->url == scrap.url;
    }

    std::strong_ordering operator<=>(const Scrapped& scrap) const
    {
        return this->url <=> scrap.url;
    }

    std::vector<ProcessTime> times;
    std::string url, name;
    std::string html;
    std::vector<std::string> text;
    std::vector<std::string> images;
    ProcessTime lifetime;

    int task_id, unique_id;
    static int unique_id_counter;
};

int get_html(const std::string& URL, std::string& html);
std::vector<std::string> parse_text_links(const std::string& html);
void parse_htmlstring(const std::string& html);


Scrapped downloader(Scrapped& scrap);                  //downloads html from url

Scrapped recipe_scrapper(Scrapped& scrap);             //parses html and appends to list all values


#include "scrapper.hpp"

#endif
