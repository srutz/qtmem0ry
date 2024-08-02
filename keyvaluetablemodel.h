#ifndef KEYVALUETABLEMODEL_H
#define KEYVALUETABLEMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include <QVector>
#include <QPair>
#include <QModelIndex>

class KeyValueTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    KeyValueTableModel(QObject *parent = nullptr)
        : QAbstractTableModel(parent)
    {
    }

    void setData(const QVector<QPair<QString, QString> > &data);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    QVector<QPair<QString, QString>> m_data;
};

#endif // KEYVALUETABLEMODEL_H
