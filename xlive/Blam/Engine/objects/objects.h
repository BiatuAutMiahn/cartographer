#pragma once
#include "emblems.h"
#include "object_placement.h"
#include "object_type_list.h"

#include "Blam/Engine/game/aim_assist.h"
#include "Blam/Engine/memory/data.h"
#include "Blam/Math/BlamMath.h"

#define k_maximum_objects_per_map 2048

enum e_game_team : short
{
	// MP
	_game_team_red = 0,
	_game_team_blue = 1,
	_game_team_yellow = 2,
	_game_team_green = 3,
	_game_team_purple = 4,
	_game_team_orange = 5,
	_game_team_brown = 6,
	_game_team_pink = 7,
	_game_team_neutral = 8,

	// SP
	_game_team_default = 0,
	_game_team_player = 1,
	_game_team_human = 2,
	_game_team_covenant = 3,
	_game_team_flood = 4,
	_game_team_sentinel = 5,
	_game_team_heretic = 6,
	_game_team_prophet = 7,

	// unassigned team ids
	_game_team_unused8 = 8,
	_game_team_unused9 = 9,
	_game_team_unused10 = 10,
	_game_team_unused11 = 11,
	_game_team_unused12 = 12,
	_game_team_unused13 = 13,
	_game_team_unused14 = 14,
	_game_team_unused15 = 15,

	// Shared
	_game_team_none = -1
};

enum e_grenades : BYTE
{
	Fragmentation,
	Plasma
};

enum e_weapon_index : WORD
{
	Primary = 0xFF00,
	Secondary = 0xFF01,
	DualWeild = 0x0201
};

enum e_object_data_flags : int32
{
	object_data_flag_0x1 = FLAG(0),
	object_data_flag_0x2 = FLAG(1),
	object_data_flag_0x4 = FLAG(2),
	object_data_flag_0x8 = FLAG(3),
	_object_has_attached_lights_bit = FLAG(4),
	_object_has_attached_looping_sounds_bit = FLAG(5),
	_object_has_unattached_lights_bit = FLAG(6),
	_object_in_limbo_bit = FLAG(7),
	_object_connected_to_map_bit = FLAG(8),
	_object_has_collision_bit = FLAG(9),
	object_data_flag_0x400 = FLAG(10),
	object_data_flag_0x800 = FLAG(11),
	object_data_flag_0x1000 = FLAG(12),
	object_data_flag_0x2000 = FLAG(13),
	_object_garbage_bit = FLAG(14),
	object_data_flag_0x8000 = FLAG(15),
	_object_does_not_cast_shadow_bit = FLAG(16),
	object_data_flag_0x20000 = FLAG(17),
	object_data_flag_0x40000 = FLAG(18),
	object_data_flag_0x80000 = FLAG(19),
	object_data_flag_0x100000 = FLAG(20),
	object_data_flag_0x200000 = FLAG(21),
	object_data_flag_0x400000 = FLAG(22),
	object_data_flag_0x800000 = FLAG(23),
	object_data_flag_0x1000000 = FLAG(24),
	_object_has_override_bit = FLAG(25),
	_object_is_child_object = FLAG(26),
	object_data_flag_0x8000000 = FLAG(27),
	object_data_flag_0x10000000 = FLAG(28),
	object_data_flag_0x20000000 = FLAG(29),
	object_data_flag_0x40000000 = FLAG(30),
	_object_has_prt_or_lighting_info = FLAG(31)
};

enum e_object_physics_flags : uint16
{
	_object_allocated_havok_component_bit = FLAG(0),
	_object_physics_flag_0x2 = FLAG(1),
	_object_is_early_mover_bit = FLAG(3),
	_object_is_early_mover_child_bit = FLAG(4),
	_object_connected_to_physics_bit = FLAG(6),
	_object_physics_flag_0x100 = FLAG(8),
};

enum e_object_damage_flags : uint16
{
	_object_is_dead_bit = FLAG(2),
};

enum e_object_header_flag : byte
{
	_object_header_active_bit = FLAG(0),
	_object_header_awake_bit = FLAG(1),
	_object_header_requires_motion_bit = FLAG(2),
	_object_header_flags_8 = FLAG(3),
	_object_header_being_deleted_bit = FLAG(4),
	_object_header_connected_to_map_bit = FLAG(6),
	_object_header_child_bit = FLAG(7)
};

struct object_header_block_reference
{
	short size;
	short offset;
};
CHECK_STRUCT_SIZE(object_header_block_reference, 4);

