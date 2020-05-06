/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyleft 2020, 版权所无
 * @file       		main
 * @university	   	CSU
 * @author     		iMag
 * @version    		v1.0
 * @target		    RT1064DVL6A
 * @date       		2020-03-25
 * @github          https://github.com/volatile-static/iMag
 ********************************************************************************************************************/

//打开新的工程或者工程移动了位置务必执行以下操作
//第一步 关闭上面所有打开的文件
//第二步 project  clean  等待下方进度条走完

#include "car.hpp"

int main(void) {
    Car.Startup();
    for (;;) {
        Car.Run();
    }
}
