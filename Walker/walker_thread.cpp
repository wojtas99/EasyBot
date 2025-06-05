#include "walker_thread.h"

#include <iostream>

#include "../Functions/memory_functions.h"
#include <QThread>
#include <__msvc_ostream.hpp>

void WalkerThread::run() {
    int currentWpt = 0;
    emit indexUpdate(currentWpt);
    while (m_running && !m_waypoints.isEmpty()) {

        const QVariantMap& wpt_data = m_waypoints[currentWpt];
        int map_x = wpt_data.value("x").toInt();
        int map_y = wpt_data.value("y").toInt();
        int map_z = wpt_data.value("z").toInt();
        std::string option = wpt_data.value("option").toString().toStdString();

        int x = MemoryFunctions::map_view->LocalPlayer->x;
        int y = MemoryFunctions::map_view->LocalPlayer->y;
        int z = MemoryFunctions::map_view->LocalPlayer->z;


        if (x == map_x && y == map_y && z == map_z) {
            currentWpt = (currentWpt + 1) % m_waypoints.size();
            emit indexUpdate(currentWpt);
            msleep(300);
            continue;
        }

        if ((x != map_x || y != map_y || z != map_z) && !MemoryFunctions::isAttacking()) {
            if (option == "Center") {
                MemoryFunctions::moveTo(map_x, map_y, map_z);
            } else {
                if (option == "North") {
                    MemoryFunctions::moveTo(map_x, map_y - 1, map_z);
                } else if (option == "South") {
                    MemoryFunctions::moveTo(map_x, map_y + 1, map_z);
                } else if (option == "East") {
                    MemoryFunctions::moveTo(map_x + 1, map_y, map_z);
                } else if (option == "West") {
                    MemoryFunctions::moveTo(map_x - 1, map_y, map_z);
                }
                currentWpt = (currentWpt + 1) % m_waypoints.size();
                emit indexUpdate(currentWpt);
            }
        }
        msleep(500);
    }
}


