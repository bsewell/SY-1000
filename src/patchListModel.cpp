#include "patchListModel.h"

PatchListModel *PatchListModel::s_instance = nullptr;

PatchListModel* PatchListModel::Instance()
{
    if (!s_instance)
        s_instance = new PatchListModel();
    return s_instance;
}

PatchListModel::PatchListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int PatchListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return currentItems().size();
}

QVariant PatchListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= currentItems().size())
        return QVariant();

    const QString &text = currentItems().at(index.row());

    switch (role) {
    case DisplayTextRole:
    case Qt::DisplayRole:
        return text;
    case BankLabelRole: {
        // Extract "U01:1" or "P01:1" prefix (first space-delimited token)
        int sp = text.indexOf(' ');
        return sp > 0 ? text.left(sp) : text;
    }
    case PatchNameRole: {
        int sp = text.indexOf(' ');
        return sp > 0 ? text.mid(sp + 1) : QString();
    }
    case BankRole: {
        int bank = 0, patch = 0;
        parseBankPatch(text, bank, patch);
        return bank;
    }
    case PatchRole: {
        int bank = 0, patch = 0;
        parseBankPatch(text, bank, patch);
        return patch;
    }
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> PatchListModel::roleNames() const
{
    return {
        { DisplayTextRole, "displayText" },
        { BankRole, "bank" },
        { PatchRole, "patch" },
        { BankLabelRole, "bankLabel" },
        { PatchNameRole, "patchName" }
    };
}

void PatchListModel::setSelectedIndex(int idx)
{
    if (m_selectedIndex != idx) {
        m_selectedIndex = idx;
        emit selectedIndexChanged();
    }
}

void PatchListModel::setPresetMode(bool preset)
{
    if (m_presetMode != preset) {
        beginResetModel();
        m_presetMode = preset;
        endResetModel();
        emit presetModeChanged();
    }
}

void PatchListModel::setUserItems(const QStringList &items)
{
    if (!m_presetMode) {
        beginResetModel();
        m_userItems = items;
        endResetModel();
    } else {
        m_userItems = items;
    }
}

void PatchListModel::setPresetItems(const QStringList &items)
{
    if (m_presetMode) {
        beginResetModel();
        m_presetItems = items;
        endResetModel();
    } else {
        m_presetItems = items;
    }
}

void PatchListModel::updateUserItem(int index, const QString &text)
{
    if (index >= 0 && index < m_userItems.size()) {
        m_userItems[index] = text;
        if (!m_presetMode) {
            QModelIndex mi = createIndex(index, 0);
            emit dataChanged(mi, mi);
        }
    }
}

void PatchListModel::itemClicked(int index)
{
    if (index < 0 || index >= currentItems().size())
        return;

    const QString &text = currentItems().at(index);
    int bank = 0, patch = 0;
    parseBankPatch(text, bank, patch);
    emit patchClicked(bank, patch);
}

const QStringList& PatchListModel::currentItems() const
{
    return m_presetMode ? m_presetItems : m_userItems;
}

void PatchListModel::parseBankPatch(const QString &text, int &bank, int &patch) const
{
    // Format: "U01:1 name" or "P01:1 name"
    // Bank: digits after U/P, before colon
    // Patch: digit after colon
    if (text.size() < 5) return;

    bool isPreset = text.startsWith('P');
    int colonPos = text.indexOf(':');
    if (colonPos < 2) return;

    bool ok;
    int bankNum = text.mid(1, colonPos - 1).toInt(&ok);
    if (!ok) return;

    int spacePos = text.indexOf(' ', colonPos);
    int patchNum = text.mid(colonPos + 1, spacePos > colonPos ? spacePos - colonPos - 1 : -1).toInt(&ok);
    if (!ok) return;

    bank = isPreset ? bankNum + 50 : bankNum;
    patch = patchNum;
}
