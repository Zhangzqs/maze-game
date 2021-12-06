#pragma once
#include "MapGenerator.hpp"
#include "MarkMap.hpp"

class DfsMapGenerator: public MapGenerator{
private:
    Point _dfs_begin;    //DFS迷宫生成算法的起始点位
public:
    explicit DfsMapGenerator(MarkMap& obstacleMap): MapGenerator(obstacleMap){
        initMaze();
        random_dfs_init_point();
    }
    explicit DfsMapGenerator(MarkMap& obstacleMap, const Point& start, const Point& end)
        : MapGenerator(obstacleMap, start, end){
        initMaze();
        random_dfs_init_point();
    }
    //随机初始化DFS初始点
    void random_dfs_init_point(){
        srand(time(nullptr));   //NOLINT
        int width = _obstacleMap.columns();
        int height = _obstacleMap.rows();
        int beginX = (rand() % (width - 2) + 1) | 1;     //NOLINT
        int beginY = (rand() % (height - 2) + 1) | 1;    //NOLINT
        _dfs_begin = Point(beginX, beginY);
    }

    void initMaze() {
        //迷宫全部填充为障碍
        _obstacleMap.reset(true);

        //清空起点，终点的障碍
        _obstacleMap.clear(_begin);
        _obstacleMap.clear(_end);
    }


    
    //深度优先遍历生成迷宫地图
    void generate() override {
        int x = _dfs_begin.x;
        int y = _dfs_begin.y;
        // 定义方向容器
        std::vector<Point> dirs{
                Point(1, 0),
                Point(-1, 0),
                Point(0, 1),
                Point(0, -1),
                Point(1, 1),
                Point(-1, 1),
                Point(1, -1),
                Point(-1, -1),
        }; //有四个方向

        // 随机打乱方向
        std::random_shuffle(dirs.begin(), dirs.end());    //NOLINT

        // 递归生成迷宫
        _obstacleMap.clear(x,y);    //打破该位置的墙面

        for (int i = 0; i < 8; ++i) {        //向四个方向探测是否有路线
            Point current_dir = dirs[i];

            int x1 = x + 2 * current_dir.x;
            int y1 = y + 2 * current_dir.y;
            if (x1 >= 1 &&                   //
                x1 <= _obstacleMap.columns() - 2 &&      //
                y1 >= 1 &&                   //
                y1 <= _obstacleMap.rows() - 2 &&      //
                _obstacleMap.hasMark(x1,y1)) {     //(x1,y1)有障碍物

                _obstacleMap.clear(x + current_dir.x, y + current_dir.y); //挖墙

                _dfs_begin = Point(x1,y1);
                generate();
            }
        }
    }
};