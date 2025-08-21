#ifndef HEAL_TAB_H
#define HEAL_TAB_H
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QListWidgetItem>
#include "heal_thread.h"

class HealTab : public QWidget {
    Q_OBJECT

public:
    explicit HealTab(QWidget* parent = nullptr);
public slots:
    void setHealEnabled(bool on);

private:
    QListWidget* healList_listWidget;
    QListWidget* profile_listWidget;

    QLineEdit* profile_lineEdit;

    void addHeal(const QString& option, const QString& heal, int hpBelow, int hpAbove, int minMp, const QString& contidion) const;
    void clearHealList() const;
    void saveProfile() const;
    void loadProfile() const;
    void healList();
    void profileList();

    HealThread* healThread = nullptr;
};




#endif

