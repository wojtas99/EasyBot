#include "target_thread.h"
#include <iostream>
#include "../Functions/memory_functions.h"
#include <QThread>
#include "../Structs/medivia_struct.h"

void TargetThread::run() {
    uint64_t address = *reinterpret_cast<uint64_t *>(*reinterpret_cast<uint64_t *>(MemoryFunctions::base_module + 0x00BEBA98));
    while (m_running && !m_targets.isEmpty()) {

        const QVariantMap& target_data = m_targets[0];
        std::string target_name = target_data.value("name").toString().toStdString();
        MapView *map_view = reinterpret_cast<MapView*>(address);
        if (!MemoryFunctions::isAttacking()) {
            uint64_t *entity_list = map_view->entityList;
            int count = MemoryFunctions::entityCount();
            for (int i = 0; i < count; i++ ) {
                Entity* entity = reinterpret_cast<Entity*>(*entity_list++);
                if (target_name == entity->name) {
                    MemoryFunctions::attackTarget(reinterpret_cast<uint64_t>(entity));
                    msleep(500);
                    break;
                }
            }
        }
        msleep(100);
    }
}


