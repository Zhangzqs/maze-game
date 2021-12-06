#pragma once

#include <string>
#include <iostream>

namespace zzq::Scanner {

    //删除首尾空格字符
    std::string trim(std::string s) {
        if (!s.empty()) {
            s.erase(0, s.find_first_not_of(' '));
            s.erase(s.find_last_not_of(' ') + 1);
        }
        return s;
    }

    std::string readline(bool trim_ = true) {
        std::string s;
        std::getline(std::cin, s);
        if (trim_) {  //如果trim_为true，则会清空首尾的空格
            s = trim(s);
        }
        return s;
    }

    //从键盘上读取一个字符串并清除首尾空格
    std::string input(const std::string &text, bool allowSpace = true) {
        std::cout << text;
        std::string in;
        do {
            in = Scanner::readline();
        } while (in.empty() && !allowSpace);    //如果不允许为空但是却为空，那么重新输入
        return in;
    }


    //从键盘上读取一个整形数字,如果输入有误则返回常量INT32_MAX
    int inputInt(const std::string &text) {
        std::string src = input(text, false);    //不允许为空

        try {
            int in = std::stoi(src);
            return in;
        } catch (std::exception &e) {
            std::cerr << "不合法的输入" << std::endl;
        }
        return INT32_MAX;
    }
};
