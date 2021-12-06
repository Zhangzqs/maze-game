#pragma once

#include "../util/Point.hpp"
#include "../map/MarkMap.hpp"
#include "Path.hpp"
#include <utility>
#include <vector>
#include <stack>
#include <list>
#include <unistd.h>

/**
 * 迷宫路径规划器
 */
class RoutePlanner {
protected:
    std::shared_ptr<MarkMap> _markMap;     //迷宫障碍物地图

public:
    /**
     * 路径规划接口，子类的路径规划器需要实现该纯虚函数
     * 其返回的结果包含起点与终点以及中间需要经过的各个迷宫坐标
     * @param start 起始坐标
     * @param end 目标点坐标
     * @return 总路径的顶点链表
     */
    virtual Path plan(const Point &start, const Point &end) = 0;

    void setMarkMap(std::shared_ptr<MarkMap> markMap){
        this->_markMap = std::move(markMap);
    }

public:
    explicit RoutePlanner(std::shared_ptr<MarkMap> markMap) : _markMap(std::move(markMap)) {
    }

    virtual ~RoutePlanner() = default;
};

