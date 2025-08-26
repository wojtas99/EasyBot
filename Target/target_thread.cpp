#include "target_thread.h"
#include "../Functions/Game.h"
#include <QThread>
#include <iostream>

#include "../Loot/loot_tab.h"
#include "../Structs/medivia_struct.h"

void TargetThread::run() {
    int closest_dist = 100;
    int idx = 0;
    Entity* closest_entity = nullptr;
    bool open_corpse = false;
    int enemy_coords[3] = {0, 0, 0};
    while (m_running && !m_targets.isEmpty()) {
        // Metadata of the target
        std::string target_name = m_targets[idx].value("name").toString().toStdString();
        int dist_threshold = m_targets[idx].value("distance").toInt();
        int hp_from = m_targets[idx].value("hpFrom").toInt();
        int hp_to = m_targets[idx].value("hpTo").toInt();
        int count = m_targets[idx].value("count").toInt();
        std::string desiredStance = m_targets[idx].value("desiredStance").toString().toStdString();
        std::string monsterAttacks = m_targets[idx].value("monsterAttacks").toString().toStdString();
        // ##########
        idx = (idx + 1) % m_targets.size();
        closest_dist = 100;
        if (Game::player_base->Entity == 0) {
            if (open_corpse && closest_entity->hp == 0) {
                open_corpse = false;
                uint64_t tile = tile = Game::queue_getTile(enemy_coords[0], enemy_coords[1], enemy_coords[2]);
                uint64_t top_thing = Game::queue_getTopThing(tile);
                int index = Game::queue_open(reinterpret_cast<Item*>(top_thing), 0);
                msleep(500);
            }
            closest_entity = nullptr;
            std::vector<Entity*> entities = Game::queue_getSpectatorsInRangeEx(dist_threshold);
            for (Entity* entity : entities) {
                int dist_x = abs(static_cast<int>(entity->x - Game::map_view->LocalPlayer->x));
                int dist_y = abs(static_cast<int>(entity->y - Game::map_view->LocalPlayer->y));
                if (dist_x + dist_y <= closest_dist &&
                    (entity->name == target_name || target_name == "*") &&
                    hp_from >= entity->hp && entity->hp > hp_to) {
                    closest_entity = entity;
                    closest_dist = dist_x + dist_y;
                    count -= 1;
                }
            }
            if (closest_entity != nullptr && count <= 0)
            {
                Game::queue_stop();
                if (desiredStance == "Chase") {
                    Game::queue_setChaseMode(true);
                }
                Game::queue_attack(closest_entity);
                msleep(500);
                Game::has_target = true;
                open_corpse = true;
            } else
            {
                Game::has_target = false;
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

