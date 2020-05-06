/** @file MCP4452
 *  @author burst-bao
 *  @brief Microchip digital resistance driver.
 */
#include "MCP4452.h"
#include "SEEKFREE_IIC.h" //ģ��I2C
#include "util.h"
#include "zf_systick.h"

// ����0���������󣻷���1������1���󣻷���2������2���󣻷���3��������
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
    return i; //�Լ�����
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʼ��MCP4452
//  Sample usage:		MCP4452_init(ADDR0_W)  ��һƬд��
//-------------------------------------------------------------------------------------------------------------------
void MCP4452_init(uint8 MCP4452_ADDR) {
    // simiic_init(); // init in Car.Startup
    systick_delay_ms(100); //�ϵ���ʱ
    MCP4452_set_all_on(MCP4452_ADDR);
    MCP4452_set_all_FScale(MCP4452_ADDR); //��ʼ����Ϊ�����ֵ
}

void MCP4452_all_init() {
    // simiic_init();
    systick_delay_ms(100);    //�ϵ���ʱ
    MCP4452_set_all_on();     //��ʼ��ȫ����
    MCP4452_set_all_FScale(); //��ʼ����Ϊ�����ֵ
}

// ���õ��迪/��
// MCP4452_ADDR:��·��λ����ַ.
// sel�� TCON_x_W				ѡ��TCON registers�Ĵ���.
// mode��				���ø�·����ģʽ
// R1R0_ALL_ON/OFF;	R3R2_ALL_ON/OFF
// MCP4452_set_mode(Res1234, TCON_0_W, R0_OFF);
void MCP4452_set_mode(uint8 MCP4452_ADDR, uint8 sel, Res_enum mode) {
    simiic_write_reg(MCP4452_ADDR, sel, mode);
}

void MCP4452_set_4on(uint8 MCP4452_ADDR) {
    simiic_write_reg(MCP4452_ADDR, TCON_0_W, R1R0_ALL_ON); //����0.1��
    simiic_write_reg(MCP4452_ADDR, TCON_1_W, R3R2_ALL_ON); //����3.2��
}

void MCP4452_set_all_on() {
    MCP4452_set_4on(Res1234);
    MCP4452_set_4on(Res5678);
}

// ���õ�����ֵ��С
// MCP4452_ADDR:��·��λ����ַ.
// ADDRx_W/R
// sel��				����·0.1.2.3��Ӧ��wiper��ֵ��С�Ĵ���.
// VWR_x_W_F            VWR_x_W_O����R1/R2/R3/R4
// val��				���ø�·������ֵ103e-10k. 256��
// ��λ��λ�� 0x00~0xFF
// eg��MCP4452_set_val(Res1234, R1, 1);      MCP4452_set_val(Res1234, R1, 0xff);
void MCP4452_set_val(uint8 MCP4452_ADDR, uint8 sel, uint8 val) {

    systick_delay_ms(100);                    //�ϵ���ʱ
    simiic_write_reg(MCP4452_ADDR, sel, val); //������ֵ
}

void MCP4452_set_4FScale(uint8 MCP4452_ADDR) {
    systick_delay_ms(100);                           //�ϵ���ʱ
    simiic_write_reg(MCP4452_ADDR, VWR_0_W_F, 0x00); //������ֵ
    simiic_write_reg(MCP4452_ADDR, VWR_1_W_F, 0x00);
    simiic_write_reg(MCP4452_ADDR, VWR_2_W_F, 0x00);
    simiic_write_reg(MCP4452_ADDR, VWR_3_W_F, 0x00);
}

// �˸���λ����Ϊ����ֵ
void MCP4452_set_all_FScale() {
    MCP4452_set_4FScale(Res1234);
    MCP4452_set_4FScale(Res5678);
}

void MCP4452_set_4ZScale(uint8 MCP4452_ADDR) {
    systick_delay_ms(100);                           //�ϵ���ʱ
    simiic_write_reg(MCP4452_ADDR, VWR_0_W_O, 0x00); //������ֵ
    simiic_write_reg(MCP4452_ADDR, VWR_1_W_O, 0x00);
    simiic_write_reg(MCP4452_ADDR, VWR_2_W_O, 0x00);
    simiic_write_reg(MCP4452_ADDR, VWR_3_W_O, 0x00);
}

// ��·������ֵ����
// MCP4452_ADDR:��·��λ����ַ.			ADDRx_W
// val0.1.2.3����·������ֵ��С
// eg��MCP4452_val_4init(Res1234, 100, 100, 100,100);
void MCP4452_val_4init(uint8 MCP4452_ADDR, uint8 val0, uint8 val1, uint8 val2,
                       uint8 val3) {
    systick_delay_ms(100);                           //�ϵ���ʱ
    simiic_write_reg(MCP4452_ADDR, VWR_0_W_O, val0); //������ֵ
    simiic_write_reg(MCP4452_ADDR, VWR_1_W_O, val1);
    simiic_write_reg(MCP4452_ADDR, VWR_2_W_O, val2);
    simiic_write_reg(MCP4452_ADDR, VWR_3_W_O, val3);
}

// ��·������ֵ����
void MCP4452_val_ALL_init(uint8 val00, uint8 val01, uint8 val02, uint8 val03,
                          uint8 val10, uint8 val11, uint8 val12, uint8 val13) {
    systick_delay_ms(100); //�ϵ���ʱ
    MCP4452_val_4init(MCP4452_ADDR_0, val00, val01, val02, val03);
    MCP4452_val_4init(MCP4452_ADDR_1, val10, val11, val12, val13);
}
