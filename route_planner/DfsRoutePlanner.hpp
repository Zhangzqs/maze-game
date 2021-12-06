//
// Created by zzq on 4/6/21.
//
#pragma once

#include "RoutePlanner.hpp"
#include <memory>
#include <utility>

class DfsRoutePlanner : public RoutePlanner {
private:
    using WalkedMarkMap = MarkMap;  //定义类型，走过的标记地图

    // 定义方向容器
    const std::vector<Point> dirs{

            Point(1, 0),
            Point(-1, 0),
            Point(0, 1),
            Point(0, -1),
            Point(1, 1),
            Point(-1, 1),
            Point(1, -1),
            Point(-1, -1),
    }; //有四个方向
    // 定义一个新的障碍物地图，用于记录曾经走过的所有路，用于回溯
    std::unique_ptr<WalkedMarkMap> _walkedPointMap;

public:
    explicit DfsRoutePlanner(std::shared_ptr<MarkMap> markMap) : RoutePlanner(std::move(markMap)){
    }

    Path plan(const Point &start, const Point &end) override {
        _walkedPointMap = std::make_unique<WalkedMarkMap>(_markMap->rows(), _markMap->columns());
        //定义链栈容器
        std::list<Point> st{};
        st.push_back(start);  //开始位置入栈
        while (!st.empty()) {    //只要栈非空
            int i;
            for (i = 0; i < 8; i++) {   //随机选取一个前进方向
                auto dir = dirs[i];
                if (st.back().x + dir.x >= 0 &&                  //满足不超过整个迷宫范围
                    st.back().x + dir.x <= _markMap->columns() - 1 &&    //
                    st.back().y + dir.y >= 0 &&                 //
                    st.back().y + dir.y <= _markMap->rows() - 1 &&   //
                    !_markMap->hasMark(st.back() + dir) && //若dir方向上无障碍物，即有路可走
                    !_walkedPointMap->hasMark(st.back() + dir)) {  //且之前没有走过这条路
                    //满足上述条件，那就判定dir可以前进
                    //前进之前先将前一时刻的路记录下来
                    _walkedPointMap->mark(st.back());
                    st.push_back(st.back() + dir);
                    if (st.back() == end) { //如果搜索到了终点
                        _walkedPointMap.reset();
                        return Path(st);
                    }
                    break;  //找到一条路，先跳出循环
                }
            }
            //若i==8,说明下一步无路可走了，需要回溯
            if (i == 8) {
                _walkedPointMap->mark(st.back()); //标记，走过这里
                st.pop_back();
            }
        }

        _walkedPointMap.reset();
        return Path(st);
    }
};

