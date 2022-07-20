#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>

namespace BasedCharts {
namespace Utils {

template <typename T>
class Vector2D {
private:  // isn't needed but it's made explicit to make unit testing of private
          // members work
    size_t rows, cols;
    T* data;

    inline constexpr static size_t map2dIdxToFlatIdx(size_t rIdx,
                                                     size_t cIdx,
                                                     size_t rows,
                                                     size_t cols);

    void resizeInsert(size_t insertRowIndex,
                      size_t insertRowCount,
                      size_t insertColIndex,
                      size_t insertColCount);  // O(n)

    void resizeDrop(size_t dropRowIndex,
                    size_t dropRowCount,
                    size_t dropColIndex,
                    size_t dropColCount);  // O(n)

public:
    using element = T;
    using reference = element&;
    using const_reference = const element&;

    Vector2D(Vector2D&) = delete;

    Vector2D(size_t rows, size_t cols);

    ~Vector2D();

    Vector2D<T>& operator=(const Vector2D<T>& other);  // O(n)
    Vector2D<T>& operator=(Vector2D<T>&& other);       // O(n)

    reference operator()(size_t rIdx, size_t cIdx);  // O(1)

    const_reference operator()(size_t rIdx, size_t cIdx) const;  // O(1)

    void resize(size_t newRows, size_t newCols);  // O(n)

    void addLastRow();  // O(n)

    void addLastCol();  // O(n)

    void addRows(size_t idx, size_t count);  // O(n)

    void addCols(size_t idx, size_t count);  // O(n)

    void removeRows(size_t idx, size_t count);  // O(n)

    void removeCols(size_t idx, size_t count);  // O(n)

    size_t rowCount() const;  // O(1)

    size_t colCount() const;  // O(1)

