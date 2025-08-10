#include "walker_thread.h"

#include <iostream>

#include "../Functions/memory_functions.h"
#include <QThread>

#include "../Target/target_thread.h"

void WalkerThread::run()
{
    int timer = 0, x = 0, y = 0;
    bool is_walking = false;
    int idx = find_wpt();


    while (m_running && !m_waypoints.isEmpty())
    {

        if (timer/1000 >= 0.5) { // Check if character is moving every 500 ms
            if (x == MemoryFunctions::map_view->LocalPlayer->x && y == MemoryFunctions::map_view->LocalPlayer->y) {
                is_walking = false;
            } else {
                is_walking = true;
            }
            x = MemoryFunctions::map_view->LocalPlayer->x;
            y = MemoryFunctions::map_view->LocalPlayer->y;
            timer = 0;
        }

        const auto& wpt = m_waypoints[idx];
        int map_x = wpt["x"].toInt();
        int map_y = wpt["y"].toInt();
        int map_z = wpt["z"].toInt();
        std::string option = wpt["option"].toString().toStdString();
        std::string direction = wpt["direction"].toString().toStdString();
        std::string action = wpt["action"].toString().toStdString();
        std::string label = wpt["label"].toString().toStdString();

        if (direction != "C" && map_z != MemoryFunctions::map_view->LocalPlayer->z) {
            idx = (idx + 1) % m_waypoints.size();
            emit indexUpdate(idx);
            continue;
        }

        if (MemoryFunctions::map_view->LocalPlayer->x == map_x &&
            MemoryFunctions::map_view->LocalPlayer->y == map_y &&
            MemoryFunctions::map_view->LocalPlayer->z == map_z) {
            idx = (idx + 1) % m_waypoints.size();
            emit indexUpdate(idx);
            continue;
        }

        if (!MemoryFunctions::has_target || option == "Lure") {
            MemoryFunctions::queue_autoWalk(map_x, map_y, map_z);
        }
        if (MemoryFunctions::queue_isAttacking() && option != "Lure" && is_walking)
        {
            MemoryFunctions::queue_stop();
            is_walking = false;
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
        std::string direction = wpt["direction"].toString().toStdString();

        if (MemoryFunctions::map_view->LocalPlayer->z == map_z &&
            abs(static_cast<int>(map_x - MemoryFunctions::map_view->LocalPlayer->x)) <= 7 &&
            abs(static_cast<int>(map_y - MemoryFunctions::map_view->LocalPlayer->y)) <= 5 &&
            direction == "C"  || option == "Lure") {
            std::cout << "Found wpt" << std::endl;
            return idx;
        }
    }

    return 0;
}

