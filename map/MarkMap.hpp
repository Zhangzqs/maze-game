#pragma once

#include <iostream>
#include <vector>
#include "../util/Point.hpp"
#include "Map.hpp"
#include <fstream>

class MarkMap : public Map<bool> {

public:
    /**
     * 初始化一个标记地图
     * @param rows 行数
     * @param cols 列数
     */
    explicit MarkMap(int rows, int cols) : Map<bool>(rows, cols) {
        reset();
    }

    /**
     * 标记
     * @param row 行号
     * @param col 列号
     */
    void mark(int row, int col) {
        set(row, col, true);
    }

    /**
     * 对指定坐标点标记标记
     * @param point 指定坐标点
     */
    void mark(const Point &point) {
        mark(point.y, point.x);
    }

    /**
     * 清除标记
     * @param row 行号
     * @param col 列号
     */
    void clear(int row, int col) {
        set(row, col, false);
    }

    void clear(const Point &point) {
        clear(point.y, point.x);
    }

    /**
     * 判断是否存在标记
     * @param row 行号
     * @param col 列号
     * @return 是否存在标记
     */
    virtual bool hasMark(int row, int col) {
        return get(row, col);
    }

    bool hasMark(const Point &point) {
        return hasMark(point.y, point.x);
    }

    /**
     * 清空场景中的一切标记
     */
    void reset(bool useMark = false) {
        fill(useMark);
    }

    /**
     * 以二维点阵方式输出整个标记地图
     */
    void printMark() {
        for (int row = 0; row < rows(); ++row) {
            for (int col = 0; col < columns(); ++col) {
                if (hasMark(row, col)) {
                    //若存在标记
                    std::cout << "██";
                } else {
                    std::cout << "  ";
                }
            }
            std::cout << std::endl;
        }
    }

    /**
     * 保存地图
     */
    void save(const std::string &fileName) {
        std::ofstream outFile(fileName);
        outFile << rows() << std::endl;
        outFile << columns() << std::endl;
        for (int row = 0; row < rows(); row++) {
            for (int col = 0; col < columns(); col++) {
                outFile << hasMark(row, col) << ' ';
            }
        }
        outFile.close();
    }

    static MarkMap load(const std::string &fileName) {
        std::ifstream inFile(fileName);
        int rows = 0;
        int columns = 0;
        inFile >> rows >> columns;
        MarkMap markMap(rows, columns);

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < columns; col++) {
                bool s;
                inFile >> s;
                if (s) {
                    markMap.mark(row, col);
                }
            }
        }
        return markMap;
    }
};

