class PluginBasicSpawnSelectClient extends PluginBase
{
    protected ref BasicSpawnSelectMenu  m_SpawnMenu;
    ref SpawnSelectConfig     m_ServerConfig = new ref SpawnSelectConfig();
    void PluginBasicSpawnSelectClient()
    {
        Init();
    }

    void Init()
    {
        LogLine("Client Init Start");
        GetRPCManager().AddRPC("BasicSpawnSelect","ServerConfigResponse", this, SingleplayerExecutionType.Client);
        GetRPCManager().AddRPC("BasicSpawnSelect","OpenSpawnSelectMenu", this, SingleplayerExecutionType.Client);
    }

    void OpenSpawnMenu()
    {
        if(GetGame().GetUIManager().GetMenu() == NULL && m_SpawnMenu == null)
        {
            m_SpawnMenu = BasicSpawnSelectMenu.Cast(GetGame().GetUIManager().EnterScriptedMenu(BASIC_SPAWNSELECTMENU, null));
        }
        #ifdef BASIC_SPAWN_SELECT_DEBUG
        else if(GetGame().GetUIManager().GetMenu() != NULL && m_SpawnMenu && m_SpawnMenu.IsSpawnMenuVisible())
        {
            //HideMenu Todo: Purge this in release version!
            GetGame().GetUIManager().HideScriptedMenu(m_SpawnMenu);
        }
        #endif
        else
        {
            if(GetGame().GetUIManager().GetMenu() == NULL && !m_SpawnMenu.IsSpawnMenuVisible())
            {
                GetGame().GetUIManager().ShowScriptedMenu(m_SpawnMenu, NULL);
            }
        }
    }

    void ServerConfigResponse(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if(type == CallType.Client)
        {
            Param1<ref SpawnSelectConfig> data;
            if ( !ctx.Read( data ) ) return;
            m_ServerConfig = data.param1;
            LogLine("Server config sucessfully recived!");
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
        if (g_Game.GetUIManager().GetMenu() == NULL)
		{
			if (g_Game.GetMissionState() == DayZGame.MISSION_STATE_GAME && GetGame().GetMission().GetHud())
			{
                OpenSpawnMenu();
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.TryToOpenSpawnSelectMenu);		
			}																					
		}
    }

    ref array<ref SpawnTicketObject> GetPossibleSpawnTickets()
    {
        return m_ServerConfig.SpawnTickets;
    }
    /* Getter & Setter Section */
    ref array<ref SpawnLocationObject> GetPossibleServerSpawns()
    {
        return m_ServerConfig.SpawnLocations;
    }
};

/* Global Plugin Getter */ 
PluginBasicSpawnSelectClient GetSpawnSelectClient()
{
	return PluginBasicSpawnSelectClient.Cast(GetPluginManager().GetPluginByType(PluginBasicSpawnSelectClient));
};