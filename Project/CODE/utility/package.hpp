/**
 * @file PACK
 * @brief 与上位机通讯的数据包
 */
#ifndef PACKAGE_H
#define PACKAGE_H

#include <vector>

void package_receive(std::vector<uint8_t> buffer);
void package_init();
void package_sendDeep(uint8_t);

#endif
