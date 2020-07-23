#include "gui.hpp"
#include "car.hpp"
#include "fsl_gpt.h"
#include "package.hpp"
#include "storage.hpp"
#include "timer.hpp"
#include "usage.hpp"
#include "util.h"
#include "zf_pit.h"

SECTION_SDRAM BackGround gui_background;
SECTION_SDRAM HomePage gui_home;
SECTION_SDRAM DebugInfo gui_debug;
SECTION_SDRAM MagadcDat gui_magadcDat;
SECTION_SDRAM SteeringConfig gui_steering;
SECTION_SDRAM ControlPanel gui_control;
SECTION_SDRAM ComEnable gui_communication;
SECTION_SDRAM MotorConfig gui_motor;
SECTION_SDRAM ModelSelect gui_model;
SECTION_SDRAM RingLoad gui_ring;

BackGround::BackGround() : BasicLayout(&tree) {
    tree.Children.push_back((LayoutBase *)&gui_home);
    battery.Parameter = SGUI_PROCBAR_PARAMETER{.PosX = 6,
                                               .PosY = 2,
                                               .Width = 16,
                                               .Height = 60,
                                               .Direction = SGUI_PROCBAR_UP,
                                               .MaxValue = 100};
}

void BackGround::Show() {
    auto pos = screen_full;
    pos.iX = 50;
    pos.iY = 10;
    CarOLED.fnClear();
    SGUI_Text_DrawText(&CarOLED, "iMag", &SGUI_DEFAULT_FONT_16, &pos,
                       &screen_innerPos, SGUI_DRAW_NORMAL);
    SGUI_ProcessBar_Repaint(&CarOLED, &battery);

    pos.iX = 30;
    pos.iY = 36;
    SGUI_Text_DrawText(&CarOLED, std::to_string(cpu_usage_get()).c_str(),
                       &SGUI_DEFAULT_FONT_16, &pos, &screen_innerPos,
                       SGUI_DRAW_NORMAL);
}

void BackGround::UpdateValue(uint8_t bat_val) {
    battery.Data.Value = bat_val;
    if (this == ActiveLayout)
        Repaint();
}

void BackGround::KeyUpPush() {}

void BackGround::KeyDownPush() { Car.Pause(); }

//////////////////////////////////////////////////////////////////////////////

HomePage::HomePage()
    : ListLayout(&tree, its, " ���˵�",
                 {
                     "������Ϣ",
                     "�������",
                     "�������",
                     "���ֵ��",
                     "�������",
                     "ͨ��ʹ��",
                     "ģ��ѡ��",
                     "ADC raw",
                 }) {
    tree.Children = {
        (LayoutBase *)&gui_debug,    (LayoutBase *)&gui_control,
        (LayoutBase *)&gui_ring,     (LayoutBase *)&gui_motor,
        (LayoutBase *)&gui_steering, (LayoutBase *)&gui_communication,
        (LayoutBase *)&gui_model,    (LayoutBase *)&gui_magadcDat};
}

//! @brief
//! 闂備胶鎳撻悘姘跺箰閸濄儲�?曢柟瀵�?�Л閸�?挸鈽夊▎蹇擃潙閻熸粍婢�?�粔鐟扮暦閿熺姵鏅搁柨鐕傛�??
void HomePage::KeyEnterPush() {
    if (tree.Children.size() <= ListObject.stItems.iSelection)
        return;
    ActiveLayout = tree.Children[ListObject.stItems.iSelection];
}

///////////////////////////////////////////////////////////////////////////////

void MagadcDat::UpdateValue() {
    if (this != ActiveLayout)
        return;
    for (int i(0); i < ADC_CNT; ++i)
        if (FigureMode) {
            Items[i + 1].UpdateValue(std::to_string(Car.MagList[i].GetRaw()));
        } else
            Items[i + 1].UpdateValue(
                std::string(Car.MagList[i].GetNormalized() / 12, '#'));
    Repaint();
}

void MagadcDat::KeyEnterPush() {
    if (ListObject.stItems.iSelection)
        FigureMode = !FigureMode;
    else {
        SaveMaxEnabled = !SaveMaxEnabled;
        if (SaveMaxEnabled) {
            for (int i(0); i < ADC_CNT; ++i)
                Car.MagList[i].ClearMax();
        }
        Items[0].UpdateValue(SaveMaxEnabled ? " ing..." : " ");
    }
}

