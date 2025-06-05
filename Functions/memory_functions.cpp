#include "memory_functions.h"
#include <iostream>

MapView* MemoryFunctions::map_view = nullptr;
uint64_t MemoryFunctions::base_module = 0;
uint64_t MemoryFunctions::local_player_address = 0;

MemoryFunctions::LoadOption MemoryFunctions::load_functions_variant = LoadOption::Altaron;
void* MemoryFunctions::move_func_address = nullptr;
void* MemoryFunctions::attack_func_address = nullptr;
void* MemoryFunctions::open_func_address = nullptr;
void* MemoryFunctions::collect_func_address = nullptr;
void* MemoryFunctions::say_func_address = nullptr;
void* MemoryFunctions::creature_func_address = nullptr;


MemoryFunctions::MemoryFunctions(LoadOption load_option) {
    load_functions_variant = load_option;
    base_module = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if (load_option == LoadOption::Medivia) { // Loading Medivia Memories
        // Variables
        uint64_t address = *reinterpret_cast<uint64_t *>(*reinterpret_cast<uint64_t *>(base_module + 0x00BEBA98));
        map_view = reinterpret_cast<MapView*>(address);
        local_player_address = base_module + 0xBEB4E0;

        // Functions Signatures
        move_func_address = reinterpret_cast<void *>(base_module + 0x14A0F0);
        attack_func_address = reinterpret_cast<void *>(base_module + 0x1325E0);
        open_func_address = reinterpret_cast<void *>(base_module + 0x136190);
        collect_func_address = reinterpret_cast<void *>(base_module + 0x135610);
        say_func_address = reinterpret_cast<void *>(base_module + 0x136D80);
        creature_func_address = reinterpret_cast<void *>(base_module + 0x271160);
    }
}

void MemoryFunctions::moveTo(int x, int y, int z) {
    int pos[3] = {x, y, z};
    using mapClick_t = void(__fastcall *)(
        uint64_t a1,       // Local Player
        int* a2            // Coords to move
        );
    auto mapClick = reinterpret_cast<mapClick_t>(move_func_address);
    mapClick(reinterpret_cast<uint64_t>(map_view->LocalPlayer), pos);
}

void MemoryFunctions::attackTarget(uint64_t target_id) {
    using attack_t = void(__fastcall *)(
        uint64_t a1,  // Local Player - 0x110
        int** target, // TargetID
        uint64_t a3  // Unknown
        );
    uint64_t a1 = local_player_address - 0x110;
    int *a2 = reinterpret_cast<int*>(target_id);
    auto attack = reinterpret_cast<attack_t>(attack_func_address);
    attack(a1, &a2, 0);
}

void MemoryFunctions::openContainer(uint64_t container_id) {
    typedef __int64(__fastcall* OpenContainerFuncMedivia)(__int64 character_base, uint64_t *a2, __int64 *a3);
    uint64_t param_2 = container_id;
    long long param_3 = 0x0;
    long long param_1 = *reinterpret_cast<long long *>(&map_view->LocalPlayer - 0x110);
    ((OpenContainerFuncMedivia)open_func_address)(param_1, &param_2, &param_3);
}

void MemoryFunctions::collectItem() {
    typedef void(__fastcall* CollectItemFuncMedivia)(__int64 character_base, long long *a2, __int64 *a3, int a4);
    long long param_2 = 0x2C36F6D2D00;
    long long param_3 = 0xFFFF;
    int param_4 = 0x01;
    long long param_1 = *reinterpret_cast<long long *>(&map_view->LocalPlayer - 0x110);
    ((CollectItemFuncMedivia)collect_func_address)(param_1, &param_2, &param_3, param_4);
}

void MemoryFunctions::say() {
    typedef void(__fastcall* SayFuncMedivia)(__int64 character_base, int speak_option, int channel_number , void* text_to_speak);
    const char* text = "Hi";
    uintptr_t args[2];
    args[0] = (uintptr_t)text; // Text To speak
    args[1] = strlen(text); // Lenght of the text
    long long param_1 = *reinterpret_cast<long long *>(&map_view->LocalPlayer - 0x110);
    ((SayFuncMedivia)say_func_address)(param_1,1,0,args);
}


bool MemoryFunctions::isWalking() {
    int walking = *(DWORD*)(*(DWORD*)(base_module + 0x02F72BC0) + 0x248);
    if (walking == 65535) {
        return false;
    }
    return true;
}
bool MemoryFunctions::isAttacking() {;
    if (*reinterpret_cast<uint64_t *>(base_module + 0xBEB4E8) != 0) {
        return true;
    }
    return false;
}

std::vector<Entity*> MemoryFunctions::entityCount() {
    // Define a struct matching the output container (3 pointers: begin, end, capacity)
    struct EntityVector {
        void* begin;
        void* end;
        void* capacity;
    };
    using GetEntitiesAround_t = volatile int32_t* (__fastcall *)(
    int64_t a1,       // Map Address RCX
    EntityVector* a2, // Entities around
    int64_t a3,       // Pointer to your base coordinate struct (a3);
    bool a4,          // a4 flag (true to search all floors (z), false for only one floor)
    int a5,           //a5 (how far left)
    int a6,           //a6 (how far right)
    int a7,           //a7 (how far up)
    int a8            //a8 (how far down)
);
    int64_t worldPtr = local_player_address + 0x288;
    // initialize output container (begin=end=capacity=null)
    EntityVector outVec = {nullptr, nullptr, nullptr};
    // My coords
    int64_t baseCoordPtr = reinterpret_cast<uintptr_t>(&(map_view->LocalPlayer->x));
    // Scan only my floor
    bool includeLayers = false;
    // Scan Whole Screen
    int radiusX_neg = 7;
    int radiusX_pos = 7;
    int radiusY_neg = 5;
    int radiusY_pos = 5;

    // Call Function
    auto GetEntitiesAround = reinterpret_cast<GetEntitiesAround_t>(creature_func_address);
    GetEntitiesAround(worldPtr, &outVec, baseCoordPtr, includeLayers,radiusX_neg, radiusX_pos, radiusY_neg, radiusY_pos);

    // Retrun empty vector if there are no Entities
    if (!outVec.begin || !outVec.end)
        return {};

    // Assign and return list of Entities:
    Entity** entities = reinterpret_cast<Entity**>(outVec.begin);
    Entity** entities_end = reinterpret_cast<Entity**>(outVec.end);
    return std::vector<Entity*>(entities, entities_end);
}




