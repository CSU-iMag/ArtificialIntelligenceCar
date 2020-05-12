#include "pack.hpp"
#include "car.hpp"
#include "peripherals.h"
#include <algorithm>

Direction_PackData Dir_PD;
Speed_PackData Speed_PD;
AI_PackData AI_PD;

/** @struct PID_PackData
 * @brief PID下行包的结构体
 * 定义存储PID下行包的所有内容
 */
struct PID_PackData {
    uint8_t head, size;
    word_t KP, KI, KD;
    uint8_t parity, tail;
} __PACKED Speed_PID_PD, Dir_PID_PD;

/** @struct Switch_PackData
 * @brief 开关下行包的结构体
 * 定义存储开关下行包的所有内容
 */
struct Switch_PackData {
    uint8_t head, size, switch_status, parity, tail;
} Sw_PD;

/** @struct Filter_PackData
 * @brief 滤波器下行包的结构体
 * 定义存储滤波器下行包的所有内容
 */
struct Filter_PackData {
    uint8_t head, size, Encoder_Filter_size, Motor_Filter_size,
        Mag_MiddleFilter_size, Mag_MovFilter_size, parity, tail;
} Fil_PD;

/**
 * @brief 各种包的有效数据的字节数
 */
static constexpr uint8_t Dir_PackData_size(5);
static constexpr uint8_t Speed_PackData_size(6);
static constexpr uint8_t AI_PackData_size(8);

static constexpr uint8_t PID_PackData_size(12);
static constexpr uint8_t Switch_PackData_size(1);
static constexpr uint8_t Filter_PackData_size(4);

/**
 * @brief 数组存放各种包的包头，包尾，有效数据的字节数
 */
const uint8_t Txpack_Head[] = {0xca, 0xcb, 0xcc};
const uint8_t Txpack_Tail[] = {0x8a, 0x8b, 0x8c};
const uint8_t Txpack_Size[] = {Dir_PackData_size, Speed_PackData_size,
                               AI_PackData_size};

const uint8_t Rxpack_Head[] = {0xea, 0xeb, 0xec, 0xed};
const uint8_t Rxpack_Tail[] = {0x5a, 0x5b, 0x5c, 0x5d};
const uint8_t Rxpack_Size[] = {PID_PackData_size, PID_PackData_size,
                               Switch_PackData_size, Filter_PackData_size};

/**
 * @brief 将buf中的所有字节异或，计算parity
 * @param[in]  buf        需要异或的字节数组的首地址
 * @param[in]  cnt        需要异或的字节数
 * @return  函数执行结果
 */
__STATIC_INLINE uint8_t XorParity(uint8_t *buf, uint8_t cnt) {
    uint8_t ret = 0;
    std::for_each(buf, buf + cnt, [&ret](uint8_t d) { ret ^= d; });
    return ret;
}

/**
 * @brief 将buf[0]~buf[cnt-1]进行crc32校验计算，得parity
 * @param[in]  buf        需要进行crc32校验的字节数组的首地址
 * @param[in]  cnt        需要进行crc32校验的字节数
 * @param[out]  parity    放置crc32校验结果的首地址
 */
void Crc32Parity(uint8_t *buf, uint8_t cnt, uint8_t *parity) {
    status_t status;
    size_t outLength = 4;

    /* Expected CRC-32 for the message.
     * CRC-32 params:
     * width=32 poly=0x04c11db7 init=0xffffffff refin=false refout=false
     * xorout=0x00000000 http://reveng.sourceforge.net/crc-catalogue/
     */
    memset(parity, 0, outLength);

    /************************ CRC-32 **************************/
    status = DCP_HASH(DCP, const_cast<dcp_handle_t *>(&DCP_handle_0),
                      kDCP_Crc32, buf, cnt, parity, &outLength);
    DEBUG_LOG("CRC-32 status=%d,output:%d,%d,%d,%d\n", status, parity[0],
              parity[1], parity[2], parity[3]);
    CAR_ERROR_CHECK(kStatus_Success == status);
}

/**
 * @brief 接收下行包时在下行包处理函数中调用的函数
 */
inline void Receive_Speed_PID_PackData(uint8_t *buf) {
    DEBUG_LOG("speed package\n");
    Speed_PID_PD.KP.Bdata[0] = buf[0];
    Speed_PID_PD.KP.Bdata[1] = buf[1];
    Speed_PID_PD.KP.Bdata[2] = buf[2];
    Speed_PID_PD.KP.Bdata[3] = buf[3];
    Speed_PID_PD.KI.Bdata[0] = buf[4];
    Speed_PID_PD.KI.Bdata[1] = buf[5];
    Speed_PID_PD.KI.Bdata[2] = buf[6];
    Speed_PID_PD.KI.Bdata[3] = buf[7];
    Speed_PID_PD.KD.Bdata[0] = buf[8];
    Speed_PID_PD.KD.Bdata[1] = buf[9];
    Speed_PID_PD.KD.Bdata[2] = buf[10];
    Speed_PID_PD.KD.Bdata[3] = buf[11];
    Car.MotorL.speedCtrl.SetPID(Speed_PID_PD.KP.Fdata, Speed_PID_PD.KI.Fdata,
                                Speed_PID_PD.KD.Fdata);
    Car.MotorR.speedCtrl.SetPID(Speed_PID_PD.KP.Fdata, Speed_PID_PD.KI.Fdata,
                                Speed_PID_PD.KD.Fdata);
}

