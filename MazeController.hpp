#include <utility>

#include "MazePlotter.hpp"
#include "route_planner/BfsRoutePlanner.hpp"
#include "ColorConst.hpp"
#include <iostream>
#include <boost/format.hpp>
#include "map/MultiMarkMap.hpp"

using namespace std;

/**
 * 迷宫控制器
 */
class MazeController {
private:
    //迷宫绘图器
    MazePlotter &_mazePlotter;

    //障碍物地图
    MarkMap &_obstacleMap;

    //妖怪地图
    MonsterMap &_monsterMap;

    //融合地图
    std::shared_ptr<MultiMarkMap> _multiMarkMap;

    //路径规划用的地图
    std::shared_ptr<MarkMap> _routePlannerMap;

    //路径
    std::unique_ptr<Path> _path;

    //画布
    cv::Mat _image;

//
//    //画布背景色
//    Color _backgroundColor;

    //画布背景图片
    cv::Mat _backgroundImage;

//
//    //障碍物颜色
//    Color _brickColor;

    //路径规划器颜色
    Color _pathColor;

    //妖怪图片素材
    cv::Mat _monsterIcon;

    //砖块贴图素材
    cv::Mat _brickIcon;

    //解救成功素材
    cv::Mat _successfulAsset;

    //遇到妖怪素材
    cv::Mat _meetMonsterAsset;

    //唐僧精灵
    std::unique_ptr<Spirit> _xuanzang;

    //猪八戒精灵
    std::unique_ptr<Spirit> _pigsy;

    //路径规划器
    std::shared_ptr<RoutePlanner> _routePlanner;

public:
    explicit MazeController(MazePlotter &mazePlotter,
                            MarkMap &obstacleMap,
                            MonsterMap &monsterMap,
                            const Point &start, //起点
                            const Point &end,   //终点
                            cv::Mat image)
            : _mazePlotter(mazePlotter), _obstacleMap(obstacleMap),
              _monsterMap(monsterMap), _image(std::move(image)) {
        //_backgroundColor = ColorConst::BACKGROUND;
        //_brickColor = ColorConst::BRICK;

        _pathColor = ColorConst::PATH;
//        _xuanzang = Spirit::createByColor(
//                end,
//                ColorConst::XUANZANG
//        );
//        _pigsy = Spirit::createByColor(
//                start,
//                ColorConst::PIGSY
//        );

        _pigsy = std::make_unique<Spirit>(start,  //猪八戒是起点
                                          cv::imread("../assets/pig.png"));

        _xuanzang = std::make_unique<Spirit>(end, //唐僧是终点
                               cv::imread("../assets/xuanzang.png"));

        //加载一些图片素材
        _monsterIcon = cv::imread("../assets/monster.png");
        _brickIcon = cv::imread("../assets/brick.png");
        _backgroundImage = cv::imread("../assets/background.jpeg");
        _successfulAsset = cv::imread("../assets/successful.jpeg");
        _meetMonsterAsset = cv::imread("../assets/meetMonster.png");

        _path = nullptr;    //初始化为空指针
        _routePlanner = nullptr;

        //融合地图
        _multiMarkMap = std::make_unique<MultiMarkMap>(obstacleMap.rows(),obstacleMap.columns());
        _multiMarkMap->addMarkMap(&_obstacleMap);
        _multiMarkMap->addMarkMap(&_monsterMap);

        //必须先初始化路径规划器，不然改变避障模式时，会空指针
        useAStarPlanner();

        //路径规划用的地图默认是避开妖怪模式
        changeToAvoidMonster();

    }

    cv::Mat *getImage() {
        return &_image;
    }

    MarkMap *getObstacleMap() {
        return &_obstacleMap;
    }

    /**
     * 更新整个游戏场景
     */
    void updateScene() {
        //绘制背景画布
        _mazePlotter.fillByImage(_backgroundImage);

        //绘制障碍物
        _mazePlotter.draw_mark_map(_obstacleMap, _brickIcon);

        //绘制妖怪
        _mazePlotter.draw_mark_map(_monsterMap, _monsterIcon);

        //绘制猪八戒与唐僧
        _mazePlotter.draw_spirit(*_xuanzang);
        _mazePlotter.draw_spirit(*_pigsy);

        //绘制路径
        if (_path != nullptr) {
            _mazePlotter.draw_path(*_path, _pathColor);
        }
        cv::imshow("image", _image);
    }

//    void setBackgroundColor(Color color) {
//        _backgroundColor = color;
//    }

    void successful() {
        //_mazePlotter.paste_image(cv::Rect(),_successfulAsset,180);
        cout << "猪八戒到达了终点，成功解救了唐僧" << endl;
        cv::imshow("successful", _successfulAsset);
        cv::moveWindow("successful",0,0);
        cv::waitKey(0);
        cv::destroyWindow("successful");
    }

    void meetMonster() {
        cout << "遇到妖怪，坐标" <<_pigsy->getPosition() << endl;
        cv::imshow("meetMonster", _meetMonsterAsset);
        cv::moveWindow("meetMonster1",0,0);
        cv::waitKey(0);
        cv::destroyWindow("meetMonster");

        _monsterMap.clear(_pigsy->getPosition());
        cout<<"成功击杀妖怪"<<endl;
    }

    /**
     * 创建妖怪
     * @param row 行号
     * @param col 列号
     */
    void addMonster(int row, int col) {
        //首先不能有障碍物
        if (_obstacleMap.hasMark(row, col)) {
            cout << "该地有障碍物，无法创建妖怪" << endl;
            return;
        }
        cout << "创建妖怪，坐标" << Point(col, row) << endl;
        _monsterMap.mark(row, col);
        updateScene();
    }

