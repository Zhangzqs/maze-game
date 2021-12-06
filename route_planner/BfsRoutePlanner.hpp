#pragma once

#include "RoutePlanner.hpp"
#include <queue>
#include <map>
#include <utility>
#include "../util/PriorityQueue.hpp"

/**
 * 使用A*启发式搜索算法，实现了路径规划
 */
class BfsRoutePlanner : public RoutePlanner {

private:
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
    /**
     * 获取附近所有的可用点
     * @return 某个点附近的所有的可行点
     */
    std::list<Point> getValidPoints(const Point &current) {
        std::list<Point> validPoints;
        for (auto &dir:dirs) {
            Point tmp = dir + current;
            if (!_markMap->hasMark(tmp)) { //如果无标记，即有空间
                validPoints.push_back(tmp);
            }
        }
        return validPoints;
    }

public:
    explicit BfsRoutePlanner(std::shared_ptr<MarkMap> obstacleMap) : RoutePlanner(std::move(obstacleMap)) {
    }

    Path plan(const Point &start, const Point &end) override {
        Point goal;
        auto frontier = PriorityQueue<Point>();   //优先队列，保证取出来的都是最小的
        frontier.push(start, 0);     //将起点添加进优先队列
        std::map<Point, Point> came_from;    //路线的映射，表示某个位置的来源
        std::map<Point, int> cost_so_far;    //已探索过的每一点的总代价
        came_from[start] = Point(-1, -1);    //起点并不是由任何一个点到达的
        cost_so_far[start] = 0;     //起点代价为0

        bool successful = false;
        while (!frontier.empty()) {   //只要优先队列不为空
            auto current = frontier.pop();  //就取出一个代价最小的作为当前扫描点

            if (current == end) {
                //如果扫描到终点
                //成功
                successful = true;
                break;
            }

            auto neighbors = getValidPoints(current);   //获取当前点可走的邻进点
            for (auto &next:neighbors) {    //获取所有可以走的路线
                auto delta = next - current;

                //若方向未改变，代价为2,方向改变了，代价为3
                int deltaCost = delta.x * delta.y == 0 ? 2 : 3;

                int new_cost = cost_so_far[current] + deltaCost;    //每走一格，当前代价+1

                if (cost_so_far.count(next) == 0 || new_cost < cost_so_far[next]) {  //如果代价未计算或新代价较小
                    cost_so_far[next] = new_cost;   //更新代价
                    int priority = new_cost;
                    frontier.push(next, priority);
                    came_from[next] = current;  //选择最小代价的路线走
                    goal = current;    //更新能够到达的目标点
                }
            }
        }

        if(!successful){
            //AStar路径规划失败，返回空路径
            return Path(std::list<Point>());
        }


        std::list<Point> route;
        for (auto a = end; a != start; a = came_from[a]) {
            route.push_front(a);
        }
        route.push_front(start);    //起点也算
        return Path(route);
    }
};