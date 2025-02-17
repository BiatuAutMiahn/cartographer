#include "stdafx.h"
#include "animation_manager.h"

#include "Blam/Engine/tag_files/global_string_ids.h"

c_animation_manager::c_animation_manager()
{
    this->initialize();
}

void c_animation_manager::initialize()
{
    this->channel1.initialize();
    this->channel2.initialize();
    this->channel3.initialize();
    this->animation_manager_flags = 0;
    this->unk6E = 0;
    this->jmad_tag_datum = DATUM_INDEX_NONE;
    this->interpolator_control_0.disable();
    this->interpolator_control_1.disable();
    this->animation_string_ids[0] = HS_ID_NONE;
    this->animation_string_ids[3] = HS_ID_NONE;
    this->animation_string_ids[1] = HS_ID_NONE;
    this->animation_string_ids[2] = HS_ID_NONE;
    this->unk80 = 0;
}

bool c_animation_manager::reset_graph(datum jmad_tag_index, datum hlmt_tag_index, bool some_bool)
{
    typedef bool(__thiscall* c_animation_manager__validate_graph_t)(c_animation_manager* _this, datum jmad_tag_datum, datum hlmt_tag_datum);
    auto c_animation_manager__validate_graph = Memory::GetAddress<c_animation_manager__validate_graph_t>(0xF325F, 0xF6CA5);
    typedef bool(__thiscall* c_animation_manager__set_goal_t)(c_animation_manager* _this, string_id a2, string_id a3, string_id a4, string_id a5, unsigned int a6, int a7);
    auto c_animation_manager__set_goal = Memory::GetAddress<c_animation_manager__set_goal_t>(0xF5435, 0xF6CA5);

    if (hlmt_tag_index != -1 && !c_animation_manager__validate_graph(this, jmad_tag_index, hlmt_tag_index))
    {
        return false;
    }

    if (some_bool)
    {
        this->animation_manager_flags |= 2u;
    }
    else
    {
        this->animation_manager_flags &= ~2u;
    }

    this->jmad_tag_datum = jmad_tag_index;
    this->animation_string_ids[0] = HS_ID_NONE;
    this->animation_string_ids[3] = HS_ID_NONE;
    this->animation_string_ids[1] = HS_ID_NONE;
    this->animation_string_ids[2] = HS_ID_NONE;

    this->channel3.reset();
    this->channel2.reset();
    this->channel1.reset();

    c_animation_manager__set_goal(this, HS_DEFAULT, HS_DEFAULT, HS_DEFAULT, HS_DEFAULT, 791u, 63);
    this->interpolator_control_1.disable();
    this->interpolator_control_1.set_interpolation_type(interpolation_type_ease_in_ease_out);
    this->interpolator_control_0.disable();
    this->unk6E = 0;
    this->unk80 = 0;
    return true;
}