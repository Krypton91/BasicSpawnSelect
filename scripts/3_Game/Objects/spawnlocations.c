class SpawnLocationObject
{
    protected string m_locationname;
    protected vector m_spawnlocation;
    protected float  m_randomradius;

    void SpawnLocationObject(string SpawnLocationName, vector SpawnLocation, float RandomRadius)
    {
        m_locationname = SpawnLocationName;
        m_spawnlocation = SpawnLocation;
        m_randomradius = RandomRadius;
    }

    string GetName()
    {
        return m_locationname;
    }

    vector GetSpawnRandomSpot()
    {
        local vector rndLoc = RandomXY(m_spawnlocation);
        local int maxCount = 50;
        while (!GetGame().SurfaceIsSea(rndLoc[0], rndLoc[2]) && !GetGame().SurfaceIsPond(rndLoc[0],rndLoc[2]) && maxCount > 0){
            maxCount--;
            rndLoc = RandomXY(m_spawnlocation);
        }
        if (maxCount <= 0){ //If couldn't get a good location return the location
            return m_spawnlocation;
        }
        rndLoc[1] = GetGame().SurfaceY(rndLoc[0],rndLoc[2]);
        return rndLoc;
        
    }

    vector RandomXY( vector spot){
        float rndT = Math.RandomFloat(0,1) * 2 * Math.PI;
        float rndR = m_randomradius * Math.Sqrt(Math.RandomFloat(0,1)) * 0.90;
        //Adding 10% of the radius to make so its not in the center feel like this will just seem less random
        float rndRD = m_randomradius * 0.10;
        rndR = rndR + rndRD;

        float NewX = spot[0] + rndR * Math.Cos(rndT);
        float NewY = spot[2] + rndR * Math.Sin(rndT);
        return Vector(NewX, 0, NewY);
    }

    float GetRandomRadius()
    {
        return m_randomradius;
    }
};