#include "target_thread.h"
#include <iostream>
#include "../Functions/memory_functions.h"
#include <QThread>
#include "../Structs/medivia_struct.h"

void TargetThread::run() {
    std::string target_name;
    int dist_threshold = 0;
    while (m_running && !m_targets.isEmpty()) {
        try {
            target_name = m_targets[0].value("name").toString().toStdString();
            dist_threshold = m_targets[0].value("distance").toInt();
            if (MemoryFunctions::player_base->Entity == 0) {
                std::vector<Entity*> entities = MemoryFunctions::entityCount();
                for (Entity* entity : entities) {
                    if (MemoryFunctions::attack(target_name, dist_threshold, entity)) {
                        MemoryFunctions::attackTarget(entity);
                        msleep(1000);
                        break;
                    }
                }
            }
            msleep(500);
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
}


