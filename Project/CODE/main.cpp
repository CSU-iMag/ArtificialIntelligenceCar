/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyleft 2020, ��Ȩ����
 * @file       		main
 * @university	   	CSU
 * @author     		iMag
 * @version    		v1.0
 * @target		    RT1064DVL6A
 * @date       		2020-03-25
 * @github          https://github.com/volatile-static/iMag
 ********************************************************************************************************************/

//���µĹ��̻��߹����ƶ���λ�����ִ�����²���
//��һ�� �ر��������д򿪵��ļ�
//�ڶ��� project  clean  �ȴ��·�����������

#include "car.hpp"

int main(void) {
    Car.Startup();
    for (;;) {
        Car.Run();
    }
}
