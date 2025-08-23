#pragma once
class LocalPlayer
{
public:
    char pad_0000[56]; //0x0000
    uint32_t x; //0x0038
    uint32_t y; //0x003C
    uint16_t z; //0x0040
    char pad_0042[906]; //0x0042
    uint32_t is_walking; //0x03CC
    char pad_03D4[80]; //0x03D0
    uint64_t *head; //0x0420
    uint64_t *necklace; //0x0428
    uint64_t *container; //0x0430
    uint64_t *armor; //0x0438
    uint64_t *right_hand; //0x0440
    uint64_t *left_hand; //0x0448
    uint64_t *legs; //0x0450
    uint64_t *boots; //0x0458
    uint64_t *ring; //0x0460
    uint64_t *arrow; //0x0468
    char pad_0470[232]; // 0x00470
    double hp; //0x0558
    double max_hp; //0x0560
    double cap; //0x0568
    char pad10_0570[16]; //0x0570
    double lvl; //0x0580
    double lvl_pc; //0x0588
    double mp; //0x0590
    double max_mp; //0x0598
};

class Entity
{
public:
    char pad_0000[56]; //0x0000
    uint32_t x; //0x0038
    uint32_t y; //0x003C
    uint16_t z; //0x0040
    char pad_0042[102]; //0x0042
    char name[32]; //0x00A8
    char pad_00B0[32]; //0x00C8
    uint8_t hp; //0x00E8
    char pad_00EA[531]; //0x00EA
    uint32_t last_x; //0x02FC
    uint32_t last_y; //0x0300
    uint16_t last_z; //0x0304
};

class PlayerBase
{
public:
    char pad_0000[272]; //0x0000
    LocalPlayer *LocalPlayer; //0x0110
    void *Entity; //0x0118
};


class Tile
{
public:
    char pad_0000[104]; //0x0000
    uint64_t *itemList; //0x0068
    char pad_0070[16]; //0x0070
    uint32_t x; //0x0080
    uint32_t y; //0x0084
    uint16_t z; //0x0088
    char pad_008A[6]; //0x008A
    uint16_t walkable; //0x0090
};

class Item
{
public:
    char pad_0000[72]; // 0x0000
    char name[32]; // 0x0048
    char pad_0068[56]; // 0x0068
    uint16_t id; // 0xA0
    char pad_00A2[2]; // 0x00A2
    uint16_t count; // 0x00A4
    char pad_00A6[146]; // 0x00A6
    uint32_t x; // 0x0138
    uint32_t y; // 0x013C
    uint16_t z; // 0x0140

};

class Container {
public:
    char pad_0000[60]; // 0x0000
    uint32_t capacity; // 0x003C
    Item *item; // 0x0040
    char name[32]; // 0x0048
    char pad_0068[24]; // 0x0068
    uint64_t *item_list; // 0x0080
    char pad_0088[16]; // 0x0088
    uint32_t number_of_items; // 0x0098
};

class MapView
{
public:
    char pad_0000[176]; //0x0000
    uint64_t *tileList; //0x00B0
    char pad_00B8[16]; //0x00B8
    uint64_t *entityList; //0x00C8
    char pad_00D0[16]; //0x00D0
    LocalPlayer* LocalPlayer; //0x00E0
};


