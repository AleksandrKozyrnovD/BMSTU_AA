#ifndef __STAGE_H__
#define __STAGE_H__

#include <functional>
#include <thread>
#include <vector>
#include <stdexcept>
#include <mutex>
#include <condition_variable>

#include "SafeQueue.h"

class StageBase
{
public:
    virtual ~StageBase() = default;
    virtual void start() = 0; // Pure virtual function to be overridden
    virtual void shutdown() = 0; // Pure virtual function to be overridden
};

template <typename InputType, typename OutputType>
class TypedStage : public StageBase
{
public:
    TypedStage(SafeQueue<InputType>& input, SafeQueue<OutputType>& output,
               std::function<OutputType(InputType)> func, int threads);

    TypedStage(std::function<OutputType(InputType)> func, int threads);


    void start() override;
    void shutdown() override;

    void link_queues(SafeQueue<InputType>* in_queue, SafeQueue<OutputType>* out_queue);

    const std::type_info& input_type() const;
    const std::type_info& output_type() const;

    SafeQueue<OutputType>* get_output_queue() const;

    template <typename OtherInputType, typename OtherOutputType>
    static bool can_connect(const TypedStage<OtherInputType, OtherOutputType>&)
    {
        return std::is_same_v<OutputType, OtherInputType>;
    }

private:
    void worker();

private:
    std::function<OutputType(InputType)> process;
    SafeQueue<InputType>& input_queue;
    SafeQueue<OutputType>& output_queue;
    int thread_count;
    bool shutdown_flag;
    std::mutex shutdown_mutex;
    std::condition_variable shutdown_cv;
};

#include "Stage.hpp"

#endif
