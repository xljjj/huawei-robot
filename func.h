//
// Created by Xu Lingjue on 2023/3/10.
//

#ifndef ROBOTV1_FUNC_H
#define ROBOTV1_FUNC_H

#include <cmath>

//计算质量
double calQuality(double density,double radius){
    return density*M_PI*radius*radius;
}

double f(double x,double maxX,double minRate){
    if (x<maxX){
        return (1- sqrt(1-(1-x/maxX)*(1-x/maxX)))*(1-minRate)+minRate;
    }
    else{
        return minRate;
    }
}

//时间价值系数
double timeValueRatio(int frame){
    return f(frame,9000,0.8);
}

//碰撞价值系数
double collideValueRatio(double impulse){
    return f(impulse,1000,0.8);
}

//判断是否有某种原材料
bool hasRawMaterial(int status,int id){
    return ((status>>id)&1)==1;
}

//两个坐标间的距离
double euDistance(double x1,double y1,double x2,double y2){
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

//两个坐标间的夹角-pi~pi
double includedAngle(double x1,double y1,double x2,double y2){
    return atan2(y2-y1,x2-x1);
}

//-pi~pi从a1旋转到a2
double angleRotate(double a1,double a2){
    double diff=a2-a1;
    if (diff>M_PI){
        return diff-2*M_PI;
    }
    else if (diff<-M_PI){
        return diff+2*M_PI;
    }
    else{
        return diff;
    }
}

#endif //ROBOTV1_FUNC_H
