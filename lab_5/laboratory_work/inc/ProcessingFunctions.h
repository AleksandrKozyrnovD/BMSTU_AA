#ifndef __PROCESSINGFUNCTIONS_H__
#define __PROCESSINGFUNCTIONS_H__

#include <iostream>
#include <regex>
#include "curl/curl.h"
#include <set>

#include "scrapper.h"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp)
{
    size_t total_size = size * nmemb;
    userp->append(static_cast<const char*>(contents), total_size);
    return total_size;
}

int get_html(const std::string& url, std::string& read_buffer)
{
    CURL* curl;
    CURLcode res = CURLE_AUTH_ERROR;

    curl = curl_easy_init();
    if (curl)
    {

        res = curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cout << "Error. Invalid URL: " << url << std::endl;
            return 1;
        }
        curl_easy_cleanup(curl);
    }
    return 0;
}

std::vector<Scrapped> parse_recipe_links(const std::string& html)
{
    std::vector<Scrapped> recipe_links;
    std::string::size_type pos = 0, href_pos;
    
    while ((href_pos = html.find("href=\"", pos)) != std::string::npos)
    {
        href_pos += 6;
        std::string::size_type end_pos = html.find("\"", href_pos);
        if (end_pos == std::string::npos)
            break;
        std::string link = html.substr(href_pos, end_pos - href_pos );
        
        // Фильтруем ссылки только на рецепты
        // if (link.find("/reczepty/") != std::string::npos)
        // {
            if (link.ends_with(".html"))
            {
                recipe_links.push_back(Scrapped(link));
            }
        // }
        pos = end_pos + 5;
    }
    return recipe_links;
}

std::vector<Scrapped> get_scrapped_entries(const size_t pagestart, const size_t pageend, const size_t max)
{
    if (pagestart > pageend)
        return std::vector<Scrapped>();

    std::vector<Scrapped> recipes;
    std::string html;
    size_t current_size = 0;
    for (size_t i = pagestart; i <= pageend && current_size < max; i++)
    {
        std::string url = "https://vkusnye-recepty.ru/page/" + std::to_string(i);
        get_html(url, html);
        auto tmp = parse_recipe_links(html);

        //merge lists
        for (size_t j = 0; j < tmp.size() && current_size < max; j++)
        {
            if (std::find(recipes.begin(), recipes.end(), tmp[i]) == recipes.end())
            {
                recipes.push_back(tmp[i]);
                current_size++;
            }
        }
    }

    return recipes;
}


// void extract_text_and_images
void extract_text_and_images(Scrapped& scrap)
{
    std::string& html = scrap.html;

    // Регулярные выражения для поиска текста и изображений
    std::regex text_regex(R"(<[^>]*>([^<>\s][^<]*)<[^>]*>)");
    std::regex img_regex(R"(<img[^>]*src=["']([^"']+)["'][^>]*>)");


    // Сначала выводим все найденные текстовые блоки
    auto text_begin = std::sregex_iterator(html.begin(), html.end(), text_regex);
    auto text_end = std::sregex_iterator();

    for (std::sregex_iterator i = text_begin; i != text_end; ++i)
    {
        std::smatch match = *i;
        std::string text = match[1].str();
        if (!text.empty())\
        {
            scrap.text.push_back(text);
        }
    }

    // Затем выводим все найденные изображения
    auto img_begin = std::sregex_iterator(html.begin(), html.end(), img_regex);
    auto img_end = std::sregex_iterator();

    for (std::sregex_iterator i = img_begin; i != img_end; ++i)
    {
        std::smatch match = *i;
        std::string img_url = match[1].str();
        scrap.images.push_back(img_url);
    }
}

#endif
