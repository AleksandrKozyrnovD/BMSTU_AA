#ifndef __CRAPPERPIPELINE_H__
#define __CRAPPERPIPELINE_H__

#include "General/PipeLine.h"
#include "General/Stage.h"
#include "TaskQueue.h"
#include "scrapper.h"

using InputType = Scrapped;
using FinalOutputType = Scrapped;

using StageOutput = Scrapped;
using StageInput = Scrapped;

template<>
class Pipeline<InputType, FinalOutputType>
{
public:
    Pipeline() = delete;
    Pipeline(size_t min_items);
    ~Pipeline();

    void add_stage(std::function<StageOutput(StageInput)> func, int thread_count); //Модифицируй эту строчку


    bool check() const;
    void start();
    bool finalize();

    void shutdown();

    void add_entry(InputType item);
    std::optional<FinalOutputType> get_result();

private:
    SafeQueue<InputType> entry_queue;
    std::shared_ptr<SafeQueue<FinalOutputType>> exit_queue;
    std::vector<std::shared_ptr<TypedStage<StageInput, StageOutput>>> stages;
    std::vector<std::shared_ptr<SafeQueue<StageOutput>>> intermediate_queues;
    int min_items, processed_items;

    bool is_finalized = false;
};

#include "CrapperPipeLine.hpp"


#endif
