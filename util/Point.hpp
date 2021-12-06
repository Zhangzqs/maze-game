#pragma once

#include <sstream>

class Point {
public:
    //使用x,y表示一个点
    int x, y;

public:
    /**
     * 初始化一个二维点
     * @param x x坐标
     * @param y y坐标
     */
    explicit Point(int x = 0, int y = 0) : x(x), y(y) {

    }

    int getHeuristic(const Point &point) const{
        auto v1 = *this - point;
        return std::abs(v1.x)+std::abs(v1.y);
    }

    virtual ~Point() = default;

    bool operator==(const Point &point) const {
        return x == point.x && y == point.y;
    }

    bool operator<(const Point &point) const{
        return this->to_string() < point.to_string();
    }

    bool operator!=(const Point &point) const {
        return !(*this == point);
    }

    Point operator+(const Point &point) const {
        return Point(x + point.x, y + point.y);
    }

    Point operator-(const Point &point) const {
        return Point(x - point.x, y - point.y);
    }

    Point operator*(int s) const {
        return Point(x * s, y * s);
    }

    std::string to_string() const {
        std::stringstream ss;
        ss << "(" << x << ", " << y << ")";
        return ss.str();
    }

    static Point generateRandomPoint(const Point &tl, const Point &br){
        Point delta = br - tl;
        int dx = rand() % (delta.x + 1);
        int dy = rand() % (delta.y + 1);
        return tl + Point(dx,dy);
    }
};

/**
 * 重载<<运算符，用于显示坐标点数据
 * @param out
 * @param point
 * @return
 */
std::ostream &operator<<(std::ostream &out, const Point &point) {
    return out<<point.to_string();
}

//向量和二维坐标点等价
using Vector = Point;