inline void Receive_Dir_PID_PackData(uint8_t *buf) {
    DEBUG_LOG("steering package\n");
    Dir_PID_PD.KP.Bdata[0] = buf[0];
    Dir_PID_PD.KP.Bdata[1] = buf[1];
    Dir_PID_PD.KP.Bdata[2] = buf[2];
    Dir_PID_PD.KP.Bdata[3] = buf[3];
    Dir_PID_PD.KI.Bdata[0] = buf[4];
    Dir_PID_PD.KI.Bdata[1] = buf[5];
    Dir_PID_PD.KI.Bdata[2] = buf[6];
    Dir_PID_PD.KI.Bdata[3] = buf[7];
    Dir_PID_PD.KD.Bdata[0] = buf[8];
    Dir_PID_PD.KD.Bdata[1] = buf[9];
    Dir_PID_PD.KD.Bdata[2] = buf[10];
    Dir_PID_PD.KD.Bdata[3] = buf[11];
    Car.Steer3010.steerCtrl.SetPID(Dir_PID_PD.KP.Fdata, Dir_PID_PD.KI.Fdata,
                                   Dir_PID_PD.KD.Fdata);
}

inline void Receive_Switch_PackData(uint8_t *buf) {
    Sw_PD.switch_status = buf[0];
}

inline void Receive_Filter_PackData(uint8_t *buf) {
    Fil_PD.Encoder_Filter_size = buf[0];
    Fil_PD.Motor_Filter_size = buf[1];
    Fil_PD.Mag_MiddleFilter_size = buf[2];
    Fil_PD.Mag_MovFilter_size = buf[3];
}

/**
 * @brief 接收下行包时在下行包处理函数中调用的函数的函数指针数组
 */
void (*RxPack_FuncPtr[4])(uint8_t *buf) = {
    Receive_Speed_PID_PackData, Receive_Dir_PID_PackData,
    Receive_Switch_PackData, Receive_Filter_PackData};

/**
 * @brief 上行包下行包的包头、包尾、有效长度的初始化以及softtimer的开始
 */
void pack_Init() {
    Dir_PD.head = Txpack_Head[0];
    Speed_PD.head = Txpack_Head[1];
    AI_PD.head = Txpack_Head[2];

    Dir_PD.tail = Txpack_Tail[0];
    Speed_PD.tail = Txpack_Tail[1];
    AI_PD.tail = Txpack_Tail[2];

    Dir_PD.size = Txpack_Size[0];
    Speed_PD.size = Txpack_Size[1];
    AI_PD.size = Txpack_Size[2];

    Speed_PID_PD.head = Rxpack_Head[0];
    Dir_PID_PD.head = Rxpack_Head[1];
    Sw_PD.head = Rxpack_Head[2];
    Fil_PD.head = Rxpack_Head[3];

    Speed_PID_PD.tail = Rxpack_Tail[0];
    Dir_PID_PD.tail = Rxpack_Tail[1];
    Sw_PD.tail = Rxpack_Tail[2];
    Fil_PD.tail = Rxpack_Tail[3];

    Speed_PID_PD.size = Rxpack_Size[0];
    Dir_PID_PD.size = Rxpack_Size[1];
    Sw_PD.size = Rxpack_Size[2];
    Fil_PD.size = Rxpack_Size[3];
}

static const uint16_t RxPackMaxSize(std::max({Rxpack_Size[0], Rxpack_Size[1],
                                              Rxpack_Size[2], Rxpack_Size[3]}));
//我觉得先把rxbuffer放到一个新数组rxbuf，然后再将这个新数组送到这个函数里处理，
//从新数组里接收多个包，依次赋给buf，每种包对应一个处理函数，
//然后判断若有剩下的未完全接收的包，先保留函数里一些状态，将最后的不完全的buf放到
// rxbuf首端，然后新的rxbuffer应该赋进来排在后面。好麻烦，但是应该不大可能，毕竟空闲中断

/**
 * @brief 下行包处理函数
 * @param rxbuf 存放下行包的地址
 * @param count 空闲中断接收到的总字节数
 * @todo { 用函数LPUART_TransferGetReceiveCount得到count？，
 *         验证 }
 */
void RxPack_Handle(uint8_t *rxbuf, uint32_t count) {
    for (uint16_t i = 0; i < count; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            auto len = Rxpack_Size[j];
            if ((rxbuf[i] != Rxpack_Head[j]) || //不符合包头
                (rxbuf[i + 1] != len) ||        //不符合长度
                (i + len + 3 < count) ||        //所剩下的长度不够
                (rxbuf[i + len + 3] != Rxpack_Tail[j]) || //不符合包尾
                (rxbuf[i + len + 2] !=
                 AccParity(rxbuf + i + 2, len))) //不符合校验位
                continue;
            RxPack_FuncPtr[j](rxbuf + i + 2);
            //感觉如果这里用std::function,有点杀鸡用宰牛刀,
            //不涉及继承类成员函数,也没有构造函数的烦恼
            //许多功能都没有用武之地,这里还是效率优先吧。
            i += len + 4;
        }
    }
}

void Unpack() {
    uint8_t p;
    for (p = 0; p < 4; ++p)
        if (communicate_rxBuffer[0] == Rxpack_Head[p] &&
            communicate_rxBuffer[1] == Rxpack_Size[p] &&
            communicate_rxBuffer[Rxpack_Size[p] + 3] == Rxpack_Tail[p] &&
            communicate_rxBuffer[Rxpack_Size[p] + 2] ==
                AccParity(communicate_rxBuffer + 2, Rxpack_Size[p]))
            break;
    CAR_ERROR_CHECK(p < 4);
    // if (p < 4)
    RxPack_FuncPtr[p](communicate_rxBuffer + 2);
}
