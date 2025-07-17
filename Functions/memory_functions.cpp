#include "memory_functions.h"
#include <iostream>
#include <qglobal.h>
#include <DbgHelp.h>

#include "../PatternScans/pattern_scan.h"
#include "../include/MinHook.h"
MapView* MemoryFunctions::map_view = nullptr;
PlayerBase* MemoryFunctions::player_base = nullptr;
uint64_t MemoryFunctions::base_module = 0;
uint64_t MemoryFunctions::local_player_address = 0;
bool MemoryFunctions::has_target = false;

MemoryFunctions::LoadOption MemoryFunctions::load_functions_variant = LoadOption::Altaron;

void* MemoryFunctions::main_func_address = nullptr;
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

        const BYTE MAIN_PATTERN[] = {
            0x41, 0x57, 0x41, 0x56, 0x41, 0x55, 0x41, 0x54, 0x56, 0x57, 0x55, 0x53, 0x48, 0x81, 0xEC,
            0x00, 0x00, 0x00, 0x00,
            0x66, 0x0F, 0x29, 0xBC, 0x24
        };
        LPCSTR MAIN_MASK = "xxxxxxxxxxxxxxx????xxxxx";

        main_func_address = FindPattern(MAIN_PATTERN, MAIN_MASK);

        std::cout << "Main Loop Address: " << main_func_address << std::endl;

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
        //4

        std::cout << "Move Func Address: "<< move_func_address << std::endl;

        const BYTE ATTACK_PATTERN[] = {
            0x55, 0x56, 0x53, 0x48, 0x83, 0xEC, 0x00,
            0x48, 0x8D, 0x6C, 0x24, 0x00,
            0x48, 0xC7, 0x45,
            0x00, 0x00, 0x00, 0x00, 0x00,
            0x48, 0x89, 0x55
        };
        LPCSTR ATTACK_MASK = "xxxxxx?xxxx?xxx?????xxx";

        attack_func_address = FindPattern(ATTACK_PATTERN, ATTACK_MASK);
        //00007FF6113EE8B0
        //5

        std::cout << "Attack Func Address: " << attack_func_address << std::endl;


        const BYTE OPEN_PATTERN[] = {
            0x41, 0x57, 0x41, 0x56, 0x41, 0x54, 0x56, 0x57, 0x53, 0x48, 0x83, 0xEC, 0x00,
            0x48, 0x8B, 0x05, 0x00, 0x00, 0x00, 0x00,
            0x48, 0x31, 0xE0, 0x48, 0x89, 0x44, 0x24, 0x00, 0xBE
        };
        LPCSTR OPEN_MASK = "xxxxxxxxxxxx?xxx????xxxxxxx?x";


        open_func_address = FindPattern(OPEN_PATTERN, OPEN_MASK);
        std::cout << "Open Func Addres: " << open_func_address << std::endl;

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
        std::cout << "Creatures Func Address: " << creature_func_address << std::endl;
    }
}

