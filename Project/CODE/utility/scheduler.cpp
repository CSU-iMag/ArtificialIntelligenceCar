/**
 * @file scheduler.cpp
 * @brief The scheduler is used for transferring execution from the interrupt
 *        context to the main context.
 */
#include "scheduler.hpp"
#include "util.h"
#include <queue>
#include <utility>

// struct sched_buf_t {
//     sched_event_data_t event_data;
//     sched_event_handler_t event_handler;
// };
typedef std::pair<sched_event_data_t, sched_event_handler_t> sched_buf_t;

SECTION_SDRAM static std::queue<sched_buf_t> sched_q;

/** @brief Function for scheduling an event.
 *
 *  @details Puts an event into the event queue.
 *
 *  @param[in]   data       Pointer to event data to be scheduled.
 *  @param[in]   handler    Event handler to receive the event.
 */
void sched_event_put(sched_event_handler_t handler,
                     const sched_event_data_t &data) {
    // DEBUG_LOG("sched_q.size= %d\n", sched_q.size());
    // generate the object at tail of queue
    sched_q.emplace(std::make_pair(data, handler));
}

/** @brief Function for executing all scheduled events.
 *
 *  @details This function must be called from within the main loop. It will
 *           execute all events scheduled since the last time it was called.
 *  @note Since this function is only called from the main loop, there is no
 *        need for a critical region here.
 */
void sched_execute() {
    while (!sched_q.empty()) {
        auto &tmp(sched_q.front());
        assert(tmp.second);
        // DEBUG_LOG("%x\n", tmp.event_handler);
        tmp.second(tmp.first);
        sched_q.pop();
    }
}
