#include "target_thread.h"
#include "../Functions/memory_functions.h"
#include <QThread>
#include <iostream>
#include "../Structs/medivia_struct.h"

void TargetThread::run() {
    int closest_dist = 100;
    int idx = 0;
    Entity* closest_entity = nullptr;
    bool open_corpse = false;
    while (m_running && !m_targets.isEmpty()) {
        std::string target_name = m_targets[idx].value("name").toString().toStdString();
        int dist_threshold = m_targets[idx].value("distance").toInt();
        int hp_from = m_targets[idx].value("hpFrom").toInt();
        int hp_to = m_targets[idx].value("hpTo").toInt();
        idx = (idx + 1) % m_targets.size();
        closest_dist = 100;
        if (MemoryFunctions::player_base->Entity == 0) {
            if (open_corpse)
            {
                int coords[3] = {int(closest_entity->x), int(closest_entity->y), int(closest_entity->z)};
                std::vector<void*> fieldsOnScreen = MemoryFunctions::getFieldsAround(7);
                for (void* fieldPtr : fieldsOnScreen) {
                    if (fieldPtr) {
                        void** objects = static_cast<void**>(fieldPtr);
                        int count = 0;
                        while (objects[count] != nullptr && open_corpse) {
                            Tile* tile = static_cast<Tile*>(objects[count]);
                            if (tile->x == closest_entity->last_x && tile->y == closest_entity->last_y && tile->z == closest_entity->last_z)
                            {
                                uintptr_t result = MemoryFunctions::getTopItem(GetCurrentProcess(), reinterpret_cast<uint64_t>(tile->itemList), coords);
                                MemoryFunctions::queueOpenItem(reinterpret_cast<Item*>(result));
                                open_corpse = false;
                                break;
                            }
                            count++;
                        }
                    }
                }
            }
            open_corpse = false;
            closest_entity = nullptr;
            std::vector<Entity*> entities = MemoryFunctions::entityCount(dist_threshold);
            for (Entity* entity : entities) {
                int dist_x = abs(static_cast<int>(entity->x - MemoryFunctions::map_view->LocalPlayer->x));
                int dist_y = abs(static_cast<int>(entity->y - MemoryFunctions::map_view->LocalPlayer->y));
                if (dist_x + dist_y <= closest_dist &&
                    (entity->name == target_name || target_name == "*") &&
                    hp_from >= entity->hp && entity->hp > hp_to) {
                    closest_entity = entity;
                    closest_dist = dist_x + dist_y;
                }
            }
            if (closest_entity != nullptr) {
                MemoryFunctions::queueAttack(closest_entity);
                msleep(500);
                MemoryFunctions::has_target = true;
                open_corpse = true;
            } else {
                MemoryFunctions::has_target = false;
            }
        }

        msleep(50);
    }
}


