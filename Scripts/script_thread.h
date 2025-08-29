#ifndef SCRIPT_THREAD_H
#define SCRIPT_THREAD_H
#include <QThread>
#include <QVariantMap>
#include "../BridgeAPI/EasyApi.h"
#include "../BridgeAPI/ScriptRuntime.h"


class ScriptThread : public QThread {
    Q_OBJECT

public:
    ScriptThread(const QVariantMap& script, QObject* parent = nullptr)
        : QThread(parent), m_script(script), m_running(true) {}

    void run() override;
public slots:
    void stop() noexcept { m_running = false; }

private:
    QVariantMap m_script;
    std::atomic<bool> m_running{true};
};



#endif //SCRIPT_THREAD_H
