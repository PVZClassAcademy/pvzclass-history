#include "Classes/ReanimatorCache.hpp"

void PVZ::ReanimatorCache::SetMemSize(int8_t plant_num, int8_t zombie_num, DWORD lawnmower_num)
{
	PVZ::Memory::WriteMemory<int>(0x452B40, 20 + ((zombie_num + lawnmower_num + plant_num) << 2));

	PVZ::Memory::WriteMemory<int>(0x46FDC7, 16 + ((zombie_num + lawnmower_num + plant_num) << 2));
	PVZ::Memory::WriteMemory<int>(0x470058, 16 + ((zombie_num + lawnmower_num + plant_num) << 2));

	int list_pos = (int)plant_num * 4;
	PVZ::Memory::WriteMemory<int>(0x44F313, list_pos);
	PVZ::Memory::WriteMemory<int>(0x452B58, list_pos);
	PVZ::Memory::WriteMemory<int>(0x452B5E, list_pos + 4);
	PVZ::Memory::WriteMemory<int>(0x452B64, list_pos + 8);
	PVZ::Memory::WriteMemory<int>(0x452B6A, list_pos + 12);

	PVZ::Memory::WriteMemory<int>(0x46FEF9, list_pos + 8);
	PVZ::Memory::WriteMemory<int>(0x46FF05, list_pos);
	PVZ::Memory::WriteMemory<int>(0x46FF10, list_pos);
	PVZ::Memory::WriteMemory<int>(0x46FF1D, list_pos + 4);
	PVZ::Memory::WriteMemory<int>(0x46FF2F, list_pos + 8);
	PVZ::Memory::WriteMemory<int>(0x46FF39, list_pos + 12);
	PVZ::Memory::WriteMemory<int>(0x46FF54, list_pos + 4);

	PVZ::Memory::WriteMemory<int>(0x46FFC3, list_pos);
	PVZ::Memory::WriteMemory<int>(0x46FFCB, list_pos);

	PVZ::Memory::WriteMemory<int8_t>(0x46FEF4, plant_num);
	PVZ::Memory::WriteMemory<int8_t>(0x46FF7D, zombie_num);
	PVZ::Memory::WriteMemory<int>(0x46FF61, lawnmower_num);

	PVZ::Memory::WriteMemory<int>(0x46FF80, 16 + ((lawnmower_num + plant_num) << 2));
	PVZ::Memory::WriteMemory<int>(0x403168, 16 + ((lawnmower_num + plant_num) << 2));
	PVZ::Memory::WriteMemory<int>(0x403179, 16 + ((lawnmower_num + plant_num) << 2));
	PVZ::Memory::WriteMemory<int>(0x403184, 16 + ((lawnmower_num + plant_num) << 2));
	PVZ::Memory::WriteMemory<int>(0x470174, 16 + ((lawnmower_num + plant_num) << 2));
	PVZ::Memory::WriteMemory<int>(0x470185, 16 + ((lawnmower_num + plant_num) << 2));
	PVZ::Memory::WriteMemory<int>(0x47018F, 16 + ((lawnmower_num + plant_num) << 2));

	PVZ::Memory::WriteMemory<int>(0x46FF5C, 16 + (plant_num << 2));
	PVZ::Memory::WriteMemory<int>(0x470114, 16 + (plant_num << 2));
	PVZ::Memory::WriteMemory<int>(0x470124, 16 + (plant_num << 2));
	PVZ::Memory::WriteMemory<int>(0x47012E, 16 + (plant_num << 2));

	byte __asm__inject[]
	{
		PUSH_EDX,
		PUSHDWORD(((zombie_num + lawnmower_num + plant_num) << 2) - 188),
		PUSH(0),
		PUSH_EDX,
		CALC_PTR_ESP(CALC_ADD, 0xD0),
		INVOKE(0x626020),
		ADD_ESP(0x0C),
		POP_EUX(REG_EDX),
		MOV_EUX_PTR_ADDR(REG_EAX, 0x6A9F38),
		MOV_PTR_EUX_ADD__EVX(REG_EDX, REG_EAX, 16 + ((zombie_num + lawnmower_num + plant_num) << 2)),
		POP_EUX(REG_EDI),
		RET
	};
	Memory::WriteArray(0x0046FE97, STRING(__asm__inject));
}
