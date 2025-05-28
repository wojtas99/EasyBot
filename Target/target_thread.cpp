#include "target_thread.h"
#include <iostream>
#include "../Functions/memory_functions.h"
#include <QThread>
#include <__msvc_ostream.hpp>
#include "../Structs/medivia_struct.h"

void TargetThread::run() {
    while (m_running && !m_targets.isEmpty()) {

        const QVariantMap& target_data = m_targets[0];
        std::string target_name = target_data.value("name").toString().toStdString();
        Entity* entity = nullptr;
        uint64_t address = *reinterpret_cast<uint64_t *>(*reinterpret_cast<uint64_t *>(MemoryFunctions::base_module + 0x00BEBA98));
        MapView *map_view = reinterpret_cast<MapView*>(address);

        if (!MemoryFunctions::isAttacking()) {
            uint64_t *entity_list = map_view->entityList;
            for (int i = 0; i < 3; ++i) {
                entity = reinterpret_cast<Entity*>(*entity_list++);
                std::cout << entity << std::endl;
                if (target_name == entity->name) {
                    MemoryFunctions::attackTarget(reinterpret_cast<uint64_t>(entity));
                    break;
                }
            }
        }
        while (MemoryFunctions::isAttacking()) {
            msleep(50);
        }
        msleep(100);
    }
}


