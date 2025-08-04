#ifndef MEMORY_FUNCTIONS_H
#define MEMORY_FUNCTIONS_H
#include <array>
#include <string>
#include <vector>
#include <Windows.h>
#include "../Structs/medivia_struct.h"
#include "../safe_queue.h"


class MemoryFunctions {

public:
    enum class LoadOption {
        Altaron,
        Medivia,
    };

    // Variables
    static MapView* map_view;
    static PlayerBase* player_base;
    static uint64_t base_module;
    static void* main_func_address;
    static bool has_target;

    static SafeQueue actionQueue;


    static void queue_attack(Entity* entity);
    static void attack(Entity* target);

    static void queue_autoWalk(int x, int y, int z);
    static void autoWalk(int x, int y, int z);


    static void queue_open(Item* item,Container* parent_container);
    static void open(Item* item, Container* parent_container);

    static std::vector<Container*> queue_getContainers();
    static std::vector<Container*> getContainers();

    static void queue_move(Item* item_src, Container* item_dest, int slot);
    static void move(Item* item_src, Container* container_dst, int slot);

    static __int64 queue_getTile(uint32_t x, uint32_t y, uint16_t z);
    static __int64 getTile(uint32_t x, uint32_t y, uint16_t z);

    static __int64 queue_getTopThing(__int64 tile);
    static __int64 getTopThing(__int64 tile);

    static Item* queue_getItem(Container *container, int index);
    static Item* getItem(Container *container, int index);

    static std::vector<Entity*> queue_getSpectatorsInRangeEx(int radius);
    static std::vector<Entity*> getSpectatorsInRangeEx(int radius);


    explicit MemoryFunctions(LoadOption load_option);


private:
    // Variables
    static uint64_t local_player_address;
    // Function Addresses
    static void* move_func_address;
    static void* attack_func_address;
    static void* open_func_address;
    static void* collect_func_address;
    static void* say_func_address;
    static void* creature_func_address;
    static void* chase_func_address;

    static LoadOption load_functions_variant;
};

#endif //MEMORY_FUNCTIONS_H
