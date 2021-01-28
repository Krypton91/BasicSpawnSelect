class PluginBasicSpawnSelectClient extends PluginBase
{
    protected ref SpawnSelectConfig     m_ServerConfig = new ref SpawnSelectConfig();
    protected ref BasicSpawnSelectMenu  m_SpawnMenu;

    void PluginBasicSpawnSelectClient()
    {
        Init();
    }

    void Init()
    {
        LogLine("Client Init Start");
        GetRPCManager().AddRPC("BasicSpawnSelect","ServerConfigResponse", this, SingleplayerExecutionType.Server);
    }

    void OpenSpawnMenu()
    {
        if(GetGame().GetUIManager().GetMenu() == NULL && m_SpawnMenu == null)
        {
            m_SpawnMenu = BasicSpawnSelectMenu.Cast(GetGame().GetUIManager().EnterScriptedMenu(BASIC_SPAWNSELECTMENU, null));
            m_SpawnMenu.SetIsSpawnMenuOpen(true);
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
                m_SpawnMenu.SetIsSpawnMenuOpen(true);
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


    /* Getter & Setter Section*/
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