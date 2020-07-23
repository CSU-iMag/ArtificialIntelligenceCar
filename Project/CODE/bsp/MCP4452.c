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
    MCP4452_set_4on(MCP4452_ADDR_2);
    if ((0x01FF != simiic_read_reg2(Res5678, TCON_0_R, SIMIIC)) &&
        (0x01FF != simiic_read_reg2(Res5678, TCON_1_R, SIMIIC)))
        i += 4;
    return i; //自检无误
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

void mcp_init(void) {
    MCP4452_set_4on(MCP4452_ADDR_0);
    MCP4452_set_4on(MCP4452_ADDR_1);
    MCP4452_set_4on(MCP4452_ADDR_2);

    simiic_write_reg(MCP4452_ADDR_0, MCP4452_R1, 0x00); // 车身右
    simiic_write_reg(MCP4452_ADDR_0, MCP4452_R2, 0x00); // 车头右横
    simiic_write_reg(MCP4452_ADDR_0, MCP4452_R3, 0xff); // 车头中间
    simiic_write_reg(MCP4452_ADDR_0, MCP4452_R4, 0x00); // 车头最右
    simiic_write_reg(MCP4452_ADDR_1, MCP4452_R1, 0x00); // 车头左横
    simiic_write_reg(MCP4452_ADDR_1, MCP4452_R2, 0x00); // 车身左
    simiic_write_reg(MCP4452_ADDR_1, MCP4452_R3, 0x00); // 车头最左边
    simiic_write_reg(MCP4452_ADDR_1, MCP4452_R4, 0xff); // 前瞻中
    simiic_write_reg(MCP4452_ADDR_2, MCP4452_R1, 0xff); // 前瞻右
    simiic_write_reg(MCP4452_ADDR_2, MCP4452_R2, 0x60); // 车头右竖
    simiic_write_reg(MCP4452_ADDR_2, MCP4452_R3, 0x00); // 前瞻左
    simiic_write_reg(MCP4452_ADDR_2, MCP4452_R4, 0xff); // 车头左竖
}
