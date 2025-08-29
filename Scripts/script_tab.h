#ifndef SCRIPTS_H
#define SCRIPTS_H
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QListWidgetItem>
#include "../Scripts/script_thread.h"

class ScriptTab : public QWidget {
    Q_OBJECT

public:
    explicit ScriptTab(QWidget* parent = nullptr);
    ~ScriptTab() override;
public slots:
    void setScriptsEnabled(bool on) const;
    void onItemChanged(QListWidgetItem* item);
    signals:
    bool saveProfileSignal(const QString& tab, const QString& profileName, QList<QVariantMap> m_items);
    QList<QVariantMap> loadProfileSignal(const QString& tab, const QString& profileName);

private:
    QListWidget* scriptList_listWidget;
    QListWidget* profile_listWidget;

    QHash<QListWidgetItem*, ScriptThread*> m_scriptThreads;

    void scriptList();
    void profileList();

    void clearList();
    void deleteSelected();
    void loadProfile(const QString& profileName);
    void saveProfile(const QString& profileName);

    void addScript(const QString& scriptData, int sleep_time) const;

    void stopAndRemoveThread(QListWidgetItem* item);
    void stopAllThreads();
};







#endif //SCRIPTS_H
