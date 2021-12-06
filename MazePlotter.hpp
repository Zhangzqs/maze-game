#pragma once

#include <opencv2/opencv.hpp>
#include <list>

#include "map/MarkMap.hpp"
#include "map/MapGenerator.hpp"
#include "route_planner/Path.hpp"
#include "Spirit.hpp"

class MazePlotter {
private:
    //画布
    cv::Mat &_image;

    //迷宫的行数，列数
    int _rows, _cols;
public:
    explicit MazePlotter(cv::Mat &image, int rows, int cols)
            : _image(image),
              _rows(rows),
              _cols(cols) {
    }


    /**
     * 使用某一颜色覆盖整块画布
     */
    void fillByColor(const Color &color) {
        //绘制填充的方格
        cv::rectangle(_image,
                      cv::Point(0, 0),
                      cv::Point(_image.cols, _image.rows),
                      color.to_cv_bgr(),
                      cv::FILLED,
                      cv::LINE_8);
    }

    /**
     * 使用某一图片覆盖整块画布
     */
    void fillByImage(const cv::Mat &image) {
        paste_image(cv::Rect(0, 0, _image.cols, _image.rows), image,255);
    }

    /**
     * 绘制一条由顶点链表构成的路径
     */
    void draw_path(const Path &path, const Color &color) {
        int sx = _image.cols / _cols;
        int sy = _image.rows / _rows;

        Point keyPoint = *path.pathPoint.begin();
        auto keyPointIt = path.pathPoint.begin();
        keyPointIt++;

        while (keyPointIt != path.pathPoint.end()) {
            Point nextKeyPoint = *keyPointIt;
            int startX = keyPoint.x * sx;
            int startY = keyPoint.y * sy;
            int endX = nextKeyPoint.x * sx;
            int endY = nextKeyPoint.y * sy;
            startX += sx / 2;
            startY += sy / 2;
            endX += sx / 2;
            endY += sy / 2;
            cv::line(_image,
                     cv::Point(startX, startY),
                     cv::Point(endX, endY),
                     color.to_cv_bgr());
            keyPoint = nextKeyPoint;
            keyPointIt++;
        }
    }


    /**
     * 绘制精灵
     * @param spirit 精灵
     */
    void draw_spirit(Spirit &spirit) {
        int sx = _image.cols / _cols;
        int sy = _image.rows / _rows;
        int lx = spirit.getPosition().x * sx;
        int ly = spirit.getPosition().y * sy;

        //获取当前精灵的贴图,准备复制到画布相应位置
        cv::Mat currentAsset = spirit.getCurrentAsset();

        //创建贴图区域
        cv::Rect roi_rect(lx, ly, sx, sy);

        //贴图
        //currentAsset.copyTo(_image(roi_rect));
        paste_image(roi_rect, currentAsset,230);

        //绘制边框
        cv::rectangle(_image,
                      roi_rect.tl(),
                      roi_rect.br(),
                      cv::Scalar(0, 0, 0),
                      1,
                      cv::LINE_8);
    }

    /**
     * 贴图
     * 将image缩放到area_rect的尺寸后，按照area_rect的位置贴到background图层上
     * @param area_rect 贴图区域
     * @param image 贴图
     */
    void paste_image(const cv::Rect &area_rect, const cv::Mat &image, uint8_t weight) {
        double weight_percent = double(weight) / 255;
        //缩放后的素材
        cv::Mat resizedAsset;
        cv::resize(image,
                   resizedAsset,
                   area_rect.size(),
                   0, 0,
                   cv::INTER_AREA);
        //cv::imshow("Hello",resizedAsset);
        //开始贴图
        for (int row = 0; row < resizedAsset.rows; row++) {
            for (int col = 0; col < resizedAsset.cols; col++) {
                int b_row = row + area_rect.tl().y;
                int b_col = col + area_rect.tl().x;
                for (int i = 0; i < 3; i++) {
                    _image.at<cv::Vec3b>(b_row, b_col)[i] = uint8_t(
                            (1.f - weight_percent) * _image.at<cv::Vec3b>(b_row, b_col)[i] +
                            weight_percent * resizedAsset.at<cv::Vec3b>(row, col)[i]);
                }

            }
        }
    }

    /**
     * 绘制标记地图
     * @param markMap 标记地图
     * @param icon 贴图
     */
    void draw_mark_map(MarkMap &markMap, const cv::Mat &icon) {
        //迷宫尺寸要求小于像素尺寸
        int sx = _image.cols / markMap.columns();
        int sy = _image.rows / markMap.rows();
        for (int row = 0; row < markMap.rows(); row++) {
            for (int col = 0; col < markMap.columns(); col++) {
                if (markMap.hasMark(row, col)) {
                    //若存在障碍物，那就渲染
                    int lx = col * sx;
                    int ly = row * sy;
                    int rx = lx + sx;
                    int ry = ly + sy;

                    paste_image(cv::Rect(lx, ly, sx, sy), icon,150);

                    //绘制边框
                    cv::rectangle(_image,
                                  cv::Point(lx, ly),
                                  cv::Point(rx, ry),
                                  cv::Scalar(0, 0, 0),
                                  1,
                                  cv::LINE_8);
                }
            }
        }
    }

    /**
     * 绘制标记地图
     * @param markMap 标记地图
     * @param color 颜色
     */
    void draw_mark_map(MarkMap &markMap, const Color &color) {
        //迷宫尺寸要求小于像素尺寸
        int sx = _image.cols / markMap.columns();
        int sy = _image.rows / markMap.rows();
        for (int row = 0; row < markMap.rows(); row++) {
            for (int col = 0; col < markMap.columns(); col++) {
                if (markMap.hasMark(row, col)) {
                    //若存在障碍物，那就渲染
                    int lx = col * sx;
                    int ly = row * sy;
                    int rx = lx + sx;
                    int ry = ly + sy;

                    //绘制填充的方格
                    cv::rectangle(_image,
                                  cv::Point(lx, ly),
                                  cv::Point(rx, ry),
                                  color.to_cv_bgr(),
                                  cv::FILLED,
                                  cv::LINE_8);

                    //绘制边框
                    cv::rectangle(_image,
                                  cv::Point(lx, ly),
                                  cv::Point(rx, ry),
                                  cv::Scalar(0, 0, 0),
                                  1,
                                  cv::LINE_8);
                }
            }
        }
    }
};
