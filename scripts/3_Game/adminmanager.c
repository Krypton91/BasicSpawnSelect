const protected static string m_AdminsPath = m_ProfileDIR + m_ConfigDIR + "/" + "adminConfig.json";

class AdminSpawnSelectConfig
{
    ref TStringArray m_AdminSteamIDs;
    void AdminSpawnSelectConfig()
    {
        m_AdminSteamIDs = new ref TStringArray();
    }

    void DefaultSettings()
    {
        m_AdminSteamIDs.Insert("ExampleSteamID");
        m_AdminSteamIDs.Insert("AnotherExampleSteamID");

        SaveAdminConfig();
    }

    void SaveAdminConfig()
    {
        if (!FileExist(m_ProfileDIR + m_ConfigDIR + "/"))
			MakeDirectory(m_ProfileDIR + m_ConfigDIR + "/");

        JsonFileLoader<AdminSpawnSelectConfig>.JsonSaveFile(m_AdminsPath, this);
    }

    static ref AdminSpawnSelectConfig LoadConfig()
    {
        ref AdminSpawnSelectConfig settings = new AdminSpawnSelectConfig();
        if(!FileExist(m_ConfigDIR))
            MakeDirectory(m_ConfigDIR);
        if(FileExist(m_AdminsPath))
        {
            JsonFileLoader<AdminSpawnSelectConfig>.JsonLoadFile(m_AdminsPath, settings);
        }
        else
        {
            settings.DefaultSettings();
        }
        return settings;
    }
};

/* Global Getter for config */
static ref AdminSpawnSelectConfig g_AdminSpawnSelectConfig;
static ref AdminSpawnSelectConfig GetAdminConfig() 
{
    if (g_Game.IsServer() && !g_AdminSpawnSelectConfig) 
    {
        g_AdminSpawnSelectConfig = AdminSpawnSelectConfig.LoadConfig();
    }
    return g_AdminSpawnSelectConfig;
};