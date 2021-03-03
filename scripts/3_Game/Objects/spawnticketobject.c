class SpawnTicketObject
{
    protected string                                m_classname;
    protected string                                m_ID;
    protected vector 				                m_UsePosition;
	protected float 				                m_UseRadius;
    protected ref array <ref SpawnLocationObject>   m_PossibleSpawn;

    void SpawnTicketObject(string Classname, string Id, vector UsePosition, float UseRadius, ref array<ref SpawnLocationObject> Spawn)
    {
        m_classname = Classname;
        m_ID = Id;
        m_PossibleSpawn = Spawn;
        m_UsePosition = UsePosition;
        m_UseRadius = UseRadius;
    }

    string GetClassName()
    {
        return m_classname;
    }

    string GetId()
    {
        return m_ID;
    }

    bool IsInUseRange(vector Position)
    {
        if(m_UsePosition && m_UseRadius && Position)
        {
            float CurrentRange = vector.Distance(Position, m_UsePosition);
            if(CurrentRange < m_UseRadius)
            {
                return true;
            }
            return false;
        }
        return true;
    }

    vector GetUsePostion()
    {
        return m_UsePosition;
    }

    float GetUseRadius()
    {
        return m_UseRadius;
    }

    ref array <ref SpawnLocationObject> GetLocationsFromTicket()
    {
        return m_PossibleSpawn;
    }
};