#include "memory_functions.h"
#include <iostream>
#include <qglobal.h>
#include <DbgHelp.h>

#include "../PatternScans/pattern_scan.h"
MapView* MemoryFunctions::map_view = nullptr;
PlayerBase* MemoryFunctions::player_base = nullptr;
uint64_t MemoryFunctions::base_module = 0;
uint64_t MemoryFunctions::local_player_address = 0;

MemoryFunctions::LoadOption MemoryFunctions::load_functions_variant = LoadOption::Altaron;

void* MemoryFunctions::move_func_address = nullptr;
void* MemoryFunctions::attack_func_address = nullptr;
void* MemoryFunctions::open_func_address = nullptr;
void* MemoryFunctions::collect_func_address = nullptr;
void* MemoryFunctions::say_func_address = nullptr;
void* MemoryFunctions::creature_func_address = nullptr;
void* MemoryFunctions::chase_func_address = nullptr;


MemoryFunctions::MemoryFunctions(LoadOption load_option) {
    load_functions_variant = load_option;
    base_module = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if (load_option == LoadOption::Medivia) { // Loading Medivia Memories
        // Variables
        uint64_t address = *reinterpret_cast<uint64_t*>(*reinterpret_cast<uint64_t*>(base_module + 0x00CDAE28));
        map_view = reinterpret_cast<MapView*>(address);
        player_base = reinterpret_cast<PlayerBase*>(base_module + 0xCDA878 - 0X118);

        const BYTE MOVE_PATTERN[] = {
            0x55, 0x41, 0x57, 0x41, 0x56, 0x41, 0x55, 0x41, 0x54, 0x56, 0x57, 0x53, 0x48, 0x81, 0xEC,
            0x00, 0x00, 0x00, 0x00,
            0x48, 0x8D, 0xAC, 0x24,
            0x00, 0x00, 0x00, 0x00,
            0x48, 0xC7, 0x45,
            0x00, 0x00, 0x00, 0x00, 0x00,
            0x48, 0x89, 0xCE, 0x8B, 0x89
        };
        LPCSTR MOVE_MASK = "xxxxxxxxxxxxxxx????xxxx????xxx?????xxxxx";

        move_func_address = FindPattern(MOVE_PATTERN, MOVE_MASK);

        std::cout << move_func_address << std::endl;

        const BYTE ATTACK_PATTERN[] = {
            0x55, 0x56, 0x53, 0x48, 0x83, 0xEC, 0x00,
            0x48, 0x8D, 0x6C, 0x24, 0x00,
            0x48, 0xC7, 0x45,
            0x00, 0x00, 0x00, 0x00, 0x00,
            0x48, 0x89, 0x55
        };
        LPCSTR ATTACK_MASK = "xxxxxx?xxxx?xxx?????xxx";

        attack_func_address = FindPattern(ATTACK_PATTERN, ATTACK_MASK);

        std::cout << attack_func_address << std::endl;


        const BYTE OPEN_PATTERN[] = {
            0x41, 0x57, 0x41, 0x56, 0x41, 0x54, 0x56, 0x57, 0x53, 0x48, 0x83, 0xEC, 0x00,
            0x48, 0x8B, 0x05, 0x00, 0x00, 0x00, 0x00,
            0x48, 0x31, 0xE0, 0x48, 0x89, 0x44, 0x24, 0x00, 0xBE
        };
        LPCSTR OPEN_MASK = "xxxxxxxxxxxx?xxx????xxxxxxx?x";


        open_func_address = FindPattern(OPEN_PATTERN, OPEN_MASK);
        std::cout << open_func_address << std::endl;

        const BYTE CREATURE_PATTERN[] = {
            0x55, 0x41, 0x57, 0x41, 0x56, 0x41, 0x55, 0x41,
            0x54, 0x56, 0x57, 0x53, 0x48, 0x81, 0xEC,
            0x00, 0x00, 0x00, 0x00,
            0x48, 0x8D, 0xAC, 0x24,
            0x00, 0x00, 0x00, 0x00,
            0x0F, 0x29, 0x75, 0x00,
            0x48, 0xC7, 0x45,
            0x00, 0x00, 0x00, 0x00, 0x00,
            0x4D, 0x89
        };
        LPCSTR CREATURE_MASK = "xxxxxxxxxxxxxxx????xxxx????xxx?xxx?????xx";

        creature_func_address = FindPattern(CREATURE_PATTERN, CREATURE_MASK);
        std::cout << creature_func_address << std::endl;

        std::cout << creature_func_address << std::endl;
        std::cout << attack_func_address << std::endl;
        std::cout << move_func_address << std::endl;
    }
}

