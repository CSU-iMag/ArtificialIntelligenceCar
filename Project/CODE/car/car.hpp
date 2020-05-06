#ifndef CAR_H
#define CAR_H

#include "car_config.h"
#include "machine.hpp"
#include "magnet.hpp"
#include "motor.hpp"
#include "pid.hpp"
#include "steer.hpp"
#include "timer.hpp"
#pragma region bsp
#include "beep.hpp"
#include "bluetooth.hpp"
#include "encoder.hpp"
#include "key.hpp"
#include "led.hpp"
#include "route.h"
#pragma endregion

//! @brief Steer PID
#define STEER_K(pid) (Car.Steer3010.steerCtrl.instance.K##pid)

/**
 * @brief Speed PID
 * @note SPEED_K(L, p) == Car.MotorL.speedCtrl.instance.Kp
 *       SPEED_K(R, d) == Car.MotorR.speedCtrl.instance.Kd
 */
#define SPEED_K(LR, pid) ((Car.Motor##LR).speedCtrl.instance.K##pid)

//! @brief 运行状态判定
#define IS_DERAIL                                                              \
    (Car.MagList[MagL_FRONT]->GetNormalized() +                                \
         Car.MagList[MagM_FRONT]->GetNormalized() +                            \
         Car.MagList[MagR_FRONT]->GetNormalized() <                            \
     MAG_DERAIL_THRESHOLD)

//! @warning need critial region protect
#define IS_STOPPED                                                             \
    (std::abs(Car.EncoderL.GetSpeed()) < 5 &&                                  \
     std::abs(Car.EncoderR.GetSpeed()) < 5)

#define _IS_STEADY_(speed)                                                     \
    (speed == lim(speed, Car.TargetSpeed - 30, Car.TargetSpeed + 30))
#define IS_STEADY                                                              \
    (_IS_STEADY_(Car.EncoderL.GetSpeed()) &&                                   \
     _IS_STEADY_(Car.EncoderR.GetSpeed()))

void LaunchDelaySchedule(sched_event_data_t dat);

enum class ControlMode { PID, AI };

class iMagCar { // 什么时候该用friend？什么时候public？
  public:
    ControlMode CtrlMode;

    //! @brief
    CarMachine Machine;

    //! @brief 目标速度
    float TargetSpeed;

    //! @brief 12路电磁传感器
    static MagSensor *MagList[ADC_CNT];

    //! @brief key pad
    static std::vector<Button> Keypad;

    //! @brief 左右电机
    Motor MotorL, MotorR;

    //! @brief 脉冲编码器
    Encoder EncoderL, EncoderR;

    //! @brief 舵机
    Steer Steer3010;

    Beep beep0;
    HardLED ledBlue, ledCore;
    SoftLED ledWhite, ledGreen;
    Bluetooth Hc06;

    //! @brief
    SoftTimer LaunchTimer;

    //! @warning 这里不能有任何操作硬件的代码！
    iMagCar()
        : TargetSpeed(TARGET_SPEED), MotorL(MOTOR_L, MOTOR_MIN, MOTOR_MAX),
          MotorR(MOTOR_R, MOTOR_MIN, MOTOR_MAX),
          EncoderL(encoderL_QTIMER, encoderL_LSB_QTIMER, encoderL_DIR_QTIMER),
          EncoderR(encoderR_QTIMER, encoderR_LSB_QTIMER, encoderR_DIR_QTIMER),
          Steer3010(S3010_PWM, STEER_MIN - STEER_CENTER,
                    STEER_MAX - STEER_CENTER),
          ledGreen(LED_PWM1), ledBlue(LED_PIN3), ledWhite(LED_PWM2),
          ledCore(LED_PIN0), Hc06(HC06_UART, HC06_TX, HC06_RX),
          LaunchTimer(LaunchDelaySchedule, 3) {}
    ~iMagCar();

    //! @brief 开机
    void Startup();

    //! @brief 只在主循环中调用Car.Run()
    void Run();

    //! @brief 启动电机
    void Launch();

    //! @brief 暂停电机
    void Pause();
};

extern iMagCar Car; // unique global var !!!

#endif
