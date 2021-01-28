  
class CfgPatches 
{
	class BasicSpawnSelect 
	{
		units[] = 
		{
			"BasicSpawnSelect"
		};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = 
		{
			"DZ_Data",
			"JM_CF_Scripts"
		};
	};
};

class CfgMods 
{
	class BasicSpawnSelect 
	{
		dir = "BasicSpawnSelect";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "Spawn Select";
		credits = "Krypton91";
		author = "Krypton91";
		authorID = "0";
		version = 0.1;
		extra = 0;
		type = "mod";
		dependencies[] = {"Mission", "World", "Game"};

	    class defs
	    {
			class gameScriptModule
            {
                value = "";
                files[] = 
				{
					"BasicSpawnSelect/scripts/common",
					"BasicSpawnSelect/scripts/3_Game"
				};
            };
			class worldScriptModule
            {
                value = "";
                files[] = 
				{
					"BasicSpawnSelect/scripts/common",
					"BasicSpawnSelect/scripts/4_World"
				};
            };
			class missionScriptModule
            {
                value = "";
                files[] = 
				{
					"BasicSpawnSelect/scripts/common",
					"BasicSpawnSelect/scripts/5_Mission"
				};
            };
        };
	};
};

class CfgVehicles
{
	class Paper;
	class BasicSpawnSelect_SpawnTicket_base: Paper
	{
	};
	class BasicSpawnSelect_SpawnTicketBandit: BasicSpawnSelect_SpawnTicket_base
	{
		displayName="Spawn Select Ticket (Bandit)";
		descriptionShort="This ticket is to open the SpawnSelect Menu!";
	};

	class BasicSpawnSelect_SpawnTicketAdmin: BasicSpawnSelect_SpawnTicket_base
	{
		displayName="Spawn Select Ticket (Admin)";
		descriptionShort="This ticket is to open the SpawnSelect Menu!";
	};
};