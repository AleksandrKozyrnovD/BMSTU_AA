#include "scrapper.h"
#include "CrapperPipeLine.h"
#include "Database.h"
#include "ProcessingFunctions.h"
#include <chrono>
#include <vector>

struct helpstruct
{
    std::chrono::system_clock::time_point time;
    std::string info;

    auto operator<=>(const helpstruct& other)
    {
        return time <=> other.time;
    }
};


int main(void)
{
    SQLiteDatabase db("lab5.db");


    auto entries = get_scrapped_entries(1, 10, 25);

    std::cout << "Generated " << entries.size() << " entries" << std::endl;

    db.execute_query("DROP TABLE IF EXISTS texttable;");
    db.execute_query("DROP TABLE IF EXISTS imagetable;");

    db.execute_query("CREATE TABLE IF NOT EXISTS texttable (id INTEGER PRIMARY KEY, url TEXT KEY, vartext TEXT);");
    db.execute_query("CREATE TABLE IF NOT EXISTS imagetable (id INTEGER PRIMARY KEY, url TEXT KEY, varimage TEXT);");

    std::vector<long> lifetimes, stage1, stage2, stage3;
    std::vector<long> waiting1, waiting2, waiting3;

    std::vector<helpstruct> log;

    Pipeline<Scrapped, Scrapped> pipeline(entries.size());

    pipeline.add_stage(
        [](Scrapped scrap)
        {
            return downloader(scrap);
        }, 1
    );

    pipeline.add_stage(
        [](Scrapped scrap)
        {
            return recipe_scrapper(scrap);
        }, 1
    );

    pipeline.add_stage(
        [&db](Scrapped scrap) -> Scrapped
        {
            scrap.times.back().starttask = std::chrono::system_clock::now();
            for (auto& item : scrap.text)
            {
                if (!db.execute_query(
                    std::format("INSERT INTO texttable (url, vartext) VALUES ('{}', '{}');", scrap.url, item)
                ))
                {
                    // std::cout << "Error inserting into database" << std::endl;
                }
                if (!db.execute_query(
                    std::format("INSERT INTO imagetable (url, varimage) VALUES ('{}', '{}');", scrap.url, item)
                ))
                {
                    // std::cout << "Error inserting into database" << std::endl;
                }
            }
            scrap.times.back().endtask = std::chrono::system_clock::now();
            scrap.lifetime.end = std::chrono::system_clock::now();
            return scrap;
        }, 1
    );

    pipeline.add_stage(
        [&](Scrapped scrap) -> Scrapped
        {
            using milli = std::chrono::milliseconds;
            //in milliseconds
            auto lifetime = std::chrono::duration_cast<milli>(scrap.lifetime.end - scrap.lifetime.start).count();
            auto stage1dur = std::chrono::duration_cast<milli>(scrap.times[0].endtask - scrap.times[0].starttask).count();
            auto stage2dur = std::chrono::duration_cast<milli>(scrap.times[1].endtask - scrap.times[1].starttask).count();
            auto stage3dur = std::chrono::duration_cast<milli>(scrap.times[2].endtask - scrap.times[2].starttask).count();

            auto stage1wait = std::chrono::duration_cast<milli>(scrap.times[0].end - scrap.times[0].start).count();
            auto stage2wait = std::chrono::duration_cast<milli>(scrap.times[1].end - scrap.times[1].start).count();
            auto stage3wait = std::chrono::duration_cast<milli>(scrap.times[2].end - scrap.times[2].start).count();

            lifetimes.push_back(lifetime);

            stage1.push_back(stage1dur);
            stage2.push_back(stage2dur);
            stage3.push_back(stage3dur);

            waiting1.push_back(stage1wait);
            waiting2.push_back(stage2wait);
            waiting3.push_back(stage3wait);

            helpstruct entry1 = { scrap.times[0].start,
            "Задача " + std::to_string(scrap.unique_id) + " вошла в очередь 1" };
            helpstruct entry2 = { scrap.times[1].start,
            "Задача " + std::to_string(scrap.unique_id) + " вошла в очередь 2" };
            helpstruct entry3 = { scrap.times[2].start,
            "Задача " + std::to_string(scrap.unique_id) + " вошла в очередь 3" };

            helpstruct exit1 = { scrap.times[0].end,
            "Задача " + std::to_string(scrap.unique_id) + " вышла из очереди 1" };
            helpstruct exit2 = { scrap.times[1].end,
            "Задача " + std::to_string(scrap.unique_id) + " вышла из очереди 2" };
            helpstruct exit3 = { scrap.times[2].end,
            "Задача " + std::to_string(scrap.unique_id) + " вышла из очереди 3" };

            helpstruct starttask1 = { scrap.times[0].starttask,
            "Задача " + std::to_string(scrap.unique_id) + " в 1-м обработчике" };
            helpstruct starttask2 = { scrap.times[1].starttask,
            "Задача " + std::to_string(scrap.unique_id) + " в 2-м обработчике" };
            helpstruct starttask3 = { scrap.times[2].starttask,
            "Задача " + std::to_string(scrap.unique_id) + " в 3-м обработчике" };


            helpstruct endtask1 = { scrap.times[0].endtask,
            "Задача " + std::to_string(scrap.unique_id) + " вышла из 1-го обработчика" };
            helpstruct endtask2 = { scrap.times[1].endtask,
            "Задача " + std::to_string(scrap.unique_id) + " вышла из 2-го обработчика" };
            helpstruct endtask3 = { scrap.times[2].endtask,
            "Задача " + std::to_string(scrap.unique_id) + " вышла из 3-го обработчика" };
            

            log.push_back(entry1);
            log.push_back(entry2);
            log.push_back(entry3);
            log.push_back(exit1);
            log.push_back(exit2);
            log.push_back(exit3);
            log.push_back(starttask1);
            log.push_back(starttask2);
            log.push_back(starttask3);
            log.push_back(endtask1);
            log.push_back(endtask2);
            log.push_back(endtask3);

            return scrap;
        }, 1
    );


    for (auto& entry : entries)
    {
        std::cout << "Link: " << entry.url << ", ";
        std::cout << "Task id: " << entry.unique_id << std::endl;
        pipeline.add_entry(entry);
    }

    pipeline.finalize();
    pipeline.start();


    sort(log.begin(), log.end());    

    for (auto& logentry : log)
    {
        std::cout << logentry.time << " & " << logentry.info << "\\\\" << std::endl;
    }

    double avg_lifetime = 0;
    for (auto& lifetime : lifetimes) {avg_lifetime += lifetime;} avg_lifetime /= lifetimes.size();

    double avg_stage1 = 0;
    for (auto& stage1 : stage1) {avg_stage1 += stage1;} avg_stage1 /= stage1.size();

    double avg_stage2 = 0;
    for (auto& stage2 : stage2) {avg_stage2 += stage2;} avg_stage2 /= stage2.size();    

    double avg_stage3 = 0;
    for (auto& stage3 : stage3) {avg_stage3 += stage3;} avg_stage3 /= stage3.size();

    double avg_waiting1 = 0;
    for (auto& waiting1 : waiting1) {avg_waiting1 += waiting1;} avg_waiting1 /= waiting1.size();

    double avg_waiting2 = 0;
    for (auto& waiting2 : waiting2) {avg_waiting2 += waiting2;} avg_waiting2 /= waiting2.size();

    double avg_waiting3 = 0;
    for (auto& waiting3 : waiting3) {avg_waiting3 += waiting3;} avg_waiting3 /= waiting3.size();

    std::cout << "Ср. время жизни: " << avg_lifetime << std::endl;
    std::cout << "Ср. вр. ожид. в очер. 1: " << avg_waiting1 << std::endl;
    std::cout << "Ср. вр. ожид. в очер. 2: " << avg_waiting2 << std::endl;
    std::cout << "Ср. вр. ожид. в очер. 3: " << avg_waiting3 << std::endl;
    std::cout << "Ср. вр. выпол. в обр. 1: " << avg_stage1 << std::endl;
    std::cout << "Ср. вр. выпол. в обр. 2: " << avg_stage2 << std::endl;
    std::cout << "Ср. вр. выпол. в обр. 3: " << avg_stage3 << std::endl;


    return 0;
}


