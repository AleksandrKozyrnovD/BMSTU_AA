#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <thread>
#include <mutex>
#include <fstream>
#include <curl/curl.h>
#include <regex>

std::mutex mtx;  // Для синхронизации доступа к общим данным
std::set<std::string> visited_links;  // Множество для хранения уникальных ссылок
std::vector<double> thread_time(64);
std::vector<std::chrono::time_point<std::chrono::system_clock>> thread_time_start(64);
std::vector<std::chrono::time_point<std::chrono::system_clock>> thread_time_end(64);


// Функция для записи данных в файл
void save_to_file(const std::string& filename, const std::string& data)
{
    std::ofstream outfile(filename);
    if (outfile.is_open())
    {
        outfile << data;
        outfile.close();
    } else
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

// Callback function to write data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp)
{
    size_t total_size = size * nmemb;
    userp->append(static_cast<const char*>(contents), total_size);
    return total_size;
}

int validate_url(CURLU *url_handle, const char *url)
{
    int rc = curl_url_set(url_handle, CURLUPART_URL, url, 0);
    // std::cout << "For URL " << url << " rc = " << rc << std::endl;
    return rc;
}

// Function to download a page
std::string download_page(const std::string& url)
{
    CURL* curl;
    CURLcode res = CURLE_AUTH_ERROR;
    std::string read_buffer;

    CURLU *url_handle = curl_url();
    if (validate_url(url_handle, url.c_str()))
    {
        std::cout << "Error. Invalid URL: " << url << std::endl;
        return "";
    }
    curl_url_cleanup(url_handle);

    // Initialize a curl session
    curl = curl_easy_init();
    if (curl)
    {

        res = curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);

        // Perform the request
        res = curl_easy_perform(curl);
        // Check for errors
        if (res != CURLE_OK)
        {
            std::cout << "Error. Invalid URL: " << url << std::endl;
            return "";
        }

        // Clean up curl session
        curl_easy_cleanup(curl);
    }
    return read_buffer; // Automatic memory management by std::string
}

// Функция для парсинга ссылок (извлекает только ссылки на рецепты)
std::vector<std::string> parse_recipe_links(const std::string& html)
{
    std::vector<std::string> recipe_links;
    std::string::size_type pos = 0, href_pos;
    
    while ((href_pos = html.find("href=\"", pos)) != std::string::npos)
    {
        href_pos += 6;
        std::string::size_type end_pos = html.find("\"", href_pos);
        if (end_pos == std::string::npos) break;
        std::string link = html.substr(href_pos, end_pos - href_pos);
        
        // Фильтруем ссылки только на рецепты
        if (link.find("/reczepty/") != std::string::npos)
        {
            recipe_links.push_back(link);
        }
        pos = end_pos;
    }
    return recipe_links;
}

// Функция для извлечения текста рецепта (ищем <p>TEXT</p>)
std::string extract_recipe_text(const std::string& html)
{
    std::string result;

    // Регулярное выражение для поиска <p>...</p>
    std::regex paragraph_regex("<p>(.*?)</p>");
    std::sregex_iterator iter(html.begin(), html.end(), paragraph_regex);
    std::sregex_iterator end;

    // Извлечение текста из <p>...</p>
    while (iter != end)
    {
        std::string paragraph = (*iter)[1].str();

        // Удаление любых тегов внутри <p>
        std::regex tag_regex("<[^>]*>");
        paragraph = std::regex_replace(paragraph, tag_regex, "");

        result += paragraph + "\n";
        ++iter;
    }

    // Регулярное выражение для поиска <li>...</li>
    std::regex list_item_regex("<li[^>]*>(.*?)</li>");
    iter = std::sregex_iterator(html.begin(), html.end(), list_item_regex);

    // Извлечение текста из <li>...</li>
    while (iter != end)
    {
        std::string list_item = (*iter)[1].str();

        // Удаление любых тегов внутри <li>
        std::regex tag_regex("<[^>]*>");
        list_item = std::regex_replace(list_item, tag_regex, "");

        result += list_item + "\n";
        ++iter;
    }

    // Регулярное выражение для поиска <td>...</td> в таблицах
    std::regex table_data_regex("<td[^>]*>(.*?)</td>");
    iter = std::sregex_iterator(html.begin(), html.end(), table_data_regex);

    // Извлечение текста из <td>...</td>
    while (iter != end)
    {
        std::string table_data = (*iter)[1].str();

        // Удаление любых тегов внутри <td>
        std::regex tag_regex("<[^>]*>");
        table_data = std::regex_replace(table_data, tag_regex, "");

        result += table_data + "\t";  // Разделяем ячейки табуляцией для удобства
        ++iter;
    }

    result += "\n";  // Добавляем новую строку после каждой строки таблицы

    return result;
}

// Функция для получения имени рецепта из URL
std::string get_recipe_name(const std::string& url)
{
    std::string::size_type start_pos = url.find_last_of("/") + 1;
    std::string::size_type end_pos = url.find_last_of(".");
    return url.substr(start_pos, end_pos - start_pos);  // Название рецепта без расширения
}

// Функция обработки страницы рецепта
void process_recipe_page(const std::string& recipe_url)
{
    std::string html = download_page(recipe_url);
    if (html == "")
    {
        return;
    }
    std::string recipe_text = extract_recipe_text(html);

    // Получаем имя рецепта из URL и создаем файл с этим именем
    std::string recipe_name = get_recipe_name(recipe_url);
    std::string output_filename = "../recepts/" + recipe_name + ".txt";

    save_to_file(output_filename, recipe_text);
}