///////////////////////////////////////////////////////////////////////////////

void SteeringConfig::UpdateValue(uint8_t row, int8_t inc) {
    switch (row) {
    case 0:
        Car.Steer3010.steerCtrl.SetK('p', inc);
        Items[0].UpdateValue(std::to_string(STEER_K(p)));
        break;
    case 1:
        Car.Steer3010.steerCtrl.SetK('i', 0.02f * inc);
        Items[1].UpdateValue(std::to_string(STEER_K(i)));
        break;
    case 2:
        Car.Steer3010.steerCtrl.SetK('d', inc);
        Items[2].UpdateValue(std::to_string(STEER_K(d)));
        break;
    case 3:
        Car.Steer3010.steerOffset += inc;
        Items[3].UpdateValue(std::to_string(Car.Steer3010.steerOffset));
        break;
    case 4:
        pulse_width += inc * 5;
        Car.Steer3010.WidthSet(pulse_width);
        Items[4].UpdateValue(std::to_string(pulse_width));
        break;
    case 5:
        if (Car.CtrlMode == ControlMode::AI)
            Car.CtrlMode = ControlMode::PID;
        else
            Car.CtrlMode = ControlMode::AI;
        Items[5].UpdateValue(Car.CtrlMode == ControlMode::AI ? "AI" : "PID");
        break;
    default:
        CAR_ERROR_CHECK(ListObject.stItems.iSelection < ConfigNum);
        break;
    }
}

void SteeringConfig::KeyLeftPush() {
    UpdateValue(ListObject.stItems.iSelection, +1);
}

void SteeringConfig::KeyRightPush() {
    UpdateValue(ListObject.stItems.iSelection, -1);
}

