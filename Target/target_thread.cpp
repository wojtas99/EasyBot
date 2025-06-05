#include "target_thread.h"
#include <iostream>
#include "../Functions/memory_functions.h"
#include <QThread>
#include "../Structs/medivia_struct.h"

void TargetThread::run() {
    std::string target_name;
    std::vector<Entity*> entities = MemoryFunctions::entityCount();
    int dist_threshold = 0;
    int hpFrom = 0;
    int hpTo = 0;
    bool attackAll = false;
    std::string action;
    while (m_running && !m_targets.isEmpty()) {
        if (!MemoryFunctions::isAttacking()) {
            std::vector<Entity*> entities = MemoryFunctions::entityCount();
            for (const auto& target : m_targets) {
                target_name = target.value("name").toString().toStdString();
                action = target.value("action").toString().toStdString();
                dist_threshold = target.value("distance").toInt();
                hpFrom = target.value("hpFrom").toInt();
                hpTo = target.value("hpTo").toInt();
                attackAll = (target_name == "*");
                for (Entity* entity : entities) {
                    int dist_x = MemoryFunctions::map_view->LocalPlayer->x - entity->x;
                    dist_x = abs(dist_x);
                    int dist_y = MemoryFunctions::map_view->LocalPlayer->y - entity->y;
                    dist_y = abs(dist_y);
                    if ((attackAll || entity->name == target_name) && entity->z == MemoryFunctions::map_view->LocalPlayer->z && dist_y <= dist_threshold && dist_x <= dist_threshold && entity->hp >= hpTo && entity->hp <= hpFrom) {
                        MemoryFunctions::attackTarget(reinterpret_cast<uint64_t>(entity));
                        break;
                    }
                }
            }
        }
        msleep(500);
    }
}


