#ifndef LOOT_THREAD_H
#define LOOT_THREAD_H
#include <QThread>
#include <QVariantMap>
#include <QList>

class LootThread : public QThread {
    Q_OBJECT

public:
    LootThread(const QList<QVariantMap>& items, QObject* parent = nullptr) :
    QThread(parent), m_items(items), m_running(true) {}

    void run() override;
public slots:
    void stop() noexcept { m_running = false; }

private:
    QList<QVariantMap> m_items;
    std::atomic<bool> m_running{true};
};


#endif //LOOT_THREAD_H
