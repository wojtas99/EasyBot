#include "target_thread.h"
#include "../Functions/Game.h"
#include <QThread>
#include <iostream>

#include "../Loot/loot_tab.h"
#include "../Structs/medivia_struct.h"


Entity* get_closest(Entity* entity, Entity* closest) {
    if (!closest) {return entity;}
    int entity_x = abs(static_cast<int>(entity->x - Game::map_view->LocalPlayer->x));
    int entity_y = abs(static_cast<int>(entity->y - Game::map_view->LocalPlayer->y));
    int closest_x = abs(static_cast<int>(closest->x - Game::map_view->LocalPlayer->x));
    int closest_y = abs(static_cast<int>(closest->y - Game::map_view->LocalPlayer->y));
    if ((closest_x + closest_y) >= (entity_x + entity_y)) {return entity;}
    return closest;

}
void TargetThread::run() {
    Entity* closest_entity = nullptr;
    bool open_corpse = false;
    int enemy_coords[3] = {0, 0, 0};
    while (m_running) {
        for (auto target : m_targets)
        {
            std::string target_name = target.value("name").toString().toStdString();
            int dist_threshold = target.value("distance").toInt();
            int hp_from = target.value("hpFrom").toInt();
            int hp_to = target.value("hpTo").toInt();
            int count = target.value("count").toInt();
            std::string desiredStance = target.value("desiredStance").toString().toStdString();
            std::string monsterAttacks = target.value("monsterAttacks").toString().toStdString();
            if (Game::player_base->Entity == 0) {
                if (open_corpse && closest_entity->hp == 0) {
                    open_corpse = false;
                    uint64_t tile = tile = Game::queue_getTile(enemy_coords[0], enemy_coords[1], enemy_coords[2]);
                    uint64_t top_thing = Game::queue_getTopThing(tile);
                    Game::queue_open(reinterpret_cast<Item*>(top_thing), 0);
                }
                closest_entity = nullptr;
                std::vector<Entity*> entities = Game::queue_getSpectatorsInRangeEx(dist_threshold);
                for (Entity* entity : entities) {
                    if ((entity->name == target_name || target_name == "*") && hp_from >= entity->hp && entity->hp > hp_to) {
                        closest_entity = get_closest(entity, closest_entity);
                        count -= 1;
                        }
                }
                if (!closest_entity || count > 0) {Game::has_target = false; continue;}
                Game::queue_stop();
                if (desiredStance == "Chase") { Game::queue_setChaseMode(true); }
                Game::queue_attack(closest_entity);
                Game::has_target = true;
                open_corpse = true;
            } else if (closest_entity) {
                if (closest_entity-> x != 65535 && closest_entity-> x != 65535 && closest_entity-> x != 255) {
                    enemy_coords[0] = closest_entity-> x;
                    enemy_coords[1] = closest_entity-> y;
                    enemy_coords[2] = closest_entity-> z;
                }
            }
        }
    }
}


