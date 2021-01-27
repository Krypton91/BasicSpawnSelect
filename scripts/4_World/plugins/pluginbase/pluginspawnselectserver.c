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
}