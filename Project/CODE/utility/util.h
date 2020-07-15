#ifndef UTIL_H
#define UTIL_H

#include "car_config.h"
#include "common.h"
#include "fsl_debug_console.h"
#include "fsl_pwm.h"

//! @brief 从QTIMER读到的脉冲数转换成车速cm/s
#define PULSE_TO_CM_S(rawSpeed)                                                \
    ((float)(rawSpeed) * 9 / 1024 / TICKS_TO_MS(PIT_ENCODER_CH_TICKS, PIT_CLK_FREQ) * 1000)

//! @brief 浮点数百分比转定时器Compare
#define PERCENT_TO_TICKS(percent) ((uint32)((percent)*PWM_DUTY_MAX / 100))

    //! @brief 
    #define TICKS_TO_MS(ticks, freq) ((float)ticks * 1000 / freq)
    
//! @brief 12位ADC转毫伏
#define ADC12b_TO_mV(bit12) ((float)bit12 * 3.30 / 4.09600)

//! @brief 舵机高电平时间转占空比
#define PULSE_TO_PERCENT(width_us) ((float)width_us * STEER_FREQ / 10000)

#define RESCALE_VALUE(value, new_scale, old_scale)                             \
    ((value) * (new_scale) / (old_scale))

#define CRITICAL_REGION_ENTER()                                                \
    do {                                                                       \
    uint32 primask = DisableGlobalIRQ()
#define CRITICAL_REGION_EXIT()                                                 \
    EnableGlobalIRQ(primask);                                                  \
    }                                                                          \
    while (0)

#define SECTION_SDRAM __attribute__((section(".bss.SDRAM_CACHE")))
#define SECTION_NONCACHE __attribute__((section("NonCacheable.init")))

/**
 * @brief Size of a field in declared structure
 *
 * Macro that returns the size of the structure field.
 * @param struct_type Variable type to get the field size from
 * @param field Field name to analyze. It can be even field inside field
 * (field.somethingelse.and_another).
 *
 * @return Size of the field
 */
#define FIELD_SIZE(struct_type, field)                                         \
    sizeof(((struct struct_type *)NULL)->field)

/**
 * @brief Number of elements in field array in declared structure
 *
 * Macro that returns number of elementy in structure field.
 * @param struct_type Variable type to get the field size from
 * @param field Field name to analyze.
 *
 * @return Number of elements in field array
 *
 * @sa FIELD_SIZE
 */
#define FIELD_ARRAY_SIZE(struct_type, field)                                   \
    (FIELD_SIZE(struct_type, field) / FIELD_SIZE(struct_type, field[0]))

#define BIT(b) (1 << (b))

/**
 * @brief Set a bit in the uint32 word.
 *
 * @param[in] W  Word whose bit is being set.
 * @param[in] B  Bit number in the word to be set.
 */
#define SET_BIT(W, B) ((W) |= (uint32_t)(1U << (B)))

/**
 * @brief Clears a bit in the uint32 word.
 *
 * @param[in] W   Word whose bit is to be cleared.
 * @param[in] B   Bit number in the word to be cleared.
 */
#define CLR_BIT(W, B) ((W) &= (~(uint32_t)(1U << (B))))

/**
 * @brief Checks if a bit is set.
 *
 * @param[in] W   Word whose bit is to be checked.
 * @param[in] B   Bit number in the word to be checked.
 *
 * @retval 1 if bit is set.
 * @retval 0 if bit is not set.
 */
#define IS_SET(W, B) (((W) >> (B)) & 1)

/**
 * @brief Macro for increasing a number to the nearest (larger) multiple of
 * another number.
 *
 * @param[in]  alignment  The number to align to.
 * @param[in]  number     The number to align (increase).
 *
 * @return The aligned (increased) @p number.
 */
#define ALIGN_NUM(alignment, number)                                           \
    (((number)-1) + (alignment) - (((number)-1) % (alignment)))

