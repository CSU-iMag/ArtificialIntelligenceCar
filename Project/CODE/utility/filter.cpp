#include "filter.hpp"

struct fir_mov_t fir_mov = {3, {0.3333333333, 0.3333333333, 0.3333333333}};

struct fir_low1_t fir_low1 = {
    26, {0.0001979996014,  -0.0006330574397, 0.001568097738,  -0.003304710612,
         0.006254237611,   -0.01095288899,   0.01811391674,   -0.02876197733,
         0.04459231347,    -0.06900795549,   0.1107223779,    -0.201850757,
         0.6330996752,     0.6330996752,     -0.201850757,    0.1107223779,
         -0.06900795549,   0.04459231347,    -0.02876197733,  0.01811391674,
         -0.01095288899,   0.006254237611,   -0.003304710612, 0.001568097738,
         -0.0006330574397, 0.0001979996014}};

filter_kalman::filter_kalman(float ProcessNiose, float MeasureNoise)
    : Q(ProcessNiose), R(MeasureNoise) {
    p_last = kalmanGain = 1;
    p_middle = p_now = x_last = x_middle = x_now = 0;
}

float filter_kalman::Filte(float Measurement) {
    p_middle = p_last + Q; // x_last=x(k-1|k-1),x_mid=x(k|k-1)
    x_middle = x_last;     // p_mid=p(k|k-1),p_last=p(k-1|k-1)
    kalmanGain = p_middle / (p_middle + R); //计算卡尔曼的增益
    x_now =
        x_middle + kalmanGain * (Measurement - x_middle); //预测出当前最优解
    p_now = (1 - kalmanGain) * p_middle; //最优解所对应的协方差
    p_last = p_now;                    //用于更新当前预测值
    x_last = x_now;                    //用于更新当前协方差
    return x_now;
}

///**
// * @name 为左轮速度和右轮速度定义的滤波器
// * @brief
// 如果要更换滤波器，只需要将滤波器参数结构体如fir_mov全部替换成另一种滤波器参数结构体
// * @{
// */
// filter_fir_t
// filter_fir_speedL(fir_mov.numTaps,fir_mov.pCoeffs,1,&Car.encoderL.speed);
// filter_fir_t
// filter_fir_speedR(fir_mov.numTaps,fir_mov.pCoeffs,1,&Car.encoderR.speed);

///** @} 为左轮速度和右轮速度定义的滤波器 */
