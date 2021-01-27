modded class PluginManager
{	
	//=================================
	// Init
	//=================================
	override void Init()
	{	
		super.Init();
		//----------------------------------------------------------------------
		// Register modules
		//----------------------------------------------------------------------
		//				Module Class Name 						Client	Server
		//----------------------------------------------------------------------
		if(GetGame().IsServer() && !GetGame().IsClient() && GetGame().IsMultiplayer())
		{
			RegisterPlugin( "PluginBasicSpawnSelectServer",			false, 	true );
		}
		else
		{
			RegisterPlugin( "PluginBasicSpawnSelectServer", 		true, 	false );
		}
	}
}