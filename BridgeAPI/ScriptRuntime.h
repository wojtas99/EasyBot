#pragma once
#include <QJSEngine>
#include <QJSValue>
#include <QString>

struct ScriptResult {
    bool repeat = false;
    int  skip = 0;
    QString error;
};

class ScriptRuntime {
public:
    ScriptRuntime();
    void exec(const QString& userCode, ScriptResult& out);

private:
    QJSEngine engine_;
};
