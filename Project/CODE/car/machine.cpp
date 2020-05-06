#include "machine.hpp"
#include "car.hpp"
#include "timer.hpp"
#include "util.h"

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
        ST_BRAKE,        // ST_Launch
        ST_BRAKE,        // ST_Running
        EVENT_IGNORED,   // ST_Brake
        ST_IDLE,         // ST_Recover
        CANNOT_HAPPEN,   // ST_Exception
        END_TRANSITION_MAP(NULL)
}

void CarMachine::Stopped() {
    BEGIN_TRANSITION_MAP // - Current State -
        EVENT_IGNORED,   // ST_Idle
        ST_EXCEPTION,    // ST_Launch
        ST_EXCEPTION,    // ST_Running
        ST_RECOVER,      // ST_Brake
        EVENT_IGNORED,   // ST_Recover
        ST_RECOVER,      // ST_Exception
        END_TRANSITION_MAP(NULL)
}

void CarMachine::ST_Idle(EventData *) {
    DEBUG_LOG("Idle\n");
    Car.Pause();
}

void CarMachine::ST_Launch(CarData *dat) {
    DEBUG_LOG("Launch\n");
    Car.Launch();
}

void CarMachine::ST_Running(CarData *) { DEBUG_LOG("Running\n"); }

void CarMachine::ST_Brake(CarData *) {
    DEBUG_LOG("Brake\n");
    Car.Pause();
}

void CarMachine::ST_Exception(CarData *) {
    DEBUG_LOG("Exception\n");
    Car.beep0.BeepFreq(0xBAD);
    InternalEvent(ST_BRAKE);
}

void CarMachine::ST_Recover(CarData *) {
    DEBUG_LOG("Recover\n");
    Car.beep0.BeepFreqDelay(5555, 111);
    InternalEvent(IS_DERAIL ? ST_IDLE : ST_LAUNCH);
}
