#include "machine.hpp"
#include "car.hpp"
#include "timer.hpp"
#include "util.h"
#include "communication.hpp"

void CarMachine::Halt(void) {
    // given the Halt event, transition to a new state based upon
    // the current state of the state machine
    BEGIN_TRANSITION_MAP // - Current State -
        EVENT_IGNORED,   // ST_Idle
        ST_IDLE,         // ST_Launch
        ST_BRAKE,        // ST_Running
        ST_IDLE,         // ST_Brake
        ST_IDLE,         // ST_Recover
        ST_BRAKE,        // ST_Exception
        END_TRANSITION_MAP(NULL)
}

void CarMachine::SetSpeed(CarData *pData) {
    //保留例程，无需调用
    BEGIN_TRANSITION_MAP // - Current State -
        EVENT_IGNORED,   // ST_Idle
        ST_LAUNCH,       // ST_Launch
        ST_BRAKE,        // ST_Running
        CANNOT_HAPPEN,   // ST_Brake
        CANNOT_HAPPEN,   // ST_Recover
        CANNOT_HAPPEN,   // ST_Exception
        END_TRANSITION_MAP(pData)
}

void CarMachine::Start() {
    BEGIN_TRANSITION_MAP // - Current State -
        ST_LAUNCH,       // ST_Idle
        CANNOT_HAPPEN,   // ST_Launch
        CANNOT_HAPPEN,   // ST_Running
        ST_LAUNCH,       // ST_Brake
        ST_LAUNCH,       // ST_Recover
        EVENT_IGNORED,   // ST_Exception
        END_TRANSITION_MAP(NULL)
}

void CarMachine::Steady() {
    BEGIN_TRANSITION_MAP // - Current State -
        ST_IDLE,         // ST_Idle
        ST_RUNNING,      // ST_Launch
        EVENT_IGNORED,   // ST_Running
        EVENT_IGNORED,   // ST_Brake
        CANNOT_HAPPEN,   // ST_Recover
        EVENT_IGNORED,   // ST_Exception
        END_TRANSITION_MAP(NULL)
}

void CarMachine::Pause() {
    BEGIN_TRANSITION_MAP // - Current State -
        EVENT_IGNORED,   // ST_Idle
        ST_IDLE,         // ST_Launch
        ST_BRAKE,        // ST_Running
        EVENT_IGNORED,   // ST_Brake
        ST_IDLE,         // ST_Recover
        ST_BRAKE,        // ST_Exception
        END_TRANSITION_MAP(NULL)
}

void CarMachine::DetectException() {
    BEGIN_TRANSITION_MAP // - Current State -
        EVENT_IGNORED,   // ST_Idle
#ifdef AUTO_STOP
        ST_BRAKE, // ST_Launch
        ST_BRAKE, // ST_Running
#else
        EVENT_IGNORED, // ST_Launch
        EVENT_IGNORED, // ST_Running
#endif
        EVENT_IGNORED, // ST_Brake
        ST_IDLE,       // ST_Recover
        CANNOT_HAPPEN, // ST_Exception
        END_TRANSITION_MAP(NULL)
}

void CarMachine::Stopped() {
    BEGIN_TRANSITION_MAP // - Current State -
        EVENT_IGNORED,   // ST_Idle
        EVENT_IGNORED,    // ST_Launch
#ifdef AUTO_STOP
        ST_EXCEPTION,    // ST_Running
#else
        EVENT_IGNORED, // ST_Running
#endif
        ST_RECOVER,      // ST_Brake
        EVENT_IGNORED,   // ST_Recover
        ST_RECOVER,      // ST_Exception
        END_TRANSITION_MAP(NULL)
}

void CarMachine::ST_Idle(EventData *) {
    com_log("Idle\n");
    Car.Pause();
}

void CarMachine::ST_Launch(CarData *dat) {
    com_log("Launch\n");
    Car.Launch();
}

void CarMachine::ST_Running(CarData *) { com_log("Running\n"); }

void CarMachine::ST_Brake(CarData *) {
    com_log("Brake\n");
    Car.Pause();
}

void CarMachine::ST_Exception(CarData *) {
    com_log("Exception\n");
    Car.Buzzer.BeepFreqDelay(7890, 666);
    InternalEvent(ST_BRAKE);
}

void CarMachine::ST_Recover(CarData *) {
    com_log("Recover\n");
    // InternalEvent(IS_DERAIL ? ST_IDLE : ST_LAUNCH);
}
