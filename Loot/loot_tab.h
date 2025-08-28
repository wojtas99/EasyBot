#ifndef LOOT_TAB_H
#define LOOT_TAB_H
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QLabel>
#include <QListWidgetItem>

#include "loot_thread.h"
#include <QPointer>
class LootTab : public QWidget {
    Q_OBJECT

public:
    explicit LootTab(QWidget* parent = nullptr);

public slots:
    void setLootEnabled(bool on);
    signals:
    bool saveProfileSignal(const QString& tab, const QString& profileName, QList<QVariantMap> m_items);
    QList<QVariantMap> loadProfileSignal(const QString& tab, const QString& profileName);

private:
    QListWidget* lootList_listWidget;
    QListWidget *profile_listWidget;
    QLabel *last_looked_item_id;

    void lootList();
    void profileList();

    void clearList() const;
    void loadProfile(const QString& profileName);
    void saveProfile(const QString& profileName);

    void addItem(int item_id, const QString& container_name, const QString& item_name) const;

    LootThread* lootThread = nullptr;

};




#endif
