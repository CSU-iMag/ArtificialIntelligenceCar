#ifndef PGA_H
#define PGA_H

#include <cstdint>

//! @brief A、B分别代表两块数字电位器。NONE是因为有些ADC通道不可调【用的逐飞模块
enum class pga_channels { A1, A2, A3, A4, B1, B2, B3, B4, NONE };

struct pga_t {
    pga_t(pga_channels ch = pga_channels::NONE) : channel(ch) {}

    //! @brief 用int赋值
    pga_t &operator=(const uint8_t &rhs);

    pga_t &operator+=(const int8_t &rhs);

    //! @brief 转换为int。若不存在返回0
    operator uint8_t();

  private:
    uint8_t resistance; //!< Keep it private!
    pga_channels channel;
};

#endif
