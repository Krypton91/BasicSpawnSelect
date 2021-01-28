class PluginBasicSpawnSelectServer extends PluginBase
{
    protected ref SpawnSelectConfig     m_ServerConfig;
    void PluginBasicSpawnSelectServer()
    {
        if(!m_ServerConfig)
            m_ServerConfig = GetBasicSpawnSelectServerConfig();

        Init();
    }

    void Init()
    {
        GetRPCManager().AddRPC("BasicSpawnSelect","SERVER_TELEPORTMETOSPAWN", this, SingleplayerExecutionType.Server);
    }

    void SERVER_TELEPORTMETOSPAWN(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if(type == CallType.Server)
        {
            Param2<ref SpawnLocationObject, PlayerBase> data;
            if ( !ctx.Read( data ) ) return;
                
            SpawnLocationObject m_choosenSpawn = data.param1;
            PlayerBase player = data.param2;
            LogLine("Client: " + sender.GetName() + "<steamID: " + sender.GetPlainId() + " > Sendet an RPC to teleport him to Locaction: " + data.param1.GetName());
            if(player)
            {
                vector FinalSpawnPos = m_choosenSpawn.GetSpawnRandomSpot();
                player.SetPosition(FinalSpawnPos);
                player.SetOrientation(player.GetOrientation());
            }
            else
            {
                Error("Player was not valid! if you see this in crash it was not because this!");
            }
        }
    }

    void SendConfigToClient(PlayerIdentity identity)
    {
        if(!identity)
            return;
        
        GetRPCManager().SendRPC("BasicSpawnSelect", "ServerConfigResponse", new Param1< ref SpawnSelectConfig >( m_ServerConfig ), true, identity);
        LogLine("Server Config sendet to player: " + identity.GetPlainId() + " name: " + identity.GetName());
    }

    void SendClientToSpawnSelectMenu(PlayerIdentity identity)
    {
        if(!identity)
            return;
        GetRPCManager().SendRPC("BasicSpawnSelect", "OpenSpawnSelectMenu", null, true, identity);
    }

    int GetCurrentSpawnsystemMode()
    {
        return m_ServerConfig.SpawnSystemMode;
    }
};

/* Global Plugin Getter */ 
PluginBasicSpawnSelectServer GetSpawnSelectServer()
{
	return PluginBasicSpawnSelectServer.Cast(GetPluginManager().GetPluginByType(PluginBasicSpawnSelectServer));
};