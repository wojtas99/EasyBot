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

private:
    QListWidget* lootList_listWidget;

    QListWidget *profile_listWidget;
    QLineEdit *profile_lineEdit;

    QLabel *last_looked_item_id;


    void addItem(int item_id, const QString& container_name, const QString& item_name) const;


    void clearItemList() const;
    void saveProfile() const;
    void loadProfile() const;


    void lootList();
    void profileList();

    LootThread* lootThread = nullptr;

};




#endif
