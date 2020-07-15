/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		OLED
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.28
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 * @note
                                        接线定义：
                                        ------------------------------------
                                            OLED液晶    单片机
                                        D0 查看SEEKFREE_OLED文件内的OLED_SCL_PIN
 宏定义 D1          查看SEEKFREE_OLED文件内的OLED_SDA_PIN   宏定义 RES
 查看SEEKFREE_OLED文件内的OLED_RST_PIN   宏定义 DC
 查看SEEKFREE_OLED文件内的OLED_DC_PIN    宏定义 CS
 查看SEEKFREE_OLED文件内的OLED_CS_PIN    宏定义
                                        ------------------------------------
 ********************************************************************************************************************/

#include "SEEKFREE_OLED.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "zf_systick.h"

//定义显示方向
// 0 横屏模式
// 1 横屏模式  旋转180
#define OLED_DISPLAY_DIR 0

#if (0 == OLED_DISPLAY_DIR || 1 == OLED_DISPLAY_DIR)
#define X_WIDTH 128
#define Y_WIDTH 64

#else
#error "OLED_DISPLAY_DIR 定义错误"
#endif

#define Brightness 0xff //设置OLED亮度  越大越亮    范围0-0XFF
#define XLevelL 0x00
#define XLevelH 0x10
#define XLevel ((XLevelH & 0x0F) * 16 + XLevelL)
#define Max_Column 128
#define Max_Row 64

#define OLED_SCL(x)                                                            \
    GPIO_PinWrite(BOARD_INITPINS_CLK_PORT, BOARD_INITPINS_CLK_PIN, x)
#define OLED_SDA(x)                                                            \
    GPIO_PinWrite(BOARD_INITPINS_MOSI_PORT, BOARD_INITPINS_MOSI_PIN, x)
#define OLED_RST(x)                                                            \
    GPIO_PinWrite(BOARD_INITPINS_RES_PORT, BOARD_INITPINS_RES_PIN, x)
#define OLED_DC(x)                                                             \
    GPIO_PinWrite(BOARD_INITPINS_DC_PORT, BOARD_INITPINS_DC_PIN, x)
#define OLED_CS(x)

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部调用，用户无需关心
//  @param      void
//  @return
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void oled_wrdat(uint8 data) {
    uint8 i = 8;

    OLED_CS(0); //使能信号端，拉低时正常使用
    OLED_DC(1);
    OLED_SCL(0);
    while (i--) {
        if (data & 0x80) {
            OLED_SDA(1);
        } else {
            OLED_SDA(0);
        }
        OLED_SCL(1);

        OLED_SCL(0);
        data <<= 1;
    }
    OLED_CS(1); //使能信号端，拉低时正常使用
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部调用，用户无需关心
//  @param      void
//  @return
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void oled_wrcmd(uint8 cmd) {
    uint8 i = 8;
    OLED_CS(0); //使能信号端，拉低时正常使用
    OLED_DC(0);
    OLED_SCL(0);

    while (i--) {
        if (cmd & 0x80) {
            OLED_SDA(1);
        } else {
            OLED_SDA(0);
        }
        OLED_SCL(1);
        OLED_SCL(0);
        cmd <<= 1;
    }
    OLED_CS(1); //使能信号端，拉低时正常使用
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示坐标设置
//  @param      x			x轴坐标设置0-127
//  @param      y           y轴坐标设置0-7
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void oled_set_pos(uint8 x, uint8 y) {
    assert(X_WIDTH > x); //参数断言，进入断言标志参数超出范围
    assert((Y_WIDTH / 8) > y);

    oled_wrcmd(0xb0 + y);
    oled_wrcmd(((x & 0xf0) >> 4) | 0x10);
    oled_wrcmd((x & 0x0f) | 0x00);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED初始化函数
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void oled_init(void) {
    OLED_SCL(1);
    OLED_RST(0);
    systick_delay_ms(50);
    OLED_RST(1);

    oled_wrcmd(0xae); //--turn off oled panel
    oled_wrcmd(0x00); //---set low column address
    oled_wrcmd(0x10); //---set high column address
    oled_wrcmd(0x40); //--set start line address  Set Mapping RAM Display Start
                      // Line (0x00~0x3F)
    oled_wrcmd(0x81); //--set contrast control register
    oled_wrcmd(Brightness); // Set SEG Output Current Brightness

#if (0 == OLED_DISPLAY_DIR)
    oled_wrcmd(0xa1); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    oled_wrcmd(0xc8); // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
#else
    oled_wrcmd(0xa0); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    oled_wrcmd(0xc0); // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
#endif

    oled_wrcmd(0xa6); //--set normal display
    oled_wrcmd(0xa8); //--set multiplex ratio(1 to 64)
    oled_wrcmd(0x3f); //--1/64 duty
    oled_wrcmd(
        0xd3); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    oled_wrcmd(0x00); //-not offset
    oled_wrcmd(0xd5); //--set display clock divide ratio/oscillator frequency
    oled_wrcmd(0x80); //--set divide ratio, Set Clock as 100 Frames/Sec
    oled_wrcmd(0xd9); //--set pre-charge period
    oled_wrcmd(0xf1); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    oled_wrcmd(0xda); //--set com pins hardware configuration
    oled_wrcmd(0x12);
    oled_wrcmd(0xdb); //--set vcomh
    oled_wrcmd(0x40); // Set VCOM Deselect Level
    oled_wrcmd(0x20); //-Set Page Addressing Mode (0x00/0x01/0x02)
    oled_wrcmd(0x02); //
    oled_wrcmd(0x8d); //--set Charge Pump enable/disable
    oled_wrcmd(0x14); //--set(0x10) disable
    oled_wrcmd(0xa4); // Disable Entire Display On (0xa4/0xa5)
    oled_wrcmd(0xa6); // Disable Inverse Display On (0xa6/a7)
    oled_wrcmd(0xaf); //--turn on oled panel
    oled_fill(0x00);  //初始清屏
    oled_set_pos(0, 0);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED清屏函数
//  @param      bmp_data	填充颜色选着(0x00 or 0xff)
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void oled_fill(uint8 bmp_data) {
    uint8 y, x;

    for (y = 0; y < 8; y++) {
        oled_wrcmd(0xb0 + y);
        oled_wrcmd(0x01);
        oled_wrcmd(0x10);
        for (x = 0; x < X_WIDTH; x++)
            oled_wrdat(bmp_data);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED控制一个坐标下8个像素的点亮与熄灭
//  @param      x			x轴坐标设置0-127
//  @param      y           y轴坐标设置0-7
//  @return     void
//  @since      v1.0
//  Sample usage:			oled_putpixel(0,0,0xff);//将0,0坐标
//  8个点全部点亮 Sample usage:
//  oled_putpixel(0,0,0x01);//将0,0坐标 最低位点亮其余7个熄灭
//  @note                   同理可以任意控制一个坐标下8个像素的点亮与熄灭
//-------------------------------------------------------------------------------------------------------------------
void oled_putpixel(uint8 x, uint8 y, uint8 data1) {
    oled_set_pos(x, y);
    oled_wrcmd(0xb0 + y);
    oled_wrcmd(((x & 0xf0) >> 4) | 0x10);
    oled_wrcmd((x & 0x0f) | 0x00);
    oled_wrdat(data1);
}