    bool operator==(const Vector2D<T>& other);  // O(n)
};

template <typename T>
inline constexpr size_t Vector2D<T>::map2dIdxToFlatIdx(size_t rIdx,
                                                       size_t cIdx,
                                                       __attribute__((unused))
                                                       size_t rows,
                                                       size_t cols) {
    assert(rIdx < rows);
    assert(cIdx < cols);
    return rIdx * cols + cIdx;
}

template <typename T>
Vector2D<T>::Vector2D(size_t rows, size_t cols)
    : rows(rows), cols(cols), data(new T[rows * cols]()) {}

template <typename T>
Vector2D<T>::~Vector2D() {
    delete[] data;
}

template <typename T>
Vector2D<T>& Vector2D<T>::operator=(const Vector2D<T>& other) {
    if (this != &other) {
        delete[] data;
        rows = other.rows;
        cols = other.cols;
        data = new T[rows * cols]();
        for (size_t i = 0; i < rows * cols; i++) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

template <typename T>
Vector2D<T>& Vector2D<T>::operator=(Vector2D<T>&& other) {
    std::swap(this->data, other.data);
    rows = other.rows;
    cols = other.cols;
    return *this;
}

template <typename T>
typename Vector2D<T>::reference Vector2D<T>::operator()(size_t rIdx,
                                                        size_t cIdx) {
    return data[map2dIdxToFlatIdx(rIdx, cIdx, rows, cols)];
}

template <typename T>
typename Vector2D<T>::const_reference Vector2D<T>::operator()(
    size_t rIdx,
    size_t cIdx) const {
    return data[map2dIdxToFlatIdx(rIdx, cIdx, rows, cols)];
}

template <typename T>
void Vector2D<T>::resize(size_t newRows, size_t newCols) {
    if (rows == newRows && cols == newCols) {
        return;
    }
    T* newData = new T[newRows * newCols]();

    for (size_t rIdx = 0; rIdx < std::min(rows, newRows); rIdx++) {
        for (size_t cIdx = 0; cIdx < std::min(cols, newCols); cIdx++) {
            newData[map2dIdxToFlatIdx(rIdx, cIdx, newRows, newCols)] =
                data[map2dIdxToFlatIdx(rIdx, cIdx, rows, cols)];
        }
    }
    rows = newRows;
    cols = newCols;
    delete[] data;
    data = newData;
}

template <typename T>
void Vector2D<T>::resizeInsert(size_t insertRowIndex,
                               size_t insertRowCount,
                               size_t insertColIndex,
                               size_t insertColCount) {
    const size_t newRows = rows + insertRowCount;
    const size_t newCols = cols + insertColCount;
    T* newData = new T[newRows * newCols]();

    for (size_t rIdx = 0; rIdx < rows; rIdx++) {
        for (size_t cIdx = 0; cIdx < cols; cIdx++) {
            newData[map2dIdxToFlatIdx(
                rIdx >= insertRowIndex ? rIdx + insertRowCount : rIdx,
                cIdx >= insertColIndex ? cIdx + insertColCount : cIdx, newRows,
                newCols)] = data[map2dIdxToFlatIdx(rIdx, cIdx, rows, cols)];
        }
    }
    rows = newRows;
    cols = newCols;
    delete[] data;
    data = newData;
}

template <typename T>
void Vector2D<T>::resizeDrop(size_t dropRowIndex,
                             size_t dropRowCount,
                             size_t dropColIndex,
                             size_t dropColCount) {
    assert(dropRowCount < rows);
    assert(dropColCount < cols);
    assert(dropRowIndex + dropRowCount <= rows);
    assert(dropColIndex + dropColCount <= cols);
    const size_t newRows = rows - dropRowCount;
    const size_t newCols = cols - dropColCount;
    T* newData = new T[newRows * newCols]();

    for (size_t rIdx = 0; rIdx < newRows; rIdx++) {
        for (size_t cIdx = 0; cIdx < newCols; cIdx++) {
            newData[map2dIdxToFlatIdx(rIdx, cIdx, newRows, newCols)] =
                data[map2dIdxToFlatIdx(
                    rIdx >= dropRowIndex ? rIdx + dropRowCount : rIdx,
                    cIdx >= dropColIndex ? cIdx + dropColCount : cIdx, rows,
                    cols)];
        }
    }
    rows = newRows;
    cols = newCols;
    delete[] data;
    data = newData;
}

template <typename T>
void Vector2D<T>::addLastRow() {
    addRows(rows, 1);
}

template <typename T>
void Vector2D<T>::addLastCol() {
    addCols(cols, 1);
}

template <typename T>
void Vector2D<T>::addRows(size_t idx, size_t count) {
    if (idx == rows) {
        resize(rows + count, cols);
    } else {
        resizeInsert(idx, count, 0, 0);
    }
}

template <typename T>
void Vector2D<T>::addCols(size_t idx, size_t count) {
    if (idx == cols) {
        resize(rows, cols + count);
    } else {
        resizeInsert(0, 0, idx, count);
    }
}

template <typename T>
void Vector2D<T>::removeRows(size_t idx, size_t count) {
    if (idx + count == rows) {
        resize(rows - count, cols);
    } else {
        resizeDrop(idx, count, 0, 0);
    }
}

template <typename T>
void Vector2D<T>::removeCols(size_t idx, size_t count) {
    if (idx + count == cols) {
        resize(rows, cols - count);
    } else {
        resizeDrop(0, 0, idx, count);
    }
}

template <typename T>
size_t Vector2D<T>::rowCount() const {
    return rows;
}

template <typename T>
size_t Vector2D<T>::colCount() const {
    return cols;
}
template <typename T>
bool Vector2D<T>::operator==(const Vector2D<T>& other) {
    if (this == &other) {
        return true;
    }
    if (rows != other.rows || cols != other.cols) {
        return false;
    }
    auto totalItems = rows * cols;
    for (int i = 0; i < totalItems; i++) {
        if (data[i] != other.data[i]) {
            return false;
        }
    }
    return true;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Vector2D<T>& v) {
    for (size_t r = 0; r < v.rowCount(); r++) {
        for (size_t c = 0; c < v.colCount(); c++) {
            out << '\t' << v(r, c);
        }
        out << std::endl;
    }
    return out;
}

}  // namespace Utils
}  // namespace BasedCharts
