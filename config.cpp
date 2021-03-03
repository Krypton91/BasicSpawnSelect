  
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
		action = "https://github.com/Krypton91/BasicSpawnSelect";
		hideName = 1;
		hidePicture = 1;
		name = "Basic Spawn Select";
		credits = "Krypton91";
		author = "Krypton91";
		authorID = "76561198796326626";
		version = "1.11";
		extra = 0;
		type = "mod";
		inputs = "BasicSpawnSelect/data/inputs.xml";
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
		scope=0;
	};
	class BasicSpawnSelect_SpawnTicketBandit: BasicSpawnSelect_SpawnTicket_base
	{
		scope=2;
		displayName="Spawn Select Ticket (Bandit)";
		descriptionShort="This ticket is to open the SpawnSelect Menu!";
	};

	class BasicSpawnSelect_SpawnTicketAdmin: BasicSpawnSelect_SpawnTicket_base
	{
		scope=2;
		displayName="Spawn Select Ticket (Admin)";
		descriptionShort="This ticket is to open the SpawnSelect Menu!";
	};
};