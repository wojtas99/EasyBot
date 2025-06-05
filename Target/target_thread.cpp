#include "target_thread.h"
#include <iostream>
#include "../Functions/memory_functions.h"
#include <QThread>
#include "../Structs/medivia_struct.h"

void TargetThread::run() {
    while (m_running && !m_targets.isEmpty()) {
        const QVariantMap& target_data = m_targets[0];
        std::string target_name = target_data.value("name").toString().toStdString();
        if (!MemoryFunctions::isAttacking()) {
            std::vector<Entity*> entities = MemoryFunctions::entityCount();
            for (Entity* entity : entities) {
                if (entity->name == target_name && entity->z == MemoryFunctions::map_view->LocalPlayer->z) {
                    int dist_x = MemoryFunctions::map_view->LocalPlayer->x - entity->x;
                    dist_x = abs(dist_x);
                    int dist_y = MemoryFunctions::map_view->LocalPlayer->y - entity->y;
                    dist_y = abs(dist_y);
                    if (dist_y <= target_data.value("distance").toInt() && dist_x <= target_data.value("distance").toInt()) {
                        MemoryFunctions::attackTarget(reinterpret_cast<uint64_t>(entity));
                        break;
                    }
                }
            }
        }
        msleep(500);
    }
}


