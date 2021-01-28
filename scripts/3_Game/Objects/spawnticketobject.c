class SpawnTicketObject
{
    protected string m_classname;
    protected string m_ID;
    protected ref array <ref SpawnLocationObject> m_PossibleSpawn;

    void SpawnTicketObject(string Classname, string Id, ref array<ref SpawnLocationObject> Spawn)
    {
        m_classname = Classname;
        m_ID = Id;
        m_PossibleSpawn = Spawn;
    }

    string GetClassName()
    {
        return m_classname;
    }

    string GetId()
    {
        return m_ID;
    }

    ref array <ref SpawnLocationObject> GetLocationsFromTicket()
    {
        return m_PossibleSpawn;
    }
};