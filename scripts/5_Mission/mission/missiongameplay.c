modded class MissionGameplay extends MissionBase 
{
    //!Just a Debug thing here to hide & show the spawnmenu.
    #ifdef BASIC_SPAWN_SELECT_DEBUG
    override void OnKeyRelease(int key)
	{
		super.OnKeyRelease(key);
        if(key == KeyCode.KC_J)
        {
            GetSpawnSelectClient().OpenSpawnMenu();
        }
	}
    #endif
}