    /**
     * 鼠标事件回调
     * @param row
     * @param col
     */
    void mouseCallback(int row, int col, int eventFlags) {

        if (eventFlags & cv::MouseEventFlags::EVENT_FLAG_LBUTTON) {
            if(eventFlags & cv::MouseEventFlags::EVENT_FLAG_CTRLKEY){
                //左键且按住ctrl
                autoNavigation(Point(col,row));
            }else{
                //左键新增障碍物
                addObstacle(row, col);
            }
        } else if (eventFlags & cv::MouseEventFlags::EVENT_FLAG_RBUTTON) {
            //右键清除障碍物
            clearObstacle(row, col);
        } else if (eventFlags & cv::MouseEventFlags::EVENT_FLAG_MBUTTON) {
            //中键添加妖怪
            addMonster(row, col);
        }


        updateScene();
    }

    /**
     * 击杀所有的妖怪
     */
    void clearAllMonster(){
        for(int row = 0;row < _monsterMap.rows();row++){
            for(int col = 0;col < _monsterMap.columns();col++){
                if(_monsterMap.hasMark(row,col)){
                    //如果有妖怪，先去妖怪处清理
                    autoNavigation(Point(col,row));
                }
            }
        }
    }

    /**
     * 沿着指定方向移动猪八戒
     * @param dir
     */
    void movePigsy(const Vector &dir) {
        movePigsy(dir.x, dir.y);
    }

    void movePigsy(int x, int y) {
        if (_pigsy->forward(x, y)) {
            cout << (boost::format("移动猪八戒，方向(%d, %d)，当前坐标%s") % x % y %
                     _pigsy->getPosition().to_string()).str()
                 << endl;
            if (_pigsy->getPosition() == _xuanzang->getPosition()) {
                updateScene();
                successful();
            }
            if(_monsterMap.hasMark(_pigsy->getPosition())){
                updateScene();
                meetMonster();
            }
        } else {
            cout << "猪八戒发生碰撞，禁止向方向" << (boost::format("(%d, %d)") % x % y).str() << "移动" << endl;
        }
    }

    /**
     * 在指定格子添加障碍物
     * @param row
     * @param col
     */
    void addObstacle(int row, int col) {
        _obstacleMap.mark(row, col);
        cout << (boost::format("添加障碍物(%d, %d)") % col % row).str() << endl;
    }

    /**
     * 在指定格子清除障碍物
     * @param row
     * @param col
     */
    void clearObstacle(int row, int col) {
        _obstacleMap.clear(row, col);
        cout << "清除障碍(" << col << ", " << row << ")" << endl;
    }

    /**
     * 自动导航
     */
    void autoNavigation(const Point& point) {
        //导航之前先进行路径规划
        if (!planRoute(point)) {
            //如果路径规划失败
            cout << "路径规划失败，无法自动导航" << endl;
            return;
        }
        //路径规划成功，开始自动导航

        //获取所有的位移段
        auto deltas = _path->calcDeltas();
        for (const auto &delta : deltas) {
            // 沿着各个位移段前行
            cout << "正在自动导航，按任意键停止导航" << endl;
            movePigsy(delta);
            updateScene();

            int key = cv::waitKey(100);
            if (key != -1) {
                //有键盘被按下，导航中断
                cout << "自动导航被终止" << endl;
                break;
            }
        }
        _path = std::make_unique<Path>(std::list<Point>());
    }

    /**
     * 避开妖怪模式
     */
    void changeToAvoidMonster(){
        cout<<"切换至避开妖怪模式"<<endl;
        _routePlannerMap = _multiMarkMap;
        _pigsy->setMovableMap(_routePlannerMap);
        _routePlanner->setMarkMap(_routePlannerMap);
    }

    /**
     * 击杀妖怪模式
     */
     void changeToKillMonster(){
         cout<<"切换至击杀妖怪模式"<<endl;
        _routePlannerMap = std::shared_ptr<MarkMap>(&_obstacleMap);
        _pigsy->setMovableMap(_routePlannerMap);
        _routePlanner->setMarkMap(_routePlannerMap);
     }

    void useAStarPlanner() {
        _routePlanner = std::make_shared<AStarRoutePlanner>(_routePlannerMap);
        cout << "使用AStar算法路径规划" << endl;
    }

    void useDfsPlanner() {
        _routePlanner = std::make_unique<DfsRoutePlanner>(_routePlannerMap);
        cout << "使用DFS算法路径规划" << endl;
    }

    void useBfsPlanner() {
        _routePlanner = std::make_unique<BfsRoutePlanner>(_routePlannerMap);
        cout << "使用BFS算法路径规划" << endl;
    }

    /**
     * 路径规划
     */
    bool planRoute(const Point& point) {
        if (_pigsy->getPosition() == point) {
            cout << "已到达终点，路径规划失败" << endl;
            return false;
        }

        cout << "正在进行路径规划，请等待..." << endl;
        _path = std::make_unique<Path>(_routePlanner->plan(_pigsy->getPosition(), point));
        if (_path->pathPoint.empty()) {
            cout << "路径规划失败，无可达路径，请尝试破墙或铲除妖怪后重试" << endl;
            return false;
        } else {
            cout << "路径规划成功，全程" << _path->calcPathLen() << "需要转弯" << _path->calcPathRot() << "次" << endl;
            return true;
        }
    }
};