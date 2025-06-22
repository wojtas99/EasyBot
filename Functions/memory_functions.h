#ifndef MEMORY_FUNCTIONS_H
#define MEMORY_FUNCTIONS_H
#include <array>
#include <string>
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
    static PlayerBase* player_base;
    static uintptr_t base_module;

    explicit MemoryFunctions(LoadOption load_option);

    static bool moveTo(int x, int y, int z);
    static bool attackTarget(Entity* target);
    static bool attack(const std::string &target_name, int dist_threshold, Entity* entity);

    static std::vector<Entity*> entityCount();

private:
    // Variables
    static uintptr_t local_player_address;
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
