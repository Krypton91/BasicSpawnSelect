class ActionOpenSpawnMenu: ActionContinuousBase
{
	void ActionOpenSpawnMenu()
	{
		//m_CallbackClass = ActionReadPaperCB;
		//m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_VIEWNOTE;
		//m_FullBody = true;
		//m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_PRONE;
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