#include "EasyApi.h"
#include "../Functions/memory_functions.h"
#include <QByteArray>
#include <QHash>
#include <QThread>

static inline auto* LP() {
    return MemoryFunctions::map_view->LocalPlayer;
}

EasyApi::EasyApi(QObject* parent) : QObject(parent) {}

double EasyApi::cap() const     { return LP() ? LP()->cap     : 0.0; }
double EasyApi::hp() const      { return LP() ? LP()->hp      : 0.0; }
double EasyApi::max_hp() const  { return LP() ? LP()->max_hp  : 0.0; }
double EasyApi::mp() const      { return LP() ? LP()->mp      : 0.0; }
double EasyApi::max_mp() const  { return LP() ? LP()->max_mp  : 0.0; }
double EasyApi::lvl() const     { return LP() ? LP()->lvl     : 0.0; }
double EasyApi::lvl_pc() const  { return LP() ? LP()->lvl_pc  : 0.0; }
int    EasyApi::x() const       { return LP() ? int(LP()->x)  : 0; }
int    EasyApi::y() const       { return LP() ? int(LP()->y)  : 0; }
int    EasyApi::z() const       { return LP() ? int(LP()->z)  : 0; }

void EasyApi::say(const QString& msg) {
    const QByteArray u = msg.toUtf8();
    MemoryFunctions::queue_talkChannel(u.constData());
}

void EasyApi::close(const QString& container_name) {
    const QByteArray u = container_name.toUtf8();
    MemoryFunctions::queue_close(u.constData());
}

void EasyApi::open(const QString& container_name) {
    const QByteArray u = container_name.toUtf8();
    MemoryFunctions::queue_open(u.constData());
}


std::function<void(const QString&)> EasyApi::onLabel = {};

void EasyApi::label(const QString& name) {
    if (onLabel) onLabel(name.trimmed());
}


void EasyApi::sleep(int ms) {
    if (ms <= 0) return;
    while (ms > 0) {
        const int step = ms > 50 ? 50 : ms;
        QThread::msleep(step);
        ms -= step;
    }
}