bool MemoryFunctions::moveTo(int x, int y, int z) {
    // Decomp by IDA __int64 __fastcall sub_7FF69838A0F0(unsigned __int64 a1, unsigned int *a2)
    using mapClick_t = __int64(__fastcall *)(
        uint64_t a1,       // Local Player
        int* a2            // Coords to move
        );
    auto mapClick = reinterpret_cast<mapClick_t>(move_func_address);
    int pos[3] = {x, y, z};
    __int64 success = 0;
    __try {
        bool is_walking = MemoryFunctions::map_view->LocalPlayer->is_walking == 0xFFFF;
        bool is_attacking = MemoryFunctions::player_base->Entity != 0;
        if (is_walking && !is_attacking) {
            success = mapClick(reinterpret_cast<uint64_t>(map_view->LocalPlayer), pos);
            return true;
        }
        return false;
    }
    __except(EXCEPTION_EXECUTE_HANDLER) {
        std::cout << "Caught exception during map click" << std::endl;
    }
    return false;
}
bool MemoryFunctions::attackTarget(Entity* entity) {
    // Decomp by IDA volatile signed __int32 **__fastcall sub_7FF79045E8B0(__int64 a1, volatile signed __int32 **a2, char a3)
    using attack_t = volatile signed __int32 **(__fastcall *)(__int64 a1, volatile signed __int32 **a2, char a3);
    auto attack = reinterpret_cast<attack_t>(attack_func_address);
    __int64 a1 = reinterpret_cast<__int64>(player_base);
    volatile signed __int32* a2 = reinterpret_cast<volatile signed __int32*>(entity);
    volatile signed __int32** result = nullptr;
    std::cout << a1 << std::endl;
    std::cout << std::hex << entity << std::endl;
    __try {
        result = attack(a1, &a2, 0);
        std::cout << "Attack function returned: " << result << std::endl;
        return true;
    } __except(EXCEPTION_EXECUTE_HANDLER) {
        std::cerr << "Exception occurred during attack." << std::endl;
        return false;
    }
}

bool MemoryFunctions::attack(const std::string& target_name, int dist_threshold, Entity* entity) {
    __try {
        int dist_x = map_view->LocalPlayer->x - entity->x;
        dist_x = abs(dist_x);
        int dist_y = map_view->LocalPlayer->y - entity->y;
        dist_y = abs(dist_y);
        if (entity->name == target_name && entity->z == map_view->LocalPlayer->z && dist_y <= dist_threshold && dist_x <= dist_threshold) {\
            return true;
        }
    }__except(EXCEPTION_EXECUTE_HANDLER) {
        std::cout << "Caught exception during targeting!" << std::endl;
        return false;
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
    // Decomp by IDA volatile signed __int32 *__fastcall sub_7FF6984B1160(__int64 a1, _QWORD *a2, __int64 a3, char a4, int a5, int a6, int a7, int a8)
    using GetEntitiesAround_t = volatile signed __int32* (__fastcall *)(
    __int64 a1,       // Map Address RCX
    EntityVector* a2, // Entities around
    __int64 a3,       // Pointer to your base coordinate struct (a3);
    bool a4,          // a4 flag (true to search all floors (z), false for only one floor)
    int a5,           //a5 (how far left)
    int a6,           //a6 (how far right)
    int a7,           //a7 (how far up)
    int a8            //a8 (how far down)
);
    __int64 worldPtr = base_module + 0xCDA878 - 0X118 + 0X398;
    // initialize output container (begin=end=capacity=null)
    EntityVector outVec = {nullptr, nullptr, nullptr};
    // My coords
    __int64 baseCoordPtr = reinterpret_cast<__int64>(&map_view->LocalPlayer->x);
    // Scan only my floor
    bool includeLayers = false;
    // Scan Whole Screen
    int radiusX_neg = 7;
    int radiusX_pos = 7;
    int radiusY_neg = 5;
    int radiusY_pos = 5;

    // Call Function
    volatile signed __int32* success;
    auto GetEntitiesAround = reinterpret_cast<GetEntitiesAround_t>(creature_func_address);
    success = GetEntitiesAround(worldPtr, &outVec, baseCoordPtr, includeLayers,radiusX_neg, radiusX_pos, radiusY_neg, radiusY_pos);

    // Retrun empty vector if there are no Entities
    if (!outVec.begin || !outVec.end)
        return {};

    // Assign and return list of Entities:
    Entity** entities = reinterpret_cast<Entity**>(outVec.begin);
    Entity** entities_end = reinterpret_cast<Entity**>(outVec.end);
    return std::vector<Entity*>(entities, entities_end);
}




