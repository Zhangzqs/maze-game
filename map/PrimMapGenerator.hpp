#pragma once
#include "MapGenerator.hpp"
#include "MarkMap.hpp"

class PrimMapGenerator: public MapGenerator{
private:
    Point _dfs_begin;    //DFS迷宫生成算法的起始点位
public:
    explicit PrimMapGenerator(ObstacleMap& obstacleMap): MapGenerator(obstacleMap){

    }
    explicit PrimMapGenerator(ObstacleMap& obstacleMap, const Point& start, const Point& end)
            : MapGenerator(obstacleMap, start, end){

    }



    //Prim生成地图
    void generate() override {
        //TODO
    }
};