void SteeringConfig::KeyEnterPush() {
    static bool SteeringEnabled;
    switch (ListObject.stItems.iSelection) {
    case 0:
    case 1:
    case 2:
        SteeringEnabled = !SteeringEnabled;
        CRITICAL_REGION_ENTER();
        if (SteeringEnabled)
            SET_BIT(Car.Enabled, SW_STEER);
        else
            CLR_BIT(Car.Enabled, SW_STEER);
        CRITICAL_REGION_EXIT();
        break;
    case 3:
        ActiveLayout = &err_curve;
        break;

    case 4:
        UpdateValue(
            4,
            (STEER_CENTER - pulse_width) /
                5); //!< 闁荤喐绮庢晶妤冩暜婵犲洦鍤堥柨鐕傛�??
        break;

    default:
        CAR_ERROR_CHECK(ListObject.stItems.iSelection < ConfigNum);
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////

ControlPanel::ControlPanel()
    : ListLayout(&tree, its, "-�������",
                 {"��������", "$$ ��ͣ $$", "Save to flash", "Load from flash",
                  "����(%)= ", "CPUռ����:", "���϶�= "}),
      battery_curve(&bat_tree, " Battery") {
    tree.Children.push_back(&battery_curve);
}

void ControlPanel::KeyEnterPush() {
    switch (ListObject.stItems.iSelection) {
    case 0:
        Car.LaunchTimer.Start(1200);
        ActiveLayout = &gui_background;
        break;

    case 1:
        Car.Machine.Pause();
        break;
    case 2:
        storage_save(SLN_DEBUG_SECTOR, SLN_DEBUG_PAGE);
        break;

    case 3:
        storage_load(SLN_DEBUG_SECTOR, SLN_DEBUG_PAGE);
        break;

    case 4:
        ActiveLayout = &battery_curve;
        break;

    default:
        break;
    }
}

void ControlPanel::UpdateValue(uint8_t row, std::string val) {
    if (this != ActiveLayout)
        return;
    if (row == lim(row, 4, 6)) {
        Items[row].UpdateValue(val);
    }
    Repaint();
}

///////////////////////////////////////////////////////////////////////////////

void ComEnable::KeyEnterPush() {
    static bool enabled[item_cnt];
    const int &row(ListObject.stItems.iSelection);

    CAR_ERROR_CHECK(item_cnt > row);
    enabled[row] = !enabled[row];
    Items[row].UpdateValue(enabled[row] ? "������" : "�ѹر�");

    CRITICAL_REGION_ENTER();
    if (enabled[0]) {
        SET_BIT(Car.Enabled, SW_SEND_AI);
    } else {
        CLR_BIT(Car.Enabled, SW_SEND_AI);
    }

    if (enabled[1]) {
        SET_BIT(Car.Enabled, SW_SEND_MOTOR);
    } else {
        CLR_BIT(Car.Enabled, SW_SEND_MOTOR);
    }
    CRITICAL_REGION_EXIT();

    if (enabled[2]) {
    } else {
    }
}

///////////////////////////////////////////////////////////////////////////////

SoftTimer MotorConfig::upd_tmr(MotorConfig::UpdateValue);

void MotorConfig::UpdateValue(sched_event_data_t dat) {
    if (ActiveLayout != &gui_motor)
        return;
    gui_motor.Items[0].UpdateValue(std::to_string(Car.TargetSpeed));
    gui_motor.Items[1].UpdateValue(std::to_string(Car.EncoderL.GetSpeed()));
    gui_motor.Items[2].UpdateValue(std::to_string(Car.EncoderR.GetSpeed()));
    gui_motor.Items[3].UpdateValue(std::to_string(Car.EncoderL.GetDistance()));
    gui_motor.Items[4].UpdateValue(std::to_string(Car.EncoderR.GetDistance()));
    gui_motor.Items[7].UpdateValue(std::to_string(Car.Deceleration));
    gui_motor.Repaint();
}

void MotorConfig::KeyEnterPush() {
    if (ListObject.stItems.iSelection == 5) {
        Car.EncoderL.ClearDistance();
        Car.EncoderR.ClearDistance();
    }
}

void MotorConfig::UpdateDuty(int8_t inc) {
    duty += inc;
    LIMITING(duty, 0, MOTOR_MAX);
    Car.MotorL.SetDuty(duty);
    Car.MotorR.SetDuty(duty);
    Items[6].UpdateValue(std::to_string(duty));
}

void MotorConfig::KeyRightPush() {
    switch (ListObject.stItems.iSelection) {
    case 0:
        Car.TargetSpeed -= 10;
        break;
    case 6:
        UpdateDuty(-5);
    case 7:
        Car.Deceleration -= 0.05;
        break;
    }
}

void MotorConfig::KeyLeftPush() {
    switch (ListObject.stItems.iSelection) {
    case 0:
        Car.TargetSpeed += 10;
        break;
    case 6:
        UpdateDuty(5);
    case 7:
        Car.Deceleration += 0.05;
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////

ModelSelect::ModelSelect() : MenuLayout(&tree, item_cnt, its) {
    its[0].cszLabelText = "model 0";
    its[1].cszLabelText = "model 1";
    its[2].cszLabelText = "model 2";
}

void ModelSelect::KeyEnterPush() {
    CRITICAL_REGION_ENTER();
    switch (MenuObject.stItems.iSelection) {
    case 0:
        Car.Model = model1;
        break;

    case 1:
        Car.Model = model2;
        break;

    case 2:
        Car.Model = model3;
        break;

    default:
        break;
    }
    deep_init();
    CRITICAL_REGION_EXIT();
}

///////////////////////////////////////////////////////////////////////////////

void RingLoad::UpdateValue(uint8_t row, uint8_t val) {
    switch (row) {
    case 0:
        Items[0].UpdateValue(std::to_string(val));
        break;
    case 1:
        Items[1].UpdateValue(std::to_string(Car.RingLoader.island += val));
        break;
    case 2:
        Items[2].UpdateValue(std::to_string(Car.RingLoader.straightL += val));
        break;
    case 3:
        gui_ring.Items[3].UpdateValue(
            std::to_string(Car.RingLoader.straightR += val));
        break;
    case 4:
        gui_ring.Items[4].UpdateValue(
            std::to_string(Car.RingLoader.straightM += val));
        break;

    default:
        break;
    }
    gui_ring.Repaint();
}

void RingLoad::KeyLeftPush() { UpdateValue(ListObject.stItems.iSelection, 2); }

void RingLoad::KeyRightPush() {
    UpdateValue(ListObject.stItems.iSelection, -2);
}

///////////////////////////////////////////////////////////////////////////////
