#pragma once
#include "simulation_game_objects.h"
#include "Blam/Engine/tag_files/string_id.h"

struct s_simulation_generic_creation_data
{
	s_simulation_object_creation_data object;
	string_id variant;
};
CHECK_STRUCT_SIZE(s_simulation_generic_creation_data, 20);
