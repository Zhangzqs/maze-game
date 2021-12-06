#pragma once

#include <list>
#include <utility>
#include "../util/Point.hpp"

struct Path {

    std::list<Point> pathPoint;

    explicit Path(std::list<Point> pathPoint) : pathPoint(std::move(pathPoint)) {

    }

    /**
     * 计算路径的所有位移
     * @return 所有位移段
     */
    std::list<Vector> calcDeltas() {
        std::list<Vector> deltas{};

        Point keyPoint = *pathPoint.begin();
        auto keyPointIt = pathPoint.begin();
        keyPointIt++;

        while (keyPointIt != pathPoint.end()) {
            Point nextKeyPoint = *keyPointIt;

            auto delta = nextKeyPoint - keyPoint;
            deltas.push_back(delta);

            keyPoint = nextKeyPoint;
            keyPointIt++;
        }

        //现在delta保存了所有的位移信息
        return deltas;
    }

    /**
     * 统计某位移段集的转弯次数
     * @param deltas 位移段集
     * @return 转弯次数
     */
    int calcPathRot() {
        auto deltas = calcDeltas();
        int c = 0;
        Vector v = *deltas.begin();
        for (const auto &item : deltas) {
            if (v != item) {
                c++;
                v = item;
            }
        }
        return c;
    }



    /**
     * 计算路径长度
     * @return
     */
    int calcPathLen() {
        int len = 0;
        auto deltas = calcDeltas();
        for (const auto &item : deltas){
            if(item.x * item.y == 0){
                len+=2;
            }else{
                len+=3;
            }
        }
        return len / 2;
    }

};