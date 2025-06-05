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

    void addTarget(const QString& name, int hpFrom, int hpTo, int distance, const QString& action) const;

    void startTargetThread(int state);

private:
    QLabel* status_label;

    QListWidget* targetList_listWidget;

    QLineEdit* targetName_lineEdit;
    QLineEdit* hpFrom_lineEdit;
    QLineEdit* hpTo_lineEdit;

    QSlider* distance_slider;

    QComboBox* action_comboBox;

    void targetList();
    void startTarget();

    TargetThread* targetThread = nullptr;
};




#endif
