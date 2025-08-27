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

private:
    QListWidget* spellList_listWidget;
    QListWidget* profile_listWidget;

    QLineEdit* profile_lineEdit;

    void addSpell(const QString& option, const QString& spell, int minHp, int minMp, int count, const QString& targetName, int hpFrom, int hpTo, int dist) const;
    void saveProfile() const;
    void loadProfile() const;
    void spellList();
    void profileList();

    SpellThread* spellThread = nullptr;
};

#endif //SPELL_TAB_H
