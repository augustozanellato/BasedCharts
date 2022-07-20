#pragma once

#include "utils/vector2d.h"

#include <QAbstractTableModel>
#include <QString>

namespace BasedCharts {
namespace Models {

class ChartDataModel : public QAbstractTableModel {
    Q_OBJECT
public:
    ChartDataModel(QObject* parent = nullptr, int rows = 3, int columns = 4);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    void setRowCount(int newCount);

    void setColumnCount(int newCount);

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    void setMinimumColumnCount(int minimumColumnCount);

    int getMinimumColumnCount() const;

    void setProtectedColumnCount(int protectedColumnCount);

    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;

    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex& index,
                 const QVariant& value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    bool insertRows(int row,
                    int count,
                    const QModelIndex& parent = QModelIndex()) override;

    bool insertColumns(int column,
                       int count,
                       const QModelIndex& parent = QModelIndex()) override;

    bool removeRows(int row,
                    int count,
                    const QModelIndex& parent = QModelIndex()) override;

    bool removeColumns(int column,
                       int count,
                       const QModelIndex& parent = QModelIndex()) override;

    bool setHeaderData(int section,
                       Qt::Orientation orientation,
                       const QVariant& value,
                       int role = Qt::EditRole) override;

    void setColumnColor(int column, const QColor& color);
    void clearColumnColor(int column);
    QColor getColumnColor(int column) const;

    QStringList getRowHeaders() const;

    void serialize(QJsonObject& json) const;
    static ChartDataModel* deserialize(QObject* parent,
                                       const QJsonObject& json);

private:
    int minimumColumns = 1;
    int protectedColumns = 0;
    Utils::Vector2D<float> gridData;
    QVector<QString> rowHeaders;
    QVector<QString> columnHeaders;
    QVector<QVariant> columnColors;
};
}  // namespace Models
}  // namespace BasedCharts