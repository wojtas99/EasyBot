#ifndef SCRIPTS_H
#define SCRIPTS_H
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QListWidgetItem>

class ScriptTab : public QWidget {
    Q_OBJECT

public:
    explicit ScriptTab(QWidget* parent = nullptr);
public slots:
    void setScriptsEnabled(bool on);
    signals:
    bool saveProfileSignal(const QString& tab, const QString& profileName, QList<QVariantMap> m_items);
    QList<QVariantMap> loadProfileSignal(const QString& tab, const QString& profileName);

private:
    QListWidget* scriptList_listWidget;
    QListWidget* profile_listWidget;

    void scriptList();
    void profileList();

    void clearList() const;
    void loadProfile(const QString& profileName);
    void saveProfile(const QString& profileName);

};







#endif //SCRIPTS_H
