#ifndef FILTER_H
#define FILTER_H

#include <algorithm>
#include <arm_math.h>
#include <cstdint>
#include <numeric>
#include <vector>

class filter_movAve_t {
  private:
    const uint8_t size;
    uint8_t header;
    std::vector<float32_t> buffer;

  public:
    explicit filter_movAve_t(uint8_t filter_size)
        : size(filter_size), buffer(size), header(0) {}

    /**
     * @brief 滑动平均滤波
     * @param[in] filter	滤波器实例
     * @param[in] value 更新的数据
     * @return	滤波后的值
     */
    __inline float32_t Moving(float32_t value) {
        if (buffer.empty())
            return 0.000001;
        header %= size;
        buffer[header++] = value;
        float32_t ret, sum;
        arm_mean_f32(&buffer[0], size, &ret);
        // sum = std::accumulate(buffer.begin(), buffer.end(), 0.0f);
        // CAR_ERROR_CHECK(ret == sum / size);
        return ret;
    }
};

/**
 * @brief Kalman Filter
 * @note Q: 过程噪声，Q增大，动态响应变快，收敛稳定性变坏
         R: 测量噪声，R增大，动态响应变慢，收敛稳定性变好
 */
struct filter_kalman {
    filter_kalman(float Q, float R);
    float Filte(float Measurement);

  private:
    float x_last, // k-1时刻的滤波值
        x_now,    //当前值
        x_middle, //过渡数值

        p_last, //
        p_middle, p_now,
        kalmanGain, //   Kalamn增益

        Q, //预测过程中噪声偏差的方差
        R; //测量噪声偏差
};

/**
 * @brief 中值滤波
 * @param[in] e   数组
 * @param[in] len 取中间多少数
 * @return  滤波后
 * @warning 元素个数必须是奇数！
 */
// template <typename T>
// static __inline T filter_median(std::vector<T> &e, uint8_t len) {
//     CAR_ERROR_CHECK(len % 2);
//     CAR_ERROR_CHECK(e.size() % 2);
//     CAR_ERROR_CHECK(e.size() >= len);

//     std::sort(e.begin(), e.end());
//     auto mid(e.begin() + e.size() / 2u);
//     double sum(std::accumulate(mid - len / 2u, mid + len / 2u + 1, 0.00000f));
//     return sum / len;
// };

/**
 * @brief fir滤波器
 * 它可以实现低通滤波（滑动滤波是其中一种）、高通滤波、带通滤波等
 */
class filter_fir_t {
  private:
    arm_fir_instance_f32 S;
    uint16_t numTaps; ///<滤波系数的个数 比如3
    float32_t *pCoeffs; ///<存放滤波系数数组的首地址 比如[1/3 1/3 1/3]
                        /**
                                pState指向数据缓冲区（一个数组）,这个缓冲区的长度刚好是
                                输入序列长度+numTaps-1，刚好符合FIR滤波器卷积的运算。
                                因为一般输入序列长度为1，所以pState长度恰好等于numTaps
                        */
    std::vector<float32_t> pState;
    uint32_t blockSize; ///<就是指一次处理多少个值 应该就是输入序列长度
    float32_t *p_Src_Dst; ///<输入序列以及输出序列
  public:
    filter_fir_t(uint16_t numTaps_t, float32_t *pCoeffs_t, uint32_t blockSize_t,
                 float32_t *p_Src_Dst_t)
        : numTaps(numTaps_t), pCoeffs(pCoeffs_t), pState(numTaps_t),
          blockSize(blockSize_t), p_Src_Dst(p_Src_Dst_t) {
        arm_fir_init_f32(&S, numTaps, pCoeffs, (float32_t *)&pState[0],
                         blockSize);
    }

    /**
     * @brief fir滤波
     */
    void filter_fir() { arm_fir_f32(&S, p_Src_Dst, p_Src_Dst, blockSize); }
};

/**
 * @struct fir_mov
 * @brief fir滤波器实现滑动平均滤波的参数
 */
struct fir_mov_t {
    uint8_t numTaps;
    float32_t pCoeffs[3];
} extern fir_mov;

/**
 * @struct fir_low1
 * @brief fir滤波器实现以下情况的低通滤波的参数
 *        Equiripple Fs=500Hz Fpass=200Hz Fstop=250Hz
 */
struct fir_low1_t {
    uint16_t numTaps;
    float32_t pCoeffs[26];
} extern fir_low1;

#endif // FILTER_H
