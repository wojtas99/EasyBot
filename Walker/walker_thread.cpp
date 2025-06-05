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

        uint32_t x = MemoryFunctions::map_view->LocalPlayer->x;
        uint32_t y = MemoryFunctions::map_view->LocalPlayer->y;
        uint16_t z = MemoryFunctions::map_view->LocalPlayer->z;


        if (x == map_x && y == map_y && z == map_z) {
            currentWpt = (currentWpt + 1) % m_waypoints.size();
            emit indexUpdate(currentWpt);
            msleep(300);
            continue;
        }
        if (x != map_x || y != map_y || z != map_z) {
            MemoryFunctions::moveTo(map_x, map_y, map_z);
        }
        msleep(100);
    }
}


