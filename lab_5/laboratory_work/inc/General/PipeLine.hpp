#ifndef __PIPELINE_HPP__
#define __PIPELINE_HPP__

#include "PipeLine.h"
#include "Stage.h"
#include <memory>

template<typename InputType, typename FinalOutputType>
Pipeline<InputType, FinalOutputType>::Pipeline(size_t min_items)
: stages(), min_items(min_items), processed_items(0)
{}

template<typename InputType, typename FinalOutputType>
Pipeline<InputType, FinalOutputType>::~Pipeline()
{
    shutdown();
}

template<typename InputType, typename FinalOutputType>
template <typename StageInput, typename StageOutput>
void Pipeline<InputType, FinalOutputType>::add_stage(std::function<StageOutput(StageInput)> func, int thread_count)
{
    // Определение типа очереди между этапами
    using CurrentStageType = TypedStage<StageInput, StageOutput>;

    // Если это первый этап, соединяем его с `entry_queue`
    if (stages.empty())
    {
        std::shared_ptr<SafeQueue<StageOutput>> intermediateQueue = std::make_shared<SafeQueue<StageOutput>>();
        std::shared_ptr<SafeQueue<StageInput>> entry = std::make_shared<SafeQueue<StageInput>>();  
        std::shared_ptr<CurrentStageType> stage = std::make_shared<CurrentStageType>(entry_queue, *intermediateQueue, func, thread_count);
        stages.push_back(std::move(stage));
        intermediate_queues.push_back(std::move(intermediateQueue));
    }
    else // иначе подключаем к выходной очереди предыдущего этапа
    {
        auto previousQueue = std::static_pointer_cast<SafeQueue<StageInput>>(intermediate_queues.back());
        auto intermediateQueue = std::make_shared<SafeQueue<StageOutput>>();
        auto stage = std::make_shared<CurrentStageType>(*previousQueue, *intermediateQueue, func, thread_count);
        stages.push_back(std::move(stage));
        intermediate_queues.push_back(std::move(intermediateQueue));
    }
}

template<typename InputType, typename FinalOutputType>
bool Pipeline<InputType, FinalOutputType>::check() const
{
    return true;
}

template <typename InputType, typename FinalOutputType>
bool Pipeline<InputType, FinalOutputType>::finalize()
{
    if (intermediate_queues.empty())
    {
        throw std::runtime_error("Pipeline has no stages to finalize.");
    }

    // Set the last intermediate queue as the exit queue
    exit_queue = std::static_pointer_cast<SafeQueue<FinalOutputType>>(intermediate_queues.back());

    return (is_finalized = check());
}

template<typename InputType, typename FinalOutputType>
void Pipeline<InputType, FinalOutputType>::start()
{
    if (!check() || !is_finalized)
    {
        throw std::runtime_error("Pipeline configuration is invalid.");
    }

    for (auto& stage : stages)
    {
        std::thread([stage_ptr = stage.get()] {
            stage_ptr->start();
        }).detach();
    }

    //killer thread. He has murderous purpose
    std::thread([&] {
        while (exit_queue->get_size() < min_items)
        {
            std::cout << "waiting for killing thread\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        shutdown();
    }).join();
}

template<typename InputType, typename FinalOutputType>
void Pipeline<InputType, FinalOutputType>::shutdown()
{
    for (auto& stage : stages)
    {
        stage->shutdown();
    }
}

template<typename InputType, typename FinalOutputType>
void Pipeline<InputType, FinalOutputType>::add_entry(InputType item)
{
    entry_queue.enqueue(std::move(item));
}

template<typename InputType, typename FinalOutputType>
std::optional<FinalOutputType> Pipeline<InputType, FinalOutputType>::get_result()
{
    auto result = exit_queue->dequeue();
    if (result)
    {
        ++processed_items;
        if (processed_items >= min_items)
        {
            std::cout << "min items reached, shutting down pipeline\n";
            shutdown();
            return result;
        }
    }
    return result;
}

#endif
