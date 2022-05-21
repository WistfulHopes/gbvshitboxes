#pragma once

#include "ue4.h"
#include "struct_util.h"

class AREDGameState_Battle : public AGameState {
public:
	static UClass *StaticClass();

	FIELD(0xB38, class asw_engine*, Engine);
	FIELD(0xB40, class asw_scene*, Scene);
};

class player_block {
	char pad[0x70];
public:
	FIELD(0x8, class asw_entity*, entity);
};

static_assert(sizeof(player_block) == 0x70);

// Used by the shared GG/BB/DBFZ engine code
class asw_engine {
public:
	static constexpr auto COORD_SCALE = .43f;

	static asw_engine *get();

	ARRAY_FIELD(0x10, player_block[2], players);
	FIELD(0xEA8, int, entity_count);
	ARRAY_FIELD(0x1BC0, class asw_entity*[110], entities);
};


class asw_scene {
public:
	static asw_scene *get();

	// "delta" is the difference between input and output position
	// position gets written in place
	// position/angle can be null
};

class hitbox {
public:
	enum class box_type : int {
		hurt = 0,
		hit = 1,
		grab = 2 // Not used by the game
	};

	box_type type;
	float x, y;
	float w, h;
	float unused;
};

enum class direction : int {
	right = 0,
	left = 1
};

class event_handler {
	char pad[0x58];

public:
	FIELD(0x28, int, trigger_value);
};

static_assert(sizeof(event_handler) == 0x58);

class asw_entity {
public:
	FIELD(0x14, bool, is_player);
	FIELD(0x44, unsigned char, player_index);
	FIELD(0x78, hitbox*, hitboxes);
	FIELD(0x104, int, hurtbox_count);
	FIELD(0x108, int, hitbox_count);
	FIELD(0x1BC, int, state_frames);
	FIELD(0x270, asw_entity*, opponent);
	FIELD(0x288, asw_entity*, parent);
	FIELD(0x2C8, asw_entity*, attached);
	BIT_FIELD(0x3A0, 1, airborne);
	BIT_FIELD(0x3A0, 256, counterhit);
	BIT_FIELD(0x3A4, 16, strike_invuln);
	BIT_FIELD(0x3A4, 32, throw_invuln);
	BIT_FIELD(0x3A4, 64, wakeup);
	FIELD(0x3B4, direction, facing);
	FIELD(0x3B8, int, pos_x);
	FIELD(0x3BC, int, pos_y);
	FIELD(0x3C0, int, pos_z);
	FIELD(0x3C4, int, angle_x);
	FIELD(0x3C8, int, angle_y);
	FIELD(0x3CC, int, angle_z);
	FIELD(0x3DC, int, scale_x);
	FIELD(0x3E0, int, scale_y);
	FIELD(0x3E4, int, scale_z);
	FIELD(0x51C, int, vel_x);
	FIELD(0x520, int, vel_y);
	FIELD(0x524, int, gravity);
	FIELD(0x548, int, pushbox_front_offset);
	FIELD(0x7A0, int, throw_box_top);
	FIELD(0x7A8, int, throw_box_bottom);
	FIELD(0x7B0, int, throw_range);

	bool is_active() const;
	bool is_pushbox_active() const;
	bool is_strike_invuln() const;
	bool is_throw_invuln() const;
	int get_pos_x() const;
	int get_pos_y() const;
	int pushbox_width() const;
	int pushbox_height() const;
	int pushbox_bottom() const;
	void get_pushbox(int *left, int *top, int *right, int *bottom) const;
};
