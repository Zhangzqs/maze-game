template<class T>
class Map {
private:
    std::vector<T> _data{}; //标记数据
    int _rows;   //地图行数
    int _cols;   //地图列数

public:
    /**
     * 初始化一个地图
     * @param rows 行数
     * @param cols 列数
     */
    explicit Map(int rows, int cols) : _rows(rows), _cols(cols) {
        _data.resize(rows*cols);
    }

    void fill(T val) {
        fill(val, Point(0, 0), Point(_cols - 1, _rows - 1));
    }

    void fill(T val, const Point &tl, const Point &br) {
        for (int row = tl.y; row <= br.y; row++) {
            for (int col = tl.x; col <= br.x; col++) {
                set(row, col, val);
            }
        }
    }

    void set(int row, int col, T val) {
        this->_data[row * _cols + col] = val;
    }

    void set(const Point &point, T val) {
        set(point.y, point.x,val);
    }

    T get(int row, int col) {
        return this->_data[row * _cols + col];
    }

    T get(const Point &point) {
        return get(point.y, point.x);
    }

    /**
     * 获取行数
     * @return 行数
     */
    int rows() const {
        return _rows;
    }

    /**
     * 获取列数
     * @return 列数
     */
    int columns() const {
        return _cols;
    }
};