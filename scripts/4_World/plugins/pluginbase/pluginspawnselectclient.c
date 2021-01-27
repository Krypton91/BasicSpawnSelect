class PluginBasicSpawnSelectClient extends PluginBase
{
    protected ref SpawnSelectConfig     m_ServerConfig;
    void PluginBasicSpawnSelectClient()
    {
        Init();
    }

    void Init()
    {
        LogLine("Client Init Start");
        GetRPCManager().AddRPC("BasicSpawnSelect","ServerConfigResponse", this, SingleplayerExecutionType.Server);
    }

    void ServerConfigResponse(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        Print("Recived RPC from Config!");
        if(type == CallType.Client)
        {
            Param1<ref SpawnSelectConfig> data;
            if ( !ctx.Read( data ) ) return;
            m_ServerConfig = data.param1;
            LogLine("Server config sucessfully recived!");
        }
    }
};