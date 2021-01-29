class BasicSpawnSelectMenu extends UIScriptedMenu
{
    protected bool                                  m_IsMenuInitialized;
    protected bool                                  m_SpawnMenuVisibleState;
    protected ButtonWidget                          m_BtnSpawnOnSelectedLocation;
    protected ButtonWidget                          m_BtnRandomSpawn;
    protected ButtonWidget                          m_BtnAdminInsertSpawn;
    protected ButtonWidget                          m_BtnAdminDeleteSpawn;
    protected ButtonWidget                          m_BtnAdminReloadConfig;
    protected MapWidget                             m_BasicSpawnMapWidget;

    /* Admin Section */
    protected Widget                                m_AdminSectionCard;
    protected Widget                                m_AdminTicketModuleCard;
    protected ButtonWidget                          m_BtnAdminCloseSection;
    protected ButtonWidget                          m_BtnAdminOpenMap;
    protected ButtonWidget                          m_BtnAdminNeedHelp;
    protected ButtonWidget                          m_BtnAdminSendToServer;
    protected EditBoxWidget                         m_AdminSpawnBoxPosX;
    protected EditBoxWidget                         m_AdminSpawnBoxPosY;
    protected EditBoxWidget                         m_AdminSpawnBoxPosZ;
    protected EditBoxWidget                         m_AdminSpawnBoxLocationName;
    protected EditBoxWidget                         m_AdminTicketUseBoxPosX;
    protected EditBoxWidget                         m_AdminTicketUseBoxPosY;
    protected EditBoxWidget                         m_AdminTicketUseBoxPosZ;
    protected EditBoxWidget                         m_AdminTicketUseRadius;
    protected EditBoxWidget                         m_AdminTicketClassName;
    protected CheckBoxWidget                        m_AdminInsertAsSpawnTicketBox;
    protected CheckBoxWidget                        m_AdminUseItemInHandsBox;

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
            m_BtnAdminInsertSpawn           = ButtonWidget.Cast(layoutRoot.FindAnyWidget("BtnInsertSpawn"));
            m_BtnAdminDeleteSpawn           = ButtonWidget.Cast(layoutRoot.FindAnyWidget("BtnDeleteSpawn"));
            m_BtnAdminReloadConfig          = ButtonWidget.Cast(layoutRoot.FindAnyWidget("BtnReloadConfig"));
            m_BtnAdminCloseSection          = ButtonWidget.Cast(layoutRoot.FindAnyWidget("BtnClose"));
            m_BtnAdminOpenMap               = ButtonWidget.Cast(layoutRoot.FindAnyWidget("BtnMarkOnMap"));
            m_BtnAdminNeedHelp              = ButtonWidget.Cast(layoutRoot.FindAnyWidget("BtnTutorial"));
            m_BtnAdminSendToServer          = ButtonWidget.Cast(layoutRoot.FindAnyWidget("BtnSendNewSpawnToServer"));
            m_AdminSpawnBoxPosX             = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("SpawnPosXEdit"));
            m_AdminSpawnBoxPosY             = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("SpawnPosYEdit"));
            m_AdminSpawnBoxPosZ             = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("SpawnPosZEdit"));
            m_AdminSpawnBoxLocationName     = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("SpawnLocationName"));
            m_AdminTicketUseBoxPosX         = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("SpawnPosXEdit0"));
            m_AdminTicketUseBoxPosY         = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("SpawnPosYEdit0"));
            m_AdminTicketUseBoxPosZ         = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("SpawnPosZEdit0"));
            m_AdminTicketUseRadius          = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("UsePosRadiusEdit"));
            m_AdminTicketClassName          = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("UsePosClassName"));
            m_AdminInsertAsSpawnTicketBox   = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("CheckBoxIsTicket"));
            m_AdminUseItemInHandsBox        = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("CheckUseItemInHands"));
            m_BasicSpawnMapWidget           = MapWidget.Cast(layoutRoot.FindAnyWidget("SpawnSelectMap"));
            m_LocationList                  = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("LocationList"));
            m_AdminSectionCard              = Widget.Cast(layoutRoot.FindAnyWidget("PanelAdminMenu"));
            m_AdminTicketModuleCard         = Widget.Cast(layoutRoot.FindAnyWidget("PanelInsertAsTicket"));
            m_IsMenuInitialized = true;
            
        }
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.UpdateMarker, 500, true);
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

        SetIsSpawnMenuOpen( true );
	}

    override void OnHide()
	{
		super.OnHide();

		PPEffects.SetBlurMenu(0);

		GetGame().GetInput().ResetGameFocus();

		GetGame().GetMission().PlayerControlEnable( false );

        GetGame().GetUIManager().ShowUICursor( false );

        SetIsSpawnMenuOpen( false );

        GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.UpdateMarker);	

		Close();
    }

    override bool OnClick( Widget w, int x, int y, int button )
	{
        if(m_LocationList)
            local int row_index = m_LocationList.GetSelectedRow();
        
        switch(w)
        {
            case m_BtnSpawnOnSelectedLocation:
                if(!row_index || row_index == -1) return false;
                HandleTeleportToSpawnPoint(row_index);
                GetGame().GetUIManager().HideScriptedMenu(this);
                break;
            case m_BtnRandomSpawn:
                HandleTeleportToRandomPoint();
                GetGame().GetUIManager().HideScriptedMenu(this);
                break;
            case m_BtnAdminInsertSpawn:
                HandleOpenAdminSection();
                break;
            case m_BtnAdminDeleteSpawn:
                HandleAdminDeleteSpawn();
                break;
            case m_BtnAdminReloadConfig:
                HandleAdminReloadConfig();
                break;
            case m_BtnAdminCloseSection:
                m_BasicSpawnMapWidget.Show(true);
                m_AdminSectionCard.Show(false);
                break;
            case m_BtnAdminNeedHelp:
                GetGame().OpenURL("https://discord.gg/NyyR547hyf");
                break;
            case m_BtnAdminSendToServer:
                HandleAdminInsertNewSpawn();
                break;
            case m_AdminInsertAsSpawnTicketBox:
                ChangeAdminUi();
                break;
            case m_AdminUseItemInHandsBox:
                UseItemInHandsAsClassName();
                break;
        }

        return super.OnClick(w, x, y, button);
    }

    void UpdateMarker()
    {
        local int row_index = m_LocationList.GetSelectedRow();
        if(row_index != -1)
        {
            HandleDrawSpotOnMap(row_index);
        }
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

    void HandleDrawSpotOnMap(int rowindex)
    {
        m_BasicSpawnMapWidget.ClearUserMarks();
        vector LocalSpawnMarkerPos = m_PossibleSpawns.Get(rowindex).GetExactSpot();
		float LocalSpawnRadius = m_PossibleSpawns.Get(rowindex).GetExactRadius();
		m_BasicSpawnMapWidget.AddUserMark(LocalSpawnMarkerPos, m_PossibleSpawns.Get(rowindex).GetName(), ARGB(255,255,0,255), "\\dz\\gear\\navigation\\data\\map_tree_ca.paa");
        m_BasicSpawnMapWidget.SetScale(0.2);
        SetMapPos(LocalSpawnMarkerPos);
    }

    void LoadSpawnPoints()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if(!player) return;

        ItemBase item_in_hands = ItemBase.Cast(player.GetItemInHands());

        if(m_LocationList && m_LocationList.GetNumItems() <= 0)
                m_LocationList.ClearItems();

        BasicSpawnSelect_SpawnTicket_base spawnTicket;
        if(item_in_hands && Class.CastTo(spawnTicket, item_in_hands))
        {
            //BlockRandomSpawn
            m_BtnRandomSpawn.Show(false);
            //Load SpawnPoints from ticket.
            if(item_in_hands.IsRuined())
                return;
            
            for(int i = 0; i < GetSpawnSelectClient().GetPossibleSpawnTickets().Count(); i++)
            {
                bool entryFound = false;                                                                                            //this is how easy ids work! :)
                if(GetSpawnSelectClient().GetPossibleSpawnTickets().Get(i).GetClassName() == item_in_hands.GetType() && spawnTicket.IsSpawnSelectTicket() /*&& spawnTicket.GetSpawnTicketId() == GetSpawnSelectClient().GetPossibleSpawnTickets().Get(i).GetId()*/)
                {
                    entryFound = true;
                    for(int n = 0; n < GetSpawnSelectClient().GetPossibleSpawnTickets().Get(i).GetLocationsFromTicket().Count(); n++)
                    {
                        m_LocationList.AddItem(GetSpawnSelectClient().GetPossibleSpawnTickets().Get(i).GetLocationsFromTicket().Get(n).GetName(), NULL, 0 );
                    }
                }
                if(entryFound)
                    break;
            }
        }
        else
        {
            m_BtnRandomSpawn.Show(true);
            for(int o = 0; o < m_PossibleSpawns.Count(); o++)
            {
                m_LocationList.AddItem(m_PossibleSpawns.Get(o).GetName(), NULL, 0 );
            }
        }
        //Admin Section
        if(GetSpawnSelectClient().IsLocalPlayerAdmin())
        {
            m_BtnAdminDeleteSpawn.Show(true);
            m_BtnAdminInsertSpawn.Show(true);
            m_BtnAdminReloadConfig.Show(true);
        }
        else
        {
            m_AdminSectionCard.Show(false);
            m_BtnAdminDeleteSpawn.Show(false);
            m_BtnAdminInsertSpawn.Show(false);
            m_BtnAdminReloadConfig.Show(false);
        }
    }

    void HandleOpenAdminSection()
    {
        if(GetSpawnSelectClient().IsLocalPlayerAdmin())
        {
            PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
            //GetRPCManager().SendRPC("BasicSpawnSelect", "SERVER_ADMINADDNEWSPAWN", null, true);
    	    m_AdminSectionCard.Show(true);
            m_BasicSpawnMapWidget.Show(false);

            if(!player) return;
            //Load Default Values.
            local float SpawnPosX = player.GetPosition()[0];
            local float SpawnPosY = player.GetPosition()[1];
            local float SpawnPosZ = player.GetPosition()[2];

            m_AdminSpawnBoxPosX.SetText(SpawnPosX.ToString());
            m_AdminSpawnBoxPosY.SetText(SpawnPosY.ToString());
            m_AdminSpawnBoxPosZ.SetText(SpawnPosZ.ToString());
        }
        else
        {
            m_AdminSectionCard.Show(false);
            m_BtnAdminDeleteSpawn.Show(false);
            m_BtnAdminInsertSpawn.Show(false);
            m_BtnAdminReloadConfig.Show(false);
        }
    }

    protected void HandleAdminInsertNewSpawn()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if(GetSpawnSelectClient().IsLocalPlayerAdmin())
        {
            if(!AdminIsAnyValueNullOrEmpty())
            {
                local float SS_SpawnPosX = m_AdminSpawnBoxPosX.GetText().ToFloat();
                local float SS_SpawnPosY = m_AdminSpawnBoxPosY.GetText().ToFloat();
                local float SS_SpawnPosZ = m_AdminSpawnBoxPosZ.GetText().ToFloat();
                local string SS_LocationName = m_AdminSpawnBoxLocationName.GetText();
                local ref SpawnLocationObject newSpawnLocObj;
                if(SS_SpawnPosX && SS_SpawnPosY && SS_SpawnPosZ && SS_LocationName)
                {
                    local vector SS_SpawnVec = Vector(SS_SpawnPosX, SS_SpawnPosY, SS_SpawnPosZ);
                    if(!SS_SpawnVec) return;
                    newSpawnLocObj = new ref SpawnLocationObject(SS_LocationName, SS_SpawnVec, 1000.00);
                    if(!m_AdminInsertAsSpawnTicketBox.IsChecked())
                    {
                        GetRPCManager().SendRPC("BasicSpawnSelect", "SERVER_ADDNEWSPAWN", new Param1< ref SpawnLocationObject >( newSpawnLocObj ), true);
                    }
                    else
                    {
                        //we need to read more and send then the rpc
                        local float SS_UsePosX = m_AdminTicketUseBoxPosX.GetText().ToFloat();
                        local float SS_UsePosY = m_AdminTicketUseBoxPosY.GetText().ToFloat();
                        local float SS_UsePosZ = m_AdminTicketUseBoxPosZ.GetText().ToFloat();
                        local float SS_UseRad  = m_AdminTicketUseRadius.GetText().ToFloat();
                        local string SS_SpawnTicketName = m_AdminTicketClassName.GetText();
                        vector SS_UseVec = Vector(SS_UsePosX, SS_UsePosY, SS_UsePosZ);

                        if(SS_UseRad && SS_SpawnTicketName && SS_UseVec && newSpawnLocObj)
                        {
                            //string Classname, string Id, vector UsePosition, float UseRadius, ref array<ref SpawnLocationObject> Spawn
                            ref array<ref SpawnLocationObject> newlocobjarray = new ref array<ref SpawnLocationObject>();
                            newlocobjarray.Insert(newSpawnLocObj);
                            local ref SpawnTicketObject newSpawnTicketObj = new ref SpawnTicketObject(SS_SpawnTicketName, "NONE", SS_UseVec, SS_UseRad, newlocobjarray);
                            GetRPCManager().SendRPC("BasicSpawnSelect", "SERVER_ADDNEWSPAWNTICKET", new Param1< ref SpawnTicketObject >( newSpawnTicketObj ), true);
                        }
                        else
                        {
                            player.MessageStatus("[BasicSpawnSelect] -> You need to fill all values correct! (Pos = float)");
                        }
                    }
                }
                else
                {
                    player.MessageStatus("[BasicSpawnSelect] -> You need to fill all values correct! (Pos = float)");
                }
            }
            else
            {
                player.MessageStatus("[BasicSpawnSelect] -> You have to fill out all fields if you want to enter a spawn.");
            }
        }
        else
        {
            m_AdminSectionCard.Show(false);
            m_BtnAdminDeleteSpawn.Show(false);
            m_BtnAdminInsertSpawn.Show(false);
            m_BtnAdminReloadConfig.Show(false);
        }
    }

    void HandleAdminDeleteSpawn()
    {
        if(GetSpawnSelectClient().IsLocalPlayerAdmin())
        {
            //GetRPCManager().SendRPC("BasicSpawnSelect", "SERVER_DELETESELECTEDSPAWN", new Param1< ref SpawnLocationObject >( m_selectedSpawn ), true);
        }
        else
        {
            m_AdminSectionCard.Show(false);
            m_BtnAdminDeleteSpawn.Show(false);
            m_BtnAdminInsertSpawn.Show(false);
            m_BtnAdminReloadConfig.Show(false);
        }
    }

    void HandleAdminReloadConfig()
    {
        if(GetSpawnSelectClient().IsLocalPlayerAdmin())
        {
            GetRPCManager().SendRPC("BasicSpawnSelect", "SERVER_RELOACONFIG", null, true);
        }
        else
        {
            m_AdminSectionCard.Show(false);
            m_BtnAdminDeleteSpawn.Show(false);
            m_BtnAdminInsertSpawn.Show(false);
            m_BtnAdminReloadConfig.Show(false);
        }
    }

    void ChangeAdminUi()
    {
        if(m_AdminInsertAsSpawnTicketBox.IsChecked())
        {
            m_AdminTicketModuleCard.Show(true);
        }
        else
        {
            m_AdminTicketModuleCard.Show(false);
        }
    }

    void UseItemInHandsAsClassName()
    {
        if(!m_AdminUseItemInHandsBox.IsChecked())
        {
            m_AdminTicketClassName.SetText("");
        }
        else
        {
            PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
            if(!player) return;

            ItemBase item_in_hands = ItemBase.Cast(player.GetItemInHands());
            if(!item_in_hands)
            {
                player.MessageStatus("[BasicSpawnSelect] -> Can`t find a usable item in your Hands!");
                return;
            }

            m_AdminTicketClassName.SetText(item_in_hands.GetType());
        }
        
    }

    protected bool AdminIsAnyValueNullOrEmpty()
    {
        //Todo add here a if case for all values.
        return false;
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