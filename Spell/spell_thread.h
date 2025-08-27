#ifndef SPELL_THREAD_H
#define SPELL_THREAD_H
#include <QThread>
#include <QVariantMap>
#include <QList>


class SpellThread : public QThread {
    Q_OBJECT

public:
    SpellThread(const QList<QVariantMap>& spells, QObject* parent = nullptr)
        : QThread(parent), m_spells(spells), m_running(true) {}

    void run() override;
    void stop() noexcept { m_running = false; }

private:
    QList<QVariantMap> m_spells;
    std::atomic<bool> m_running{true};
};



#endif //SPELL_THREAD_H
