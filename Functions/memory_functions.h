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

    static uintptr_t getTopItem(HANDLE hProcess, uintptr_t tileList, int coords[3]);
    static void queueAttack(Entity* entity);
    static void queueMove(int x, int y, int z);
    static void queueOpenItem(Item* item);
    static void queueOpenCorpse(Tile* tile);
    static std::vector<Container*> listContainers();
    static std::vector<void*> getFieldsAround(int radius);
    static void queueMoveItem(Item* item_src, Item* item_dest);

    explicit MemoryFunctions(LoadOption load_option);

    static void moveTo(int x, int y, int z);
    static void attackTarget(Entity* target);
    static void openItem(Item* item);
    static void openCorpse(Tile* tile);

    static void collectItem(__int64 container, __int64 item);

    static void collectItem(Item* item_src, Item* item_dest);

    static __int64 findItem(__int64 container);

    static Item* getItem(Container *container, int index);

    static __int64 findItems(__int64 container);

    static std::vector<Entity*> entityCount(int radius);

    static __int64 getTile(uint32_t x, uint32_t y, uint16_t z);

    static __int64 getTopThing(__int64 tile);

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
