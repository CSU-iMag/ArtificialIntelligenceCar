#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <functional>
#include <initializer_list>

typedef std::initializer_list<void *> sched_event_data_t;
typedef std::function<void(sched_event_data_t)> sched_event_handler_t;
// typedef void (*sched_event_handler_t)(sched_event_data_t event_data);

void sched_execute();
void sched_event_put(sched_event_handler_t event_handler,
                     const sched_event_data_t &event_data = {});

#endif
