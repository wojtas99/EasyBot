#ifndef SPELL_TAB_H
#define SPELL_TAB_H
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QListWidgetItem>
#include "spell_thread.h"

class SpellTab : public QWidget {
    Q_OBJECT

public:
    explicit SpellTab(QWidget* parent = nullptr);
public slots:
    void setSpellEnabled(bool on);
    signals:
    bool saveProfileSignal(const QString& tab, const QString& profileName, QList<QVariantMap> m_items);
    QList<QVariantMap> loadProfileSignal(const QString& tab, const QString& profileName);

private:
    QListWidget* spellList_listWidget;
    QListWidget* profile_listWidget;

    void spellList();
    void profileList();

    void loadProfile(const QString& profileName);
    void saveProfile(const QString& profileName);

    void addSpell(const QString& option, const QString& spell, int minHp, int minMp, int count, const QString& targetName, int hpFrom, int hpTo, int dist) const;

    SpellThread* spellThread = nullptr;
};

#endif //SPELL_TAB_H
