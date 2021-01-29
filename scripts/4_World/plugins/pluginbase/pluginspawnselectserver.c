class PluginBasicSpawnSelectServer extends PluginBase
{
    protected ref SpawnSelectConfig         m_ServerConfig;
    protected ref AdminSpawnSelectConfig    m_AdminConfig;
    void PluginBasicSpawnSelectServer()
    {
        Init();
    }

    void Init()
    {
        if(!m_ServerConfig)
            m_ServerConfig = GetBasicSpawnSelectServerConfig();

        if(!m_AdminConfig)
            m_AdminConfig = GetAdminConfig();
        
        for(int i = 0; i < m_AdminConfig.m_AdminSteamIDs.Count(); i++)
        {
            Print("Found Admin entry: " + m_AdminConfig.m_AdminSteamIDs.Get(i));
        }

        GetRPCManager().AddRPC("BasicSpawnSelect","SERVER_TELEPORTMETOSPAWN", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("BasicSpawnSelect","SERVER_ADDNEWSPAWNTICKET", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("BasicSpawnSelect","SERVER_ADDNEWSPAWN", this, SingleplayerExecutionType.Server);
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

    void SERVER_ADDNEWSPAWNTICKET(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if(type == CallType.Server)
        {
            if(IsSenderSSAdmin(sender.GetPlainId()))
            {
                Param1<ref SpawnTicketObject> data;
                if( !ctx.Read( data ) ) return;

                ref SpawnTicketObject   m_newTicketObj = data.param1;
                //string Classname, string Id, vector UsePosition, float UseRadius, ref array<ref SpawnLocationObject> Spawn
                m_ServerConfig.UpdateTickets(m_newTicketObj.GetClassName(), m_newTicketObj.GetId(), m_newTicketObj.GetUsePostion(), m_newTicketObj.GetUseRadius(), m_newTicketObj.GetLocationsFromTicket());
            }
            else
            {
                GetGame().AdminLog("[BasicSpawnSelect] -> Player with SteamID: " + sender.GetPlainId() + " tryed to send an Admin RPC maybe its an Hacker!");
            }
        }
    }

    void SERVER_ADDNEWSPAWN(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if(type == CallType.Server)
        {
            if(IsSenderSSAdmin(sender.GetPlainId()))
            {
                Param1<ref SpawnLocationObject> data;
                if( !ctx.Read( data ) ) return;

                ref SpawnLocationObject   m_newLocationObj = data.param1;
                //string SpawnLocationName, vector SpawnLocation, float RandomRadius
                m_ServerConfig.UpdateLocations(m_newLocationObj.GetName(), m_newLocationObj.GetExactSpot(), m_newLocationObj.GetExactRadius());
            }
            else
            {
                GetGame().AdminLog("[BasicSpawnSelect] -> Player with SteamID: " + sender.GetPlainId() + " tryed to send an Admin RPC maybe its an Hacker!");
            }
        }
    }

    void SendConfigToClient(PlayerIdentity identity)
    {
        if(!identity)
            return;
        
        GetRPCManager().SendRPC("BasicSpawnSelect", "ServerConfigResponse", new Param2< ref SpawnSelectConfig, bool >( m_ServerConfig, IsSenderSSAdmin(identity.GetPlainId())), true, identity);
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

    bool IsSenderSSAdmin(string PlainID)
    {
        if(!m_AdminConfig || !PlainID) return false;

        for(int i = 0; i < m_AdminConfig.m_AdminSteamIDs.Count(); i++)
        {
            if(m_AdminConfig.m_AdminSteamIDs.Get(i) == PlainID)
                return true;
        }
        return false;
    }
};

/* Global Plugin Getter */ 
PluginBasicSpawnSelectServer GetSpawnSelectServer()
{
	return PluginBasicSpawnSelectServer.Cast(GetPluginManager().GetPluginByType(PluginBasicSpawnSelectServer));
};