#pragma once

#include <string>
#include <functional>
#include <utility>
#include <vector>

#include "Scanner.hpp"
#include <iostream>
#include "Scanner.hpp"

namespace zzq{
    class MenuItem;
    class Menu;
}

class zzq::MenuItem{
public:
    using CallbackType = std::function<void()>;
private:
    std::string content;    //菜单项文本
    CallbackType callback;  //菜单回调函数

public:
    explicit MenuItem(std::string content, MenuItem::CallbackType callback):content(std::move(content)),callback(std::move(callback)) {

    }
    virtual ~MenuItem() = default;

    void setContent(std::string content_) {
        this->content = std::move(content_);
    }

    std::string getContent() {
        return content;
    }

    void setCallback(MenuItem::CallbackType callback_) {
        this->callback = std::move(callback_);
    }

    CallbackType getCallback() {
        return callback;
    }
};

class zzq::Menu {
private:
    std::vector<MenuItem*> items;
    bool showNext{};
    std::string title;

public:
    explicit Menu(std::string title) : title(std::move(title)) {
        showNext = true;
    }

    void setTitle(std::string title_) {
        title = std::move(title_);
    }

    virtual ~Menu() {
        for (auto &it:items) {
            delete it;
        }
    }

    void addItem(MenuItem *item) {
        items.push_back(item);
    }

    void addItem(std::string content,MenuItem::CallbackType callback){
        addItem(new MenuItem(std::move(content),std::move(callback)));
    }

    void addExitItem(const std::string &content = "退出") {
        addItem(new MenuItem(content, [this]() {
            this->showNext = false;
        }));
    }

    //结束菜单
    void exitMenu(){
        this->showNext = false;
    }

    void show() {
        std::cout << std::endl << title << std::endl;
        int index = 1;
        for (auto &item:items) {
            std::cout << index << ". " << item->getContent() << std::endl;
            index++;
        }

        int in = Scanner::inputInt("请选择列表项，按Enter键确定：");

        if (in > 0 && in <= items.size()) {
            //如果用户输入的合法
            items[in - 1]->getCallback()();
        } else {
            std::cerr << "下标输入的不合法！！！";
        }
    }
    void loop(){
        do {
            show();
        } while (showNext);
    }
};
