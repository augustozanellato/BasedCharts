#include <QtTest>
// NOLINTNEXTLINE
#define private public
// NOLINTNEXTLINE
#define protected public

#include "utils/vector2d.h"

#undef private
#undef protected

using BasedCharts::Utils::Vector2D;

class Vector2DTest : public QObject {
    Q_OBJECT
public:
    virtual ~Vector2DTest() {}

private:
    void fill(Vector2D<int>& v) {
        for (size_t r = 0; r < v.rowCount(); r++) {
            for (size_t c = 0; c < v.colCount(); c++) {
                v(r, c) = (r + 1) * 10 + (c + 1);  // NOLINT
            }
        }
    }

    void checkFill(Vector2D<int>& v, size_t startRow = 0, size_t startCol = 0) {
        for (size_t r = startRow; r < v.rowCount(); r++) {
            for (size_t c = startCol; c < v.colCount(); c++) {
                QCOMPARE(v(r, c), (r + 1 - startRow) * 10 + (c + 1 - startCol));
            }
        }
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantParameter"

    void checkRow(Vector2D<int>& v, size_t r, int value) {
#pragma clang diagnostic pop
        for (size_t c = 0; c < v.colCount(); c++) {
            QCOMPARE(v(r, c), value);
        }
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantParameter"

    void checkCol(Vector2D<int>& v, size_t c, int value) {
#pragma clang diagnostic pop
        for (size_t r = 0; r < v.rowCount(); r++) {
            QCOMPARE(v(r, c), value);
        }
    }

    void checkSize(Vector2D<int>& v, size_t r, size_t c) {
        QCOMPARE(v.rowCount(), r);
        QCOMPARE(v.colCount(), c);
    }

private slots:

    __attribute__((unused)) void rows() {
        Vector2D<int> v12(1, 2);
        Vector2D<int> v21(2, 1);
        QCOMPARE(v12.rowCount(), 1);
        QCOMPARE(v21.rowCount(), 2);
    }

    __attribute__((unused)) void cols() {
        Vector2D<int> v12(1, 2);
        Vector2D<int> v21(2, 1);
        QCOMPARE(v12.colCount(), 2);
        QCOMPARE(v21.colCount(), 1);
    }

    __attribute__((unused)) void setterGetter() {
        Vector2D<int> v(1, 2);
        fill(v);
        checkFill(v);
    }

    __attribute__((unused)) void expand() {
        Vector2D<int> v(1, 1);
        fill(v);
        v.resize(2, 2);
        checkSize(v, 2, 2);
        QCOMPARE(v(0, 0), 11);
        QCOMPARE(v(0, 1), 0);
        QCOMPARE(v(1, 0), 0);
        QCOMPARE(v(1, 1), 0);
        fill(v);
        checkFill(v);
    }

    __attribute__((unused)) void reduce() {
        Vector2D<int> v(3, 3);
        fill(v);
        v.resize(2, 1);
        checkSize(v, 2, 1);
        checkFill(v);
    }

    __attribute__((unused)) void skipAllocateOnFakeResize() {
        Vector2D<int> v(3, 3);
        int* oldPtr = v.data;
        v.resize(v.rowCount(), v.colCount());
        checkSize(v, 3, 3);
        QCOMPARE(v.data, oldPtr);
    }

    __attribute__((unused)) void addFirstRows() {
        Vector2D<int> v(2, 2);
        fill(v);
        v.addRows(0, 2);
        checkSize(v, 4, 2);
        checkRow(v, 0, 0);
        checkRow(v, 1, 0);
        checkFill(v, 2, 0);
    }

    __attribute__((unused)) void addMiddleRows() {
        Vector2D<int> v(2, 2);
        fill(v);
        v.addRows(1, 2);
        checkSize(v, 4, 2);
        checkRow(v, 1, 0);
        checkRow(v, 2, 0);
        QCOMPARE(v(0, 0), 11);
        QCOMPARE(v(0, 1), 12);
        QCOMPARE(v(3, 0), 21);
        QCOMPARE(v(3, 1), 22);
    }

    __attribute__((unused)) void addLastRows() {
        Vector2D<int> v(2, 2);
        fill(v);
        v.addLastRow();
        checkSize(v, 3, 2);
        checkRow(v, 2, 0);
        v.addRows(v.rowCount(), 2);
        checkSize(v, 5, 2);
        checkRow(v, 3, 0);
        checkRow(v, 4, 0);
    }

    __attribute__((unused)) void addFirstCols() {
        Vector2D<int> v(2, 2);
        fill(v);
        v.addCols(0, 2);
        checkSize(v, 2, 4);
        checkCol(v, 0, 0);
        checkCol(v, 1, 0);
        checkFill(v, 0, 2);
    }

    __attribute__((unused)) void addMiddleCols() {
        Vector2D<int> v(2, 2);
        fill(v);
        v.addCols(1, 2);
        checkSize(v, 2, 4);
        checkCol(v, 1, 0);
        checkCol(v, 2, 0);
        QCOMPARE(v(0, 0), 11);
        QCOMPARE(v(1, 0), 21);
        QCOMPARE(v(0, 3), 12);
        QCOMPARE(v(1, 3), 22);
    }

    __attribute__((unused)) void addLastCols() {
        Vector2D<int> v(2, 2);
        fill(v);
        v.addLastCol();
        checkSize(v, 2, 3);
        checkCol(v, 2, 0);
        v.addCols(v.colCount(), 2);
        checkSize(v, 2, 5);
        checkCol(v, 3, 0);
        checkCol(v, 4, 0);
    }

    __attribute__((unused)) void removeFirstRows() {
        Vector2D<int> v(4, 4);
        fill(v);
        v.removeRows(0, 2);
        checkSize(v, 2, 4);
        QCOMPARE(v(0, 0), 31);
        QCOMPARE(v(0, 3), 34);
        QCOMPARE(v(1, 0), 41);
        QCOMPARE(v(1, 3), 44);
    }

    __attribute__((unused)) void removeLastRows() {
        Vector2D<int> v(4, 4);
        fill(v);
        v.removeRows(v.rowCount() - 2, 2);
        checkSize(v, 2, 4);
        checkFill(v);
    }

    __attribute__((unused)) void removeFirstCols() {
        Vector2D<int> v(4, 4);
        fill(v);
        v.removeCols(0, 2);
        checkSize(v, 4, 2);
        QCOMPARE(v(0, 0), 13);
        QCOMPARE(v(3, 0), 43);
        QCOMPARE(v(0, 1), 14);
        QCOMPARE(v(3, 1), 44);
    }

    __attribute__((unused)) void removeLastCols() {
        Vector2D<int> v(4, 4);
        fill(v);
        v.removeCols(v.colCount() - 2, 2);
        checkSize(v, 4, 2);
        checkFill(v);
    }
};

#include "vector2d_test.moc"

QTEST_MAIN(Vector2DTest)