// Функция обработки страницы со списком рецептов
void process_page_for_links(const std::string& url)
{
    std::string html = download_page(url);
    if (html == "")
    {
        return;
    }
    std::vector<std::string> links = parse_recipe_links(html);

    mtx.lock();
    for (const auto& link : links) {
        visited_links.insert(link);  // Добавляем все ссылки в множество
    }
    mtx.unlock();
}

// Функция для обработки рецептов (уже найденных ссылок)
void process_links_batch(const std::vector<std::string>& links, size_t thread_id)
{
    // mtx.lock();
    thread_time[thread_id] = 0.0f;
    thread_time_start[thread_id] = std::chrono::system_clock::now();
    for (const auto& link : links)
    {
        process_recipe_page(link);
    }
    thread_time_end[thread_id] = std::chrono::system_clock::now();
    thread_time[thread_id] += std::chrono::duration_cast<std::chrono::milliseconds>(thread_time_end[thread_id] - thread_time_start[thread_id]).count();
    // mtx.unlock();
}

// Основная функция многопоточной обработки
void parallel_processing(const std::vector<std::string>& urls, size_t num_threads)
{
    std::vector<std::thread> threads;

    // Этап 1: Сбор всех уникальных ссылок
    for (const auto& url : urls)
    {
        threads.emplace_back(process_page_for_links, url);
        if (threads.size() >= num_threads)
        {
            for (auto& th : threads) th.join();
            threads.clear();
        }
    }
    for (auto& th : threads) th.join();  // Ожидаем завершения всех потоков

    // Копируем уникальные ссылки в вектор
    std::vector<std::string> all_links(visited_links.begin(), visited_links.end());

    // Если уникальных ссылок меньше, чем потоков, уменьшаем количество потоков
    if (all_links.size() < num_threads)
    {
        num_threads = all_links.size();
    }

    // Этап 2: Делим ссылки на группы для обработки
    size_t links_per_thread = all_links.size() / num_threads;
    size_t remainder = all_links.size() % num_threads;

    size_t start_index = 0;
    for (size_t i = 0; i < num_threads; ++i)
    {
        size_t end_index = start_index + links_per_thread + (i < remainder ? 1 : 0);  // Равномерное распределение ссылок

        if (end_index > start_index)
        {
            // Выбираем подмножество ссылок для данного потока
            std::vector<std::string> batch(all_links.begin() + start_index, all_links.begin() + end_index);
            threads.emplace_back(process_links_batch, batch, i);
        }

        start_index = end_index;
    }

    // Ожидаем завершения всех потоков
    for (auto& th : threads)
    {
        if (th.joinable())
        {
            th.join();
        }
    }
}


// Последовательная обработка страниц
void sequential_processing(const std::vector<std::string>& urls)
{
    for (const auto& url : urls)
    {
        process_page_for_links(url);
    }

    // Копируем уникальные ссылки в вектор
    std::vector<std::string> all_links(visited_links.begin(), visited_links.end());

    // Обрабатываем ссылки последовательно
    process_links_batch(all_links, 0);
}

#include <chrono>

int main()
{
    std::string base_url = "https://vkusnye-recepty.ru/page/";  // Базовый URL для пагинации
    // std::string base_url = "";  // Базовый URL для пагинации

    size_t num_pages = 5;  // Количество страниц для обработки
    size_t num_threads = 64; // Количество потоков (для параллельного режима)

    // Список страниц для обработки
    std::vector<std::string> urls;
    for (size_t i = 1; i <= num_pages; ++i)
    {
        urls.push_back(base_url + std::to_string(i));
    }
    bool use_parallel = false;  // Выбор режима выполнения (последовательный или параллельный)
    bool is_experiment = true;
    size_t num_runs = 5;



    if (!is_experiment)
    {
        if (use_parallel)
        {
            parallel_processing(urls, num_threads);
        } else
        {
            sequential_processing(urls);
        }
    }
    else
    {
        if (use_parallel)
        {
            for (size_t ii = 1; ii <= num_pages; ++ii)
            {
                urls.clear();
                for (size_t i = 1; i <= ii; ++i)
                {
                    urls.push_back(base_url + std::to_string(i));
                }
                double time = 0.0f;
                thread_time.clear();
                thread_time_start.clear();
                thread_time_end.clear();
                for (size_t i = 0; i < num_runs; ++i)
                {
                    parallel_processing(urls, 16);
                    time = *std::max_element(thread_time.begin(), thread_time.end());
                }
                std::cout << ii << " " << time / num_runs << std::endl;
            }
        } else
        {
            for (size_t ii = 1; ii <= num_pages; ++ii)
            {
                urls.clear();
                for (size_t i = 1; i <= ii; ++i)
                {
                    urls.push_back(base_url + std::to_string(i));
                }
                double time = 0.0f;
                thread_time.clear();
                thread_time_start.clear();
                thread_time_end.clear();
                for (size_t i = 0; i < num_runs; ++i)
                {
                    sequential_processing(urls);
                    time = *std::max_element(thread_time.begin(), thread_time.end());
                }
                std::cout << ii << " " << time / num_runs << std::endl;
            }
            // for (size_t i = 0; i < num_runs; ++i)
            // {
            //     sequential_processing(urls);
            // }
            // std::cout << 1 << " " << thread_time[0] / num_runs << std::endl;
        }
    }

    return 0;
}
