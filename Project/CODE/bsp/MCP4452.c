/** @file MCP4452
 *  @author burst-bao
 *  @brief Microchip digital resistance driver.
 */
#include "MCP4452.h"
#include "SEEKFREE_IIC.h" //模拟I2C
#include "util.h"
#include "zf_systick.h"

// 返回0，电阻无误；返回1，电阻1错误；返回2，电阻2错误；返回3，都错误
uint8 MCP4452_self_check(void) {
    uint8 i = 0;
    MCP4452_set_4on(Res1234);
    if ((0x01FF != simiic_read_reg2(Res1234, TCON_0_R, SIMIIC)) &&
        (0x01FF != simiic_read_reg2(Res1234, TCON_1_R, SIMIIC)))
        i++;
    MCP4452_set_4on(Res5678);
    if ((0x01FF != simiic_read_reg2(Res5678, TCON_0_R, SIMIIC)) &&
        (0x01FF != simiic_read_reg2(Res5678, TCON_1_R, SIMIIC)))
        i += 2;
    return i; //自检无误
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化MCP4452
//  Sample usage:		MCP4452_init(ADDR0_W)  第一片写入
//-------------------------------------------------------------------------------------------------------------------
void MCP4452_init(uint8 MCP4452_ADDR) {
    // simiic_init(); // init in Car.Startup
    systick_delay_ms(100); //上电延时
    MCP4452_set_all_on(MCP4452_ADDR);
    MCP4452_set_all_FScale(MCP4452_ADDR); //初始化均为最大阻值
}

void MCP4452_all_init() {
    // simiic_init();
    systick_delay_ms(100);    //上电延时
    MCP4452_set_all_on();     //初始化全部打开
    MCP4452_set_all_FScale(); //初始化均为最大阻值
}

// 设置电阻开/关
// MCP4452_ADDR:四路电位器地址.
// sel： TCON_x_W				选择TCON registers寄存器.
// mode：				设置该路电阻模式
// R1R0_ALL_ON/OFF;	R3R2_ALL_ON/OFF
// MCP4452_set_mode(Res1234, TCON_0_W, R0_OFF);
void MCP4452_set_mode(uint8 MCP4452_ADDR, uint8 sel, Res_enum mode) {
    simiic_write_reg(MCP4452_ADDR, sel, mode);
}

void MCP4452_set_4on(uint8 MCP4452_ADDR) {
    simiic_write_reg(MCP4452_ADDR, TCON_0_W, R1R0_ALL_ON); //电阻0.1打开
    simiic_write_reg(MCP4452_ADDR, TCON_1_W, R3R2_ALL_ON); //电阻3.2打开
}

void MCP4452_set_all_on() {
    MCP4452_set_4on(Res1234);
    MCP4452_set_4on(Res5678);
}

// 设置电阻阻值大小
// MCP4452_ADDR:四路电位器地址.
// ADDRx_W/R
// sel：				电阻路0.1.2.3对应的wiper阻值大小寄存器.
// VWR_x_W_F            VWR_x_W_O—：R1/R2/R3/R4
// val：				设置该路电阻阻值103e-10k. 256挡
// 八位电位器 0x00~0xFF
// eg：MCP4452_set_val(Res1234, R1, 1);      MCP4452_set_val(Res1234, R1, 0xff);
void MCP4452_set_val(uint8 MCP4452_ADDR, uint8 sel, uint8 val) {

    systick_delay_ms(100);                    //上电延时
    simiic_write_reg(MCP4452_ADDR, sel, val); //设置阻值
}

void MCP4452_set_4FScale(uint8 MCP4452_ADDR) {
    systick_delay_ms(100);                           //上电延时
    simiic_write_reg(MCP4452_ADDR, VWR_0_W_F, 0x00); //设置阻值
    simiic_write_reg(MCP4452_ADDR, VWR_1_W_F, 0x00);
    simiic_write_reg(MCP4452_ADDR, VWR_2_W_F, 0x00);
    simiic_write_reg(MCP4452_ADDR, VWR_3_W_F, 0x00);
}

// 八个电位器都为满阻值
void MCP4452_set_all_FScale() {
    MCP4452_set_4FScale(Res1234);
    MCP4452_set_4FScale(Res5678);
}

void MCP4452_set_4ZScale(uint8 MCP4452_ADDR) {
    systick_delay_ms(100);                           //上电延时
    simiic_write_reg(MCP4452_ADDR, VWR_0_W_O, 0x00); //设置阻值
    simiic_write_reg(MCP4452_ADDR, VWR_1_W_O, 0x00);
    simiic_write_reg(MCP4452_ADDR, VWR_2_W_O, 0x00);
    simiic_write_reg(MCP4452_ADDR, VWR_3_W_O, 0x00);
}

// 四路电阻阻值设置
// MCP4452_ADDR:四路电位器地址.			ADDRx_W
// val0.1.2.3：四路电阻阻值大小
// eg：MCP4452_val_4init(Res1234, 100, 100, 100,100);
void MCP4452_val_4init(uint8 MCP4452_ADDR, uint8 val0, uint8 val1, uint8 val2,
                       uint8 val3) {
    systick_delay_ms(100);                           //上电延时
    simiic_write_reg(MCP4452_ADDR, VWR_0_W_O, val0); //设置阻值
    simiic_write_reg(MCP4452_ADDR, VWR_1_W_O, val1);
    simiic_write_reg(MCP4452_ADDR, VWR_2_W_O, val2);
    simiic_write_reg(MCP4452_ADDR, VWR_3_W_O, val3);
}

// 八路电阻阻值设置
void MCP4452_val_ALL_init(uint8 val00, uint8 val01, uint8 val02, uint8 val03,
                          uint8 val10, uint8 val11, uint8 val12, uint8 val13) {
    systick_delay_ms(100); //上电延时
    MCP4452_val_4init(MCP4452_ADDR_0, val00, val01, val02, val03);
    MCP4452_val_4init(MCP4452_ADDR_1, val10, val11, val12, val13);
}
