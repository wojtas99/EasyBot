#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <windows.h>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include "injector.h"

class InjectWidget : public QWidget {
    Q_OBJECT
public:
    InjectWidget(QWidget* parent = nullptr) : QWidget(parent) {
        auto mainLayout = new QVBoxLayout(this);

        auto dllLayout = new QHBoxLayout();
        dllLineEdit = new QLineEdit();
        dllLineEdit->setPlaceholderText("Choose .dll");
        dllLineEdit->setEnabled(false);
        auto dllButton = new QPushButton("...");
        dllButton->setFixedWidth(25);
        dllLayout->addWidget(dllLineEdit);
        dllLayout->addWidget(dllButton);
        mainLayout->addLayout(dllLayout);

        auto exeLayout = new QHBoxLayout();;
        exeLineEdit = new QLineEdit();
        exeLineEdit->setEnabled(false);
        exeLineEdit->setPlaceholderText("Choose .exe");
        auto exeButton = new QPushButton("...");
        exeButton->setFixedWidth(25);
        exeLayout->addWidget(exeLineEdit);
        exeLayout->addWidget(exeButton);
        mainLayout->addLayout(exeLayout);

        // Inject button
        injectButton = new QPushButton("Inject");
        injectButton->setEnabled(false);
        mainLayout->addWidget(injectButton);

        connect(dllButton, &QPushButton::clicked, this, &InjectWidget::selectDll);
        connect(exeButton, &QPushButton::clicked, this, &InjectWidget::selectExe);
        connect(injectButton, &QPushButton::clicked, this, [this](){
            inject(
                dllLineEdit->text().toStdString(),
                exeLineEdit->text().toStdString()
            );
        });


    }


private slots:
    void selectDll() {
        QString file = QFileDialog::getOpenFileName(this, "Choose DLL", "", "DLL (*.dll)");
        if (!file.isEmpty()) {
            dllLineEdit->setText(file);
        }
        checkReady();
    }
    void selectExe() {
        QString file = QFileDialog::getOpenFileName(this, "Choose EXE", "", "EXE (*.exe)");
        if (!file.isEmpty()) {
            exeLineEdit->setText(file);
        }
        checkReady();
    }
    void checkReady() {
        injectButton->setEnabled(!dllLineEdit->text().isEmpty() && !exeLineEdit->text().isEmpty());
    }


private:
    QLineEdit *dllLineEdit, *exeLineEdit;
    QPushButton* injectButton;
};

#include "EasyBotLoader.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    InjectWidget window;
    window.setWindowTitle("EasyBotLoader");
    window.setFixedSize(300, 200);
    window.show();
    return app.exec();
}
