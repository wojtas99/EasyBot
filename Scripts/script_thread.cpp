#include "script_thread.h"

void ScriptThread::run() {

    const QString script = m_script.value("script").toString().trimmed();
    int sleep_time = m_script.value("loop").toInt();

    ScriptRuntime runtime;
    while (m_running) {
        ScriptResult res;
        runtime.exec(script, res);
        msleep(sleep_time);
    }
}