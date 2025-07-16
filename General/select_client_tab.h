#ifndef SELECT_CLIENT_TAB_H
#define SELECT_CLIENT_TAB_H
#pragma once
#include "main_window_tab.h"
#include <QPushButton>


class SelectClientTab : public QWidget {
    Q_OBJECT

public:
    SelectClientTab(QWidget *parent = nullptr);

private slots:
    void load_altaron();
    void load_medivia();

private:
    QPushButton *altaron_button;
    QPushButton *medivia_button;

    MainWindowTab *main_window_tab = nullptr;

    bool m_hookInitialized = false;
    bool m_itemHookInitialized = false;

};




#endif