void MemoryFunctions::moveTo(int x, int y, int z) {
    // Decomp by IDA for Medivia __int64 __fastcall sub_7FF69838A0F0(unsigned __int64 a1, unsigned int *a2)
    using mapClick_t = __int64(__fastcall *)(
        uint64_t a1,       // Local Player
        int* a2            // Coords to move
        );
    auto mapClick = reinterpret_cast<mapClick_t>(move_func_address);
    int pos[3] = {x, y, z};
    mapClick(reinterpret_cast<uint64_t>(map_view->LocalPlayer), pos);
}
void MemoryFunctions::attackTarget(Entity* entity) {
    // Decomp by IDA for Medivia volatile signed __int32 **__fastcall sub_7FF79045E8B0(__int64 a1, volatile signed __int32 **a2, char a3)
    using attackTarget_t = volatile signed __int32 **(__fastcall *)(
        __int64 a1, // Player Base
        volatile signed __int32 **a2, // Target ID
        char a3 // 0
        );
    auto attack = reinterpret_cast<attackTarget_t>(attack_func_address);
    auto a1 = reinterpret_cast<__int64>(player_base);
    volatile auto* a2 = reinterpret_cast<volatile signed __int32*>(entity);
    attack(a1, &a2, 0);
}
std::vector<Entity*> MemoryFunctions::entityCount(int radius) {
    // Define a struct matching the output container (3 pointers: begin, end, capacity)
    struct EntityVector {
        void* begin;
        void* end;
        void* capacity;
    };
    // Decomp by IDA for Medivia volatile signed __int32 *__fastcall sub_7FF6984B1160(__int64 a1, _QWORD *a2, __int64 a3, char a4, int a5, int a6, int a7, int a8)
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
    int radiusX_neg = radius;
    int radiusX_pos = radius;
    int radiusY_neg = radius;
    int radiusY_pos = radius;

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

struct LookupKey {
    uint32_t x;
    uint32_t y;
    uint16_t z;
};

using LookupFieldFunc_t = __int64* (__fastcall *)(__int64 tablePtr, __int64 keyPtr);

std::vector<void*> MemoryFunctions::getFieldsAround(int radius) {
    auto LookupField = reinterpret_cast<LookupFieldFunc_t>(MemoryFunctions::base_module + 0x298750);
    __int64 tablePtr = 0x7FF7925CAAF8;

    std::vector<void*> resultFields;

    int localX = map_view->LocalPlayer->x;
    int localY = map_view->LocalPlayer->y;
    int localZ = map_view->LocalPlayer->z;

    for (int dx = -radius; dx <= radius; ++dx) {
        for (int dy = -radius; dy <= radius; ++dy) {
            LookupKey key{};
            key.x = localX + dx;
            key.y = localY + dy;
            key.z = localZ;

            __int64* fieldPtr = LookupField(tablePtr, reinterpret_cast<__int64>(&key));

            if (fieldPtr && fieldPtr  != reinterpret_cast<__int64*>(0x7FF70B7EA470)) {
                resultFields.push_back(reinterpret_cast<void*>(fieldPtr));
            }
        }
    }

    return resultFields;
}

void MemoryFunctions::collectItem(Item* item) {
    //Decomp by IDA for Medivia void __fastcall sub_7FF791A31B00(__int64 a1, void (__fastcall ****a2)(__int64, __int64), __int64 a3, int a4)
    using collectItem_t = void(__fastcall *)(
    __int64 a1, // RCX - Player Base
    __int64 *a2, // RDX - Item ID Src
    __int64 *a3, // R8 - Item ID Dst
    __int64 a4 // R9 - Number of objects moved
    );
    auto a1 = reinterpret_cast<__int64>(player_base);
    auto a2 = reinterpret_cast<__int64>(0x3AD7DF72B00);
    auto a3 = reinterpret_cast<__int64>(0x3AD7DF76000);
    auto a4 = 1;
    auto collect = reinterpret_cast<collectItem_t>(base_module + 0x141B00);
    collect(a1, &a2, &a3, a4);

}

void MemoryFunctions::openItem(Item* item)
{
    using openItem_t = __int64(__fastcall *)(
        __int64 a1, // Player Base
        uint64_t* a2, // Item ID
    __int64 *a3 // 0
    );
    auto open = reinterpret_cast<openItem_t>(open_func_address);
    auto a1 = reinterpret_cast<__int64>(player_base);
    auto a2 = reinterpret_cast<uint64_t>(item);
    long long a3 = 0x0;
    open(a1, &a2, &a3);
}


void MemoryFunctions::queueAttack(Entity* entity) {
    actionQueue.enqueue([entity]() {
        attackTarget(entity);
    });
}
void MemoryFunctions::queueMove(int x, int y, int z) {
    actionQueue.enqueue([x, y, z]() {
        moveTo(x, y, z);
    });
}

void MemoryFunctions::queueOpenItem(Item* item) {
    actionQueue.enqueue([item]() {
        openItem(item);
    });
}

void MemoryFunctions::queueMoveItem(Item* item) {
    actionQueue.enqueue([item]() {
        collectItem(item);
    });
}


uintptr_t MemoryFunctions::getTopItem(HANDLE hProcess, uintptr_t tileList, int coords[3])
{
    constexpr int maxItems = 10;
    constexpr SIZE_T ptrSize = 8;
    uintptr_t topItemPtr = 0;
    uint64_t itemId = 0;
    uintptr_t currentItemPtr = 0;
    uint64_t itemrealId = 0;

    for (int i = 0; i < maxItems; ++i)
    {
        if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(tileList + i * ptrSize), &currentItemPtr, sizeof(currentItemPtr), nullptr))
            break;
        if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(currentItemPtr), &itemId, sizeof(itemId), nullptr))
            continue;
        if (i == 0) {
            itemrealId = itemId;
        }
        if (itemId == itemrealId)
        {
            std::cout << std::hex << currentItemPtr << std::endl;
            /*
            Item* item = reinterpret_cast<Item*>(currentItemPtr);
            if (item->x != coords[0] || item->y != coords[1] || item->z != coords[2]) {
                break;
            }
            */
            topItemPtr = currentItemPtr;
        } else
        {
            break;
        }
    }

    return topItemPtr;
}


