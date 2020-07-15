#include "timer.hpp"
#include "peripherals.h"
#include "util.h"
#include <queue>

volatile static uint32_t _timer_ticks; //!< 单调递增

struct TimeComparer {
    bool operator()(SoftTimer *lhs, SoftTimer *rhs) { return !(*lhs < *rhs); }
};

SECTION_SDRAM static std::priority_queue<SoftTimer *, std::vector<SoftTimer *>,
                                         TimeComparer>
    _q; //!< 这是一棵平衡树

void SoftTimer::Restart() {
    timeout = period + _timer_ticks;
    repeat_cnt = repeat;
    _q.push(this);
}

void SoftTimer::Start(uint32_t timeout_ms, sched_event_data_t event_context) {
    period = timeout_ms;
    context = event_context;
    Restart();
}

void SoftTimer::Stop() { repeat_cnt = 1; }

void SoftTimer::Expire() {
    sched_event_put(handler, context);
    if (repeat_cnt == 1)
        return;
    if (repeat_cnt > 1)
        --repeat_cnt;
    Restart();
}

void timer_tick() {
    ++_timer_ticks;
    for (SoftTimer *it; !_q.empty();) {
        it = _q.top();
        if (it->timeout <= _timer_ticks) {
            _q.pop();
            it->Expire();
        } else
            return;
    }
}

extern "C" void SOFTTIMER_GPT_IRQHANDLER(void) {
    if (GPT_GetStatusFlags(SOFTTIMER_PERIPHERAL, kGPT_OutputCompare1Flag)) {
        timer_tick();
        GPT_ClearStatusFlags(SOFTTIMER_PERIPHERAL, kGPT_OutputCompare1Flag);
    }
    __DSB();
}
