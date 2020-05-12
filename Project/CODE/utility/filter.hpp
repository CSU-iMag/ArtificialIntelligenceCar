#ifndef FILTER_H
#define FILTER_H

#include "util.h"
#include "route.h"
#include <algorithm>
#include <arm_math.h>
#include <cstdint>
#include <numeric>
#include <vector>

class filter_movAve_t {
  private:
    const uint8_t size;
    uint8_t header;
    std::vector<float> buffer;

  public:
    filter_movAve_t(uint8_t filter_size)
        : size(filter_size), buffer(size), header(0) {}

    /**
     * @brief 滑动平均滤波
     * @param[in] filter	滤波器实例
     * @param[in] value 更新的数据
     * @return	滤波后的值
     */
    __inline float Moving(float value) {
        if (buffer.empty())
            return 0.000001;
        header %= size;
        buffer[header++] = value;
        float ret, sum;
        arm_mean_f32(&buffer[0], size, &ret);
        // sum = std::accumulate(buffer.begin(), buffer.end(), 0.0f);
        // CAR_ERROR_CHECK(ret == sum / size);
        return ret;
    }
};

/**
 * @brief 中值滤波
 * @param[in] e   数组
 * @param[in] len 取中间多少数
 * @return  滤波后
 * @warning 元素个数必须是奇数！
 */
template <typename T>
static __inline T filter_median(std::vector<T> &e, uint8_t len) {
    CAR_ERROR_CHECK(len % 2);
    CAR_ERROR_CHECK(e.size() % 2);
    CAR_ERROR_CHECK(e.size() >= len);

    std::sort(e.begin(), e.end());
    auto mid(e.begin() + e.size() / 2u);
    double sum(std::accumulate(mid - len/2u, mid + len/2u + 1, 0.00000f));
    return sum / len;
};

#endif // FILTER_H
