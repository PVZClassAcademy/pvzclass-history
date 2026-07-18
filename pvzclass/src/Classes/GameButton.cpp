#include "Classes/GameButton.hpp"

PVZ::PVZApp PVZ::GameButton::GetPVZApp()
{
    return PVZ::Memory::ReadMemory<PVZ::PVZApp>(BaseAddress);
}
PVZ::Widget PVZ::GameButton::GetParentWidget()
{
    return PVZ::Memory::ReadMemory<PVZ::Widget>(BaseAddress + 0x4);
}
PVZ::PVZString PVZ::GameButton::GetLabel()
{
    return PVZ::PVZString::PVZString(BaseAddress + 0x80);
}
