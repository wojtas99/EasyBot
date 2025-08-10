#ifndef LOOT_THREAD_H
#define LOOT_THREAD_H
#include <QThread>
#include <QVariantMap>
#include <QList>

class LootThread : public QThread {
    Q_OBJECT

public:
    LootThread(const QList<QVariantMap>& items, int container_index, QObject* parent = nullptr) :
    QThread(parent), m_items(items), m_container_index(container_index), m_running(true) {}

    void run() override;
    void stop() noexcept { m_running = false; }

private:
    QList<QVariantMap> m_items;
    int m_container_index;
    std::atomic<bool> m_running{true};
};


#endif //LOOT_THREAD_H
