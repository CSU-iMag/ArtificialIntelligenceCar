#include "gui.hpp"
#include "SEEKFREE_MPU6050.h"
#include "car.hpp"
#include "fsl_gpt.h"
#include "storage.hpp"
#include "timer.hpp"
#include "usage.hpp"
#include "util.h"

SECTION_SDRAM BackGround gui_background;
SECTION_SDRAM HomePage gui_home;
SECTION_SDRAM DebugInfo gui_debug;
SECTION_SDRAM AttitudeDat gui_attitude;
SECTION_SDRAM Resistance gui_resistance;
SECTION_SDRAM MagadcDat gui_magadcDat;
SECTION_SDRAM HC06AT gui_hc06;
SECTION_SDRAM SteeringConfig gui_steering;
SECTION_SDRAM ControlPanel gui_control;

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

    // float temp = TEMPMON_GetCurrentTemperature(TEMPMON);
    pos.iX = 30;
    pos.iY = 10;
    // SGUI_Text_DrawText(&CarOLED, std::to_string(temp).c_str(),
    //                    &SGUI_DEFAULT_FONT_16, &pos, &screen_innerPos,
    //                    SGUI_DRAW_NORMAL);

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
                 {"������Ϣ", "�������", "�������", "���ֵ�λ��", "ADC raw",
                  " ��̬", "HC06"}) {
    tree.Children = {
        (LayoutBase *)&gui_debug,     (LayoutBase *)&gui_control,
        (LayoutBase *)&gui_steering,  (LayoutBase *)&gui_resistance,
        (LayoutBase *)&gui_magadcDat, (LayoutBase *)&gui_attitude,
        (LayoutBase *)&gui_hc06}; //!< ע��˳��Ҫ�������ַ������Ӧ
}

//! @brief ���Ӳ˵�
void HomePage::KeyEnterPush() {
    if (tree.Children.size() <= ListObject.stItems.iSelection)
        return;
    ActiveLayout = tree.Children[ListObject.stItems.iSelection];
}

///////////////////////////////////////////////////////////////////////////////

void AttitudeDat::UpdateValue() {
    get_accdata();
    Items[0].UpdateValue(std::to_string(mpu_acc_x));
}

///////////////////////////////////////////////////////////////////////////////

void Resistance::UpdateValue(uint8_t i) {
    CAR_ERROR_CHECK(i < Items.size());
    Items[i].UpdateValue(std::to_string(
        uint8_t(dynamic_cast<MagSensorPGA *>(Car.MagList[i])->Gain)));
}

void Resistance::KeyLeftPush() {
    dynamic_cast<MagSensorPGA *>(Car.MagList[ListObject.stItems.iSelection])
        ->Gain += -1;
    UpdateValue(ListObject.stItems.iSelection);
}

void Resistance::KeyRightPush() {
    dynamic_cast<MagSensorPGA *>(Car.MagList[ListObject.stItems.iSelection])
        ->Gain += 1;
    UpdateValue(ListObject.stItems.iSelection);
}

void Resistance::KeyEnterPush() {
    dynamic_cast<MagSensorPGA *>(Car.MagList[ListObject.stItems.iSelection])
        ->Calibrate();
    UpdateValue(ListObject.stItems.iSelection);
}

///////////////////////////////////////////////////////////////////////////////

void MagadcDat::UpdateValue() {
    if (this != ActiveLayout)
        return;
    for (int i(0); i < ADC_CNT; ++i)
        if (FigureMode) {
            Items[i + 1].UpdateValue(std::to_string(Car.MagList[i]->GetRaw()));
        } else
            Items[i + 1].UpdateValue(
                std::string(Car.MagList[i]->GetNormalized() / 6, '#'));
    Repaint();
}

void MagadcDat::KeyLeftPush() {
    if (ListObject.stItems.iSelection < PGA_CNT)
        dynamic_cast<MagSensorPGA *>(Car.MagList[ListObject.stItems.iSelection])
            ->Gain += -1;
}

