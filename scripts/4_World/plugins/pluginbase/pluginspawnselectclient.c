class PluginBasicSpawnSelectClient extends PluginBase
{
    protected ref BasicSpawnSelectMenu  m_SpawnMenu;
    ref SpawnSelectConfig               m_ServerConfig;
    protected bool                      m_Admin;

    void PluginBasicSpawnSelectClient()
    {
        Init();
    }

    void ~PluginBasicSpawnSelectClient()
    {
        SpawnSelectConfigRecived = false;
        if(m_SpawnMenu)
            delete m_SpawnMenu;
        if(m_ServerConfig)
            delete m_ServerConfig;
        
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.TryToOpenSpawnSelectMenu);
    }

    void Init()
    {
        if(!m_ServerConfig)
            m_ServerConfig = new ref SpawnSelectConfig();

        GetRPCManager().AddRPC("BasicSpawnSelect","ServerConfigResponse", this, SingleplayerExecutionType.Client);
        GetRPCManager().AddRPC("BasicSpawnSelect","OpenSpawnSelectMenu", this, SingleplayerExecutionType.Client);
        GetRPCManager().SendRPC("BasicSpawnSelect", "SERVER_SENDCONFIGTOCLIENT", null, true);
    }

    void OpenSpawnMenu()
    {
        if(!SpawnSelectConfigRecived) return; //Make sure we recived the server config.
        if(GetGame().GetUIManager().GetMenu() == NULL && m_SpawnMenu == null)
        {
            m_SpawnMenu = BasicSpawnSelectMenu.Cast(GetGame().GetUIManager().EnterScriptedMenu(BASIC_SPAWNSELECTMENU, null));
        }
        else if(GetGame().GetUIManager().GetMenu() != NULL && m_SpawnMenu && m_SpawnMenu.IsSpawnMenuVisible() && IsLocalPlayerAdmin())
        {
            GetGame().GetUIManager().Back();
        }
        else
        {
            if(GetGame().GetUIManager().GetMenu() == NULL && !m_SpawnMenu.IsSpawnMenuVisible())
            {
                GetGame().GetUIManager().ShowScriptedMenu(m_SpawnMenu, NULL);
            }
        }
    }

    //Just for Admins.
    void CloseSpawnMenu()
    {
        if(m_SpawnMenu && m_SpawnMenu.IsVisible())
            m_SpawnMenu.HideMenu();
    }

    void ServerConfigResponse(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if(type == CallType.Client)
        {
            Param2<ref SpawnSelectConfig, bool> data;
            if ( !ctx.Read( data ) ) return;
            m_ServerConfig = data.param1;
            m_Admin        = data.param2;
            SpawnSelectConfigRecived = true;
        }
    }

    void OpenSpawnSelectMenu(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if(type == CallType.Client)
        {
            GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.TryToOpenSpawnSelectMenu, 1500, true);
        }
    }

    void TryToOpenSpawnSelectMenu()
    {
        if (GetGame().GetUIManager().GetMenu() == NULL)
		{
			if (g_Game.GetMissionState() == DayZGame.MISSION_STATE_GAME && GetGame().GetMission().GetHud() && SpawnSelectConfigRecived)
			{
                OpenSpawnMenu();
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.TryToOpenSpawnSelectMenu);		
			}																					
		}
    }

    /* Getter & Setter Section */
    ref array<ref SpawnTicketObject> GetPossibleSpawnTickets()
    {
        return m_ServerConfig.SpawnTickets;
    }
    
    ref array<ref SpawnLocationObject> GetPossibleServerSpawns()
    {
        if(m_ServerConfig.SpawnSystemMode == 2)
        {
            PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
            ItemBase item_in_hands = ItemBase.Cast(player.GetItemInHands());
            if(item_in_hands)
            {
                for(int i = 0; i < GetPossibleSpawnTickets().Count(); i++)
                {
                    if(GetPossibleSpawnTickets().Get(i).GetClassName() == item_in_hands.GetType())
                    {
                        //Spawn Ticket found retrun Location Obj.
                        return GetPossibleSpawnTickets().Get(i).GetLocationsFromTicket();
                    }
                }
            }
        }
        return m_ServerConfig.SpawnLocations;
    }

    bool IsLocalPlayerAdmin()
    {
        return m_Admin;
    }
};

/* Global Plugin Getter */ 
PluginBasicSpawnSelectClient GetSpawnSelectClient()
{
	return PluginBasicSpawnSelectClient.Cast(GetPluginManager().GetPluginByType(PluginBasicSpawnSelectClient));
};