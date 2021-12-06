#include "MazeGame.hpp"
#include "util/Scanner.hpp"

using namespace std;

const int DEFAULT_MAZE_ROW = 23;
const int DEFAULT_MAZE_COLUMN = 23;
const int DEFAULT_SCALE_FACTOR = 30;

int getRow() {
    cout << "是否自定义迷宫行数？(默认值：" << DEFAULT_MAZE_ROW << ")Y/[N]" << endl;
    char ch;
    cin >> ch;
    if (ch == 'y' || ch == 'Y') {
        //要自定义迷宫行数
        int row = zzq::Scanner::inputInt("请输入迷宫行数");
        return row;
    }
    return DEFAULT_MAZE_ROW;
}

int getCol() {
    cout << "是否自定义迷宫列数？(默认值：" << DEFAULT_MAZE_COLUMN << ")Y/[N]" << endl;
    char ch;
    cin >> ch;
    if (ch == 'y' || ch == 'Y') {
        //要自定义迷宫行数
        int row = zzq::Scanner::inputInt("请输入迷宫列数");
        return row;
    }
    return DEFAULT_MAZE_COLUMN;
}

int getScale() {
    cout << "是否自定义迷宫显示比例？(默认值：" << DEFAULT_SCALE_FACTOR << ")Y/[N]" << endl;
    char ch;
    cin >> ch;
    if (ch == 'y' || ch == 'Y') {
        //要自定义迷宫行数
        int row = zzq::Scanner::inputInt("请输入迷宫列数");
        return row;
    }
    return DEFAULT_SCALE_FACTOR;
}

int main() {
    srand(time(nullptr));

    zzq::Menu menu("欢迎来到西游迷宫小游戏");
    menu.addItem("开始游戏",[](){
        int row = getRow();
        int col = getCol();
        int scale = getScale();
        cout<<"Row:"<<row<<endl
            <<"Column:"<<col<<endl;
        MazeGame mg(row, col, scale); //初始化这个游戏
        mg.loop();  //进入游戏主循环
    });
    menu.addExitItem("我不想玩");
    menu.loop();    //状态机循环

    return 0;
}