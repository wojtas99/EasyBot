#ifndef STATUS_TAB_H
#define STATUS_TAB_H
#include <QCheckBox>
#include <QListWidgetItem>

class StatusTab : public QWidget {
    Q_OBJECT

public:
    explicit StatusTab(QWidget* parent = nullptr);
    signals:
    void walkerToggled(bool on);
private:
    QCheckBox* walker_checkBox = nullptr;

};




#endif
