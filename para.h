//
// Created by Xu Lingjue on 2023/3/10.
//

#ifndef ROBOTV1_PARA_H
#define ROBOTV1_PARA_H

#include <cmath>

int P_totalTime=180;  //比赛时长s
double P_mapLen=50;  //地图尺寸m
double P_gridLen=0.5;  //格子边长
int P_robotNum=4;  //机器人数量
int P_frameNum=50;  //帧数pfs
int P_initialFund=200000;  //初始资金
double P_contactTableLen=0.4;  //机器人与工作台接触距离m
double P_normalRadius=0.45;  //机器人常态半径m
double P_articleRadius=0.53;  //机器人持有物品半径m
double P_RobotDensity=20;  //机器人密度kg/m2
double P_maxForwardSpeed=6;  //最大前进速度m/s
double P_maxBackSpeed=2;  //最大后退速度m/s
double P_maxRotateSpeed=M_PI;  //最大旋转速度/s
double P_maxPullForce=150;  //最大牵引力N
double P_maxMoment=50;  //最大力矩N*m

#endif //ROBOTV1_PARA_H
