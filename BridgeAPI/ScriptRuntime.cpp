#include "ScriptRuntime.h"
#include "EasyApi.h"
#include <QRegularExpression>

ScriptRuntime::ScriptRuntime() {
    auto* api = new EasyApi(&engine_);
    engine_.globalObject().setProperty("api", engine_.newQObject(api));
}

void ScriptRuntime::exec(const QString& userCode, ScriptResult& out) {
    out = ScriptResult{};
    QJSValue r = engine_.evaluate(userCode, "action", 1);
    if (r.isError()) {
        out.error = r.toString();
        return;
    }
    if (r.isNumber()) {
        const int v = r.toInt();
        if (v == -1) { out.repeat = true; return; }
        if (v > 0)   { out.skip = v; return; }
    }
}

