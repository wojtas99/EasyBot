#ifndef WALKER_TAB_H
#define WALKER_TAB_H
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QListWidgetItem>
#include "walker_thread.h"

class WalkerTab : public QWidget {
    Q_OBJECT

public:
    explicit WalkerTab(QWidget* parent = nullptr);

private slots:
    void saveProfile();
    void loadProfile();
    void addWaypoint(int index);
    void clearWaypointList();
    void startWalkerThread(int state);
    void startRecordThread(int state);

private:
    QListWidget* waypointList_listWidget;
    QListWidget* profile_listWidget;

    QLineEdit* profile_lineEdit;
    QTextEdit* action_textEdit;

    QCheckBox* record_checkBox;
    QCheckBox* start_checkBox;

    QComboBox* option_comboBox;

    QLabel* status_label;


    void profileList();
    void waypointList();
    void start_walker();

    WalkerThread* walkerThread = nullptr;
};




#endif
