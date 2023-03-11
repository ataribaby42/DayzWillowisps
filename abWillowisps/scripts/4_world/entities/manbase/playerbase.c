static ref array<string> ab_Willowisp_Stealable_Items = {};

modded class PlayerBase extends ManBase
{
	private float ab_WillowispKillTimer;
	private float ab_WillowispUncoTimer;
	private bool ab_WillowispUnco;
	private string ab_WillowispKillName;
	private bool ab_WillowispKillRequested;
	
	override void Init()
	{
		super.Init();
		
		ab_WillowispKillTimer = 0;
		ab_WillowispUncoTimer = 0;
		ab_WillowispUnco = false;
		ab_WillowispKillRequested = false;
	}
	
	override void EOnFrame(IEntity other, float timeSlice)
	{
		super.EOnFrame(other, timeSlice);
		
		if (GetGame() && GetGame().IsServer())	
		{
			if (ab_WillowispKillRequested)
			{
				ab_WillowispKillTimer += timeSlice;
				
				if (ab_WillowispKillTimer >= 0.50)
				{
					ab_WillowispKillTimer = 0;	
					ab_WillowispKillRequested = false;
					PlayerIdentity identity = GetIdentity();
					
					if (identity)
					{
						Print("Player <" + GetIdentity().GetName() + "> has been harmed by Will-o'-Wisp <" + ab_WillowispKillName + ">.");
					}
					else
					{
						Print("Player <unknown> has been harmed by Will-o'-Wisp <" + ab_WillowispKillName + ">.");
					}
					
					if (GetAllowDamage())
					{
						float currenthealth = GetHealth("GlobalHealth", "Health");
						currenthealth = currenthealth - Math.RandomFloatInclusive(5, 10);
						SetHealth("GlobalHealth", "Health", currenthealth);
						RequestUnconsciousness(true);
						ab_WillowispRandomItemSteal(ab_WillowispKillName);
						ab_WillowispUnco = true;
					}
					
					ab_WillowispKillName = "";
				}
			}
			
			if (ab_WillowispUnco)
			{
				ab_WillowispUncoTimer += timeSlice;
				
				if (ab_WillowispUncoTimer >= 10)
				{
					RequestUnconsciousness(false);
					ab_WillowispUncoTimer = 0;
					ab_WillowispUnco = false;
				}
			}
		}
	}
	
	private bool ab_WillowispIsStealableItem(ItemBase item)
	{
		foreach (string type : ab_Willowisp_Stealable_Items)
		{
			if (item && item.IsKindOf(type))
			{
				return true;
			}
		}

		return false;
	}
	
	private void ab_WillowispRandomItemSteal(string stealer)
	{
		if (ab_Willowisp_Stealable_Items.Count() == 0) return;
		
		array<EntityAI> itemsArray = new array<EntityAI>;
		array<ItemBase> itemsStealable = new array<ItemBase>;;
		ItemBase item;
		
		GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);
		
		for (int i = 0; i < itemsArray.Count(); i++)
		{
			Class.CastTo(item, itemsArray.Get(i));
			
			if (item && !item.IsInherited(SurvivorBase))
			{
				if (ab_WillowispIsStealableItem(item))
				{
					itemsStealable.Insert(item);
				}
			}
		}
		
		if (itemsStealable.Count() > 0)
		{
			item = itemsStealable.GetRandomElement();
			PlayerIdentity identity = GetIdentity();
			GetInventory().LocalDestroyEntity(item);
			
			if (identity)
			{
				Print("Will-o'-Wisp <" + stealer + "> stealed <" + item.GetType() + "> from Player <" + GetIdentity().GetName() + ">.");
			}
			else
			{
				Print("Will-o'-Wisp <" + stealer + "> stealed <" + item.GetType() + "> from Player <unknown>.");
			}
}
	}
	
	void ab_RequestWillowispKill(string name)
	{
		ab_WillowispKillRequested = true;
		ab_WillowispKillName = name;
	}
};