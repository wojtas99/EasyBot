#include "walker_thread.h"

#include <iostream>

#include "../Functions/Game.h"
#include <QThread>

#include "../Target/target_thread.h"
#include "../BridgeAPI/EasyApi.h"
void WalkerThread::run()
{

    m_labelIndex.clear();
    for (int i = 0; i < m_waypoints.size(); ++i) {
        const auto &wp = m_waypoints[i];
        if (wp.value("option").toString() == "Label") {
            const QString name = wp.value("label").toString().trimmed();
            if (!name.isEmpty() && !m_labelIndex.contains(name))
                m_labelIndex.insert(name, i);
        }
    }
    ScriptRuntime runtime;
    int timer = 0, x = 0, y = 0;
    bool is_walking = false;
    bool jumped = false;
    int idx = 0;
    EasyApi::onLabel = [this, &idx, &jumped](const QString& name) { // NEW
        auto it = m_labelIndex.constFind(name);
        if (it != m_labelIndex.cend()) {
            idx = it.value();
            jumped = true;
            emit indexUpdate(idx);
        }
    };
    idx = find_wpt();
    while (m_running && !m_waypoints.isEmpty())
    {
        if (timer >= 500) { // Check if character is moving every 500 ms
            if (x == Game::map_view->LocalPlayer->x && y == Game::map_view->LocalPlayer->y) {
                is_walking = false;
            } else {
                is_walking = true;
            }
            x = Game::map_view->LocalPlayer->x;
            y = Game::map_view->LocalPlayer->y;
            timer = 0;
        }

        const auto& wpt = m_waypoints[idx];
        int map_x = wpt["x"].toInt();
        int map_y = wpt["y"].toInt();
        int map_z = wpt["z"].toInt();
        std::string option = wpt["option"].toString().toStdString();
        std::string direction = wpt["direction"].toString().toStdString();
        const QString action = wpt["action"].toString().trimmed();
        if (!action.isEmpty()) {
            ScriptResult res;
            runtime.exec(action, res);
            if (jumped) { jumped = false; continue; }
            msleep(1500);
        }


        if (direction != "C" && map_z != Game::map_view->LocalPlayer->z) {
            idx = (idx + 1) % m_waypoints.size();
            emit indexUpdate(idx);
            continue;
        }

        if (Game::map_view->LocalPlayer->x == map_x &&
            Game::map_view->LocalPlayer->y == map_y &&
            Game::map_view->LocalPlayer->z == map_z) {
            if (idx == (idx + 1) % m_waypoints.size()) {continue;}
            idx = (idx + 1) % m_waypoints.size();
            emit indexUpdate(idx);
            continue;
        }
        if (((!Game::has_loot && !Game::has_target) || option == "Lure") &&
            (option != "Action" && option != "Label" && option != "Use")) {
            Game::queue_autoWalk(map_x, map_y, map_z);
        }
        if (Game::queue_isAttacking() && option != "Lure" && is_walking)
        {
            Game::queue_stop();
            is_walking = false;
        }
        msleep(100);
        timer += 100;
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

        if (Game::map_view->LocalPlayer->z == map_z &&
            abs(static_cast<int>(map_x - Game::map_view->LocalPlayer->x)) <= 7 &&
            abs(static_cast<int>(map_y - Game::map_view->LocalPlayer->y)) <= 5 &&
            direction == "C"  || option == "Lure") {
            std::cout << "Found wpt" << std::endl;
            return idx;
        }
    }
    return 0;
}

