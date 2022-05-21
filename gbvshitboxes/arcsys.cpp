#include "arcsys.h"
#include "sigscan.h"

using StaticClass_t = UClass*(*)();
const auto AREDGameState_Battle_StaticClass = (StaticClass_t)(
	sigscan::get().scan("\x00\x00\x48\x8D\x05\xF7\x71\x34\x00", "xxxxxxxxx") - 0x20);

using asw_entity_is_active_t = bool(*)(const asw_entity*, bool);
const auto asw_entity_is_active = (asw_entity_is_active_t)(
	sigscan::get().scan("\xE0\x01\x74\x10\xF7\x81\xA0\x01", "xxxxxxxx") - 0x10);

using asw_entity_is_pushbox_active_t = bool(*)(const asw_entity*);
const auto asw_entity_is_pushbox_active = (asw_entity_is_pushbox_active_t)(
	sigscan::get().scan("\x48\x8B\xC3\x84\xC9\x48\x0F\x44\xC2", "xxxxxxxxx") - 0xF);

using asw_entity_get_pos_x_t = int(*)(const asw_entity*);
const auto asw_entity_get_pos_x = (asw_entity_get_pos_x_t)(
	sigscan::get().scan("\x8B\x8B\x40\x07\x00\x00\xE8\xB5\xFF\xFF\xFF", "xxxxxxxxxxx") - 0x40);

using asw_entity_get_pos_y_t = int(*)(const asw_entity*);
const auto asw_entity_get_pos_y = (asw_entity_get_pos_y_t)(
	sigscan::get().scan("\x04\x00\x75\x18\x48\x8B\x8B\x40\x07\x00\x00\xE8\xB0", "xxxxxxxxxxxxx") - 0x40);

using asw_entity_pushbox_width_t = int(*)(const asw_entity*);
const auto asw_entity_pushbox_width = (asw_entity_pushbox_width_t)(
	sigscan::get().scan("\xD2\x48\x0F\x44\xD9\x8B\x83\x24\x07\x01\x00", "xxxxxxxxxxx") - 0x70);

using asw_entity_pushbox_height_t = int(*)(const asw_entity*);
const auto asw_entity_pushbox_height = (asw_entity_pushbox_height_t)(
	sigscan::get().scan("\xD2\x48\x0F\x44\xD9\x8B\x83\x28\x07\x01\x00", "xxxxxxxxxxx") - 0x70);

using asw_entity_pushbox_bottom_t = int(*)(const asw_entity*);
const auto asw_entity_pushbox_bottom = (asw_entity_pushbox_bottom_t)(
	sigscan::get().scan("\x38\x43\x14\x48\x0F\x44\xD8", "xxxxxxx") - 0x30);

using asw_entity_get_pushbox_t = void(*)(const asw_entity*, int*, int*, int*, int*);
const auto asw_entity_get_pushbox = (asw_entity_get_pushbox_t)(
	sigscan::get().scan("\xFD\xFF\x48\x8B\x8B\x98\x02\x00\x00", "xxxxxxxxx") - 0x30);

UClass *AREDGameState_Battle::StaticClass()
{
	return AREDGameState_Battle_StaticClass();
}

asw_engine *asw_engine::get()
{
	if (*GWorld == nullptr)
		return nullptr;

	const auto *GameState = (*GWorld)->GameState;
	if (GameState == nullptr || !GameState->IsA<AREDGameState_Battle>())
		return nullptr;

	return ((AREDGameState_Battle*)GameState)->Engine;
}

asw_scene *asw_scene::get()
{
	if (*GWorld == nullptr)
		return nullptr;

	const auto *GameState = (*GWorld)->GameState;
	if (GameState == nullptr || !GameState->IsA<AREDGameState_Battle>())
		return nullptr;

	return ((AREDGameState_Battle*)GameState)->Scene;
}

bool asw_entity::is_active() const
{
	// Otherwise returns false during COUNTER
	return asw_entity_is_active(this, !is_player);
}

bool asw_entity::is_pushbox_active() const
{
	return asw_entity_is_pushbox_active(this);
}

bool asw_entity::is_strike_invuln() const
{
	return strike_invuln || wakeup;
}

bool asw_entity::is_throw_invuln() const
{
	return throw_invuln || wakeup;
}

int asw_entity::get_pos_x() const
{
	return asw_entity_get_pos_x(this);
}

int asw_entity::get_pos_y() const
{
	return asw_entity_get_pos_y(this);
}

int asw_entity::pushbox_width() const
{
	return asw_entity_pushbox_width(this);
}

int asw_entity::pushbox_height() const
{
	return asw_entity_pushbox_height(this);
}

int asw_entity::pushbox_bottom() const
{
	return asw_entity_pushbox_bottom(this);
}

void asw_entity::get_pushbox(int *left, int *top, int *right, int *bottom) const
{
	asw_entity_get_pushbox(this, left, top, right, bottom);
}
