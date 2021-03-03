class ActionOpenSpawnMenu: ActionContinuousBase
{
	void ActionOpenSpawnMenu()
	{
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override bool HasTarget()
	{
		return false;
	}
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if(!item && item.IsRuined())
			return false;
			
		BasicSpawnSelect_SpawnTicket_base spawn_ticket;
		if(Class.CastTo(spawn_ticket, item))
		{
			if(spawn_ticket.CanUseOnPosition(player.GetPosition()))
			{  
				return true;
			}
		}
		return false;
	}
	override string GetText()
	{
		return "Open Spawn Menu";
	}
	
	override void OnStartClient( ActionData action_data )
	{
		GetSpawnSelectClient().OpenSpawnMenu();
	}

	override void OnStartServer( ActionData action_data )
	{
		if(action_data && action_data.m_MainItem)
		{
			GetGame().ObjectDelete(action_data.m_MainItem);
		}
	}
};