/**
 * @brief Macro for calculating the number of words that are needed to hold a
 * number of bytes.
 *
 * @details Adds 3 and divides by 4.
 *
 * @param[in]  n_bytes  The number of bytes.
 *
 * @return The number of words that @p n_bytes take up (rounded up).
 */
#define BYTES_TO_WORDS(n_bytes) (((n_bytes) + 3) >> 2)

/**
 * @brief Macro for performing integer division, making sure the result is
 * rounded up.
 *
 * @details One typical use for this is to compute the number of objects with
 * size B is needed to hold A number of bytes.
 *
 * @param[in]   A   Numerator.
 * @param[in]   B   Denominator.
 *
 * @return      Integer result of dividing A by B, rounded up.
 */
#define CEIL_DIV(A, B) (((A) + (B)-1) / (B))

/** @brief Macro for performing rounded integer division (as opposed to
 * truncating the result).
 *
 * @param[in]   A   Numerator.
 * @param[in]   B   Denominator.
 *
 * @return      Rounded (integer) result of dividing A by B.
 */
#define ROUNDED_DIV(A, B) (((A) + ((B) / 2)) / (B))

/**
 * @brief Function for changing the value unit.
 *
 * @param[in]   value               Value to be rescaled.
 * @param[in]   old_unit_reversal   Reversal of the incoming unit.
 * @param[in]   new_unit_reversal   Reversal of the desired unit.
 *
 * @return      Number of bytes written.
 */
static __INLINE uint64_t value_rescale(uint32_t value,
                                       uint32_t old_unit_reversal,
                                       uint16_t new_unit_reversal) {
    return (uint64_t)ROUNDED_DIV((uint64_t)value * new_unit_reversal,
                                 old_unit_reversal);
}

/** @brief Function for converting the input voltage (in milli volts) into
 * percentage of 3.0 Volts.
 *
 *  @details The calculation is based on a linearized version of the
 * battery's discharge curve. 3.0V returns 100% battery level. The limit for
 * power failure is 2.1V and is considered to be the lower boundary.
 *
 *           The discharge curve for CR2032 is non-linear. In this model it
 * is split into 4 linear sections:
 *           - Section 1: 3.0V - 2.9V = 100% - 42% (58% drop on 100 mV)
 *           - Section 2: 2.9V - 2.74V = 42% - 18% (24% drop on 160 mV)
 *           - Section 3: 2.74V - 2.44V = 18% - 6% (12% drop on 300 mV)
 *           - Section 4: 2.44V - 2.1V = 6% - 0% (6% drop on 340 mV)
 *
 *           These numbers are by no means accurate. Temperature and
 *           load in the actual application is not accounted for!
 *
 *  @param[in] mvolts The voltage in mV
 *
 *  @return    Battery level in percent.
 */
static __INLINE uint8_t battery_level_in_percent(const uint16_t mvolts) {
    uint8_t battery_level;

    if (mvolts >= 3000) {
        battery_level = 100;
    } else if (mvolts > 2900) {
        battery_level = 100 - ((3000 - mvolts) * 58) / 100;
    } else if (mvolts > 2740) {
        battery_level = 42 - ((2900 - mvolts) * 24) / 160;
    } else if (mvolts > 2440) {
        battery_level = 18 - ((2740 - mvolts) * 12) / 300;
    } else if (mvolts > 2100) {
        battery_level = 6 - ((2440 - mvolts) * 6) / 340;
    } else {
        battery_level = 0;
    }

    return battery_level;
}

/**
 * @brief Function for calculating CRC-16 in blocks.
 *
 * Feed each consecutive data block into this function, along with the current
 * value of p_crc as returned by the previous call of this function. The first
 * call of this function should pass NULL as the initial value of the crc in
 * p_crc.
 *
 * @param[in] p_data The input data block for computation.
 * @param[in] size   The size of the input data block in bytes.
 * @param[in] p_crc  The previous calculated CRC-16 value or NULL if first call.
 *
 * @return The updated CRC-16 value, based on the input supplied.
 */
