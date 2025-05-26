#include "memory_functions.h"
#include <iostream>

uintptr_t MemoryFunctions::base_module = 0;
uintptr_t MemoryFunctions::character_address = 0;
uintptr_t MemoryFunctions::my_x_address = 0;
uintptr_t MemoryFunctions::my_y_address = 0;
uintptr_t MemoryFunctions::my_z_address = 0;
MemoryFunctions::LoadOption MemoryFunctions::load_functions_variant = LoadOption::Altaron;
void* MemoryFunctions::move_func_address = nullptr;
void* MemoryFunctions::attack_func_address = nullptr;
void* MemoryFunctions::open_func_address = nullptr;
void* MemoryFunctions::collect_func_address = nullptr;
void* MemoryFunctions::say_func_address = nullptr;


MemoryFunctions::MemoryFunctions(LoadOption load_option) {
    load_functions_variant = load_option;
    base_module = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if (load_option == LoadOption::Altaron) { // Loading Altaron Memories
        //MoveFunc = (_MoveFunc)(baseModule + 0xF8870);
        move_func_address = reinterpret_cast<void *>(base_module + 0xF8870);
        character_address = 0x2F72BC0;
        my_x_address = 0x2F72D30;
        my_y_address = my_x_address + 0x4;
        my_z_address = my_x_address + 0x8;
        attack_func_address = reinterpret_cast<void *>(base_module + 0xEED10);
        std::cout << "Altaron module loaded: 0x" << std::hex << base_module << std::endl;
        std::cout << "AttackFunc Address: 0x" << std::hex << attack_func_address << std::endl;
    }
    if (load_option == LoadOption::Medivia) { // Loading Medivia Memories
        move_func_address = reinterpret_cast<void *>(base_module + 0x14A0F0);
        character_address = 0xBEB4E0;
        my_x_address = 0xBEC560;
        my_y_address = my_x_address + 0x4;
        my_z_address = my_x_address + 0x8;
        attack_func_address = reinterpret_cast<void *>(base_module + 0x1325E0);
        open_func_address = reinterpret_cast<void *>(base_module + 0x136190);
        collect_func_address = reinterpret_cast<void *>(base_module + 0x135610);
        say_func_address = reinterpret_cast<void *>(base_module + 0x136D80);
        std::cout << "Medivia module loaded: 0x" << std::hex << base_module << std::endl;
        std::cout << "MoveFunc Address: 0x" << std::hex << move_func_address << std::endl;
        std::cout << "AttackFunc Address: 0x" << std::hex << attack_func_address << std::endl;
        std::cout << "OpenFunc Address: 0x" << std::hex << open_func_address << std::endl;
        std::cout << "CollectFunc Address: 0x" << std::hex << collect_func_address << std::endl;
        std::cout << "SayFunc Address: 0x" << std::hex << say_func_address << std::endl;
    }
}

std::array<int, 3> MemoryFunctions::read_my_wpt() {
    int x = *reinterpret_cast<DWORD*>(base_module + my_x_address);
    int y = *reinterpret_cast<DWORD*>(base_module + my_y_address);
    int z = *reinterpret_cast<SHORT*>(base_module + my_z_address);
    return {x, y, z};
}

void MemoryFunctions::moveTo(int x, int y, int z) {
    if (load_functions_variant == LoadOption::Altaron) {
        DWORD char_ptr_addr = base_module + character_address;
        WORD* player_struct = reinterpret_cast<WORD*>(*reinterpret_cast<DWORD*>(char_ptr_addr));

        std::cout << "[moveTo] base_module: " << std::hex << base_module
          << " move_func: " << move_func_address
          << " dyn_char_addr: " << player_struct
          << " x: " << x << " y: " << y << " z: " << z << std::endl;
        if (!player_struct) {
            std::cout << "[moveTo] character_address == 0! Skip move." << std::endl;
            return;
        }
        //typedef void(__thiscall* MoveFuncAltaron)(int, int, int, short, char);
        auto param_3 = static_cast<__int16>(z);
        typedef int(__thiscall* MoveFuncAltaron)(WORD*, int, int, __int16, char);
        ((MoveFuncAltaron)move_func_address)(player_struct, x, y, param_3, '0');
    } else if (load_functions_variant == LoadOption::Medivia) {
        typedef void(__fastcall* MoveFuncMedivia)(long long, int*);
        int pos[3] = {x, y, z};
        long long param_1 = *reinterpret_cast<long long *>(base_module + character_address);
        std::cout << "Param1 Address: 0x" << std::hex << param_1 << std::endl;
        ((MoveFuncMedivia)move_func_address)(param_1, pos);
    }
}

void MemoryFunctions::attackTarget(int target_id) {
    if (load_functions_variant == LoadOption::Altaron) {
        typedef void(__thiscall* AttackFuncAltaron)(int*, int*);
        ((AttackFuncAltaron)attack_func_address)(reinterpret_cast<int *>(base_module + character_address), &target_id);
    } else if (load_functions_variant == LoadOption::Medivia) {
        typedef __int32(__fastcall* AttackFuncMedivia)(__int64 character_base, volatile signed __int32 **target, __int64 a3);
        volatile signed __int32 *param_2 = reinterpret_cast<volatile signed __int32 *>(target_id);
        ((AttackFuncMedivia)attack_func_address)(base_module + character_address - 0x110, &param_2, 0);
    }
}

void MemoryFunctions::openContainer() {
    typedef __int64(__fastcall* OpenContainerFuncMedivia)(__int64 character_base, long long *a2, __int64 *a3);
    long long param_2 = 0x2FAE9F29F00;
    long long param_3 = 0x0;
    ((OpenContainerFuncMedivia)open_func_address)(0x7FF71B9AB3D0, &param_2, &param_3);
}

void MemoryFunctions::collectItem() {
    typedef void(__fastcall* CollectItemFuncMedivia)(__int64 character_base, long long *a2, __int64 *a3, int a4);
    long long param_2 = 0x2C36F6D2D00;
    long long param_3 = 0xFFFF;
    int param_4 = 0x01;
    ((CollectItemFuncMedivia)collect_func_address)(base_module + character_address - 0x110, &param_2, &param_3, param_4);
}

void MemoryFunctions::say() {
    typedef void(__fastcall* SayFuncMedivia)(__int64 character_base, int speak_option, int channel_number , void* text_to_speak);
    const char* text = "Hi";
    uintptr_t args[2];
    args[0] = (uintptr_t)text; // Text To speak
    args[1] = strlen(text); // Lenght of the text
    ((SayFuncMedivia)say_func_address)(base_module + character_address - 0x110,1,0,args);
}





bool MemoryFunctions::isWalking() {
    int walking = *(DWORD*)(*(DWORD*)(base_module + 0x02F72BC0) + 0x248);
    if (walking == 65535) {
        return false;
    }
    return true;
}


