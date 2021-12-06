#pragma once

#include <opencv2/opencv.hpp>

struct Color {
    int r, g, b;

    /**
     * 转换为opencv表示的颜色
     * @return opencv的bgr颜色
     */
    cv::Scalar to_cv_bgr() const {
        return {double(b), double(g), double(r)};
    }
};