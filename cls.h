//
// Created by Xu Lingjue on 2023/3/10.
//

#ifndef ROBOTV1_CLS_H
#define ROBOTV1_CLS_H

#include <utility>
#include <vector>

//物品
class Article{
public:
    int id{};
    std::vector<int> prescription;  //配方
    int purchasePrice{};  //购买价
    int sellingPrice{};  //售出价
    Article()=default;
    Article(int id,std::vector<int> pre,int p,int s): id(id),prescription(std::move(pre)),purchasePrice(p),sellingPrice(s){

    }
};

//工作台类型
class TableType{
public:
    int id{};
    std::vector<int> rawMaterial;  //原材料
    int workCycle{};  //工作周期
    int product{};  //产品 0表示没有
    TableType()=default;
    TableType(int id,std::vector<int> r,int w,int p):id(id),rawMaterial(std::move(r)),workCycle(w),product(p){

    }
};

//工作台
class Table{
public:
    int tableType;
    double x;
    double y;
    int remainTime;  //剩余生产时间 -1没有生产 0输出格阻塞 >=0剩余生产帧数
    int rawMaterialStatus;  //原材料格状态 二进制表示
    int productStatus;  //产品格状态 0无1有

    Table()=default;
    Table(int t,double x,double y): tableType(t),x(x),y(y),remainTime(-1),rawMaterialStatus(0),productStatus(0){

    }

    void update(int tab,double cur_x,double cur_y,int remain,int raw,int pro){
        tableType=tab;
        x=cur_x;
        y=cur_y;
        remainTime=remain;
        rawMaterialStatus=raw;
        productStatus=pro;
    }
};

//机器人
class Robot{
public:
    int location;  //所处工作台ID，-1表示不处于任何工作台附近
    int articleCarried;  //携带物品类型 0表示未携带
    double timeRatio;  //时间价值系数 0未携带
    double collideRatio;  //碰撞价值系数 0未携带
    double angularSpeed;  //角速度逆时针为正 弧度/秒
    double linearSpeedX;
    double linearSpeedY;  //线速度x,y方向 单位m/s
    double direction;  //朝向 -pi~pi 0右方向 pi/2上方向 -pi/2下方向
    double x;
    double y;  //坐标

    Robot()=default;
    Robot(double x,double y):location(-1),articleCarried(0),timeRatio(0),collideRatio(0),angularSpeed(0),
    linearSpeedX(0),linearSpeedY(0),direction(0),x(x),y(y){

    }

    void update(int loc,int art,double time,double collide,double angular,double lx,double ly,double dir,
                double cur_x,double cur_y){
        location=loc;
        articleCarried=art;
        timeRatio=time;
        collideRatio=collide;
        angularSpeed=angular;
        linearSpeedX=lx;
        linearSpeedY=ly;
        direction=dir;
        x=cur_x;
        y=cur_y;
    }
};

#endif //ROBOTV1_CLS_H
