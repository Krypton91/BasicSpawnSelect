const protected static string m_ProfileDIR = "$profile:";
const protected static string m_ConfigDIR = "BasicSpawnSelect";
const protected static string m_ConfigPath = m_ProfileDIR + m_ConfigDIR + "/" + "server-config.json";

class SpawnSelectConfig
{
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
        SpawnSystemMode = 1;
        SpawnLocations.Insert(new ref SpawnLocationObject("Kamenka", Vector(1892.109985,6.171125,2244.129883), 500));
        SpawnLocations.Insert(new ref SpawnLocationObject("West Balota", Vector(4025.159912,6.494447,2620.459961), 1000));
        SpawnLocations.Insert(new ref SpawnLocationObject("Chernogorsk", Vector(6387.430176,9.290138,2699.479980), 1000));
        SpawnLocations.Insert(new ref SpawnLocationObject("Elektrozavodsk", Vector(10322.400391,5.811278,2171.159912), 1000));
        SpawnTickets.Insert(new ref SpawnTicketObject("BasicSpawnSelect_SpawnTicket", "Bandit", Vector(4111.30, 0, 8912.79), 50, SpawnLocations));
        SpawnTickets.Insert(new ref SpawnTicketObject("BasicSpawnSelect_SpawnTicket", "Admin",  Vector(4111.30, 0, 8912.79), 50, SpawnLocations));

        SaveSpawnConfig();
    }

    void SaveSpawnConfig()
    {
        if (!FileExist(m_ProfileDIR + m_ConfigDIR + "/"))
			MakeDirectory(m_ProfileDIR + m_ConfigDIR + "/");

        JsonFileLoader<SpawnSelectConfig>.JsonSaveFile(m_ConfigPath, this);
    }

    void UpdateTickets(string Classname, string Id, vector UsePosition, float UseRadius, ref array<ref SpawnLocationObject> Spawn)
    {
        SpawnTickets.Insert(new ref SpawnTicketObject(Classname, Id, UsePosition, UseRadius, Spawn));
        JsonFileLoader<SpawnSelectConfig>.JsonSaveFile(m_ConfigPath, this);
    }

    void UpdateLocations(string SpawnLocationName, vector SpawnLocation, float RandomRadius)
    {
        if(!SpawnLocationName || !SpawnLocation || !RandomRadius) return;
        SpawnLocations.Insert(new ref SpawnLocationObject(SpawnLocationName, SpawnLocation, RandomRadius));
        JsonFileLoader<SpawnSelectConfig>.JsonSaveFile(m_ConfigPath, this);
    }

    void DeleteLocation(string SpawnLocationName, vector SpawnLocation, float RandomRadius)
    {
        for(int i = 0; i < SpawnLocations.Count(); i++)
        {
            if(SpawnLocations.Get(i).GetName() == SpawnLocationName && SpawnLocations.Get(i).GetExactSpot() == SpawnLocation && SpawnLocations.Get(i).GetExactRadius() == RandomRadius)
            {
                SpawnLocations.Remove(i);
            }
            JsonFileLoader<SpawnSelectConfig>.JsonSaveFile(m_ConfigPath, this);
        }
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