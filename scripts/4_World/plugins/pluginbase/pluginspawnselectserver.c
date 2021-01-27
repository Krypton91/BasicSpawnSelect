class PluginBasicSpawnSelectServer extends PluginBase
{
    protected ref SpawnSelectConfig     m_ServerConfig;
    void PluginBasicSpawnSelectServer()
    {
        if(!m_ServerConfig)
            m_ServerConfig.GetBasicSpawnSelectServerConfig();

        Init();
    }

    void Init()
    {
        GetRPCManager().AddRPC("BasicSpawnSelect","SpawnOnSelectedSpawn", this, SingleplayerExecutionType.Server);
    }

    void SendConfigToClient(PlayerIdentity identity)
    {
        if(!identity)
            return;
        GetRPCManager().SendRPC("BasicSpawnSelect", "ServerConfigResponse", new Param1< ref SpawnSelectConfig >( m_ServerConfig ), true, identity);
        LogLine("Server Config sendet to player: " + identity.GetPlainId() + " name: " + identity.GetName());
    }
};

/* Global Plugin Getter */ 
PluginBasicSpawnSelectServer GetSpawnSelectServer()
{
	return PluginBasicSpawnSelectServer.Cast(GetPluginManager().GetPluginByType(PluginBasicSpawnSelectServer));
};