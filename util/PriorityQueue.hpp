//
// Created by zzq on 4/7/21.
//

#pragma once

#include <iostream>
#include <list>
#include <tuple>
#include <queue>

/**
 * 优先队列，可以自动排序
 * 默认取出元素值均为最小值
 * @tparam T
 */
template<class T>
class PriorityQueue {
private:
    struct Element {
        T element;
        int priority;

        bool operator<(const Element &a) const{
            return priority > a.priority;
        }
    };

    std::priority_queue<Element> _pq;

public:
    void push(T t, int priority) {
        _pq.push(Element{
                t, priority
        });
    }

    T pop() {
        auto ret = _pq.top();
        _pq.pop();
        return ret.element;
    }

    bool empty() {
        return _pq.empty();
    }
};