class SpawnTicketObject
{
    protected string                                m_classname;
    protected string                                m_ID;
    protected string                                m_UseDaysUTC;
    protected string                                m_UseHoursUTC;
    protected vector 				                m_UsePosition;
	protected float 				                m_UseRadius;
    protected ref array <ref SpawnLocationObject>   m_PossibleSpawn;

    void SpawnTicketObject(string Classname, string Id, vector UsePosition, float UseRadius, ref array<ref SpawnLocationObject> Spawn, string UseDaysUTC, string UseHoursUTC)
    {
        m_classname = Classname;
        m_ID = Id;
        m_PossibleSpawn = Spawn;
        m_UsePosition = UsePosition;
        m_UseDaysUTC = UseDaysUTC;
        m_UseHoursUTC = UseHoursUTC;
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

	bool IsInUseTime()
	{
		if((m_UseDaysUTC.Length() == 0 && m_UseHoursUTC.Length() == 0))
		{
			return true;
		}
		
		int /*year, month, day, hour, minute, */utcHour, utcMinute, utcSecond, i, timeUTC;
		GetHourMinuteSecondUTC(utcHour, utcMinute, utcSecond);
		//GetGame().GetWorld().GetDate( year, month, day, hour, minute );
		timeUTC = (utcHour*60)+utcMinute;
		
		// m_UseDaysUTC "1 2 3 4 5 6 7"
		if(m_UseDaysUTC.Length() > 0)
		{
			int utcWeekDay = GetDayOfWeek();
			if (m_UseDaysUTC.IndexOf(utcWeekDay.ToString()) == -1) {
				return false;
			}
		}
		// m_UseHoursUTC "8 9-10 11-11:30 12:15"
		if(m_UseHoursUTC.Length() > 0)
		{
			bool timeMatch = false;
			int ticketTimeMin, ticketTimeMax;
			TStringArray hours = new TStringArray;
			m_UseHoursUTC.Split( " ", hours );
			for ( i = 0; i < hours.Count(); i++ )
			{
				if(hours.Get(i).IndexOf( "-" ) != -1)
				{
					TStringArray hourRange = new TStringArray;
					hours.Get(i).Split( "-", hourRange );
					ticketTimeMin = ConvertToMinutes(hourRange.Get(0));
					ticketTimeMax = ConvertToMinutes(hourRange.Get(1));
					if(timeUTC >= ticketTimeMin && timeUTC <= ticketTimeMax)
					{
						timeMatch = true;
						break;
					}
				}
				else
				{
					int ticketTime = ConvertToMinutes(hours.Get(i));
					if(ticketTime == timeUTC)
					{
						timeMatch = true;
						break;
					}
				}
			}
			if(!timeMatch)
			{
				return false;
			}
		}
		
		return true;
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

    string GetUseDaysUTC()
    {
        return m_UseDaysUTC;
    }

    string GetUseHoursUTC()
    {
        return m_UseHoursUTC;
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
	
	// Converts string formatted hour[:minute] to minutes
	// e.g. "1" => 60, "3:15" => 195
	private int ConvertToMinutes(string hoursStr)
	{
		int hours, minutes = 0;
		if(hoursStr.IndexOf( ":" ) != -1)
		{
			TStringArray hoursMinutes = new TStringArray;
			hoursStr.Split( ":", hoursMinutes );
			hours = hoursMinutes.Get(0).ToInt();
			minutes = hoursMinutes.Get(1).ToInt();
		}
		else
		{
			hours = hoursStr.ToInt();
		}
		return (hours*60) + minutes;
	}
	
	// Returns today's number of the day in a week.
	private int GetDayOfWeek()
	{
		int c, y, m, d;
		GetYearMonthDayUTC(y, m, d);

		c = y/100;
		if (m < 3)
		{
			y -= 1;
			m += 10;
		}
		else m -= 2;
		
		int dowraw = (d + Math.Floor(2.6 * m - 0.2) - 2 * c + y + Math.Floor(y / 4) + Math.Floor(c / 4));

		// Here we get the remainder of dowraw divided by 7.
		// Basically, should have been just "dowraw % 7", but DayZ says it doesn't know the % operator,
		// Yet, it is definitely used in enscript though. Odd stuff.
		int dow = Math.Floor(dowraw / 7);
		dow = dow * 7;
		dow = dowraw - dow;
		if (dow == 0) dow = 7;

		// Move days for Mon-Sun week.
		dow -= 1;
		if (dow == 0) dow = 7;

		return dow;
	}
};