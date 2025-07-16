#include "walker_thread.h"

#include <iostream>

#include "../Functions/memory_functions.h"
#include <QThread>

#include "../Target/target_thread.h"

void WalkerThread::run()
{
    int idx = 0;
    int timer = 0;
    while (m_running && !m_waypoints.isEmpty())
    {
        if (timer/1000 > 10) {
            //idx = find_wpt();
            timer = 0;
        }
        const auto& wpt = m_waypoints[idx];

        int map_x = wpt["x"].toInt();
        int map_y = wpt["y"].toInt();
        int map_z = wpt["z"].toInt();
        std::string option = wpt["option"].toString().toStdString();
        std::cout << std::hex << MemoryFunctions::map_view->LocalPlayer->is_walking << std::endl;
        if (MemoryFunctions::has_target) {
            MemoryFunctions::queueMove(MemoryFunctions::map_view->LocalPlayer->x, MemoryFunctions::map_view->LocalPlayer->y, MemoryFunctions::map_view->LocalPlayer->z);
        }
        if ((!MemoryFunctions::has_target || option == "Lure") &&
            MemoryFunctions::map_view->LocalPlayer->z == map_z)
        {
            if (option == "North") {
                map_y = map_y-1;
            } else if (option == "South") {
                map_y = map_y+1;
            } else if (option == "East") {
                map_x = map_x+1;
            } else if (option == "West") {
                map_x = map_x-1;
            } else if (option != "Center") {
                idx = (idx + 1) % m_waypoints.size();
                emit indexUpdate(idx);
            }
            std::cout << "Walking" << std::endl;
            MemoryFunctions::queueMove(map_x, map_y, map_z);
        }
        if (MemoryFunctions::map_view->LocalPlayer->x == map_x &&
            MemoryFunctions::map_view->LocalPlayer->y == map_y &&
            MemoryFunctions::map_view->LocalPlayer->z == map_z) {
            idx = (idx + 1) % m_waypoints.size();
            emit indexUpdate(idx);
            continue;
        }
        msleep(200);
        timer += 200;
    }
}

int WalkerThread::find_wpt()
{
    for (int idx = 0; idx < m_waypoints.size(); ++idx) {
        const auto& wpt = m_waypoints[idx];
        int map_x = wpt["x"].toInt();
        int map_y = wpt["y"].toInt();
        int map_z = wpt["z"].toInt();
        std::string option = wpt["option"].toString().toStdString();

        if (MemoryFunctions::map_view->LocalPlayer->z == map_z &&
            abs(static_cast<int>(map_x - MemoryFunctions::map_view->LocalPlayer->x)) <= 7 &&
            abs(static_cast<int>(map_y - MemoryFunctions::map_view->LocalPlayer->y)) <= 5 &&
            option == "Center"  || option == "Lure") {
            std::cout << "Found wpt" << std::endl;
            return idx;
        }
    }

    return 0;
}

