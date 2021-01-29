const protected static string m_SSAdminsPath = m_SSProfileDIR + m_SSConfigDIR + "/" + "admin-config.json";

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
        if (!FileExist(m_SSProfileDIR + m_SSConfigDIR + "/"))
			MakeDirectory(m_SSProfileDIR + m_SSConfigDIR + "/");

        JsonFileLoader<AdminSpawnSelectConfig>.JsonSaveFile(m_SSAdminsPath, this);
    }

    static ref AdminSpawnSelectConfig LoadConfig()
    {
        ref AdminSpawnSelectConfig settings = new AdminSpawnSelectConfig();
        if(!FileExist(m_SSConfigDIR))
            MakeDirectory(m_SSConfigDIR);
        if(FileExist(m_SSAdminsPath))
        {
            JsonFileLoader<AdminSpawnSelectConfig>.JsonLoadFile(m_SSAdminsPath, settings);
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