void MagadcDat::KeyRightPush() {
    if (ListObject.stItems.iSelection < PGA_CNT)
        dynamic_cast<MagSensorPGA *>(Car.MagList[ListObject.stItems.iSelection])
            ->Gain += 1;
}

void MagadcDat::KeyEnterPush() {
    if (ListObject.stItems.iSelection)
        FigureMode = !FigureMode;
    else {
        SaveMaxEnabled = !SaveMaxEnabled;
        if (SaveMaxEnabled) {
            for (int i(0); i < ADC_CNT; ++i)
                Car.MagList[i]->MaxRawData = 0;
        }
        Items[0].UpdateValue(SaveMaxEnabled ? " ing..." : " ");
        Car.beep0.BeepFreqDelay(beep_musicFreq[SaveMaxEnabled ? 0 : 6], 555);
    }
}

///////////////////////////////////////////////////////////////////////////////

void HC06AT::UpdateValue() {
    Items[0].UpdateValue(Car.Hc06.rx_buffer);
    Items[2].UpdateValue(std::to_string(Car.Hc06.baud->baudrate));
    Items[3].UpdateValue(Car.Hc06.password);
    Items[4].UpdateValue(Car.Hc06.isMaster ? "true" : "false");
    Items[5].UpdateValue(std::to_string(Car.Hc06.parity_bit));
}

void HC06AT::KeyLeftPush() {
    switch (ListObject.stItems.iSelection) {
    case 2:
        Car.Hc06.ModifyBaud(-1);
        break;
    case 4:
        Car.Hc06.SetRole(-1);
        break;
    case 5:
        Car.Hc06.ModifyValid(-1);
        break;
    default:
        CAR_ERROR_CHECK(ListObject.stItems.iSelection < Hc06Num);
        break;
    }
    UpdateValue();
}

void HC06AT::KeyRightPush() {
    switch (ListObject.stItems.iSelection) {
    case 2:
        Car.Hc06.ModifyBaud(1);
        break;
    case 4:
        Car.Hc06.SetRole(1);
        break;
    case 5:
        Car.Hc06.ModifyValid(1);
        break;
    default:
        CAR_ERROR_CHECK(ListObject.stItems.iSelection < Hc06Num);
        break;
    }
    UpdateValue();
}

void HC06AT::KeyEnterPush() {
    switch (ListObject.stItems.iSelection) {
    case 0:
        Car.Hc06.TestComunication();
        break;
    case 1:
        Car.Hc06.TestComunication();
        break;
    case 2:
        Car.Hc06.ModifyBaud(0);
        break;
    case 3:
        Car.Hc06.SendPassword();
        break;
    case 4:
        Car.Hc06.SetRole(0);
        break;
    case 5:
        Car.Hc06.ModifyValid(0);
        break;
    case 6:
        Car.Hc06.GetVersion();
        break;
    default:
        CAR_ERROR_CHECK(ListObject.stItems.iSelection < Hc06Num);
        break;
    }
    UpdateValue();
}

///////////////////////////////////////////////////////////////////////////////

void SteeringConfig::UpdateValue(uint8_t row, int8_t inc) {
    switch (row) {
    case 0:
        Car.Steer3010.steerCtrl.SetK('p', inc);
        Items[0].UpdateValue(std::to_string(STEER_K(p)));
        break;
    case 1:
        Car.Steer3010.steerCtrl.SetK('i', inc);
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
        if (SteeringEnabled)
            pit_interrupt_ms(STEER_CHANNEL, STEER_PERIOD);
        else
            pit_close(STEER_CHANNEL);
        break;
    case 3:
        ActiveLayout = &err_curve;
        break;

    case 4:
        UpdateValue(4, (STEER_CENTER - pulse_width) / 5); //!< ����
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
        Car.beep0.BeepFreq(beep_musicFreq[3]);
        Car.LaunchTimer.Start(500);
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
        CAR_ERROR_CHECK(ListObject.stItems.iSelection < item_cnt);
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
