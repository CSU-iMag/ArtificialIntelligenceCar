/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		OLED
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 * @note
                                        ���߶��壺
                                        ------------------------------------
                                            OLEDҺ��    ��Ƭ��
                                        D0 �鿴SEEKFREE_OLED�ļ��ڵ�OLED_SCL_PIN
 �궨�� D1          �鿴SEEKFREE_OLED�ļ��ڵ�OLED_SDA_PIN   �궨�� RES
 �鿴SEEKFREE_OLED�ļ��ڵ�OLED_RST_PIN   �궨�� DC
 �鿴SEEKFREE_OLED�ļ��ڵ�OLED_DC_PIN    �궨�� CS
 �鿴SEEKFREE_OLED�ļ��ڵ�OLED_CS_PIN    �궨��
                                        ------------------------------------
 ********************************************************************************************************************/

#include "SEEKFREE_OLED.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "zf_systick.h"

//������ʾ����
// 0 ����ģʽ
// 1 ����ģʽ  ��ת180
#define OLED_DISPLAY_DIR 0

#if (0 == OLED_DISPLAY_DIR || 1 == OLED_DISPLAY_DIR)
#define X_WIDTH 128
#define Y_WIDTH 64

#else
#error "OLED_DISPLAY_DIR �������"
#endif

#define Brightness 0xff //����OLED����  Խ��Խ��    ��Χ0-0XFF
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
//  @brief      �ڲ����ã��û��������
//  @param      void
//  @return
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void oled_wrdat(uint8 data) {
    uint8 i = 8;

    OLED_CS(0); //ʹ���źŶˣ�����ʱ����ʹ��
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
    OLED_CS(1); //ʹ���źŶˣ�����ʱ����ʹ��
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ڲ����ã��û��������
//  @param      void
//  @return
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void oled_wrcmd(uint8 cmd) {
    uint8 i = 8;
    OLED_CS(0); //ʹ���źŶˣ�����ʱ����ʹ��
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
    OLED_CS(1); //ʹ���źŶˣ�����ʱ����ʹ��
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED��ʾ��������
//  @param      x			x����������0-127
//  @param      y           y����������0-7
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void oled_set_pos(uint8 x, uint8 y) {
    assert(X_WIDTH > x); //�������ԣ�������Ա�־����������Χ
    assert((Y_WIDTH / 8) > y);

    oled_wrcmd(0xb0 + y);
    oled_wrcmd(((x & 0xf0) >> 4) | 0x10);
    oled_wrcmd((x & 0x0f) | 0x00);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED��ʼ������
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
    oled_wrcmd(0xa1); //--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    oled_wrcmd(0xc8); // Set COM/Row Scan Direction   0xc0���·��� 0xc8����
#else
    oled_wrcmd(0xa0); //--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    oled_wrcmd(0xc0); // Set COM/Row Scan Direction   0xc0���·��� 0xc8����
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
    oled_fill(0x00);  //��ʼ����
    oled_set_pos(0, 0);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED��������
//  @param      bmp_data	�����ɫѡ��(0x00 or 0xff)
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
//  @brief      OLED����һ��������8�����صĵ�����Ϩ��
//  @param      x			x����������0-127
//  @param      y           y����������0-7
//  @return     void
//  @since      v1.0
//  Sample usage:			oled_putpixel(0,0,0xff);//��0,0����
//  8����ȫ������ Sample usage:
//  oled_putpixel(0,0,0x01);//��0,0���� ���λ��������7��Ϩ��
//  @note                   ͬ������������һ��������8�����صĵ�����Ϩ��
//-------------------------------------------------------------------------------------------------------------------
void oled_putpixel(uint8 x, uint8 y, uint8 data1) {
    oled_set_pos(x, y);
    oled_wrcmd(0xb0 + y);
    oled_wrcmd(((x & 0xf0) >> 4) | 0x10);
    oled_wrcmd((x & 0x0f) | 0x00);
    oled_wrdat(data1);
}
