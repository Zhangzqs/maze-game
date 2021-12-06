
#pragma once

#include <iostream>
#include <boost/format.hpp>
#include <utility>

#include "util/Color.hpp"
#include "util/Point.hpp"

class Spirit {
private:
    const cv::Mat _asset;//精灵素材
    Point _position;    //当前精灵的坐标
    std::shared_ptr<MarkMap> _movableMarkMap;    //可移动性判定地图
public:

    explicit Spirit(const Point &position, cv::Mat asset)
            : _position(position), _asset(std::move(asset)) {
        _movableMarkMap = nullptr;
    }

    virtual ~Spirit() = default;

    /**
     * 根据纯颜色创建一个精灵
     * @param position 精灵位置
     * @param color 精灵颜色
     * @return 精灵对象
     */
    inline static Spirit *createByColor(const Point& position,const Color& color){
        cv::Mat asset(20,20,CV_8UC3);
        //绘制填充的方格
        cv::rectangle(asset,
                      cv::Point(0,0),
                      cv::Point(asset.cols,asset.rows),
                      color.to_cv_bgr(),
                      cv::FILLED,
                      cv::LINE_8);
        return new Spirit(position,asset);
    }

    inline Point getPosition() {
        return _position;
    }

    void setMovableMap(std::shared_ptr<MarkMap> movableMap) {
        this->_movableMarkMap = movableMap;
    }

    /**
     *
     * @param x
     * @param y
     * @return
     */
    bool forward(int x, int y) {
        return forward(Vector(x, y));
    }

    //前进
    bool forward(const Vector &dir) {
        Point tmp = _position + dir;   //先假设可以移动

        //如果该地图没有设置，说明可以任意移动
        if (_movableMarkMap == nullptr || !_movableMarkMap->hasMark(tmp)) {
            _position = tmp;
            //std::cout << "当前到达点：" << _position << std::endl;
            return true;    //成功移动
        }
        return false;   //移动不成功
    }

    cv::Mat getCurrentAsset() const {
        return _asset;
    }
};

