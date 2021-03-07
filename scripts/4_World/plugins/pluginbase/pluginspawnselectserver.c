class PluginBasicSpawnSelectServer extends PluginBase
{
    protected ref SpawnSelectConfig         m_ServerConfig;
    protected ref AdminSpawnSelectConfig    m_AdminConfig;
    protected vector                        m_FreeSpawnSize = Vector(3,5,9);
    void PluginBasicSpawnSelectServer()
    {
        Init();
    }

    void Init()
    {
        if(!m_ServerConfig)
            m_ServerConfig = GetBasicSpawnSelectServerConfig(false);

        if(!m_AdminConfig)
            m_AdminConfig = GetAdminConfig();

        GetRPCManager().AddRPC("BasicSpawnSelect","SERVER_TELEPORTMETOSPAWN", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("BasicSpawnSelect","SERVER_SENDCONFIGTOCLIENT", this, SingleplayerExecutionType.Server);

        /* Admin Stuff */
        GetRPCManager().AddRPC("BasicSpawnSelect","SERVER_ADDNEWSPAWNTICKET", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("BasicSpawnSelect","SERVER_ADDNEWSPAWN", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("BasicSpawnSelect","SERVER_RELOACONFIG", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("BasicSpawnSelect","SERVER_DELETESPAWN", this, SingleplayerExecutionType.Server);
    }

    void SERVER_SENDCONFIGTOCLIENT(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if(type == CallType.Server)
        {
            SendConfigToClient(sender);
        }
    }

    void SERVER_TELEPORTMETOSPAWN(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if(type == CallType.Server)
        {
            Param2<ref SpawnLocationObject, PlayerBase> data;
            if ( !ctx.Read( data ) ) return;
                
            SpawnLocationObject m_choosenSpawn = data.param1;
            PlayerBase player = data.param2;
            if(player)
            {
                vector FinalSpawnPos = m_choosenSpawn.GetSpawnRandomSpot();
                local int maxtrycounter = 0;
                array<Object> excluded_objects = new array<Object>;
		        array<Object> nearby_objects = new array<Object>;

                while(GetGame().IsBoxColliding(FinalSpawnPos, player.GetOrientation(), m_FreeSpawnSize, excluded_objects, nearby_objects) && maxtrycounter < 50)
                {
                    FinalSpawnPos = m_choosenSpawn.GetSpawnRandomSpot();
                    maxtrycounter++;
                }
                player.SetPosition(FinalSpawnPos);
                player.SetOrientation(player.GetOrientation());
            }
            else
            {
                Error("Player was not valid! if you see this befor an crash it was not because this!");
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
                m_ServerConfig.UpdateTickets(m_newTicketObj.GetClassName(), m_newTicketObj.GetId(), m_newTicketObj.GetUsePostion(), m_newTicketObj.GetUseRadius(), m_newTicketObj.GetLocationsFromTicket(), m_newTicketObj.GetUseDaysUTC(), m_newTicketObj.GetUseHoursUTC());
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

    void SERVER_DELETESPAWN(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if(type == CallType.Server)
        {
            if(IsSenderSSAdmin(sender.GetPlainId()))
            {
                Param1<ref SpawnLocationObject> data;
                if( !ctx.Read( data ) ) return;

                ref SpawnLocationObject   m_newLocationObj = data.param1;
                m_ServerConfig.DeleteLocation(m_newLocationObj.GetName(), m_newLocationObj.GetExactSpot(), m_newLocationObj.GetExactRadius());
            }
            else
            {
                GetGame().AdminLog("[BasicSpawnSelect] -> Player with SteamID: " + sender.GetPlainId() + " tryed to send an Admin RPC maybe its an Hacker!");
            }
        }
    }

    void SERVER_RELOACONFIG(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if(type == CallType.Server)
        {
            if(IsSenderSSAdmin(sender.GetPlainId()))
            {
                //Reload Config
                m_ServerConfig = GetBasicSpawnSelectServerConfig(true);
                array<Man> AllOnlinePlayers = new array<Man>();
                GetGame().GetPlayers(AllOnlinePlayers);

                for(int i = 0; i < AllOnlinePlayers.Count(); i++)
                {
                    SendConfigToClient(AllOnlinePlayers.Get(i).GetIdentity());//Sync new config to all Clients on server.
                }
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