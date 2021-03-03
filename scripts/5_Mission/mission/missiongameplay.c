modded class MissionGameplay extends MissionBase 
{
    override void OnKeyRelease(int key)
	{
        if(key == KeyCode.KC_ESCAPE && GetSpawnSelectClient().IsLocalPlayerAdmin())
        {
            GetSpawnSelectClient().CloseSpawnMenu();
        }
        super.OnKeyRelease(key);
    }

    override void OnUpdate(float timeslice) 
    {
        super.OnUpdate(timeslice);
        if (GetUApi() && !m_UIManager.IsMenuOpen(MENU_CHAT_INPUT))
        {
            Input input = GetGame().GetInput();
            if (input.LocalPress("UABasicSpawnSelection", false) && GetSpawnSelectClient().IsLocalPlayerAdmin())
            {
                GetSpawnSelectClient().OpenSpawnMenu();
            }
        }
    }
}