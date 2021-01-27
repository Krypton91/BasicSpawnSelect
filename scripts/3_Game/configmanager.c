const protected static string m_ProfileDIR = "$profile:";
const protected static string m_ConfigDIR = "BasicSpawnSelect";
const protected static string m_ConfigPath = m_ProfileDIR + m_ConfigDIR + "/" + "server-config.json";

class SpawnSelectConfig
{
    float                               MinSpawnDistanceToDeathBody;
    float                               CooldownForSelectedLocation;
    int                                 SpawnSystemMode;
    ref array<ref SpawnLocationObject>  SpawnLocations;
    ref array<ref SpawnTicketObject>    SpawnTickets;

    void SpawnSelectConfig()
    {
        SpawnLocations = new ref array<ref SpawnLocationObject>();
        SpawnTickets = new ref array<ref SpawnTicketObject>();
    }

    void DefaultSettings()
    {
        MinSpawnDistanceToDeathBody = 5000;
        CooldownForSelectedLocation = 10;
        SpawnSystemMode = 1;
        SpawnLocations.Insert(new ref SpawnLocationObject("Kamenka", Vector(1892.109985,6.171125,2244.129883), 500));
        SpawnLocations.Insert(new ref SpawnLocationObject("West Balota", Vector(4025.159912,6.494447,2620.459961), 1000));
        SpawnLocations.Insert(new ref SpawnLocationObject("Chernogorsk", Vector(6387.430176,9.290138,2699.479980), 1000));
        SpawnLocations.Insert(new ref SpawnLocationObject("Elektrozavodsk", Vector(10322.400391,5.811278,2171.159912), 1000));
        SpawnTickets.Insert(new ref SpawnTicketObject("BasicSpawnSelect_SpawnTicket", "Bandit", "Green Mtn", Vector(1234.21,233.2,23432.4), 200));
        SpawnTickets.Insert(new ref SpawnTicketObject("BasicSpawnSelect_SpawnTicket", "Admin", "Green Mtn", Vector(1234.21,233.2,23432.4), 200));

        SaveSpawnConfig();
    }

    void SaveSpawnConfig()
    {
        if (!FileExist(m_ProfileDIR + m_ConfigDIR + "/"))
			MakeDirectory(m_ProfileDIR + m_ConfigDIR + "/");

        JsonFileLoader<SpawnSelectConfig>.JsonSaveFile(m_ConfigPath, this);
    }

    static ref SpawnSelectConfig LoadConfig()
    {
        ref SpawnSelectConfig settings = new SpawnSelectConfig();
        if(!FileExist(m_ConfigDIR))
            MakeDirectory(m_ConfigDIR);
        if(FileExist(m_ConfigPath))
        {
            LogLine("Found server-config loading existing config...");
            JsonFileLoader<SpawnSelectConfig>.JsonLoadFile(m_ConfigPath, settings);
        }
        else
        {
            LogLine("No server-config found try to write default config...");
            settings.DefaultSettings();
        }
        return settings;
    }
};
/* Global Getter for config */
static ref SpawnSelectConfig g_SpawnSelectConfig;
static ref SpawnSelectConfig GetBasicSpawnSelectServerConfig() 
{
    if (g_Game.IsServer() && !g_SpawnSelectConfig) 
    {
        g_SpawnSelectConfig = SpawnSelectConfig.LoadConfig();
    }
    return g_SpawnSelectConfig;
};