#ifndef WALKER_TAB_H
#define WALKER_TAB_H
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QLabel>
#include <QListWidgetItem>
#include "walker_thread.h"

class WalkerTab : public QWidget {
    Q_OBJECT

public:
    explicit WalkerTab(QWidget* parent = nullptr);

private slots:
    void saveProfile() const;
    void loadProfile() const;
    void addWaypoint(const QString &index) const;
    void clearWaypointList() const;
    void startWalkerThread(int state);

private:
    QListWidget* waypointList_listWidget;
    QListWidget* profile_listWidget;

    QLineEdit* profile_lineEdit;
    QTextEdit* action_textEdit;

    QComboBox* option_comboBox;

    QLabel* status_label;


    void profileList();
    void waypointList();

    WalkerThread* walkerThread = nullptr;
};




#endif
