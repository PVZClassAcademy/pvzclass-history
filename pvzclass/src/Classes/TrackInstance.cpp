#include "../../PVZ.h"

PVZ::TrackInstance PVZ::TrackInstance::GetByIndex(uint32_t index)
{
	return TrackInstance(Memory::ReadPointer(0x6A9EC0, 0x820, 0xC, 0) + index * 0x30C);
}

PVZ::AttachmentID PVZ::TrackInstance::GetAttachmentID()
{
	return PVZ::AttachmentID(BaseAddress + 0x40);
}

PVZ::Attachment PVZ::TrackInstance::GetAttachment()
{
	int ID = Memory::ReadMemory<int>(BaseAddress + 0x40);
	return (PVZ::Attachment(ID & 0x00FFFF));
}
