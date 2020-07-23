/*
    数字电位器：MCP4452_103e
    I2C
    接线定义：
    ------------------------------------
        SCL       B28 查看SEEKFREE_IIC文件内的SEEKFREE_SCL宏定义
        SDA       B29 查看SEEKFREE_IIC文件内的SEEKFREE_SDA宏定义
        A0 0 A1 0/1
    ------------------------------------
*/

#ifndef _MCP4452_H
#define _MCP4452_H

#include "common.h"

// Control Byte  ‘01011’+A1+A0+W/R
// 两片的地址：
// A1.A0=00	A1.A0=10
// Write：0	Read：1
#define MCP4452_ADDR_0 0x2C // 0101 100
#define MCP4452_ADDR_1 0x2E // 0101 110
#define MCP4452_ADDR_2 0x2D // 0101 101

#define Res1234 (MCP4452_ADDR_0)
#define Res5678 (MCP4452_ADDR_1)

/*
registor			P
Wiper0+TCON0	P0A,W,B
Wiper1+TCON0	P1A,W,B
Wiper2+TCON1	P2A,W,B
Wiper3+TCON1	P3A,W,B
*/
// 定义内部地址
// 设置阻值大小的寄存器地址
// the volatile wiper registers(AD3:AD0 = 00h, 01h, 06h, and 07h)
// Wiper:Full Scale
#define VWR_0_W_F 0x01 // 0b0000 0001
#define VWR_1_W_F 0x11 // 0b0001 0001
#define VWR_2_W_F 0x61 // 0b0110 0001
#define VWR_3_W_F 0x71 // 0b0111 0001

// Wiper:Other Scale
#define VWR_0_W_O 0x00 // 0b0000 0000
#define VWR_1_W_O 0x10 // 0b0001 0000
#define VWR_2_W_O 0x60 // 0b0110 0000
#define VWR_3_W_O 0x70 // 0b0111 0000
#define MCP4452_R1 VWR_0_W_O
#define MCP4452_R2 VWR_1_W_O
#define MCP4452_R3 VWR_2_W_O
#define MCP4452_R4 VWR_3_W_O

//#define VWR_0_R 0x0D // 0b0000 1101
//#define VWR_1_R 0x1D // 0b0001 1101
//#define VWR_2_R 0x6D // 0b0110 1101
//#define VWR_3_R 0x7D // 0b0111 1101

// 设置电阻模式的寄存器地址
// the TCON registers (AD3:AD0 = 04h and 0Ah)
#define TCON_0_W 0x41 // 0b0100 0001
#define TCON_1_W 0xA1 // 0b1010 0001
#define TCON_0_R 0x4C // 0b0100 1100
#define TCON_1_R 0xAC // 0b1010 1100

typedef enum { // 枚举
    R0_ON = 0x0F,
    R0_OFF = 0x00,
    R1_ON = 0xF0,
    R1_OFF = 0x00,
    R2_ON = 0x0F,
    R2_OFF = 0x00,
    R3_ON = 0xF0,
    R3_OFF = 0x00,
    R1R0_ALL_ON = 0xFF,
    R1R0_ALL_OFF = 0x00,
    R3R2_ALL_ON = 0xFF,
    R3R2_ALL_OFF = 0x00,
} Res_enum;

// 电阻模式
/*
#define R0_ON 0x0F
#define R0_OFF 0x00
#define R1_ON 0xF0
#define R1_OFF 0x00
#define R2_ON 0x0F
#define R2_OFF 0x00
#define R3_ON 0xF0
#define R3_OFF 0x00
#define R1R0_ALL_ON 0xFF
#define R1R0_ALL_OFF 0x00
#define R3R2_ALL_ON 0xFF
#define R3R2_ALL_OFF 0x00
*/

typedef enum { Scale0, Scale1, Scale2 } ResVal_enum;

// 带有4的是对一片MCP4452进行初始化
// 带有all的是同时对两片MCP4452进行初始化

#ifdef __cplusplus
extern "C" {
#endif

uint8 MCP4452_self_check(void);

// gpio初始化
void MCP4452_init(uint8 MCP4452_ADDR); //对一片初始化
void MCP4452_all_init();               //两片同时初始化

//设置电阻开/关
void MCP4452_set_mode(uint8 MCP4452_ADDR, uint8 sel, Res_enum mode);
void MCP4452_set_4on(uint8 MCP4452_ADDR);
void MCP4452_set_all_on();

//设置电阻阻值大小
void MCP4452_set_val(uint8 MCP4452_ADDR, uint8 sel, uint8 val);
void MCP4452_set_4FScale(uint8 MCP4452_ADDR);
void MCP4452_set_4ZScale(uint8 MCP4452_ADDR);
void MCP4452_set_all_FScale();

uint8 MCP4452_self_check(void);
//电阻阻值初始化
void MCP4452_val_4init(uint8 MCP4452_ADDR, uint8 val0, uint8 val1, uint8 val2,
                       uint8 val3);
void MCP4452_val_ALL_init(uint8 val00, uint8 val01, uint8 val02, uint8 val03,
                          uint8 val10, uint8 val11, uint8 val12, uint8 val13);

void mcp_init(void);

#ifdef __cplusplus
}
#endif

#endif
