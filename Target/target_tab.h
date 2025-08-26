#ifndef TARGET_TAB_H
#define TARGET_TAB_H
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QListWidgetItem>
#include "target_thread.h"

class TargetTab : public QWidget {
    Q_OBJECT

public:
    explicit TargetTab(QWidget* parent = nullptr);
    signals:
    void requestLoot(int containerIndex);
public slots:
    void setTargetEnabled(bool on);


private:
    QListWidget* targetList_listWidget;
    QListWidget* profile_listWidget;

    void targetList();
    void profileList();
    void addTarget(const QString& name, int hpFrom, int hpTo, int distance, const QString& desiredStance, const QString& monsterAttacks, int count) const;


    TargetThread* targetThread = nullptr;
};




#endif
