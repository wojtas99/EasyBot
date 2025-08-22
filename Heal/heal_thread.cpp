#include "heal_thread.h"
#include <iostream>
#include "../Functions/memory_functions.h"
#include <QThread>


void HealThread::run()
{
    int timer = 0;


    while (m_running && !m_healing.isEmpty())
    {
        for (auto item : m_healing)
        {
            auto option = item["option"].toString().toStdString();
            auto below = item["hpBelow"].toDouble();
            auto above = item["hpAbove"].toDouble();
            auto min_mp = item["minMp"].toDouble();
            auto condition = item["contidion"];
            double current_hp = MemoryFunctions::map_view->LocalPlayer->hp;
            double current_maxhp = MemoryFunctions::map_view->LocalPlayer->max_hp;
            double current_mp = MemoryFunctions::map_view->LocalPlayer->mp;
            double current_maxmp = MemoryFunctions::map_view->LocalPlayer->max_mp;
            if (condition == "HP%") {
                current_hp = 100*(current_hp/current_maxhp);
                if (below >= current_hp && current_hp >= above && current_mp >= min_mp)
                {
                    if (option == "Say") {
                        auto heal = item["heal"].toString().toStdString();
                        MemoryFunctions::queue_talkChannel(heal.c_str());
                    } else {
                        auto heal = item["heal"].toInt();
                        auto item = MemoryFunctions::queue_findItemInContainers(heal);
                        auto player = reinterpret_cast<uint64_t>(MemoryFunctions::map_view->LocalPlayer);
                        if (item) {
                            MemoryFunctions::queue_useWith(item, player);
                        }
                    }
                    msleep(500);
                }
            } else {
                current_mp = 100*(current_mp/current_maxmp);
                if (below >= current_mp &&  current_mp >= above)
                {
                    if (option == "Say") {
                        auto heal = item["heal"].toString().toStdString();
                        MemoryFunctions::queue_talkChannel(heal.c_str());
                    } else {
                        auto player = reinterpret_cast<uint64_t>(MemoryFunctions::map_view->LocalPlayer);
                        auto heal = item["heal"].toInt();
                        auto item = MemoryFunctions::queue_findItemInContainers(heal);
                        if (item) {
                            MemoryFunctions::queue_useWith(item, player);
                        }
                    }
                    msleep(500);
                }
            }
            msleep(25);
        }

        msleep(25);
    }
}


