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
void* MemoryFunctions::getTile_func_address = nullptr;
void* MemoryFunctions::getSpectatorsInRangeEx_func_address = nullptr;
void* MemoryFunctions::autoWalk_func_address = nullptr;
void* MemoryFunctions::stop_func_address = nullptr;
void* MemoryFunctions::move_func_address = nullptr;
void* MemoryFunctions::open_func_address = nullptr;
void* MemoryFunctions::attack_func_address = nullptr;
void* MemoryFunctions::talkChannel_func_address = nullptr;
void* MemoryFunctions::isAttacking_func_address = nullptr;
void* MemoryFunctions::getContainer_func_address = nullptr;
void* MemoryFunctions::getItem_func_address = nullptr;
void* MemoryFunctions::isContainer_func_address = nullptr;
void* MemoryFunctions::isLyingCorpse_func_address = nullptr;
void* MemoryFunctions::getTopThing_func_address = nullptr;


MemoryFunctions::MemoryFunctions(LoadOption load_option) {
    load_functions_variant = load_option;
    base_module = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if (load_option == LoadOption::Medivia) { // Loading Medivia Memories
        // Variables
        uint64_t address = *reinterpret_cast<uint64_t*>(*reinterpret_cast<uint64_t*>(base_module + 0x00CDAE28));
        map_view = reinterpret_cast<MapView*>(address);
        player_base = reinterpret_cast<PlayerBase*>(base_module + 0xCDA878 - 0X118);

        const BYTE main_PATTERN[] = {
            0x41, 0x57, 0x41, 0x56, 0x41, 0x55, 0x41, 0x54, 0x56, 0x57, 0x55, 0x53, 0x48, 0x81, 0xEC,
            0x00, 0x00, 0x00, 0x00,
            0x66, 0x0F, 0x29, 0xBC, 0x24
        };
        LPCSTR main_MASK = "xxxxxxxxxxxxxxx????xxxxx";

        main_func_address = FindPattern(main_PATTERN, main_MASK);

        std::cout << "Main Loop Address: " << main_func_address << std::endl;

        const BYTE getTile_PATTERN[] = {
            0x41, 0x57, 0x41, 0x56, 0x41, 0x54, 0x56, 0x57, 0x55, 0x53, 0x44, 0x8B, 0x02
        };
        LPCSTR getTile_MASK = "xxxxxxxxxxxxx";

        getTile_func_address = FindPattern(getTile_PATTERN, getTile_MASK);

        std::cout << "getTile Func Address: "<< getTile_func_address << std::endl;

        const BYTE getSpectatorsInRangeEx_PATTERN[] = {
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
        LPCSTR getSpectatorsInRangeEx_MASK = "xxxxxxxxxxxxxxx????xxxx????xxx?xxx?????xx";

        getSpectatorsInRangeEx_func_address = FindPattern(getSpectatorsInRangeEx_PATTERN, getSpectatorsInRangeEx_MASK);
        std::cout << "getSpectatorsInRangeEx Func Address: " << getSpectatorsInRangeEx_func_address << std::endl;

        const BYTE autoWalk_PATTERN[] = {
            0x55, 0x41, 0x57, 0x41, 0x56, 0x41, 0x55, 0x41, 0x54, 0x56, 0x57, 0x53, 0x48, 0x81, 0xEC,
            0x00, 0x00, 0x00, 0x00,
            0x48, 0x8D, 0xAC, 0x24,
            0x00, 0x00, 0x00, 0x00,
            0x48, 0xC7, 0x45,
            0x00, 0x00, 0x00, 0x00, 0x00,
            0x48, 0x89, 0xCE, 0x8B, 0x89
        };
        LPCSTR autoWalk_MASK = "xxxxxxxxxxxxxxx????xxxx????xxx?????xxxxx";

        autoWalk_func_address = FindPattern(autoWalk_PATTERN, autoWalk_MASK);

        std::cout << "autoWalk Func Address: "<< autoWalk_func_address << std::endl;

        const BYTE stop_PATTERN[] = {
            0x56, 0x48, 0x83, 0xEC,
            0x00,
            0x48, 0x8B, 0x05,
            0x00, 0x00, 0x00, 0x00,
            0x48, 0x31, 0xE0, 0x48, 0x89, 0x44, 0x24,
            0x00,
            0x80, 0x39
        };
        LPCSTR stop_MASK = "xxxx?xxx????xxxxxxx?xx";

        stop_func_address = FindPattern(stop_PATTERN, stop_MASK);

        std::cout << "Stop Func Address: " << stop_func_address << std::endl;

        const BYTE move_PATTERN[] = {
            0x41, 0x57, 0x41, 0x56, 0x41, 0x54, 0x56, 0x57, 0x53, 0x48, 0x83, 0xEC,
            0x00,
            0x41, 0x83, 0xF9,
        };
        LPCSTR move_MASK = "xxxxxxxxxxxx?xxx";

        move_func_address = FindPattern(move_PATTERN, move_MASK);

        std::cout << "Move Func Address: " << move_func_address << std::endl;

        const BYTE open_PATTERN[] = {
            0x41, 0x57, 0x41, 0x56, 0x41, 0x54, 0x56, 0x57, 0x53, 0x48, 0x83, 0xEC, 0x00,
            0x48, 0x8B, 0x05, 0x00, 0x00, 0x00, 0x00,
            0x48, 0x31, 0xE0, 0x48, 0x89, 0x44, 0x24, 0x00, 0xBE
        };
        LPCSTR open_MASK = "xxxxxxxxxxxx?xxx????xxxxxxx?x";


        open_func_address = FindPattern(open_PATTERN, open_MASK);
        std::cout << "Open Func Addres: " << open_func_address << std::endl;

        const BYTE attack_PATTERN[] = {
            0x55, 0x56, 0x53, 0x48, 0x83, 0xEC, 0x00,
            0x48, 0x8D, 0x6C, 0x24, 0x00,
            0x48, 0xC7, 0x45,
            0x00, 0x00, 0x00, 0x00, 0x00,
            0x48, 0x89, 0x55
        };
        LPCSTR attack_MASK = "xxxxxx?xxxx?xxx?????xxx";

        attack_func_address = FindPattern(attack_PATTERN, attack_MASK);

        std::cout << "Attack Func Address: " << attack_func_address << std::endl;

        const BYTE talkChannel_PATTERN[] = {
            0x48, 0x83, 0xEC, 0x00,
            0x48, 0x8B, 0x05, 0x00, 0x00, 0x00, 0x00,
            0x48, 0x31, 0xE0,
            0x48, 0x89, 0x44, 0x24, 0x00,
            0x80, 0x39, 0x00,
            0x75, 0x00,
            0x48, 0x8B, 0x81, 0x00, 0x00, 0x00, 0x00,
            0x48, 0x85, 0xC0,
            0x74, 0x00,
            0x80, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x74, 0x00,
            0x80, 0x79, 0x00, 0x00,
            0x75, 0x00,
            0x48, 0x8B, 0x89, 0x00, 0x00, 0x00, 0x00,
            0x48, 0x85, 0xC9,
            0x74, 0x00,
            0x48, 0x8B, 0x81, 0x00, 0x00, 0x00, 0x00,
            0x48, 0x85, 0xC0,
            0x74, 0x00,
            0x80, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x75, 0x00,
            0x49, 0x83, 0x79
        };
        LPCSTR talkChannel_MASK = "xxx?xxx????xxxxxxx?xx?x?xxx????xxxx?xx?????x?xx??x?xxx????xxxx?xxx????xxxx?xx?????x?xxx";

        talkChannel_func_address = FindPattern(talkChannel_PATTERN, talkChannel_MASK);

        std::cout << "TalkChannel Func Address: " << talkChannel_func_address << std::endl;

        //isAttacking_func_address = nullptr;

        const BYTE getContainer_PATTERN[] = {
            0x56, 0x48, 0x83, 0xEC,
            0x00,
            0x48, 0x89, 0xD6, 0x48, 0x8B, 0x05,
            0x00, 0x00, 0x00, 0x00,
            0x48, 0x31, 0xE0, 0x48, 0x89, 0x44, 0x24,
            0x00,
            0x44, 0x89, 0x44, 0x24
        };
        LPCSTR getContainer_MASK = "xxxx?xxxxxx????xxxxxxx?xxxx";

        getContainer_func_address = FindPattern(getContainer_PATTERN, getContainer_MASK);

        std::cout << "getContainer Func Address: " << getContainer_func_address << std::endl;

        const BYTE getItem_PATTERN[] = {
            0x48, 0x89, 0xD0, 0x45, 0x85, 0xC0, 0x78,
            0x00,
            0x44, 0x39, 0x81,
        };
        LPCSTR getItem_MASK = "xxxxxxx?xxx";

        getItem_func_address = FindPattern(getItem_PATTERN, getItem_MASK);

        std::cout << "getItem Func Address: " << getItem_func_address << std::endl;

        //MemoryFunctions::isContainer_func_address = nullptr;
        //MemoryFunctions::isLyingCorpse_func_address = nullptr;
        const BYTE getTopThing_PATTERN[] = {
            0x41, 0x56, 0x56, 0x57, 0x53, 0x48, 0x83, 0xEC,
            0x00,
            0x48, 0x89, 0xD6,0x48, 0x8B, 0x59,
        };
        LPCSTR getTopThing_MASK = "xxxxxxxx?xxxxxx";

        getTopThing_func_address = FindPattern(getTopThing_PATTERN, getTopThing_MASK);

        std::cout << "getTopThing Func Address: " << getTopThing_func_address << std::endl;
    }
}

//  Functions

struct Position {
    uint32_t x;
    uint32_t y;
    uint16_t z;
};

__int64 MemoryFunctions::getTile(uint32_t x, uint32_t y, uint16_t z) {
    //Decomp by IDA for Medivia __int64 *__fastcall sub_7FF719EC8750(__int64 a1, __int64 a2)
    using getTile_t = __int64(__fastcall*)(
        __int64 a1,  // RCX - Tile Base ?
        Position* a2 // RAX - Corrds of tile
        );
    auto GetTile = reinterpret_cast<getTile_t>(getTile_func_address);
    __int64 a1 = base_module + 0xCDAAF8;
    Position position{};
    position.x = x;
    position.y = y;
    position.z = z;
    __int64 result = GetTile(a1, &position);
    result = *reinterpret_cast<__int64*>(result);
    return result;
}

std::vector<Entity*> MemoryFunctions::getSpectatorsInRangeEx(int radius) {
    // Define a struct matching the output container (3 pointers: begin, end, capacity)
    struct EntityVector {
        void* begin;
        void* end;
        void* capacity;
    };
    // Decomp by IDA for Medivia volatile signed __int32 *__fastcall sub_7FF6984B1160(__int64 a1, _QWORD *a2, __int64 a3, char a4, int a5, int a6, int a7, int a8)
    using getSpectatorsInRangeEx_t = volatile signed __int32* (__fastcall *)(
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
    auto GetSpectatorsInRangeEx = reinterpret_cast<getSpectatorsInRangeEx_t>(getSpectatorsInRangeEx_func_address);
    GetSpectatorsInRangeEx(worldPtr, &outVec, baseCoordPtr, includeLayers,radiusX_neg, radiusX_pos, radiusY_neg, radiusY_pos);

    // Retrun empty vector if there are no Entities
    if (!outVec.begin || !outVec.end)
        return {};

    // Assign and return list of Entities:
    Entity** entities = reinterpret_cast<Entity**>(outVec.begin);
    Entity** entities_end = reinterpret_cast<Entity**>(outVec.end);
    return std::vector<Entity*>(entities, entities_end);
}


void MemoryFunctions::autoWalk(int x, int y, int z) {
    // Decomp by IDA for Medivia __int64 __fastcall sub_7FF69838A0F0(unsigned __int64 a1, unsigned int *a2)
    using autoWalk_t = __int64(__fastcall *)(
        uint64_t a1,       // Local Player
        int* a2            // Coords to move
        );
    auto AutoWalk = reinterpret_cast<autoWalk_t>(autoWalk_func_address);
    int pos[3] = {x, y, z};
    AutoWalk(reinterpret_cast<uint64_t>(map_view->LocalPlayer), pos);
}

void MemoryFunctions::stop()
{
    //Decomp by IDA for Medivia void __fastcall sub_7FF780D51980(__int64 a1)
    using stop_t = void(__fastcall*)(
        __int64 a1  // RCX - Player Base
        );
    auto Stop = reinterpret_cast<stop_t>(stop_func_address);
    auto a1 = reinterpret_cast<__int64>(player_base);
    Stop(a1);
}

// Look = 0x141A70

struct CollectKey {
    uint32_t x;      // offset 0x00 Nothing 0xFFFF
    uint32_t y;      // offset 0x04 Some kind of ID container
    uint16_t z;         // offset 0x08 Slot to move
    uint64_t ptrItem;   // offset 0x0C Item address
};


void MemoryFunctions::move(Item* item_src, Container* container_dst, int slot) {
    //Decomp by IDA for Medivia void __fastcall sub_7FF791A31B00(__int64 a1, void (__fastcall ****a2)(__int64, __int64), __int64 a3, int a4)
    using move_t = void(__fastcall *)(
    __int64 a1, // RCX - Player Base
    __int64 a2, // RDX - Item ID Src
    __int64 a3, // R8 - Item ID Src
    __int64 a4 // R9 - Number of objects moved
    );
    auto a1 = reinterpret_cast<__int64>(player_base);
    CollectKey container{};
    // Get Position Dest item
    container.x = 0xFFFF;
    container.y = 128 + slot;
    container.z = container_dst->capacity - 1;
    container.ptrItem = reinterpret_cast<uint64_t>(item_src);

    auto Move = reinterpret_cast<move_t>(move_func_address);

    Move(a1, reinterpret_cast<__int64>(&container.ptrItem), reinterpret_cast<__int64>(&container), item_src->count);
}

int MemoryFunctions::open(Item* item, Container* parent_container)
{
    using open_t = __int64(__fastcall *)(
        __int64 a1, // Player Base
        uint64_t *a2, // Item ID
        uint64_t *a3 // Parent Container ID
    );
    auto Open = reinterpret_cast<open_t>(open_func_address);
    auto a1 = reinterpret_cast<__int64>(player_base);
    auto a2 = reinterpret_cast<uint64_t>(item);
    auto a3 = reinterpret_cast<uint64_t>(parent_container);
    return Open(a1, &a2, &a3);
}


void MemoryFunctions::attack(Entity* entity) {
    // Decomp by IDA for Medivia volatile signed __int32 **__fastcall sub_7FF79045E8B0(__int64 a1, volatile signed __int32 **a2, char a3)
    using attack_t = volatile signed __int32 **(__fastcall *)(
        __int64 a1, // Player Base
        volatile signed __int32 **a2, // Target ID
        char a3 // 0
        );
    auto Attack = reinterpret_cast<attack_t>(attack_func_address);
    auto a1 = reinterpret_cast<__int64>(player_base);
    volatile auto* a2 = reinterpret_cast<volatile signed __int32*>(entity);
    Attack(a1, &a2, 0);
}

void MemoryFunctions::talkChannel(const char *message)
{
    //Decomp by IDA for Medivia void __fastcall sub_7FF771812E50(__int64 a1, int a2, int a3, __int128 *a4)
    using talkChannel_t = void(__fastcall*)(
        __int64 a1,  // RCX - Player Base
        int a2,  // RDX - Msg type
        int a3,  // R8 - Channel ID
        void *a4 // R9 - Message
        );
    auto TalkChannel = reinterpret_cast<talkChannel_t>(talkChannel_func_address);
    auto a1 = reinterpret_cast<__int64>(player_base);
    uintptr_t a4[2];
    a4[0] = reinterpret_cast<uintptr_t>(message); // Text To speak
    a4[1] = strlen(message); // Lenght of the text
    TalkChannel(a1, 1, 0, a4);
}

bool MemoryFunctions::isAttacking()
{
    //Decomp by IDA for Medivia bool __fastcall sub_7FF6BA73C250(__int64 a1)
    using isAttacking_t = bool(__fastcall*)(
        __int64 a1  // RCX - Player
        );
    auto IsAttacking = reinterpret_cast<isAttacking_t>(MemoryFunctions::base_module + 0x1DC280);
    auto a1 = reinterpret_cast<__int64>(player_base);
    return IsAttacking(a1);
}

Container* MemoryFunctions::getContainer(int index) {
    using getContainer_t = void(__fastcall*)(
        void* a1,  // RCX - Player Base
        void** a2, // RAX - result ptr
        int a3     // R8 - Number of container
        );
    auto GetContainer = reinterpret_cast<getContainer_t>(getContainer_func_address);
    void* container = nullptr;
    void* g_GamePointer = player_base;
    GetContainer(g_GamePointer, &container, index);
    return static_cast<Container*>(container);
}

std::vector<Container*> MemoryFunctions::getContainers() {
    using getContainer_t = void(__fastcall*)(
        void* a1,  // RCX - Player Base
        void** a2, // RAX - result ptr
        int a3     // R8 - Number of container
        );
    auto GetContainer = reinterpret_cast<getContainer_t>(getContainer_func_address);
    void* container = nullptr;
    void* g_GamePointer = player_base;
    std::vector<Container*> resultContainers;
    int i = 0;
    while (true) {
        GetContainer(g_GamePointer, &container, i);
        if (container) {
            resultContainers.push_back(reinterpret_cast<Container*>(container));
        } else {
            break;
        }
        ++i;
    }
    return resultContainers;
}

Item* MemoryFunctions::getItem(Container *container, int index)
{
    //Decomp by IDA for Medivia __int64 *__fastcall sub_7FF719EC8750(__int64 a1, int a2)
    using getItem_t = __int64(__fastcall*)(
        Container *a1,  // RCX - Container
        void* a2, // RDX - Result
        int index // R8 - Index numer
        );
    auto GetItem = reinterpret_cast<getItem_t>(getItem_func_address);
    void *a2 = nullptr;
    GetItem(container, &a2, index);
    return reinterpret_cast<Item*>(a2);
}

bool MemoryFunctions::isContainer(Item* item)
{
    //Decomp by IDA for Medivia bool __fastcall sub_7FF780DEEEE0(__int64 a1)
    using isContainer_t = bool(__fastcall*)(
        Item *a1  // RCX - Item
        );
    auto IsContainer = reinterpret_cast<isContainer_t>(MemoryFunctions::base_module + 0x1DEF10);
    return IsContainer(item);;
}

bool MemoryFunctions::isLyingCorpse(Item* item)
{
    //Decomp by IDA for Medivia bool __fastcall sub_7FF6BA73F190(__int64 a1)
    using isLyingCorpse_t = bool(__fastcall*)(
        Item *a1  // RCX - Item
        );
    auto IsLyingCorpse = reinterpret_cast<isLyingCorpse_t>(MemoryFunctions::base_module + 0x1DF1C0);
    bool result = IsLyingCorpse(item);
    std::cout << result << std::endl;
    return result;
}

__int64 MemoryFunctions::getTopThing( __int64 tile) {
    //decomp by ida _QWORD *__fastcall sub_7FF719F81CC0(__int64 a1, _QWORD *a2)
    using getTopThing_t = __int64(__fastcall*)(
    __int64 a1,  // RCX - Tile ?
    void* a2 // RAX - Result
    );
    auto GetTopThing = reinterpret_cast<getTopThing_t>(getTopThing_func_address);
    void* a2 = nullptr;
    GetTopThing(tile, &a2);
    return reinterpret_cast<__int64>(a2);
}

__int64 MemoryFunctions::queue_getTile(uint32_t x, uint32_t y, uint16_t z) {
    return actionQueue.enqueue([x, y, z]() {
        return getTile(x, y, z);
    }).get();
}

std::vector<Entity*> MemoryFunctions::queue_getSpectatorsInRangeEx(int radius) {
    return actionQueue.enqueue([radius]() {
        return getSpectatorsInRangeEx(radius);
    }).get();
}

void MemoryFunctions::queue_autoWalk(int x, int y, int z) {
    actionQueue.enqueue([x, y, z]() {
        autoWalk(x, y, z);
    }).get();
}

void MemoryFunctions::queue_stop() {
    actionQueue.enqueue([]() {
        stop();
    }).get();
}

void MemoryFunctions::queue_move(Item* item_src, Container* item_dest, int slot) {
    actionQueue.enqueue([item_src, item_dest, slot]() {
        move(item_src, item_dest, slot);
    }).get();
}

int MemoryFunctions::queue_open(Item* item, Container* parent_container) {
    return actionQueue.enqueue([item, parent_container]() {
        return open(item, parent_container);
    }).get();
}

void MemoryFunctions::queue_attack(Entity* entity) {
    actionQueue.enqueue([entity]() {
        attack(entity);
    }).get();
}

void MemoryFunctions::queue_talkChannel(const char *message) {
    actionQueue.enqueue([message]() {
        talkChannel(message);
    }).get();
}

bool MemoryFunctions::queue_isAttacking() {
    return actionQueue.enqueue([]() {
        return isAttacking();
    }).get();
}

Container* MemoryFunctions::queue_getContainer(int index) {
    return actionQueue.enqueue([index]() {
        return getContainer(index);
    }).get();
}

std::vector<Container*> MemoryFunctions::queue_getContainers() {
    return actionQueue.enqueue([]() {
        return getContainers();
    }).get();
}


Item* MemoryFunctions::queue_getItem(Container* container, int index) {
    return actionQueue.enqueue([container, index]() {
        return getItem(container, index);
    }).get();
}

bool MemoryFunctions::queue_isContainer(Item* item) {
    return actionQueue.enqueue([item]() {
        return isContainer(item);
    }).get();
}


bool MemoryFunctions::queue_isLyingCorpse(Item* item) {
    return actionQueue.enqueue([item]() {
        return isLyingCorpse(item);
    }).get();
}

__int64 MemoryFunctions::queue_getTopThing(__int64 tile) {
    return actionQueue.enqueue([tile]() {
        return getTopThing(tile);
    }).get();
}



