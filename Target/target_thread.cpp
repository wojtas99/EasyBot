#include "target_thread.h"
#include "../Functions/Game.h"
#include <QThread>

#include "../Loot/loot_tab.h"
#include "../Structs/medivia_struct.h"

void TargetThread::run() {
    bool open_corpse = false;
    Entity *target = nullptr;
    while (m_running && !m_targets.isEmpty()) {
        for (const auto& m_target : m_targets) {
            std::string target_name = m_target.value("name").toString().toStdString();
            int dist_threshold = m_target.value("distance").toInt();
            int hp_from = m_target.value("hpFrom").toInt();
            int hp_to = m_target.value("hpTo").toInt();
            int count = m_target.value("count").toInt();
            std::string desiredStance = m_target.value("desiredStance").toString().toStdString();
            std::string monsterAttacks = m_target.value("monsterAttacks").toString().toStdString();
            bool open = m_target.value("open").toBool();
            if (Game::player_base->Entity == 0) {
                if (open_corpse) {
                    open_corpse = false;
                    //uint64_t tile = tile = Game::queue_getTile(enemy_coords[0], enemy_coords[1], enemy_coords[2]);
                    //uint64_t top_thing = Game::queue_getTopThing(tile);
                    //int index = Game::queue_open(reinterpret_cast<Item*>(top_thing), 0);
                    msleep(500);
                }
                target = nullptr;
                std::vector<Entity*> entities = Game::queue_getSpectatorsInRangeEx(dist_threshold);
                for (Entity* entity : entities) {
                    if ((entity->name == target_name || target_name == "*") &&
                    hp_from >= entity->hp && entity->hp > hp_to) {
                        target = entity;
                        count -= 1;
                    }
                }
                if (target && count <= 0) {
                    Game::has_target = true;
                    Game::queue_stop();
                    open_corpse = open;
                    if (desiredStance == "Chase") {Game::queue_setChaseMode(true);}
                    Game::queue_attack(target);
                    msleep(500);
                } else {Game::has_target = false;}
            }
            msleep(50);
        }
    }
}