static __inline uint16_t crc16_compute(uint8_t const *p_data, uint32_t size,
                                       uint16_t const *p_crc) {
    uint16_t crc = (p_crc == NULL) ? 0xFFFF : *p_crc;

    for (uint32_t i = 0; i < size; ++i) {
        crc = (uint8_t)(crc >> 8) | (crc << 8);
        crc ^= p_data[i];
        crc ^= (uint8_t)(crc & 0xFF) >> 4;
        crc ^= (crc << 8) << 4;
        crc ^= ((crc & 0xFF) << 4) << 1;
    }

    return crc;
}

/**
 * @brief 将buf中的所有字节累加，计算parity
 * @param[in]  buf        需要累加的字节数组的首地址
 * @param[in]  cnt        需要累加的字节数
 * @return  函数执行结果
 */
__STATIC_INLINE uint8_t AccParity(const uint8_t *buf, uint8_t cnt) {
    uint8_t sum = 0;
    while (cnt--)
        sum += buf[cnt];
    return sum;
}

/**
 * @brief Macro for checking if an integer is a power of two.
 *
 * @param[in]   A   Number to be tested.
 *
 * @return      true if value is power of two.
 * @return      false if value not power of two.
 */
#define IS_POWER_OF_TWO(A) (((A) != 0) && ((((A)-1) & (A)) == 0))

/**
 * @brief Function for decoding a uint32 value.
 *
 * @param[in]   p_encoded_data   Buffer where the encoded data is stored.
 *
 * @return      Decoded value.
 */
static __INLINE uint32_t uint32_decode(const uint8_t *p_encoded_data) {
    return ((((uint32_t)((uint8_t *)p_encoded_data)[0]) << 0) |
            (((uint32_t)((uint8_t *)p_encoded_data)[1]) << 8) |
            (((uint32_t)((uint8_t *)p_encoded_data)[2]) << 16) |
            (((uint32_t)((uint8_t *)p_encoded_data)[3]) << 24));
}

/**
 * @brief Function for decoding a uint32 value in big-endian format.
 *
 * @param[in]   p_encoded_data   Buffer where the encoded data is stored.
 *
 * @return      Decoded value.
 */
static __INLINE uint32_t uint32_big_decode(const uint8_t *p_encoded_data) {
    return ((((uint32_t)((uint8_t *)p_encoded_data)[0]) << 24) |
            (((uint32_t)((uint8_t *)p_encoded_data)[1]) << 16) |
            (((uint32_t)((uint8_t *)p_encoded_data)[2]) << 8) |
            (((uint32_t)((uint8_t *)p_encoded_data)[3]) << 0));
}

/**
 * @brief Function for encoding an uint16 value in big-endian format.
 *
 * @param[in]   value            Value to be encoded.
 * @param[out]  p_encoded_data   Buffer where the encoded data will be written.
 *
 * @return      Number of bytes written.
 */
static __INLINE uint8_t uint16_big_encode(uint16_t value,
                                          uint8_t *p_encoded_data) {
    p_encoded_data[0] = (uint8_t)(value >> 8);
    p_encoded_data[1] = (uint8_t)(value & 0xFF);

    return sizeof(uint16_t);
}

/*lint -esym(526, __rev) */
/*lint -esym(628, __rev) */
/**
 * @brief Function for encoding a uint32 value in big-endian format.
 *
 * @param[in]   value            Value to be encoded.
 * @param[out]  p_encoded_data   Buffer where the encoded data will be written.
 *                               The address pointed to must be word alligned.
 *
 * @return      Number of bytes written.
 */
static __INLINE uint8_t uint32_big_encode(uint32_t value,
                                          uint8_t *p_encoded_data) {
    *(uint32_t *)p_encoded_data = __REV(value);
    return sizeof(uint32_t);
}

//! @warning 参数会被调用多次
#if !defined(MIN) && !defined(MAX)
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif
#define lim(_val, _min, _max) MAX(_min, MIN(_max, _val))

#define LIMITING(_val, _min, _max) (_val = lim(_val, _min, _max))

//! @brief 一个字是32位
typedef union {
    float Fdata;
    unsigned long Idata;
    unsigned char Bdata[4];
} word_t;

#endif
