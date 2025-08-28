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
    signals:
    bool saveProfileSignal(const QString& tab, const QString& profileName, QList<QVariantMap> m_items);
    QList<QVariantMap> loadProfileSignal(const QString& tab, const QString& profileName);

private:
    QListWidget* healList_listWidget;
    QListWidget* profile_listWidget;

    void healList();
    void profileList();

    void clearList() const;
    void loadProfile(const QString& profileName);
    void saveProfile(const QString& profileName);

    void addHeal(const QString& option, const QString& heal, int hpBelow, int hpAbove, int minMp, const QString& contidion) const;


    HealThread* healThread = nullptr;
};




#endif

