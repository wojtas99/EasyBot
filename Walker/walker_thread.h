#ifndef WALKER_THREAD_H
#define WALKER_THREAD_H
#include <QThread>
#include <QVariantMap>
#include <QList>

class WalkerThread : public QThread {
    Q_OBJECT

public:
    WalkerThread(const QList<QVariantMap>& waypoints, QObject* parent = nullptr)
        : QThread(parent), m_waypoints(waypoints), m_running(true) {}

    void run() override;
    void stop() { m_running = false; }

    signals:
        void indexUpdate(int wpt_index);

private:
    QList<QVariantMap> m_waypoints;
    bool m_running;
};

#endif // WALKER_THREAD_H
