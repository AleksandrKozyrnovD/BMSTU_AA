#ifndef __PIPELINE_H__
#define __PIPELINE_H__

#include <functional>
#include <thread>
#include <vector>
#include <memory>
#include <optional>
#include <stdexcept>
#include <iostream>

#include "General/Stage.h"


// Pipeline class to manage stages with different input/output types
template <typename InputType, typename FinalOutputType>
class Pipeline
{
public:
    Pipeline() = delete;
    Pipeline(size_t min_items);
    ~Pipeline();

    template <typename StageInput, typename StageOutput>
    void add_stage(std::function<StageOutput(StageInput)> func, int thread_count);


    bool check() const;
    void start();
    bool finalize();

    void shutdown();

    void add_entry(InputType item);
    std::optional<FinalOutputType> get_result();

private:
    SafeQueue<InputType> entry_queue;
    // SafeQueue<FinalOutputType> exit_queue;
    std::shared_ptr<SafeQueue<FinalOutputType>> exit_queue;
    std::vector<std::shared_ptr<StageBase>> stages;
    std::vector<std::shared_ptr<void>> intermediate_queues;
    int min_items, processed_items;

    bool is_finalized = false;
};

#include "PipeLine.hpp"

#endif