struct s_object_header
{
	int16 datum_salt;
	e_object_header_flag flags;
	e_object_type object_type;
	int16 cluster_index;
	int16 object_data_size;
	void* object;
};
CHECK_STRUCT_SIZE(s_object_header, 12);

struct object_datum
{
	datum tag_definition_index;
	e_object_data_flags object_flags;
	void* object_header_block;
	datum next_index;
	datum current_weapon_datum;
	datum parent_datum;
	uint16 inhibited_flags;
	int16 placement_index;
	uint8 gap_1C[8];
	s_emblem_info emblem_info;
	uint8 pad;
	s_location location;
	real_point3d center;
	real32 radius;
	real_point3d object_origin_point;
	real32 shadow_sphere_radius;
	real_point3d dynamic_light_sphere_offset;
	real32 dynamic_light_sphere_radius;
	int32 first_cluster_reference;
	real_point3d position;
	real_vector3d forward;
	real_vector3d up;
	real_point3d translational_velocity;
	real_vector3d angular_velocity;
	real32 scale;
	c_object_identifier object_identifier;
	int16 name_list_index;
	uint8 structure_bsp_index;
	int8 netgame_equipment_index;
	e_bsp_policy placement_policy;
	int8 field_b1;
	int8 field_b2;
	int8 field_b3;
	datum havok_datum;
	datum early_mover_index;
	uint32 unkBC;
	e_object_physics_flags physics_flags;
	uint16 damage_owner_unk3;
	uint32 damage_owner_unk1;
	datum damage_owner_object_datum;
	uint32 cached_object_render_state_index;
	int16 field_D0;
	int8 model_variant_id;					// hlmt variant tag_block index
	int8 gap_D3;
	datum simulation_entity_index;
	bool attached_to_simulation;
	int8 gap_D9[3];
	datum object_projectile_datum;
	uint16 destroyed_constraints_flag;
	uint16 loosened_constraints_flag;
	real32 body_max_vitality;
	real32 shield_max_vitality;
	real32 body_current_vitality;
	real32 shield_current_vitality;
	uint32 gap_F4[3];
	real32 field_100;
	uint16 shield_stun_ticks;
	uint16 body_stun_ticks;
	int8 byte_108;
	int8 byte_109;
	e_object_damage_flags object_damage_flags;
	object_header_block_reference original_orientation_block;
	object_header_block_reference node_orientation_block;
	object_header_block_reference nodes_block;
	object_header_block_reference collision_regions_block;
	object_header_block_reference object_attachments_block;
	object_header_block_reference damage_sections_block;
	object_header_block_reference change_color_block;
	object_header_block_reference animation_manager_block;
};
CHECK_STRUCT_SIZE(object_datum, 300);

static s_data_array* get_objects_header()
{
	return *Memory::GetAddress<s_data_array**>(0x4E461C, 0x50C8EC);
};

// Gets the header of the object, containing some details
static s_object_header* get_objects_header(datum object_idx)
{
	auto objects_header = get_objects_header();
	return (s_object_header*)(&objects_header->data[objects_header->datum_element_size * DATUM_INDEX_TO_ABSOLUTE_INDEX(object_idx)]);
}

// Get the object fast, with no validation from datum index
template<typename T = object_datum>
static T* object_get_fast_unsafe(datum object_idx)
{
	return (T*)get_objects_header(object_idx)->object;
}

// Gets the object and verifies the type, returns NULL if object doesn't match object type flags
template<typename T = object_datum>
static T* object_try_and_get_and_verify_type(datum object_idx, int object_type_flags)
{
	auto p_object_try_and_get_and_verify_type = Memory::GetAddress<char* (__cdecl*)(datum, int)>(0x1304E3, 0x11F3A6);
	return (T*)p_object_try_and_get_and_verify_type(object_idx, object_type_flags);
}

void* object_header_block_get(const datum object_datum, const object_header_block_reference* reference);
void* object_header_block_get_with_count(const datum object_datum, const object_header_block_reference* reference, DWORD element_size, DWORD* element_count);

void __cdecl object_placement_data_new(object_placement_data* object_placement_data, datum object_definition_idx, datum object_owner_idx, s_damage_owner* damage_owner);
datum __cdecl object_new(object_placement_data* placement_data);
void __cdecl simulation_action_object_create(datum object_idx);
void __cdecl object_delete(datum object_idx);

int object_get_count();
int object_count_from_iter();

datum object_get_damage_owner(datum damaged_unit_index);
real_matrix4x3* object_get_node_matrices(datum object_datum, DWORD* out_node_count);
