#include "../../PVZ.h"

PVZ::Matrix3 PVZ::AttachEffect::GetOffset()
{
	return(PVZ::Matrix3(this->BaseAddress + 8));
}