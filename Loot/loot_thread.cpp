#include "loot_thread.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <unordered_map>

#include "../Functions/Game.h"
#include "../Structs/medivia_struct.h"

inline std::string to_lower_copy(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
    return s;
}
void ensure_space_in(Container* dest) {
    for (int k = dest->number_of_items - 1; k >= 0; --k) {
        if (Item* open = Game::queue_getItem(dest, k)) {
            Game::queue_open(open, dest);
        }
    }
}

struct DestInfo {
    Container* ptr{};
    int index{-1};
};

void LootThread::run() {
    while (m_running && !m_items.empty()) {
        const auto containers = Game::queue_getContainers();
        if (containers.empty()) {
            msleep(100);
            continue;
        }
        std::unordered_map<std::string, DestInfo> by_name;
        by_name.reserve(containers.size());
        for (int idx = 0; idx < static_cast<int>(containers.size()); ++idx) {
            Container* c = containers[idx];
            if (!c) continue;
            by_name.emplace(c->name, DestInfo{c, idx});
        }
        for (const auto& loot : m_items) {
            const int desired_item_id = loot.value("id").toInt();
            std::string desired_container_name = to_lower_copy(loot.value("container").toString().toStdString());

            const auto itDest = by_name.find(desired_container_name);
            if (itDest == by_name.end()) {
                continue;
            }
            Container* const dest = itDest->second.ptr;
            const int dest_index   = itDest->second.index;
            if (!dest || dest_index < 0) continue;

            for (Container* src : containers) {
                if (!src || src == dest) continue;
                for (int slot = src->number_of_items - 1; slot >= 0; --slot) {
                    Item* item = Game::queue_getItem(src, slot);
                    if (!item)               continue;
                    if (item->id != desired_item_id) continue;
                    if (dest->capacity == dest->number_of_items) {ensure_space_in(dest); msleep(500);}
                    Game::queue_move(item, dest, dest_index);
                    msleep(rand() % 500 + 500);
                }
            }
        }
        msleep(100);
    }
}
