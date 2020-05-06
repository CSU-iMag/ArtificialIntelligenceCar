/**
 * @file storage
 * @brief flash
 */
#ifndef STORAGE_H
#define STORAGE_H

void storage_save(unsigned long sector, unsigned long page);
void storage_load(unsigned long sector, unsigned long page);

#endif
