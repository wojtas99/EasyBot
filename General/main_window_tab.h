#ifndef MAIN_WINDOW_TAB_H
#define MAIN_WINDOW_TAB_H
#pragma once
#include <QPushButton>
#include "../Walker/walker_tab.h"
class MainWindowTab : public QWidget {
    Q_OBJECT

public:
    MainWindowTab(QWidget *parent = nullptr);

private slots:
    void targetLoot();
    void walker();
    void healing();
    void settings();
    void smartHotkeys();
    void training();

private:
    QPushButton *targetLootTab_button;
    QPushButton *walkerTab_button;
    QPushButton *healingTab_button;
    QPushButton *settingsTab_button;
    QPushButton *smartHotkeysTab_button;
    QPushButton *trainingTab_button;

    WalkerTab* walker_tab = nullptr;
};




#endif
