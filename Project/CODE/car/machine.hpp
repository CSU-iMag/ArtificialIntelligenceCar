#ifndef MACHINE_H
#define MACHINE_H

#include "StateMachine.hpp"

//! @brief structure to hold event data passed into state machine
struct CarData : public EventData {
    explicit CarData(float speed) : TargetSpeed(speed) {}
    float TargetSpeed;
};

//! @brief the Car state machine class
class CarMachine : public StateMachine {
  public:
    CarMachine() : StateMachine(ST_MAX_STATES) {}

    // external events taken by this state machine

    //! @brief set motor speed external event
    void SetSpeed(CarData *);

    //! @brief halt motor external event
    void Halt();

    //! @brief 启动电机
    void Start();

    //! @brief 暂停电机
    void Pause();

    //! @brief 稳定运行
    void Steady();

    //! @brief 触发异常
    void DetectException();

    //! @brief 停稳了
    void Stopped();

  private:
    // state machine state functions

    //! @brief 空闲时关闭电机
    void ST_Idle(EventData *);

    //! @brief 启动时调用Car.Launch()并检测异常
    void ST_Launch(CarData *);

    //! @brief 稳定运行，开启保护定时器
    void ST_Running(CarData *);

    //! @brief 正在刹车，停稳后转移状态
    void ST_Brake(CarData *);

    //! @brief 运行异常：堵转、出轨、离地
    void ST_Exception(CarData *);

    //! @brief 自动恢复
    void ST_Recover(CarData *);

    // state map to define state function order
    BEGIN_STATE_MAP
    STATE_MAP_ENTRY(&CarMachine::ST_Idle)
    STATE_MAP_ENTRY(&CarMachine::ST_Launch)
    STATE_MAP_ENTRY(&CarMachine::ST_Running)
    STATE_MAP_ENTRY(&CarMachine::ST_Brake)
    STATE_MAP_ENTRY(&CarMachine::ST_Recover)
    STATE_MAP_ENTRY(&CarMachine::ST_Exception)
    END_STATE_MAP

    /** @warning state enumeration order must match the
     * order of state method entries in the state map
     */
    enum E_States {
        ST_IDLE = 0,
        ST_LAUNCH,
        ST_RUNNING,
        ST_BRAKE,
        ST_RECOVER,
        ST_EXCEPTION,
        ST_MAX_STATES
    };
};

#endif

/*
void CarMachine::(CarData *pData) {
    BEGIN_TRANSITION_MAP // - Current State -
        , // ST_Idle
        , // ST_Launch
        , // ST_Running
        , // ST_Brake
        , // ST_Recover
        , // ST_Exception
    END_TRANSITION_MAP(pData)
}
*/
