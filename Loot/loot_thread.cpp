#include "loot_thread.h"
#include <iostream>
#include "../Functions/memory_functions.h"
#include "../Structs/medivia_struct.h"

void LootThread::run() {
    Container* container = MemoryFunctions::queue_getContainer(m_container_index);
    if (!container || m_items.isEmpty() || container->number_of_items <= 0)
        return;
    auto containers = MemoryFunctions::queue_getContainers();
    for (auto loot : m_items) {
        int item_id = loot.value("id").toInt();
        std::string container_name = loot.value("container").toString().toStdString();
        for (int i = container->number_of_items -1; i >= 0; i--) {
            Item* item = MemoryFunctions::queue_getItem(container, i);
            if (item->id == item_id)
            {
                std::transform(container_name.begin(), container_name.end(), container_name.begin(),[](unsigned char c){ return std::tolower(c); });
                for (int j = 0; j < containers.size(); ++j) {
                    if (containers[j]->name == container_name) {
                        if (containers[j]->number_of_items == containers[j]->capacity) {
                            MemoryFunctions::queue_open(containers[j]->item, containers[j]);
                            msleep(300);
                        }
                        MemoryFunctions::queue_move(item, containers[j], j);
                    }
                }
            }
        }
    }

}



