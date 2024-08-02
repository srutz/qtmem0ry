#include "keyvaluetablemodel.h"

// Set up the model data (key/value pairs)
void KeyValueTableModel::setData(const QVector<QPair<QString, QString>> &data)
{
    beginResetModel();
    m_data = data;
    endResetModel();
}

int KeyValueTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.size();
}

int KeyValueTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2; // We have two columns: Key and Value
}

QVariant KeyValueTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    const auto &pair = m_data.at(index.row());
    if (index.column() == 0)
        return pair.first; // Key
    else if (index.column() == 1)
        return pair.second; // Value

    return QVariant();
}

QVariant KeyValueTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if (section == 0)
            return QStringLiteral("Key");
        else if (section == 1)
            return QStringLiteral("Value");
    }
    return QVariant();
}
