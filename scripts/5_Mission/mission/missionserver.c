modded class MissionServer extends MissionBase
{    
    override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);
        GetSpawnSelectServer().SendConfigToClient(identity);
	}
};