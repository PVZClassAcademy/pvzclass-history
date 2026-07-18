#pragma once
#include "../../PVZ.h"
#include "GameObject.hpp"

namespace PVZ
{
	class PVZApp;
	class Board;
	class MousePointer : public GameObject//+138
	{
	public:
		MousePointer(int address);
		INT_PROPERTY(CardIndex, __get_CardIndex, __set_CardIndex, 0x24);
		T_PROPERTY(SeedType::SeedType, ContentCard, __get_ContentCard, __set_ContentCard, 0x28);
		T_PROPERTY(SeedType::SeedType, ContentCardImitative, __get_ContentCardImitative, __set_ContentCardImitative, 0x2C);
		T_PROPERTY(MouseType::MouseType, Type, __get_Type, __set_Type, 0x30);
		INT_PROPERTY(CoinID, __get_CoinID, __set_CoinID, 0x34);
		INT_PROPERTY(GlovePlantID, __get_GlovePlantID, __set_GlovePlantID, 0x38);
		INT_PROPERTY(ImitativePlantID, __get_ImitativePlantID, __set_ImitativePlantID, 0x3C);
		INT_PROPERTY(CobCannonID, __get_CobCannonID, __set_CobCannonID, 0x40);
		INT_PROPERTY(HammerDownCount, __get_HammerDownCount, __set_HammerDownCount, 0x44);
		PVZ::Animation GetAnimation();
		READONLY_PROPERTY_BINDING(int, __get_Row, Memory::ReadPointer(PVZBASEADDRESS + 0x13C, 0x28)) Row;
		READONLY_PROPERTY_BINDING(int, __get_Column, Memory::ReadPointer(PVZBASEADDRESS + 0x13C, 0x24)) Column;
	};
}