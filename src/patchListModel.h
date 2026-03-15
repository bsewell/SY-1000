#ifndef PATCHLISTMODEL_H
#define PATCHLISTMODEL_H

#include <QAbstractListModel>
#include <QStringList>

class PatchListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int selectedIndex READ selectedIndex WRITE setSelectedIndex NOTIFY selectedIndexChanged)
    Q_PROPERTY(bool presetMode READ presetMode WRITE setPresetMode NOTIFY presetModeChanged)

public:
    enum Roles {
        DisplayTextRole = Qt::UserRole + 1,
        BankRole,
        PatchRole,
        BankLabelRole,   // "U01:1" or "P01:1"
        PatchNameRole    // just the name part
    };

    static PatchListModel* Instance();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    int selectedIndex() const { return m_selectedIndex; }
    void setSelectedIndex(int idx);

    bool presetMode() const { return m_presetMode; }
    void setPresetMode(bool preset);

    // Called by bankTreeList to populate data
    void setUserItems(const QStringList &items);
    void setPresetItems(const QStringList &items);
    void updateUserItem(int index, const QString &text);

    Q_INVOKABLE void itemClicked(int index);

signals:
    void selectedIndexChanged();
    void presetModeChanged();
    void patchClicked(int bank, int patch);  // consumed by bankTreeList

private:
    explicit PatchListModel(QObject *parent = nullptr);
    static PatchListModel *s_instance;

    QStringList m_userItems;
    QStringList m_presetItems;
    int m_selectedIndex = -1;
    bool m_presetMode = false;

    const QStringList& currentItems() const;
    void parseBankPatch(const QString &text, int &bank, int &patch) const;
};

#endif // PATCHLISTMODEL_H
