modded class MissionBase 
{
    override UIScriptedMenu CreateScriptedMenu (int id) 
    {
        UIScriptedMenu menu = NULL;
        menu = super.CreateScriptedMenu(id);
        if (!menu) 
        {
            switch (id) 
            {
                case BASIC_SPAWNSELECTMENU:
                    menu = new BasicSpawnSelectMenu;
                    break;
            }
            if (menu)
            {
                menu.SetID(id);
            }
        }
        return menu;
    }
}