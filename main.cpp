#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "para.h"
#include "func.h"
#include "cls.h"

//物品1-7
std::vector<Article> articles={
        Article(),
        Article(1,{},3000,6000),
        Article(2,{},4400,7600),
        Article(3,{},5800,9200),
        Article(4,{1,2},15400,22500),
        Article(5,{1,3},17200,25000),
        Article(6,{2,3},19200,27500),
        Article(7,{4,5,6},76000,105000)
};

//每件物品可以出售给的工作台类型
std::vector<std::vector<int>> articleCanSellTo={
        {},
        {4,5,9},
        {4,6,9},
        {5,6,9},
        {7,9},
        {7,9},
        {7,9},
        {8,9}
};

//储存工作台和机器人
std::vector<Table> tables;
std::vector<Robot> robots;

//记录每个机器人前往购买和销售的目的地
std::vector<int> buyDestination(P_robotNum,-1);
std::vector<int> sellDestination(P_robotNum,-1);

//估计一个机器人到达某个地点的时间 单位秒
//考虑可能的碰撞时间
double timeEstimate(double direction,double x1,double y1,double x2,double y2){
    double angle=angleRotate(direction,
                             includedAngle(x1,y1,x2,y2));
    double distance= euDistance(x1,y1,x2,y2);
    return std::abs(angle)/M_PI+distance/6+1.5;
}

//为机器人规划一条路线进行购买和销售
void designRoute(int id,int k,int frameID){
    //各机器人前往购买的工具台应不一样
    std::vector<int> buyForbid;
    for (int i=0;i<P_robotNum;i++){
        if (buyDestination[i]!=-1){
            buyForbid.emplace_back(buyDestination[i]);
        }
    }
    int newBuyTarget=-1;
    int newSellTarget=-1;
    double maxQuota=0;
    //寻找可购买产品的工具台
    for (int i=0;i<k;i++){
        if (tables[i].tableType>7|| std::find(buyForbid.begin(),buyForbid.end(),i)!=buyForbid.end()
            ||tables[i].remainTime==-1){
            continue;
        }
        double buyTime= timeEstimate(robots[id].direction,robots[id].x,robots[id].y,tables[i].x,tables[i].y);
        double newDirection= includedAngle(robots[id].x,robots[id].y,tables[i].x,tables[i].y);
        //如果到达时产品无法生产出来则不选择此路线
        if (tables[i].remainTime==-1||(tables[i].remainTime>0&&buyTime*P_frameNum<tables[i].remainTime)){
            continue;
        }
        int articleHave=tables[i].tableType;
        //同种商品（1-6）不卖给同个工具台（9类型除外）
        std::vector<int> sellForbid;
        if (articleHave<7){
            for (int j=0;j<P_robotNum;j++){
                if (sellDestination[j]!=-1){
                    int articleThat=buyDestination[j]==-1?robots[j].articleCarried:tables[buyDestination[j]].tableType;
                    if (articleHave==articleThat&&tables[sellDestination[j]].tableType!=9){
                        sellForbid.emplace_back(sellDestination[j]);
                    }
                }
            }
        }
        for (int j=0;j<k;j++){
            int tableType=tables[j].tableType;
            //选择合适类型的工具台
            if (std::find(articleCanSellTo[articleHave].begin(),articleCanSellTo[articleHave].end(),tableType)
                ==articleCanSellTo[articleHave].end()|| std::find(sellForbid.begin(),sellForbid.end(),j)!=sellForbid.end()){
                continue;
            }
            //如果槽位满了则不选择此路线
            if (articleHave<7){
                if (hasRawMaterial(tables[j].rawMaterialStatus,articleHave)){
                    continue;
                }
            }
            double sellTime= timeEstimate(newDirection,tables[i].x,tables[i].y,tables[j].x,tables[j].y);
            //没时间购买出售则不选择此路线
            if ((buyTime+sellTime)*P_frameNum>(P_totalTime*P_frameNum-frameID)){
                continue;
            }
            //综合考虑利润时间
            double quota=(articles[articleHave].sellingPrice-articles[articleHave].purchasePrice)
                    /(buyTime+sellTime);
            //卖给9类工具台的优先级最低
            if (tableType==9){
                quota/=2;
            }
            if (quota>maxQuota){
                maxQuota=quota;
                newBuyTarget=i;
                newSellTarget=j;
            }
        }
    }
    buyDestination[id]=newBuyTarget;
    sellDestination[id]=newSellTarget;
}

//判断机器人是否与前面id的机器人碰撞
bool collideWithOther(int robotID){
    for (int i=0;i<robotID;i++){
        if (euDistance(robots[i].x,robots[i].y,robots[robotID].x,robots[robotID].y)<=2*P_articleRadius){
            return true;
        }
    }
    return false;
}

