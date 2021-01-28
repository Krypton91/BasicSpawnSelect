class BasicSpawnSelect_SpawnTicket_base extends ItemBase
{
	protected string 				m_TicketID;
	protected ref SpawnSelectConfig m_ServerConfig;

	void BasicSpawnSelect_SpawnTicket_base()
	{
		if(!m_ServerConfig)
			m_ServerConfig = GetSpawnSelectClient().m_ServerConfig;
		if(m_ServerConfig && m_ServerConfig.SpawnSystemMode == 2)
		{
			for(int i = 0; i < m_ServerConfig.SpawnTickets.Count(); i++)
			{
				if(m_TicketID)
					break;
				if(m_ServerConfig.SpawnTickets.Get(i).GetClassName() && m_ServerConfig.SpawnTickets.Get(i).GetClassName() == this.GetType())
				{
					AssignNewIdToTicket(m_ServerConfig.SpawnTickets.Get(i).GetId());
				}
			}
		}
		else
		{
			Print("[BasicSpawnSelect]-> Wrong SystemSpawnMode in server config to use this!");
		}
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{   

		if ( !super.OnStoreLoad(ctx, version) )
			return false;
		
		if ( !ctx.Read(m_TicketID) )
			return false;
				
		return true;
	}

	void AssignNewIdToTicket(string newID)
	{
		m_TicketID = newID;	
	}

	bool IsSpawnSelectTicket()
	{
		return true;
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		
		ctx.Write(m_TicketID);		
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionOpenSpawnMenu);
	}

	string GetSpawnTicketId()
	{
		return m_TicketID;
	}
};
//class BasicSpawnSelect_SpawnTicketBandit: BasicSpawnSelect_SpawnTicket_base {};
//class BasicSpawnSelect_SpawnTicketAdmin : BasicSpawnSelect_SpawnTicket_base {};