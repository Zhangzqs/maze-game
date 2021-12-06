#include "map/MarkMap.hpp"
#include "map/DfsMapGenerator.hpp"
#include "MazePlotter.hpp"
#include "route_planner/DfsRoutePlanner.hpp"
#include "route_planner/AStarRoutePlanner.hpp"
#include "MazeController.hpp"
#include <opencv2/opencv.hpp>
#include <map>
#include "ColorConst.hpp"
#include "util/Menu.hpp"

void MouseCallback(int event, int x, int y, int flags, void *userdata) {
    auto *pMc = (MazeController *) userdata;
    auto map = pMc->getObstacleMap();
    auto image = pMc->getImage();

    int sx = image->cols / map->columns();
    int sy = image->rows / map->rows();

    int mx = x / sx;
    int my = y / sy;

    pMc->mouseCallback(my, mx, flags);
}

class MazeGame {
private:
    const int _row;
    const int _column;

    cv::Mat _image;  //画布

    MarkMap *_obstacleMap;
    MarkMap *_monsterMap;

    Point _start;    //起点
    Point _end;      //终点

    MazePlotter *_mazePlotter;  //迷宫绘图器

    MazeController *_mazeController;    //迷宫控制器

    std::map<int, function<void()>> keyMap; //键盘功能映射

    /**
     * 随机生成起点与终点
     */
    void randomStartEnd() {
        do {
            _start = Point::generateRandomPoint(Point(0, 0), Point(_column - 1, _row - 1));
            _end = Point::generateRandomPoint(Point(0, 0), Point(_column - 1, _row - 1));
        } while (_start == _end);
    }

    /**
     * 迷宫生成
     */
    void generateMap() {
        DfsMapGenerator g(*_obstacleMap, _start, _end);
        g.generate();
    }

    /**
     * 键盘映射
     */
    void initKeyMap() {
        keyMap['q'] = [&]() { _mazeController->movePigsy(-1, -1); };
        keyMap['w'] = [&]() { _mazeController->movePigsy(0, -1); };
        keyMap['e'] = [&]() { _mazeController->movePigsy(1, -1); };
        keyMap['a'] = [&]() { _mazeController->movePigsy(-1, 0); };
        keyMap['s'] = [&]() { _mazeController->movePigsy(0, 0); };
        keyMap['d'] = [&]() { _mazeController->movePigsy(1, 0); };
        keyMap['z'] = [&]() { _mazeController->movePigsy(-1, 1); };
        keyMap['x'] = [&]() { _mazeController->movePigsy(0, 1); };
        keyMap['c'] = [&]() { _mazeController->movePigsy(1, 1); };

        keyMap['p'] = [&]() { _mazeController->planRoute(_end); };
        keyMap['l'] = [&]() { _mazeController->autoNavigation(_end); };
        keyMap['u'] = [&]() { _mazeController->changeToAvoidMonster(); };
        keyMap['i'] = [&]() { _mazeController->changeToKillMonster(); };
        keyMap['1'] = [&]() { _mazeController->useAStarPlanner(); };
        keyMap['2'] = [&]() { _mazeController->useDfsPlanner(); };
        keyMap['3'] = [&]() { _mazeController->useBfsPlanner(); };
        keyMap['o'] = [&]() {
            _mazeController->clearAllMonster();
            _mazeController->autoNavigation(_end);
        };
    }

    /**
     * 路径规划器菜单
     */
    void showChangeRoutePlannerMenu() {
        zzq::Menu menu("改变路径规划器");
        menu.addItem("使用AStar路径规划器", [&]() {
            _mazeController->useAStarPlanner();
        });
        menu.addItem("使用Dfs路径规划器", [&]() {
            _mazeController->useAStarPlanner();
        });
        menu.addItem("使用Bfs路径规划器", [&]() {
            _mazeController->useAStarPlanner();
        });
        menu.show();    //启动菜单
    }

    /**
     * 遇见妖怪方案
     */
    void showChangeMonsterPlanMenu() {
        zzq::Menu menu("改变遇到妖怪的方案");
        menu.addItem("避让所有妖怪（可能导致路径无法规划）", [&]() {
            _mazeController->changeToAvoidMonster();
        });
        menu.addItem("允许击杀妖怪", [&]() {
            _mazeController->changeToKillMonster();
        });
        menu.show();
    }

    /**
     * 显示游戏菜单
     */
    void showGameMenu() {
        zzq::Menu menu("游戏菜单");
        menu.addItem("设置对妖怪的处理方案", [&]() { showChangeMonsterPlanMenu(); });
        menu.addItem("设置路径规划器",[&](){showChangeRoutePlannerMenu();});
        menu.addItem("退出游戏",[&](){
           _stopFlag = true;
            menu.exitMenu();    //向状态机发送停止信号
        });
        menu.addExitItem("返回游戏");
        menu.loop();    //启动自动机
    }

public:
    explicit MazeGame(int row, int column, int scale_factor)
            : _row(row), _column(column) {

        //定义画布
        _image = cv::Mat(row * scale_factor, column * scale_factor, CV_8UC3);

        //开辟迷宫和妖怪地图
        _obstacleMap = new MarkMap(row, column);
        _monsterMap = new MarkMap(row, column);

        //随机生成起点终点
        randomStartEnd();

        //生成迷宫图
        generateMap();

        _mazePlotter = new MazePlotter(_image, row, column);
        _mazeController = new MazeController(*_mazePlotter, *_obstacleMap, *_monsterMap, _start, _end, _image);

        _mazeController->updateScene();

        cv::imshow("image", _image);
        cv::moveWindow("image",0,0);
        cv::setMouseCallback("image", MouseCallback, _mazeController);

        initKeyMap();
    }

    bool _stopFlag = false;

    void loop() {

        for (int key = cv::waitKey(0);
             !_stopFlag;
             key = cv::waitKey(0)) {

            if (key == 27) {  //Esc键盘
                showGameMenu();
                if(_stopFlag){  //菜单设置里可能会改变stopFlag
                    break;
                }
            }
            cout << "正在监听键盘" << endl;
            if (key >= 'A' && key <= 'Z') {
                //如果是大写，转换为小写
                key += 32;
            }

            if (keyMap.count(key) != 0) {
                keyMap[key]();
            }

            _mazeController->updateScene();
        }
    }

};
