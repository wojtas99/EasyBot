#ifndef HEAL_THREAD_H
#define HEAL_THREAD_H
#include <QThread>
#include <QVariantMap>
#include <QList>

class HealThread : public QThread {
    Q_OBJECT

public:
    HealThread(const QList<QVariantMap>& healing, QObject* parent = nullptr)
        : QThread(parent), m_healing(healing), m_running(true) {}

    void run() override;
public slots:
    void stop() noexcept { m_running = false; }

private:
    QList<QVariantMap> m_healing;
    std::atomic<bool> m_running{true};
};

#endif



