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

private slots:
    void addTarget(int index);
    void startTargetThread(int state);

private:
    QListWidget* targetList_listWidget;

    QLineEdit* targetName_textEdit;

    QCheckBox* start_checkBox;

    QLabel* status_label;

    void targetList();
    void startTarget();

    TargetThread* targetThread = nullptr;
};




#endif
