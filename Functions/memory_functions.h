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
    static void* look_func_address;

    static bool has_target;

    static SafeQueue actionQueue;


    static __int64 queue_getTile(uint32_t x, uint32_t y, uint16_t z);
    static __int64 getTile(uint32_t x, uint32_t y, uint16_t z);

    static std::vector<Entity*> queue_getSpectatorsInRangeEx(int radius);
    static std::vector<Entity*> getSpectatorsInRangeEx(int radius);

    static void queue_autoWalk(int x, int y, int z);
    static void autoWalk(int x, int y, int z);

    static void queue_stop();
    static void stop();

    static void queue_move(Item* item_src, Container* item_dest, int slot);
    static void move(Item* item_src, Container* container_dst, int slot);

    static void queue_useWith(uint64_t item, uint64_t toThing);
    static void useWith(uint64_t item, uint64_t toThing);

    static uint64_t queue_findItemInContainers(uint32_t item_id);
    static uint64_t findItemInContainers(uint32_t item_id);

    static int queue_open(Item* item,Container* parent_container);
    static int open(Item* item, Container* parent_container);
    static void queue_open(std::string container_name);
    static void open(std::string container_name);

    static void queue_close(std::string container_name);
    static void close(std::string container_name);

    static void queue_attack(Entity* entity);
    static void attack(Entity* target);

    static void queue_talkChannel(const char *message);
    static void talkChannel(const char *message);

    static void queue_setChaseMode(bool chaseMode);
    static void setChaseMode(bool chaseMode);

    static bool queue_isAttacking();
    static bool isAttacking();

    static Container* queue_getContainer(int index);
    static Container* getContainer(int index);

    static std::vector<Container*> queue_getContainers();
    static std::vector<Container*> getContainers();

    static Item* queue_getItem(Container *container, int index);
    static Item* getItem(Container *container, int index);


    static bool queue_isContainer(Item* item);
    static bool isContainer(Item* item);

    static bool queue_isLyingCorpse(Item* item);
    static bool isLyingCorpse(Item* item);

    static __int64 queue_getTopThing(__int64 tile);
    static __int64 getTopThing(__int64 tile);


    explicit MemoryFunctions(LoadOption load_option);


private:
    // Variables
    static uint64_t local_player_address;
    // Function Addresses
    static void* getTile_func_address;
    static void* getSpectatorsInRangeEx_func_address;
    static void* autoWalk_func_address;
    static void* stop_func_address;
    static void* move_func_address;
    static void* useWith_func_address;
    static void* findItemInContainers_func_address;
    static void* open_func_address;
    static void* close_func_address;
    static void* attack_func_address;
    static void* talkChannel_func_address;
    static void* setChaseMode_func_address;
    static void* isAttacking_func_address;
    static void* getContainer_func_address;
    static void* getItem_func_address;
    static void* isContainer_func_address;
    static void* isLyingCorpse_func_address;
    static void* getTopThing_func_address;

    static LoadOption load_functions_variant;
};

#endif //MEMORY_FUNCTIONS_H
