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

    //void addHeal(const QString& option, const QString& heal, int hpBelow, int hpAbove, int minMp, const QString& contidion) const;
    void saveProfile() const;
    void loadProfile() const;
    void spellList();
    void profileList();

    SpellThread* spellThread = nullptr;
};

#endif //SPELL_TAB_H
