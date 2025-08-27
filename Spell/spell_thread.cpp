#include "spell_thread.h"

#include <iostream>

#include "../Functions/Game.h"
#include <QThread>
#include <__msvc_ostream.hpp>


void SpellThread::run()
{
    Entity *target = nullptr;
    while (m_running && !m_spells.isEmpty())
    {
        for (auto item : m_spells)
        {
            auto option = item["option"].toString().toStdString();
            auto minHp = item["minHp"].toDouble();
            auto minMp = item["minMp"].toDouble();
            auto count = item["count"].toInt();
            auto targetName = item["targetName"];
            auto hpFrom = item["hpFrom"].toDouble();
            auto hpTo = item["hpTo"].toDouble();
            auto dist = item["dist"].toInt();
            target = nullptr;
            std::vector<Entity*> entities = Game::queue_getSpectatorsInRangeEx(dist);
            for (Entity* entity : entities) {
                if ((entity->name == targetName || targetName == "*") &&
                hpFrom >= entity->hp && entity->hp > hpTo) {
                    count -= 1;
                    target = entity;
                }
            }
            double current_hp = Game::map_view->LocalPlayer->hp;
            double current_maxhp = Game::map_view->LocalPlayer->max_hp;
            current_hp = 100*(current_hp/current_maxhp);
            std::cout << count << std::endl;
            std::cout << current_hp << std::endl;
            std::cout << option << std::endl;
            if (target && count <= 0 && current_hp >= minHp && Game::map_view->LocalPlayer->mp >= minMp) {
                if (option == "Say") {
                    auto spell = item["spell"].toString().toStdString().c_str();
                    Game::queue_talkChannel(spell);
                } else if (option == "Use on yourself") {
                    auto spell = item["spell"].toInt();
                    auto item_use = Game::queue_findItemInContainers(spell);
                    auto player = reinterpret_cast<uint64_t>(Game::map_view->LocalPlayer);
                    if (item_use) {
                        Game::queue_useWith(item_use, player);
                    }
                } else {
                    auto spell = item["spell"].toInt();
                    auto item_use = Game::queue_findItemInContainers(spell);
                    auto tile = Game::queue_getTile(target->x, target->y, target->z);
                    if (tile) {
                        uint64_t top_thing = Game::queue_getTopThing(tile);
                        if (top_thing) {
                            Game::queue_useWith(item_use, top_thing);
                        }
                    }
                }
                msleep(1000);
            }
        }
    }
}


