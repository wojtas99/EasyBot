#ifndef MEMORY_FUNCTIONS_H
#define MEMORY_FUNCTIONS_H
#include <array>
#include <vector>
#include <Windows.h>
#include "../Structs/medivia_struct.h"


class MemoryFunctions {

public:
    enum class LoadOption {
        Altaron,
        Medivia,
    };

    // Variables
    static MapView* map_view;

    explicit MemoryFunctions(LoadOption load_option);
    static void moveTo(int x, int y, int z);
    static void attackTarget(uint64_t target_id);
    static void openContainer(uint64_t container_id);
    static void collectItem();
    static void say();
    static std::vector<Entity*> entityCount();


    static bool isWalking();
    static uint64_t isAttacking();

private:
    // Variables
    static uintptr_t base_module;
    static uintptr_t local_player_address;
    // Function Addresses
    static void* move_func_address;
    static void* attack_func_address;
    static void* open_func_address;
    static void* collect_func_address;
    static void* say_func_address;
    static void* creature_func_address;

    static LoadOption load_functions_variant;
};

#endif //MEMORY_FUNCTIONS_H
