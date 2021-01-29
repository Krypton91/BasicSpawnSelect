modded class MissionGameplay extends MissionBase 
{
    override void OnKeyRelease(int key)
	{
        if(key == KeyCode.KC_J)
        {
            GetSpawnSelectClient().OpenSpawnMenu();
        }
        super.OnKeyRelease(key);
	}
}