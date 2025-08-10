#ifndef WALKER_TAB_H
#define WALKER_TAB_H
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QLabel>
#include <QListWidgetItem>
#include "walker_thread.h"
#include <QButtonGroup>

class WalkerTab : public QWidget {
    Q_OBJECT

public:
    explicit WalkerTab(QWidget* parent = nullptr);

public slots:
    void setWalkerEnabled(bool on);



private:
    QListWidget* waypointList_listWidget;
    QListWidget* profile_listWidget;

    QLineEdit* profile_lineEdit;
    QTextEdit* action_textEdit;
    QButtonGroup* directions_buttonGroup= nullptr;
    QButtonGroup* options_buttonGroup = nullptr;




    void addWaypoint() const;
    void clearWaypointList() const;
    void saveProfile() const;
    void loadProfile() const;


    void waypointList();
    void profileList();

    WalkerThread* walkerThread = nullptr;
};




#endif
