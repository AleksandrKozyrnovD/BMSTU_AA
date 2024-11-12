#ifndef __SCRAPPER_HPP__
#define __SCRAPPER_HPP__

#include "scrapper.h"
#include "ProcessingFunctions.h"

int Scrapped::unique_id_counter = 0;


Scrapped downloader(Scrapped& scrap)                  //downloads html from url
{
    // std::cout << "Downloader task = " << scrap.unique_id << std::endl;
    scrap.times.back().starttask = std::chrono::system_clock::now();
    if (get_html(scrap.url, scrap.html))
    {
        std::cout << "Something went wrong" << std::endl;
        scrap.html = "<body></body>";
    }
    scrap.times.back().endtask = std::chrono::system_clock::now();
    return scrap;
}

Scrapped recipe_scrapper(Scrapped& scrap)
{
    scrap.times.back().starttask = std::chrono::system_clock::now();
    extract_text_and_images(scrap);
    scrap.times.back().endtask = std::chrono::system_clock::now();
    return scrap;
}

#endif
