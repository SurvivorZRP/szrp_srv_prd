#include "$CurrentDir:mpmissions\\dayzOffline.enoch\\StaticBuilds.c"

void SpawnObject( string type, vector position, vector orientation )
{
        auto obj = GetGame().CreateObject( type, position );
        obj.SetPosition( position );
        obj.SetOrientation( orientation );
        obj.SetOrientation( obj.GetOrientation() ); //Collision fix
        obj.Update();
        obj.SetAffectPathgraph( true, false );
        if( obj.CanAffectPathgraph() ) GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 100, false, obj );
}
void main()
{
	StaticBuilds()

	//INIT WEATHER BEFORE ECONOMY INIT------------------------
	Weather weather = g_Game.GetWeather();

	weather.MissionWeather(false);    // false = use weather controller from Weather.c

	weather.GetOvercast().Set( Math.RandomFloatInclusive(0.02, 0.1), 1, 0);
	weather.GetRain().Set( 0, 1, 0);
	weather.GetFog().Set( 0, 1, 0);

	//INIT ECONOMY--------------------------------------
	Hive ce = CreateHive();
	if ( ce )
		ce.InitOffline();

	//DATE RESET AFTER ECONOMY INIT-------------------------
	int year, month, day, hour, minute;
	int reset_month = 8, reset_day = 10;
	GetGame().GetWorld().GetDate(year, month, day, hour, minute);

	if ((month == reset_month) && (day < reset_day))
	{
		GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
	}
	else
	{
		if ((month == reset_month + 1) && (day > reset_day))
		{
			GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
		}
		else
		{
			if ((month < reset_month) || (month > reset_month + 1))
			{
				GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
			}
		}
	}
}

class CustomMission: MissionServer
{
	void SetRandomHealth(EntityAI itemEnt)
	{
		if ( itemEnt )
		{
			int rndHlt = Math.RandomInt(55,100);
			itemEnt.SetHealth("","",rndHlt);
		}
	}

	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		Entity playerEnt;
		playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");//Creates random player
		Class.CastTo(m_player, playerEnt);

		GetGame().SelectPlayer(identity, m_player);

		return m_player;
	}

	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
		EntityAI itemTop;
		EntityAI itemEnt;
		ItemBase itemBs;
		float rand;

		itemTop = player.FindAttachmentBySlotName("Body");

		if ( itemTop )
		{
			itemEnt = itemTop.GetInventory().CreateInInventory("Rag");
			if ( Class.CastTo(itemBs, itemEnt ) )
				itemBs.SetQuantity(4);
			SetRandomHealth(itemEnt);

			itemEnt = itemTop.GetInventory().CreateInInventory("HuntingKnife");
			itemEnt = itemTop.GetInventory().CreateInInventory("RoadFlare");
			itemEnt = itemTop.GetInventory().CreateInInventory("MakarovIJ70");
			itemEnt = itemTop.GetInventory().CreateInInventory("Mag_IJ70_8Rnd");
			itemEnt = itemTop.GetInventory().CreateInInventory("AmmoBox_380_35rnd");

			rand = Math.RandomFloatInclusive(0.0, 1.0);
			if ( rand < 0.35 )
				itemEnt = player.GetInventory().CreateInInventory("WaterBottle");
			else if ( rand > 0.65 )
				itemEnt = player.GetInventory().CreateInInventory("Canteen");
			else
				itemEnt = player.GetInventory().CreateInInventory("Vodka");
			SetRandomHealth(itemEnt);

			rand = Math.RandomFloatInclusive(0.0, 1.0);
			if ( rand < 0.35 )
				itemEnt = player.GetInventory().CreateInInventory("sandwich_mung");
			else if ( rand > 0.65 )
				itemEnt = player.GetInventory().CreateInInventory("PeachesCan_Opened");
			else
				itemEnt = player.GetInventory().CreateInInventory("TacticalBaconCan_Opened");
			SetRandomHealth(itemEnt);
		}
	}
};

Mission CreateCustomMission(string path)
{
	return new CustomMission();
}