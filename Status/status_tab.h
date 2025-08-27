#ifndef STATUS_TAB_H
#define STATUS_TAB_H
#include <QCheckBox>
#include <QListWidgetItem>

class StatusTab : public QWidget {
    Q_OBJECT

public:
    explicit StatusTab(QWidget* parent = nullptr);
    signals:
    void walkerToggled(bool on);
    void targetToggled(bool on);
    void lootToggled(bool on);
    void healingToggled(bool on);
    void spellsToggled(bool on);
private:
    QCheckBox* walker_checkBox = nullptr;
    QCheckBox* target_checkBox = nullptr;
    QCheckBox* loot_checkBox = nullptr;
    QCheckBox* healing_checkBox = nullptr;
    QCheckBox* spells_checkBox = nullptr;

};




#endif
