class BasicSpawnSelectMenu extends UIScriptedMenu
{
    protected bool                                  m_IsMenuInitialized;
    protected bool                                  m_SpawnMenuVisibleState;
    protected ButtonWidget                          m_BtnSpawnOnSelectedLocation;
    protected ButtonWidget                          m_BtnRandomSpawn;
    protected MapWidget                             m_BasicSpawnMapWidget;
    protected TextListboxWidget                     m_LocationList;
    protected ref array<ref SpawnLocationObject>    m_PossibleSpawns;

    void BasicSpawnSelectMenu()
    {
        if(!m_PossibleSpawns)
            m_PossibleSpawns = GetSpawnSelectClient().GetPossibleServerSpawns();
    }
    override Widget Init()
    {
        if(!m_IsMenuInitialized) 
        {
            layoutRoot                      = GetGame().GetWorkspace().CreateWidgets("BasicSpawnSelect/GUI/layouts/SpawnSelectMenu.layout");
            m_BtnSpawnOnSelectedLocation    = ButtonWidget.Cast(layoutRoot.FindAnyWidget("BtnSpawnOnLocation"));
            m_BtnRandomSpawn                = ButtonWidget.Cast(layoutRoot.FindAnyWidget("BtnRandom"));
            m_BasicSpawnMapWidget           = MapWidget.Cast(layoutRoot.FindAnyWidget("SpawnSelectMap"));
            m_LocationList                  = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("LocationList"));
            m_IsMenuInitialized = true;
        }

        return layoutRoot;
    }
    
    override void OnShow()
	{
        LoadSpawnPoints();
        
		super.OnShow();

		PPEffects.SetBlurMenu(0.5);

		GetGame().GetInput().ChangeGameFocus(1);

		SetFocus( layoutRoot );

		GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);

        GetGame().GetUIManager().ShowUICursor( true );
	}

    override void OnHide()
	{
		super.OnHide();

		PPEffects.SetBlurMenu(0);

		GetGame().GetInput().ResetGameFocus();

		GetGame().GetMission().PlayerControlEnable(false);

        GetGame().GetUIManager().ShowUICursor( false );

        SetIsSpawnMenuOpen( false );

		Close();
    }

    override bool OnClick( Widget w, int x, int y, int button )
	{
        if(m_LocationList)
            local int row_index = m_LocationList.GetSelectedRow();
        switch(w)
        {
            case m_BtnSpawnOnSelectedLocation:
                HandleTeleportToSpawnPoint(row_index);
                break;
            case m_BtnRandomSpawn:
                HandleTeleportToRandomPoint();
                break;
        }

        return super.OnClick(w, x, y, button);
    }

    void HandleTeleportToRandomPoint()
    {
        SpawnLocationObject m_selectedSpawn;
        m_selectedSpawn = m_PossibleSpawns.GetRandomElement();

        if(m_selectedSpawn)
        {
            PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
            if(!player) return;
            GetRPCManager().SendRPC("BasicSpawnSelect", "SERVER_TELEPORTMETOSPAWN", new Param2< ref SpawnLocationObject, PlayerBase >( m_selectedSpawn, player ), true);
        }
        else
        {
            Error("Error m_selectedSpawn was null, if you see this message a few seconds befor, then the crash was not because this.");
        }
    }

    void HandleTeleportToSpawnPoint(int selected_rowIndex)
    {
        if(!selected_rowIndex && selected_rowIndex == -1) return;

            SpawnLocationObject m_selectedSpawn;
            m_selectedSpawn = m_PossibleSpawns.Get(selected_rowIndex);
            if(m_selectedSpawn)
            {
                PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
                if(!player) return;
                GetRPCManager().SendRPC("BasicSpawnSelect", "SERVER_TELEPORTMETOSPAWN", new Param2< ref SpawnLocationObject, PlayerBase >( m_selectedSpawn, player ), true);
            }
            else
            {
                Error("Error m_selectedSpawn was null, if you see this message a few seconds befor, then the crash was not because this.");
            }
    }

    void LoadSpawnPoints()
    {
        if(m_LocationList && m_LocationList.GetNumItems() <= 0)
            m_LocationList.ClearItems();
        
        for(int i = 0; i < m_PossibleSpawns.Count(); i++)
        {
            m_LocationList.AddItem( m_PossibleSpawns.Get(i).GetName(), NULL, 0 );
        }
    }

    /* Getter & Setter */
    bool IsSpawnMenuVisible()
    {
        return m_SpawnMenuVisibleState;
    }
    void SetMapPos(vector pos)
    {
		m_BasicSpawnMapWidget.SetMapPos(pos);
	}

    void SetIsSpawnMenuOpen(bool state)
    {
        m_SpawnMenuVisibleState = state;
    }
}