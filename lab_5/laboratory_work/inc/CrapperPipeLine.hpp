#ifndef __CRAPPERPIPELINE_HPP__
#define __CRAPPERPIPELINE_HPP__

#include "CrapperPipeLine.h"

Pipeline<InputType, FinalOutputType>::Pipeline(size_t min_items)
: stages(), min_items(min_items), processed_items(0)
{}

Pipeline<InputType, FinalOutputType>::~Pipeline()
{
    shutdown();
}

void Pipeline<InputType, FinalOutputType>::add_stage(std::function<StageOutput(StageInput)> func, int thread_count)
{
    // Определение типа очереди между этапами
    using CurrentStageType = TypedStage<StageInput, StageOutput>;

    // Если это первый этап, соединяем его с `entry_queue`
    if (stages.empty())
    {
        std::cout << "Добавляем от начала" << std::endl;
        std::shared_ptr<SafeQueue<StageOutput>> intermediateQueue = std::make_shared<SafeQueue<StageOutput>>();
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

bool Pipeline<InputType, FinalOutputType>::check() const
{
    return true;
}


bool Pipeline<InputType, FinalOutputType>::finalize()
{
    if (intermediate_queues.empty())
    {
        throw std::runtime_error("Pipeline has no stages to finalize.");
    }

    // Set the last intermediate queue as the exit queue
    // exit_queue = std::static_pointer_cast<SafeQueue<FinalOutputType>>(intermediate_queues.back());

    std::cout << "Finalizing pipeline" << std::endl;
    exit_queue = intermediate_queues.back();

    return (is_finalized = check());
}

void Pipeline<InputType, FinalOutputType>::start()
{
    if (!check() || !is_finalized)
    {
        throw std::runtime_error("Pipeline configuration is invalid.");
    }

    for (auto& stage : stages)
    {
        std::thread([stage_ptr = stage.get()] -> void
        {
            stage_ptr->start();
        }).detach();
    }

    //killer thread. He has murderous purpose
    std::thread([&] {
        while (exit_queue->get_size() < min_items)
        {
            std::cout << "waiting for killing thread, exiting queue size = " << exit_queue->get_size() << "\n";
            // std::cout << "Entry queue size = " << entry_queue.get_size() << "\n";
            // for (auto& queue : intermediate_queues)
            // {
            //     std::cout << "Queue size = " << queue->get_size() << std::endl;
            // }
            // std::cout << "min items = " << min_items << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        shutdown();
    }).join();
}

void Pipeline<InputType, FinalOutputType>::shutdown()
{
    for (auto& stage : stages)
    {
        stage->shutdown();
    }
}

void Pipeline<InputType, FinalOutputType>::add_entry(InputType item)
{
    entry_queue.enqueue(std::move(item));
}

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
