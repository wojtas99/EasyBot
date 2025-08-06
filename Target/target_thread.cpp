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
    int enemy_coords[3] = {0, 0, 0};
    while (m_running && !m_targets.isEmpty()) {
        std::string target_name = m_targets[idx].value("name").toString().toStdString();
        int dist_threshold = m_targets[idx].value("distance").toInt();
        int hp_from = m_targets[idx].value("hpFrom").toInt();
        int hp_to = m_targets[idx].value("hpTo").toInt();
        idx = (idx + 1) % m_targets.size();
        closest_dist = 100;
        if (!MemoryFunctions::queue_isAttacking()) {
            if (open_corpse && !m_looting.isEmpty())
            {
                open_corpse = false;
                uint64_t tile = tile = MemoryFunctions::queue_getTile(enemy_coords[0], enemy_coords[1], enemy_coords[2]);
                uint64_t top_thing = MemoryFunctions::queue_getTopThing(tile);
                MemoryFunctions::queue_open(reinterpret_cast<Item*>(top_thing), 0);
                msleep(1000);
                for (auto loot : m_looting)
                {
                    int item_id = loot.value("id").toString().toInt();
                    int index = loot.value("action").toString().toInt();
                    auto containers = MemoryFunctions::queue_getContainers();
                    for (int j = 0; j < containers.size(); ++j)
                    {
                        if (j == index) continue;
                        for (int i = containers[j]->number_of_items - 1; i >= 0; --i)
                        {
                            Item* item = MemoryFunctions::queue_getItem(containers[j], i);
                            if (item->id == item_id)
                            {
                                MemoryFunctions::queue_move(item, containers[index], index);
                            }
                        }
                    }
                }
            }
            closest_entity = nullptr;
            std::vector<Entity*> entities = MemoryFunctions::queue_getSpectatorsInRangeEx(dist_threshold);
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
            if (closest_entity != nullptr)
            {
                MemoryFunctions::queue_attack(closest_entity);
                MemoryFunctions::has_target = true;
                open_corpse = true;
            } else
            {
                MemoryFunctions::has_target = false;
            }
        } else
        {
            if (closest_entity-> x != 65535 && closest_entity-> x != 65535 && closest_entity-> x != 255) {
                enemy_coords[0] = closest_entity-> x;
                enemy_coords[1] = closest_entity-> y;
                enemy_coords[2] = closest_entity-> z;
            }
        }
        msleep(50);
    }
}


