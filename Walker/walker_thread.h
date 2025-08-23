#ifndef WALKER_THREAD_H
#define WALKER_THREAD_H
#include <QThread>
#include <QVariantMap>
#include <QList>
#include "../BridgeAPI/ScriptRuntime.h"

class WalkerThread : public QThread {
    Q_OBJECT

public:
    WalkerThread(const QList<QVariantMap>& waypoints, QObject* parent = nullptr)
        : QThread(parent), m_waypoints(waypoints), m_running(true) {}

    void run() override;
    int find_wpt();
    void stop() noexcept { m_running = false; }

    signals:
        void indexUpdate(int wpt_index);

private:
    QList<QVariantMap> m_waypoints;
    std::atomic<bool> m_running{true};
    QHash<QString,int> m_labelIndex;
};

#endif // WALKER_THREAD_H
