#include "chartdatamodel.h"
#include "utils/exceptions.h"

#include <QApplication>
#include <QBrush>
#include <QColor>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>

using BasedCharts::Models::ChartDataModel;
using BasedCharts::Utils::Exceptions::DeserializationException;

ChartDataModel::ChartDataModel(QObject* parent, int rows, int columns)
    : QAbstractTableModel(parent),
      gridData(rows, columns),
      rowHeaders(rows),
      columnHeaders(columns),
      columnColors(columns) {}

int ChartDataModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) {  // required as per Qt docs for
                             // QAbstractItemModel::rowCount
        return 0;
    }
    return gridData.rowCount();
}

int ChartDataModel::columnCount(const QModelIndex& parent) const {
    if (parent.isValid()) {  // required as per Qt docs for
                             // QAbstractItemModel::columnCount
        return 0;
    }
    return gridData.colCount();
}

QVariant ChartDataModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return gridData(index.row(), index.column());
    } else if (role == Qt::BackgroundRole) {
        auto variant = columnColors[index.column()];
        if (variant.canConvert<QColor>()) {
            auto color = variant.value<QColor>();
            color.setAlpha(96);
            return QBrush(color);
        }
    }
    return {};
}

bool ChartDataModel::setData(const QModelIndex& index,
                             const QVariant& value,
                             int role) {
    if (index.isValid() && role == Qt::EditRole) {
        gridData(index.row(), index.column()) = value.toFloat();
        emit dataChanged(index, index, {Qt::DisplayRole});
        return true;
    }
    return false;
}

