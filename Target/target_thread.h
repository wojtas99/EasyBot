#ifndef TARGET_THREAD_H
#define TARGET_THREAD_H
#include <QThread>
#include <QVariantMap>
#include <QList>

class TargetThread : public QThread {
    Q_OBJECT

public:
    TargetThread(const QList<QVariantMap>& targets, QObject* parent = nullptr)
        : QThread(parent), m_targets(targets), m_running(true) {}

    void run() override;
    void stop() { m_running = false; }

private:
    QList<QVariantMap> m_targets;
    bool m_running;
};

#endif