int main() {
    //初始化..........
    //读入地图
    int grid=int(P_mapLen/P_gridLen);
    for (int i=0;i<grid;i++){
        for (int j=0;j<grid;j++){
            char c;
            std::cin>>c;
            if (c=='A'){
                robots.emplace_back(P_gridLen*j+P_gridLen/2,P_mapLen-P_gridLen*i-P_gridLen/2);
            }
            else if (c>='1'&&c<='9'){
                tables.emplace_back(Table(int(c-'0'),P_gridLen*j+P_gridLen/2,P_mapLen-P_gridLen*i-P_gridLen/2));
            }
        }
    }
    std::string temp;  //"OK"
    std::cin>>temp;
    if (temp!="OK"){
        return 0;
    }
    std::cout<<"OK"<<std::endl;
    std::cout<<std::flush;

    //每帧处理
    int frameID;  //帧序号
    while (true){
        std::cin>>frameID;
        if (frameID==EOF){
            break;
        }
        int curFund;  //当前资金
        int k;  //工作台数量
        std::cin>>curFund>>k;
        for (int i=0;i<k;i++){
            //更新工作台状态
            int type;
            double x,y;
            int remain,raw,product;
            std::cin>>type>>x>>y>>remain>>raw>>product;
            tables[i].update(type,x,y,remain,raw,product);
        }
        for (int i=0;i<P_robotNum;i++){
            //更新机器人状态
            int loc,type;
            double time,collide,angular,lx,ly,dir,x,y;
            std::cin>>loc>>type>>time>>collide>>angular>>lx>>ly>>dir>>x>>y;
            robots[i].update(loc,type,time,collide,angular,lx,ly,dir,x,y);
        }
        std::string innerTemp;  //"OK"
        std::cin>>innerTemp;
        if (innerTemp!="OK"){
            break;
        }

        //发出指令
        std::cout<<frameID<<std::endl;

        //如果机器人没有目标或已到达目标则需规划路线
        for (int i=0;i<P_robotNum;i++){
            if (sellDestination[i]==-1){
                designRoute(i,k,frameID);
            }
        }

        //处于工作台且持有物品的机器人进行出售操作 处于工作台且不持有物品的机器人进行购买操作
        for (int i=0;i<P_robotNum;i++){
            if (robots[i].location!=-1&&robots[i].location==buyDestination[i]){
                //如果没生产好进行等待
                if (tables[robots[i].location].productStatus==1){
                    std::cout<<"buy "<<i<<std::endl;
                    buyDestination[i]=-1;
                }
            }
            if (robots[i].location!=-1&&robots[i].location==sellDestination[i]){
                std::cout<<"sell "<<i<<std::endl;
                sellDestination[i]=-1;
            }
        }

        //所有机器人向合适的工作台移动
        for (int i=0;i<P_robotNum;i++){
            //如果与其他机器人碰撞先退让
            if (collideWithOther(i)){
                std::cout<<"rotate "<<i<<" 3.14159"<<std::endl;
                std::cout<<"forward "<<i<<" -2"<<std::endl;
                continue;
            }
            //没有目标则暂停等待（需离开工具台）
            if (sellDestination[i]==-1){
                if (robots[i].location!=-1){
                    std::cout<<"rotate "<<i<<" 3.14159"<<std::endl;
                    std::cout<<"forward "<<i<<" -2"<<std::endl;
                }
                else{
                    std::cout<<"rotate "<<i<<" 0"<<std::endl;
                    std::cout<<"forward "<<i<<" 0"<<std::endl;
                }
                continue;
            }
            //先调整角度再前进
            int goalTableID=buyDestination[i]==-1?sellDestination[i]:buyDestination[i];
            double angle=angleRotate(robots[i].direction,
                                     includedAngle(robots[i].x,robots[i].y,
                                                   tables[goalTableID].x,tables[goalTableID].y));
            if (std::abs(angle)>=0.01){
                std::cout<<"forward "<<i<<" 0"<<std::endl;
                double rotateSpeed=angle*P_frameNum;
                rotateSpeed=std::min(rotateSpeed,M_PI);
                rotateSpeed=std::max(rotateSpeed,-M_PI);
                std::cout<<"rotate "<<i<<" "<<rotateSpeed<<std::endl;
            }
            else{
                std::cout<<"rotate "<<i<<" 0"<<std::endl;
                double forwardSpeed= euDistance(robots[i].x,robots[i].y,tables[goalTableID].x,tables[goalTableID].y)*P_frameNum;
                forwardSpeed= std::min(6.0,forwardSpeed);
                std::cout<<"forward "<<i<<" "<<forwardSpeed<<std::endl;
            }
        }

        std::cout<<"OK"<<std::endl;
        std::cout<<std::flush;
    }

    return 0;
}
