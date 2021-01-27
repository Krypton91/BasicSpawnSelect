static void LogLine(string InputMsg)
{
    #ifdef BASIC_SPAWN_SELECT_DEBUG
    Print("[BasicSpawnSelect]-> " + InputMsg);
    #endif
};