#include "walker_thread.h"

#include <iostream>

#include "../Functions/memory_functions.h"
#include <QThread>

#include "../Target/target_thread.h"

void WalkerThread::run()
{
    int idx = 0;
    int timer = 0;
    bool walking = false;
    int x = 0, y = 0;
    while (m_running && !m_waypoints.isEmpty())
    {
        // Every 1 sec check if character is walking
        if (timer/1000 > 1) {
            if (x == MemoryFunctions::map_view->LocalPlayer->x && y == MemoryFunctions::map_view->LocalPlayer->y) {
                walking = false;
            } else {
                walking = true;
            }
            x = MemoryFunctions::map_view->LocalPlayer->x;
            y = MemoryFunctions::map_view->LocalPlayer->y;
            //idx = find_wpt();
            timer = 0;
        }

        const auto& wpt = m_waypoints[idx];

        int map_x = wpt["x"].toInt();
        int map_y = wpt["y"].toInt();
        int map_z = wpt["z"].toInt();
        std::string option = wpt["option"].toString().toStdString();

        if (option != "Center" && option != "Lure" &&
            map_z != MemoryFunctions::map_view->LocalPlayer->z) {
            idx = (idx + 1) % m_waypoints.size();
            emit indexUpdate(idx);
            continue;
        }

        if (MemoryFunctions::map_view->LocalPlayer->x == map_x &&
            MemoryFunctions::map_view->LocalPlayer->y == map_y &&
            MemoryFunctions::map_view->LocalPlayer->z == map_z) {
            walking = false;
            idx = (idx + 1) % m_waypoints.size();
            emit indexUpdate(idx);
            continue;
        }

        if ((!MemoryFunctions::has_target || option == "Lure") &&
            MemoryFunctions::map_view->LocalPlayer->z == map_z &&
            !walking) {
            MemoryFunctions::queueMove(map_x, map_y, map_z);
            walking = true;
        }
        msleep(25);
        timer += 25;
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

