#include "pga.hpp"
#include "MCP4452.h"
#include "util.h"

pga_t &pga_t::operator=(const uint8_t &rhs) {
    if (resistance == rhs || channel == pga_channels::NONE)
        return *this;
    resistance = rhs;

    switch (channel) {
    case pga_channels::A1:
        MCP4452_set_val(MCP4452_ADDR_0, MCP4452_R1, this->resistance);
        break;
    case pga_channels::A2:
        MCP4452_set_val(MCP4452_ADDR_0, MCP4452_R2, this->resistance);
        break;
    case pga_channels::A3:
        MCP4452_set_val(MCP4452_ADDR_0, MCP4452_R3, this->resistance);
        break;
    case pga_channels::A4:
        MCP4452_set_val(MCP4452_ADDR_0, MCP4452_R4, this->resistance);
        break;
    case pga_channels::B1:
        MCP4452_set_val(MCP4452_ADDR_1, MCP4452_R1, this->resistance);
        break;
    case pga_channels::B2:
        MCP4452_set_val(MCP4452_ADDR_1, MCP4452_R2, this->resistance);
        break;
    case pga_channels::B3:
        MCP4452_set_val(MCP4452_ADDR_1, MCP4452_R3, this->resistance);
        break;
    case pga_channels::B4:
        MCP4452_set_val(MCP4452_ADDR_1, MCP4452_R4, this->resistance);
        break;
    default:
        break;
    }
    return *this;
}

pga_t &pga_t::operator+=(const int8_t &rhs) { return *this = resistance + rhs; }

pga_t::operator uint8_t() {
    if (channel == pga_channels::NONE)
        return 0;
    else
        return resistance;
}
