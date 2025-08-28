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
public slots:
    void setTargetEnabled(bool on);
    signals:
    bool saveProfileSignal(const QString& tab, const QString& profileName, QList<QVariantMap> m_items);
    QList<QVariantMap> loadProfileSignal(const QString& tab, const QString& profileName);


private:
    QListWidget* targetList_listWidget;
    QListWidget* profile_listWidget;

    void targetList();
    void profileList();

    void clearList() const;
    void loadProfile(const QString& profileName);
    void saveProfile(const QString& profileName);

    void addTarget(const QString& name, int hpFrom, int hpTo, int distance, const QString& desiredStance, const QString& monsterAttacks, int count, bool open) const;


    TargetThread* targetThread = nullptr;
};




#endif
