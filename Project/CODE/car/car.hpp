#ifndef CAR_H
#define CAR_H

#include "car_config.h"
#include "machine.hpp"
#include "magnet.hpp"
#include "motor.hpp"
#include "peripherals.h"
#include "pid.hpp"
#include "steer.hpp"
#include "timer.hpp"
#include "deep.hpp"
#pragma region bsp
#include "encoder.hpp"
#include "key.hpp"
#include "led.hpp"
#include "route.h"
#pragma endregion

#define SW_MASK_STEER 0u
#define SW_MASK_SPEED 1u
#define SW_MASK_AUTO_STOP 2u
#define SW_MASK_SEND_MOTOR 3u
#define SW_MASK_SEND_DIR 5u
#define SW_MASK_SEND_AI 6u

//! @brief Steer PID
#define STEER_K(pid) (Car.Steer3010.steerCtrl.instance.K##pid)

/**
 * @brief Speed PID
 * @code
 *      SPEED_K(L, p) == Car.MotorL.speedCtrl.instance.Kp
 *      SPEED_K(R, d) == Car.MotorR.speedCtrl.instance.Kd
 * @endcode
 */
#define SPEED_K(LR, pid) ((Car.Motor##LR).speedCtrl.instance.K##pid)

//! @brief 运行状态判定
#define IS_DERAIL                                                              \
    (Car.MagList[MagFrontL].GetNormalized() +                                \
         Car.MagList[MagFrontM].GetNormalized() +                            \
         Car.MagList[MagFrontR].GetNormalized() <                            \
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

//! @brief err == 0 时停车报错
#define CAR_ERROR_CHECK(err)                                                   \
    if (!(err)) {                                                              \
        Car.Pause();                                                           \
        DisableGlobalIRQ();                                                    \
        PRINTF("CAR ASSERT ERROR \" %s \": file \"%s\" Line \"%d\" \n", #err,  \
               __FILE__, __LINE__);                                            \
        __BKPT(0);                                                             \
    }

void LaunchDelaySchedule(sched_event_data_t dat);

enum class ControlMode { PID, AI };

class iMagCar { // 什么时候该用friend？什么时候public？
  public:
    //! @brief 8 Switches
    uint8_t Enabled;

    //! @brief 神经网络模型
    const unsigned char *Model = model1;

    //! @brief 方向控制模式
    ControlMode CtrlMode = ControlMode::PID;

    //! @brief 转向时减速
    float Deceleration = 0.26;

    //! @brief 转向时差速
    float MotorDiffer = 0;

    //! @brief 状态机
    CarMachine Machine;

    //! @brief 目标速度
    float TargetSpeed;

    //! @brief 12路电磁传感器
    static std::vector<MagSensor> MagList;

    //! @brief key pad
    static std::vector<Button> Keypad;

    //! @brief 左右电机
    Motor MotorL, MotorR;

    //! @brief 脉冲编码器
    Encoder EncoderL, EncoderR;

    //! @brief 舵机
    Steer Steer3010;

    //! @brief
    SoftTimer LaunchTimer;

    //! @brief 核心板蓝灯
    HardLED CoreLED;

    //! @warning 这里不能有任何操作硬件的代码！
    iMagCar()
        : TargetSpeed(69), MotorL(MOTOR_L, MOTOR_MIN, MOTOR_MAX),
          MotorR(MOTOR_R, MOTOR_MIN, MOTOR_MAX),
          EncoderL(PULSEENCODER_CHANNEL_2_CHANNEL),
          EncoderR(PULSEENCODER_CHANNEL_0_CHANNEL),
          Steer3010(S3010_PWM, STEER_MIN - STEER_CENTER,
                    STEER_MAX - STEER_CENTER),
          LaunchTimer(LaunchDelaySchedule, 1), CoreLED(B9, 0) {}
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
