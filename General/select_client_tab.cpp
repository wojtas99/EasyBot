#include "select_client_tab.h"
#include <QGridLayout>
#include "../Functions/Game.h"
#include <iostream>
#include <QMessageBox>

SelectClientTab::SelectClientTab(QWidget *parent) : QWidget(parent) {
    setFixedSize(100, 400);
    setWindowTitle("EasyBot");

    QGridLayout *layout = new QGridLayout(this);

    medivia_button = new QPushButton("Medivia", this);

    layout->addWidget(medivia_button, 0, 0);

    setLayout(layout);

    connect(medivia_button, &QPushButton::clicked, this, &SelectClientTab::load_medivia);
}
void SelectClientTab::load_medivia() {
    Game mf(Game::LoadOption::Medivia);
    this->close();
    main_window_tab = new MainWindowTab();
    main_window_tab->show();
}


