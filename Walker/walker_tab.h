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
    signals:
    bool saveProfileSignal(const QString& tab, const QString& profileName, QList<QVariantMap> m_items);
    QList<QVariantMap> loadProfileSignal(const QString& tab, const QString& profileName);

private:
    QListWidget* waypointList_listWidget;
    QListWidget* profile_listWidget;
    QTextEdit* action_textEdit;
    QButtonGroup* directions_buttonGroup= nullptr;
    QButtonGroup* options_buttonGroup = nullptr;

    void waypointList();
    void profileList();

    void clearList() const;
    void loadProfile(const QString& profileName);
    void saveProfile(const QString& profileName);

    void addWaypoint() const;

    WalkerThread* walkerThread = nullptr;

    private slots:
    void onWalkerIndexUpdate(int idx) const;
};




#endif
