const static int BASIC_SPAWNSELECTMENU = 958432;
static bool SpawnSelectConfigRecived = false;
static void LogLine(string InputMsg)
{
    #ifdef BASIC_SPAWN_SELECT_DEBUG
    Print("[BasicSpawnSelect]-> " + InputMsg);
    #endif
};