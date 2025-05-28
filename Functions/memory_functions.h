#ifndef MEMORY_FUNCTIONS_H
#define MEMORY_FUNCTIONS_H
#include <array>
#include <Windows.h>


class MemoryFunctions {

public:
    enum class LoadOption {
        Altaron,
        Medivia,
    };

    static uintptr_t base_module;

    static std::array<int, 3> read_my_wpt();
    explicit MemoryFunctions(LoadOption load_option);
    static void moveTo(int x, int y, int z);
    static void attackTarget(uint64_t target_id);
    static void openContainer(uint64_t container_id);
    static void collectItem();
    static void say();


    static bool isWalking();
    static bool isAttacking();

private:
    static uintptr_t character_address;
    static uintptr_t my_x_address;
    static uintptr_t my_y_address;
    static uintptr_t my_z_address;

    //
    static void* move_func_address;
    static void* attack_func_address;
    static void* open_func_address;
    static void* collect_func_address;
    static void* say_func_address;

    static LoadOption load_functions_variant;
};

#endif //MEMORY_FUNCTIONS_H
