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
extern struct RingLoad gui_ring;

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
    static const int ChildrenCnt = 9;
    struct TreeNode tree = {.Parent = &gui_background};
    SGUI_ITEMS_ITEM its[ChildrenCnt];

    //! @brief 闂佺懓鐏氶幐鍝ユ閹寸姭鍋撳☉娆忓鐟滄澘绉瑰畷锟犳晸閿燂拷
    void KeyEnterPush();
};

struct DebugInfo : ListLayout {
    DebugInfo() : ListLayout(&tree, its, "DebugInfo", {"1", "2", "3"}) {}

    //! @brief 闂佸�?��ら悺銊╁蓟婵犲�?绠旈柣鐐跺皺閹壆浠﹂悙顑芥灃缂備�?鍋撻柧蹇氼潐閻ｉ亶鎮楀☉娆樻畼妞ゆ垳鐒︾粙澶愭晸閿燂拷
    __inline void UpdateValue(unsigned id, std::string str) {
//        if (this != ActiveLayout)
//            return;
        Items[id].UpdateValue(str);
        Repaint();
    }

  private:
    static const int ChildrenCnt = 3;
    struct TreeNode tree = {.Parent = &gui_home};
    SGUI_ITEMS_ITEM its[ChildrenCnt];
};

struct MagadcDat : ListLayout {
    MagadcDat()
        : ListLayout(&tree, its, " ��Ŵ�����",
                     {"Record Max", "1: ", "2: ", "3: ", "4: ", "5: ", "6: ",
                      "7: ", "8: ", "9: ", "10: ", "11: ", "12: "}) {}
    void UpdateValue();
 
  private:
    bool FigureMode;
    static const uint8_t AdcNum = ADC_CNT + 1;
    struct TreeNode tree = {.Parent = &gui_home};
    SGUI_ITEMS_ITEM its[AdcNum];

    //! @brief Switch display mode: figure or graph
    virtual void KeyEnterPush();
};

struct SteeringConfig : ListLayout {
    static const uint8_t ConfigNum = 6;
    CurveLayout err_curve;

    SteeringConfig()
        : ListLayout(&tree, its, "-�������",
                     {"Kp: ", "Ki: ", "Kd: ", "�������", "�ߵ�ƽʱ��",
                      "ctrl mode: "}),
          err_curve(&err_tree, " Steering PID ErrNow",
                    (SGUI_RTGRAPH_CONTROL){20, -20, SGUI_TRUE, 1, 0}) {}

    void UpdateValue(uint8_t row, int8_t inc);

  private:
    uint16_t pulse_width = STEER_CENTER;
    struct TreeNode tree = {.Parent = &gui_home, .Children = {&err_curve}},
                    err_tree = {.Parent = &gui_steering};
    SGUI_ITEMS_ITEM its[ConfigNum];

    //! @brief 闂佸憡鍩婇幏锟�
    virtual void KeyLeftPush();

    //! @brief 闂佸憡妫戦幏锟�
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
        : ListLayout(&tree, its, "-��λ��ͨ��ʹ��",
                     {"����AI����", "�ٶȰ���", "���ʾ������"}) {}

  private:
    static const uint8_t item_cnt = 3;
    struct TreeNode tree = {.Parent = &gui_home};
    SGUI_ITEMS_ITEM its[item_cnt];

    virtual void KeyEnterPush();
};

struct MotorConfig : ListLayout {
    static SoftTimer upd_tmr;

    MotorConfig()
        : ListLayout(&tree, its, "-���ֵ������",
                     {"Ŀ���ٶȣ�", "�����ٶȣ�", "�����ٶȣ�", "���־��룺",
                      "���־��룺", "��վ���", "�̶�ռ�ձȣ�", "������٣�", "������٣�"}) {}
    static void UpdateValue(sched_event_data_t);

  private:
    static const uint8_t item_cnt = 9;
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
    for (uint8_t i(0); i < gui_steering.ConfigNum; ++i)
        gui_steering.UpdateValue(i, 0);
}

struct RingLoad : ListLayout {
    RingLoad()
        : ListLayout(&tree, its, "-����",
                     {"״̬ = ","�뻷��ֵ��", "ƽ����հ��", "ƽ����հ��", "ֱ��ǰհ��"
                       }) {}
    void UpdateValue(uint8_t row, uint8_t val);
  private:
    static const uint8_t item_cnt = 5;
    struct TreeNode tree = {.Parent = &gui_home};
    SGUI_ITEMS_ITEM its[item_cnt];
    virtual void KeyLeftPush();
    virtual void KeyRightPush();
};

#endif
