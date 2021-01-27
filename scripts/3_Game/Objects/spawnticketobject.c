class SpawnTicketObject
{
    protected string m_classname;
    protected string m_ID;
    protected string m_SpawnName;
    protected vector m_Locations;
    protected float  m_randomradius;

    void SpawnTicketObject(string Classname, string Id, string SpawnName, vector Location, float RandomRadius)
    {
        m_classname = Classname;
        m_ID = Id;
        m_Locations = Location;
        m_SpawnName = m_SpawnName;
        m_randomradius = RandomRadius;
    }

    string GetClassName()
    {
        return m_classname;
    }

    string GetId()
    {
        return m_ID;
    }
    
    string GetSpawnName()
    {
        return m_SpawnName;        
    }

    vector GetLocation()
    {
        return m_Locations;
    }

    float GetRandomRadius()
    {
        return m_randomradius;
    }
}