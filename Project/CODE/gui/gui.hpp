#ifndef GUI_H
#define GUI_H

#include "SGUI_FontResource.h"
#include "SGUI_IconResource.h"
#include "car.hpp"
#include "common.h"
#include "layout.hpp"
#include "screen.h"
#include "timer.hpp"
#include "tree.hpp"

#include <string>

extern struct BackGround gui_background;
extern struct HomePage gui_home;
extern struct DebugInfo gui_debug;
extern struct SteeringConfig gui_steering;
extern struct Resistance gui_resistance;
extern struct ControlPanel gui_control;
extern struct MagadcDat gui_magadcDat;
extern struct MotorConfig gui_motor;

struct BackGround : BasicLayout {
    BackGround();
    void UpdateValue(uint8_t bat_val);

  private:
    SGUI_PROCBAR_STRUCT battery;
    struct TreeNode tree;
    virtual void Show();
    virtual __inline void KeyEnterPush() { ActiveLayout = tree.Children[0]; }
    virtual void KeyUpPush();
    virtual void KeyDownPush();
};

struct HomePage : ListLayout {
    HomePage();

  private:
    static const int ChildrenCnt = 10;
    struct TreeNode tree = {.Parent = &gui_background};
    SGUI_ITEMS_ITEM its[ChildrenCnt];

    //! @brief 打开子菜单
    void KeyEnterPush();
};

struct DebugInfo : ListLayout {
    DebugInfo() : ListLayout(&tree, its, "DebugInfo", {"1", "2", "3"}) {}

    //! @brief 更新第i行显示的字符串
    __inline void UpdateValue(unsigned id, std::string str) {
        if (this != ActiveLayout)
            return;
        Items[id].UpdateValue(str);
        Repaint();
    }

  private:
    static const int ChildrenCnt = 3;
    struct TreeNode tree = {.Parent = &gui_home};
    SGUI_ITEMS_ITEM its[ChildrenCnt];
};

struct AttitudeDat : ListLayout {
    AttitudeDat()
        : ListLayout(&tree, its, "Attitude", {"acc_x", "acc_y", "acc_z"}) {}
    void UpdateValue();

  private:
    static const uint8_t cnt = 6;
    struct TreeNode tree = {.Parent = &gui_home};
    SGUI_ITEMS_ITEM its[cnt];
};

struct Resistance : ListLayout {
    Resistance()
        : ListLayout(&tree, its, "-数字电位器",
                     {"R_A1 = ", "R_A2 = ", "R_A3 = ", "R_A4 = ", "R_B1 = ",
                      "R_B2 = ", "R_B3 = ", "R_B4 = "}) {}

    //! @brief 更新第几个电阻值
    void UpdateValue(uint8_t sel);

  private:
    static const uint8_t ResNum = PGA_CNT;
    struct TreeNode tree = {.Parent = &gui_home};
    SGUI_ITEMS_ITEM its[ResNum];

    //! @brief R--
    virtual void KeyLeftPush();

    //! @brief R++
    virtual void KeyRightPush();

    //! @brief Calibrate
    virtual void KeyEnterPush();
};

struct MagadcDat : ListLayout {
    MagadcDat()
        : ListLayout(&tree, its, " 电磁传感器",
                     {"Record Max", "1: ", "2: ", "3: ", "4: ", "5: ", "6: ",
                      "7: ", "8: ", "9: ", "10: ", "11: ", "12: "}) {}
    void UpdateValue();

  private:
    bool FigureMode;
    static const uint8_t AdcNum = ADC_CNT + 1;
    struct TreeNode tree = {.Parent = &gui_home};
    SGUI_ITEMS_ITEM its[AdcNum];

    //! @brief R--
    virtual void KeyLeftPush();

    //! @brief R++
    virtual void KeyRightPush();

    //! @brief Switch display mode: figure or graph
    virtual void KeyEnterPush();
};

struct HC06AT : ListLayout {
    HC06AT()
        : ListLayout(&tree, its, "HC06",
                     {"RxBuff= ", "TestAT  ", "Baud  = ", "passw = ",
                      "MA/SL = ", "Valida= ", "Version "}) {}
    void UpdateValue();

  private:
    static const uint8_t Hc06Num = 7;
    struct TreeNode tree = {.Parent = &gui_home};
    SGUI_ITEMS_ITEM its[Hc06Num];

    //! @brief
    virtual void KeyLeftPush();

    //! @brief
    virtual void KeyRightPush();

    //! @brief
    virtual void KeyEnterPush();
};

struct SteeringConfig : ListLayout {
    static const uint8_t ConfigNum = 6;
    CurveLayout err_curve;

    SteeringConfig()
        : ListLayout(&tree, its, "-方向控制",
                     {"Kp: ", "Ki: ", "Kd: ", "软件调零：", "高电平时间：",
                      "ctrl mode: "}),
          err_curve(&err_tree, " Steering PID ErrNow",
                    (SGUI_RTGRAPH_CONTROL){20, -20, SGUI_TRUE, 1, 0}) {}

    void UpdateValue(uint8_t row, int8_t inc);

  private:
    uint16_t pulse_width = STEER_CENTER;
    struct TreeNode tree = {.Parent = &gui_home, .Children = {&err_curve}},
                    err_tree = {.Parent = &gui_steering};
    SGUI_ITEMS_ITEM its[ConfigNum];

    //! @brief 减
    virtual void KeyLeftPush();

    //! @brief 加
    virtual void KeyRightPush();

    virtual void KeyEnterPush();
};

struct ControlPanel : ListLayout {
    ControlPanel();
    void UpdateValue(uint8_t row, std::string val);
    CurveLayout battery_curve;

  private:
    static const uint8_t item_cnt = 7;
    struct TreeNode tree = {.Parent = &gui_home},
                    bat_tree = {.Parent = &gui_control};
    SGUI_ITEMS_ITEM its[item_cnt];

    virtual void KeyEnterPush();
};

struct ComEnable : ListLayout {
    ComEnable()
        : ListLayout(&tree, its, "-上位机通信使能",
                     {"发送AI包：", "速度包：", "逐飞示波器："}) {}

  private:
    static const uint8_t item_cnt = 3;
    struct TreeNode tree = {.Parent = &gui_home};
    SGUI_ITEMS_ITEM its[item_cnt];

    virtual void KeyEnterPush();
};

struct MotorConfig : ListLayout {
    static SoftTimer upd_tmr;

    MotorConfig()
        : ListLayout(&tree, its, "-后轮电机配置",
                     {"目标速度：", "左轮速度：", "右轮速度：", "左轮距离：",
                      "右轮距离：", "清空距离", "固定占空比："}) {}
    static void UpdateValue(sched_event_data_t);

  private:
    static const uint8_t item_cnt = 7;
    struct TreeNode tree = {.Parent = &gui_home};
    SGUI_ITEMS_ITEM its[item_cnt];
    int8_t duty;
   
    void UpdateDuty(int8_t inc);
    virtual void KeyEnterPush();
    virtual void KeyLeftPush();
    virtual void KeyRightPush();
};

struct ModelSelect : MenuLayout {
    ModelSelect();

  private:
    static const uint8_t item_cnt = 3;
    struct TreeNode tree = {.Parent = &gui_home};
    SGUI_ITEMS_ITEM its[item_cnt];

    virtual void KeyEnterPush();
};

static __inline void gui_reloadVal() {
    for (uint8_t i(0); i < PGA_CNT; ++i)
        gui_resistance.UpdateValue(i);
    for (uint8_t i(0); i < gui_steering.ConfigNum; ++i)
        gui_steering.UpdateValue(i, 0);
}

#endif
