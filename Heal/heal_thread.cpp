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
            auto option = item["option"];
            auto heal = item["heal"].toString().toStdString();
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
                    MemoryFunctions::queue_talkChannel(heal.c_str());
                    msleep(500);
                }
            } else {
                current_mp = 100*(current_mp/current_maxmp);
                if (below >= current_mp &&  current_mp >= above)
                {
                    MemoryFunctions::queue_talkChannel(heal.c_str());
                    msleep(500);
                }
            }
            msleep(25);
        }

        msleep(25);
    }
}


