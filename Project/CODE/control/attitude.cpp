#include "SEEKFREE_MPU6050.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_OLED.h"
#include "attitude.hpp"

void MPU6050_displayRawData(void){
	get_accdata();
	get_gyro();
  oled_p8x16str(2 + 6*0 ,0 ,"gyro");
  oled_int16(2 + 6*0 ,2 , mpu_gyro_x);
  oled_int16(2 + 6*0 ,3 , mpu_gyro_y);
  oled_int16(2 + 6*0 ,4 , mpu_gyro_z);

  oled_p8x16str(2 + 6*10 ,0 ,"acc");
  oled_int16(2 + 6*10,2 , mpu_acc_x);
  oled_int16(2 + 6*10,3 , mpu_acc_y);
  oled_int16(2 + 6*10,4 , mpu_acc_z);

}
