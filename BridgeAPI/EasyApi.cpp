#include "EasyApi.h"

#include <ios>
#include <iostream>

#include "../Functions/Game.h"
#include <QByteArray>
#include <qmap.h>
#include <QThread>
#include <__msvc_ostream.hpp>

static inline auto* PlayerBase() {
    return Game::player_base;
}

EasyApi::EasyApi(QObject* parent) : QObject(parent) {}

double EasyApi::cap() { return PlayerBase() ? PlayerBase()->LocalPlayer->cap     : 0.0; }
double EasyApi::hp() { return PlayerBase() ? PlayerBase()->LocalPlayer->hp      : 0.0; }
double EasyApi::mp() { return PlayerBase() ? PlayerBase()->LocalPlayer->mp      : 0.0; }
double EasyApi::lvl() { return PlayerBase() ? PlayerBase()->LocalPlayer->lvl     : 0.0; }
int EasyApi::x() { return PlayerBase() ? static_cast<int>(PlayerBase()->LocalPlayer->x)  : 0; }
int EasyApi::y() { return PlayerBase() ? static_cast<int>(PlayerBase()->LocalPlayer->y)  : 0; }
int EasyApi::z() { return PlayerBase() ? static_cast<int>(PlayerBase()->LocalPlayer->z)  : 0; }

void EasyApi::say(const QString& msg) {
    const QByteArray u = msg.toUtf8();
    Game::queue_talkChannel(u.constData());
}

void EasyApi::close(const QString& container_name) {
    const QByteArray u = container_name.toUtf8();
    Game::queue_close(u.constData());
}

void EasyApi::openAll(int index) {
    Game::queue_openAll(index);
}

void EasyApi::useWith(int item_id, int x, int y, int z) {
    if (auto tile = Game::queue_getTile(x, y, z)) {
        if (uint64_t top_thing = Game::queue_getTopThing(tile)) {
            if (item_id == 0) {
                Game::queue_open(reinterpret_cast<Item*>(top_thing), 0);
            } else {
                auto item_use = Game::queue_findItemInContainers(item_id);
                Game::queue_useWith(item_use, top_thing);
            }
        }
    }
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

QVariantList EasyApi::getSpectatorsInRangeEx(int radius) {
    QVariantList out;
    const auto entities = Game::queue_getSpectatorsInRangeEx(radius);
    out.reserve(int(entities.size()));
    for (auto* entity : entities) {
        if (!entity) continue;
        QVariantMap m;
        m["address"]   = quint64(reinterpret_cast<uint64_t>(entity));
        m["name"] = QString::fromStdString(entity->name);
        m["x"] = entity->x;
        m["y"] = entity->y;
        m["z"] = entity->z;
        m["hp"] = entity->hp;
        out.push_back(m);
    }
    return out;
}

QVariantMap EasyApi::getTopThing(int x, int y, int z) {
    QVariantMap out;
    const auto tile = Game::queue_getTile(x, y, z);
    if (!tile) return out;
    const auto top = Game::queue_getTopThing(tile);
    if (!top) return out;
    auto* item = reinterpret_cast<Item*>(top);
    out["address"]= quint64(reinterpret_cast<uint64_t>(item));;
    out["id"]= item->id;
    out["x"] = item->x;
    out["y"] = item->y;
    out["z"] = item->z;
    out["count"] = item->count;
    return out;
}

QVariant EasyApi::isTargeting() {
    const auto target = Game::player_base->Entity;
    if (!target) return{};
    QVariantMap out;
    auto* entity = reinterpret_cast<Entity*>(target);
    out["address"]   = quint64(reinterpret_cast<uint64_t>(entity));
    out["name"]= entity->name;
    out["x"] = entity->x;
    out["y"] = entity->y;
    out["z"] = entity->z;
    return out;
}

