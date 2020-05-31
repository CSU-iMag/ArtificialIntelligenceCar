#include "car_config.h"
#include "route.h"
#include "timer.hpp"
#include "util.h"
#include <new>

std::uint_fast64_t cpu_usage_cnt, cpu_usage_raw;

void no_memory() {
    DEBUG_LOG("No more memory!\n");
    abort();
}

unsigned cpu_usage_get() { return cpu_usage_raw; }

void cpu_usage_count() { ++cpu_usage_cnt; }

void cpu_usage_sched(sched_event_data_t dat) {
    cpu_usage_raw = cpu_usage_cnt;
    cpu_usage_cnt = 0;
}

static SoftTimer cpu_usage_tim(cpu_usage_sched);

void cpu_usage_init() {
    cpu_usage_tim.Start(CPU_USAGE_PERIOD);
    std::set_new_handler(no_memory);
}