Qt::ItemFlags ChartDataModel::flags(const QModelIndex& index) const {
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

bool ChartDataModel::insertRows(int row, int count, const QModelIndex& parent) {
    beginInsertRows(parent, row, row + count - 1);
    gridData.addRows(row, count);
    rowHeaders.insert(row, count, QString());
    endInsertRows();
    emit dataChanged(index(row, 0), index(row + count, columnCount()),
                     {Qt::DisplayRole});
    return true;
}

bool ChartDataModel::insertColumns(int column,
                                   int count,
                                   const QModelIndex& parent) {
    beginInsertColumns(parent, column, column + count - 1);
    gridData.addCols(column, count);
    columnHeaders.insert(column, count, QString());
    columnColors.insert(column, count, QColor());
    endInsertColumns();
    emit dataChanged(index(0, column), index(rowCount(), column + count),
                     {Qt::DisplayRole});
    return true;
}

bool ChartDataModel::removeRows(int row, int count, const QModelIndex& parent) {
    beginRemoveRows(parent, row, row + count - 1);
    gridData.removeRows(row, count);
    rowHeaders.remove(row, count);
    endRemoveRows();
    emit dataChanged(index(row, 0), index(row + count, columnCount()),
                     {Qt::DisplayRole});
    emit headerDataChanged(Qt::Vertical, row, rowCount() - 1);
    return true;
}

bool ChartDataModel::removeColumns(int column,
                                   int count,
                                   const QModelIndex& parent) {
    if (columnCount() - count < minimumColumns || column < protectedColumns) {
        // this message box should be created by the caller of the method, it's
        // created here to avoid duplicating error handling code in 3 different
        // places
        QMessageBox::critical(qApp->activeWindow(), "Error",
                              "Invalid removal attempted!");
        return false;
    }
    beginRemoveColumns(parent, column, column + count - 1);
    gridData.removeCols(column, count);
    columnHeaders.remove(column, count);
    columnColors.remove(column, count);
    endRemoveColumns();
    emit dataChanged(index(0, column), index(rowCount(), column + count),
                     {Qt::DisplayRole});
    emit headerDataChanged(Qt::Horizontal, column, columnCount() - 1);
    return true;
}
QVariant ChartDataModel::headerData(int section,
                                    Qt::Orientation orientation,
                                    int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal && !columnHeaders[section].isNull()) {
            return columnHeaders[section];
        } else if (orientation == Qt::Vertical &&
                   !rowHeaders[section].isNull()) {
            return rowHeaders[section];
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}
bool ChartDataModel::setHeaderData(int section,
                                   Qt::Orientation orientation,
                                   const QVariant& value,
                                   int role) {
    if (role != Qt::EditRole) {
        return false;
    }
    auto strValue = value.toString().trimmed();
    if (strValue ==
        QAbstractItemModel::headerData(section, orientation, role)) {
        strValue = QString();
    }

    if (orientation == Qt::Horizontal) {
        columnHeaders[section] = strValue;
    } else {
        rowHeaders[section] = strValue;
    }
    emit headerDataChanged(orientation, section, section);
    return true;
}

QStringList ChartDataModel::getRowHeaders() const {
    QStringList l;
    for (int i = 0; i < rowCount(); i++) {
        l << headerData(i, Qt::Vertical).toString();
    }
    return l;
}

QJsonArray stringVectorToJsonArray(const QVector<QString>& vector) {
    QJsonArray arr;
    for (const auto& item : vector) {
        if (item.isNull()) {
            arr.append(QJsonValue());
        } else {
            arr.append(item);
        }
    }
    return arr;
}

QJsonValue serializeColor(const QVariant& variant) {
    if (!variant.canConvert<QColor>()) {
        return {};
    }
    QJsonObject obj;
    const auto rgbaColor = variant.value<QColor>().toRgb();
    obj["r"] = rgbaColor.red();
    obj["g"] = rgbaColor.green();
    obj["b"] = rgbaColor.blue();
    obj["a"] = rgbaColor.alpha();
    return obj;
}

void ChartDataModel::serialize(QJsonObject& json) const {
    json["rows"] = stringVectorToJsonArray(rowHeaders);
    json["columns"] = stringVectorToJsonArray(columnHeaders);

    QJsonArray colors;
    for (const auto& item : columnColors) {
        colors.append(serializeColor(item));
    }
    json["colors"] = colors;

    QJsonArray data;
    for (int r = 0; r < rowCount(); r++) {
        QJsonArray rowArray;
        for (int c = 0; c < columnCount(); c++) {
            rowArray.append(gridData(r, c));
        }
        data.append(rowArray);
    }
    json["data"] = data;
}

bool isInColorRange(const QJsonValue& v) {
    if (!v.isDouble()) {
        return false;
    }
    auto intVal = v.toInt();
    return intVal >= 0 && intVal <= 255;
}

bool validateColor(const QJsonObject& obj) {
    return isInColorRange(obj["r"]) && isInColorRange(obj["g"]) &&
           isInColorRange(obj["b"]) && isInColorRange(obj["a"]);
}

QVariant deserializeColor(const QJsonValue& val) {
    if (val.isNull()) {
        return {};
    }
    auto obj = val.toObject();
    return QColor(obj["r"].toInt(), obj["g"].toInt(), obj["b"].toInt(),
                  obj["a"].toInt());
}

ChartDataModel* ChartDataModel::deserialize(QObject* parent,
                                            const QJsonObject& json) {
    if (!json["rows"].isArray() || !json["columns"].isArray() ||
        !json["data"].isArray() || !json["colors"].isArray()) {
        throw DeserializationException("Invalid data");
    }
    const QJsonArray rows = json["rows"].toArray();
    const QJsonArray columns = json["columns"].toArray();
    const QJsonArray data = json["data"].toArray();
    const QJsonArray colors = json["colors"].toArray();

    const int rowCount = rows.size();
    const int columnCount = columns.size();

    for (const auto& row : rows) {
        if (!row.isString() && !row.isNull()) {
            throw DeserializationException("Invalid row header");
        }
    }

    for (const auto& column : columns) {
        if (!column.isString() && !column.isNull()) {
            throw DeserializationException("Invalid column header");
        }
    }

    if (data.size() != rowCount) {
        throw DeserializationException("Row count != row header count");
    }

    if (columnCount != colors.count()) {
        throw DeserializationException("Column count != column color count");
    }

    for (const auto& row : data) {
        if (!row.isArray() || row.toArray().size() != columnCount) {
            throw DeserializationException("Invalid data row");
        }
        const auto rowArray = row.toArray();
        for (const auto& item : rowArray) {
            if (!item.isDouble()) {
                throw DeserializationException("Invalid data");
            }
        }
    }

    for (const auto& item : colors) {
        if (!item.isNull() &&
            (!(item.isObject()) || !validateColor(item.toObject()))) {
            throw DeserializationException("Invalid color");
        }
    }

    auto model = new ChartDataModel(parent, rowCount, columnCount);

    for (int r = 0; r < rowCount; r++) {
        if (!rows[r].isNull()) {
            model->rowHeaders[r] = rows[r].toString();
        }
    }

    for (int c = 0; c < columnCount; c++) {
        if (!columns[c].isNull()) {
            model->columnHeaders[c] = columns[c].toString();
        }
    }

    for (int r = 0; r < rowCount; r++) {
        const auto& row = data[r].toArray();
        for (int c = 0; c < columnCount; c++) {
            model->gridData(r, c) = row[c].toDouble();
        }
    }

    for (int i = 0; i < colors.size(); ++i) {
        model->columnColors[i] = deserializeColor(colors[i]);
    }

    return model;
}
void ChartDataModel::setColumnColor(int column, const QColor& color) {
    columnColors[column] = color;
    emit dataChanged(index(0, column), index(rowCount(), column),
                     {Qt::BackgroundRole});
}
QColor ChartDataModel::getColumnColor(int column) const {
    auto variant = columnColors[column];
    if (variant.canConvert<QColor>()) {
        return variant.value<QColor>();
    } else {
        return {};
    }
}
void ChartDataModel::clearColumnColor(int column) {
    columnColors[column] = {};
    emit dataChanged(index(0, column), index(rowCount(), column),
                     {Qt::BackgroundRole});
}
void ChartDataModel::setMinimumColumnCount(int minimumColumnCount) {
    minimumColumns = minimumColumnCount;
    if (columnCount() < minimumColumns) {
        insertColumns(columnCount(), minimumColumns - columnCount());
    }
}
void ChartDataModel::setProtectedColumnCount(int protectedColumnCount) {
    protectedColumns = protectedColumnCount;
    if (columnCount() < protectedColumns) {
        insertColumns(columnCount(), minimumColumns - columnCount());
    }
}
int ChartDataModel::getMinimumColumnCount() const {
    return minimumColumns;
}
void ChartDataModel::setRowCount(int newCount) {
    const int currentRowCount = rowCount();
    if (newCount == currentRowCount) {
        return;
    }
    if (newCount < currentRowCount) {
        const int removeCount = currentRowCount - newCount;
        removeRows(newCount, removeCount);
    } else {
        const int addCount = newCount - currentRowCount;
        insertRows(currentRowCount, addCount);
    }
}

void ChartDataModel::setColumnCount(int newCount) {
    const int currentColumnCount = columnCount();
    if (newCount == currentColumnCount) {
        return;
    }
    if (newCount < currentColumnCount) {
        const int removeCount = currentColumnCount - newCount;
        removeColumns(newCount, removeCount);
    } else {
        const int addCount = newCount - currentColumnCount;
        insertColumns(currentColumnCount, addCount);
    }
}
