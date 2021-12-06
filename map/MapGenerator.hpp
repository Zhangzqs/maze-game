//
// Created by zhiqiang on 2021/1/12.
//
#pragma once

#include "../util/Point.hpp"
#include "MarkMap.hpp"
#include <vector>
#include <algorithm>


//妖怪地图
using MonsterMap = MarkMap;

/*
 * 迷宫地图生成器的抽象基类
 * */
class MapGenerator{
protected:
    MarkMap& _obstacleMap;   //障碍物地图
    Point _begin;                //定义迷宫起点
    Point _end;                  //定义迷宫终点

public:
    inline Point getBegin(){
        return _begin;
    }
    inline Point getEnd(){
        return _end;
    }
public:
    /**
     * 定义一个地图生成器
     * 该构造方法定义的迷宫起点为(0,1)，终点为右下角上一格子
     * @param obstacleMap 一张空白的障碍物地图
     */
    explicit MapGenerator(MarkMap& obstacleMap): _obstacleMap(obstacleMap){
        _begin = Point(0, 1);    //默认从左上角开辟起点
        _end = Point(obstacleMap.columns() - 1, obstacleMap.rows() - 2);   //默认从右下角开辟终点
    }

    /**
     * 定义一个地图生成器
     * @param obstacleMap 一张空白的障碍物地图
     * @param start 地图起点
     * @param end 地图终点
     */
    explicit MapGenerator(MarkMap& obstacleMap, const Point& start, const Point& end)
    : _obstacleMap(obstacleMap), _begin(start), _end(end){

    }
    virtual ~MapGenerator() = default;

    /**
     * 地图生成
     */
    virtual void generate() = 0;
};
