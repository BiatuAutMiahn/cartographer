#pragma once

#include "Blam/Cache/DataTypes/BlamDataTypes.h"
#include "H2MOD/Variants/VariantSystem.h"

extern class DeviceShop* deviceShop;

class FireFight : public ICustomGameVariant
{
public:
	FireFight();
	virtual ~FireFight() = default;
	virtual void Initialize() override;
	virtual void Dispose() override;
	virtual CustomVariantId GetVariantId();
	
	// on map load can be used as Initialize
	virtual void OnMapLoad(ExecTime execTime, s_game_options* options) override;
	virtual void OnPlayerSpawn(ExecTime execTime, datum playerIdx) override;
	virtual void OnPlayerDeath(ExecTime execTime, datum playerIdx) override;
	virtual void OnObjectDamage(ExecTime execTime, datum unitDatumIdx, int a2, bool a3, bool a4) override;

	// unused bellow
	virtual bool OnAutoPickupHandler(ExecTime execTime, datum playerIdx, datum objectIdx) override
	{
		return false;
	}

	virtual bool c_game_statborg__adjust_player_stat(ExecTime execTime, c_game_statborg* statborg, datum player_datum, e_statborg_entry statistic, short count, int game_results_statistic, bool adjust_team_stat) override;


	static void KilledAI(datum killedAi, datum killerPlayerIdx);
};
