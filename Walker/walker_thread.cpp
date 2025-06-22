#include "walker_thread.h"

#include <iostream>

#include "../Functions/memory_functions.h"
#include <QThread>
void WalkerThread::run()
{
    int idx = 0;

    while (m_running && !m_waypoints.isEmpty())
    {
        const auto& wpt = m_waypoints[idx];

        int map_x = wpt["x"].toInt();
        int map_y = wpt["y"].toInt();
        int map_z = wpt["z"].toInt();
        int x = MemoryFunctions::map_view->LocalPlayer->x;
        int y = MemoryFunctions::map_view->LocalPlayer->y;
        int z = MemoryFunctions::map_view->LocalPlayer->z;
        std::string option = wpt["option"].toString().toStdString();
        if (x == map_x && y == map_y && z == map_z)
        {
            idx = (idx + 1) % m_waypoints.size();
            emit indexUpdate(idx);
            continue;
        }
        if      (option == "North") {
            map_y = map_y-1;
        }
        if      (option == "South") {
            map_y = map_y+1;
        }
        if      (option == "East") {
            map_x = map_x+1;
        }
        if      (option == "West") {
            map_x = map_x-1;
        }
        if (MemoryFunctions::moveTo(map_x,   map_y,   map_z)) {
            idx = (idx + 1) % m_waypoints.size();
            emit indexUpdate(idx);
            msleep(1500);
        }
        msleep(2000);
    }
}

