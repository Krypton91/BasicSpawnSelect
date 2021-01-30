modded class MissionServer extends MissionBase
{    

	override PlayerBase OnClientNewEvent(PlayerIdentity identity, vector pos, ParamsReadContext ctx)
	{
		if(GetSpawnSelectServer().GetCurrentSpawnsystemMode() == 1)
		{
			string characterType;
			SyncRespawnModeInfo(identity);

			if(ProcessLoginData(ctx) && (m_RespawnMode == GameConstants.RESPAWN_MODE_CUSTOM) && GetGame().ListAvailableCharacters().Find(GetGame().GetMenuDefaultCharacterData().GetCharacterType()) > -1 && !GetGame().GetMenuDefaultCharacterData(false).IsRandomCharacterForced())
			{
				characterType = GetGame().GetMenuDefaultCharacterData().GetCharacterType();
			}
			else
			{
				characterType = GetGame().CreateRandomPlayer();
				GetGame().GetMenuDefaultCharacterData().GenerateRandomEquip();
			}
			if(CreateCharacter(identity, pos, ctx, characterType))
			{
				EquipCharacter(GetGame().GetMenuDefaultCharacterData());
				GetSpawnSelectServer().SendClientToSpawnSelectMenu(identity);
			}
			return m_player;
		}
		return super.OnClientNewEvent(identity, pos, ctx);
	}
};