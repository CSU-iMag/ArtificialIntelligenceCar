#ifndef TIMER_H
#define TIMER_H

#include "scheduler.hpp"
#include <cstdint>

struct SoftTimer {
    friend void timer_tick();
    
    explicit SoftTimer(sched_event_handler_t EventHandler, uint8_t Repeat = 0)
        : repeat(Repeat), handler(EventHandler) {}

    /**
     * @brief     开始定时
     * @param[in] timeout_us  定时周期
     * @param[in] context     事件的参数，默认空
     * @note      重复次数在初始化时指定
     */
    void Start(uint32_t timeout_ms,
               sched_event_data_t context = {});
    //! @brief 暂停定时器，重启后只能单次触发
    void Stop();
    //! @note 暂停后重新开始，而不是继续！
    void Restart();

    bool operator<(const SoftTimer &rhs) {
        return timeout < rhs.timeout;
    }

  private:
    uint32_t timeout; //!< 还有多久触发事件
    uint16_t repeat, repeat_cnt;   //!< 还需要触发几次事件 @note 0为无限循环
    sched_event_handler_t handler; //!< 要触发的事件
    sched_event_data_t context;    //!< 给事件传参数
    uint32_t period;               //!< 事件触发间隔

    //! @brief 调度事件
    void Expire();
};

#endif
