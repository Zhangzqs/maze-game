#pragma once

#include "MarkMap.hpp"
#include <list>

class MultiMarkMap:public MarkMap{
private:
    std::list<MarkMap*> maps{};

public:


    explicit MultiMarkMap(int rows, int cols): MarkMap(rows,cols){

    }

    void addMarkMap(MarkMap* markMap){
        if(markMap->rows() == rows() && markMap->columns() == columns()){
            maps.push_back(markMap);
        }else{
            throw std::runtime_error("地图宽高不一致");
        }
    }

    /**
     * 判断是否存在标记
     * @param row 行号
     * @param col 列号
     * @return 是否存在标记
     */
    virtual bool hasMark(int row, int col) override{
        for (const auto &map : maps){
            if(map->hasMark(row,col)){
                return true;
            }
        }
        return false;
    }

};