modded class PlayerBase extends ManBase
{
	float willowispKillTimer;
	float willowispUncoTimer;
	bool willowispUnco;
	string willowispKillName;
	bool willowispKillRequested;
	
	override void Init()
	{
		super.Init();
		
		willowispKillTimer = 0;
		willowispUncoTimer = 0;
		willowispUnco = false;
		willowispKillRequested = false;
	}
	
	override void EOnFrame(IEntity other, float timeSlice)
	{
		super.EOnFrame(other, timeSlice);
		
		if (GetGame() && GetGame().IsServer())	
		{
			if (willowispKillRequested)
			{
				willowispKillTimer += timeSlice;
				
				if (willowispKillTimer >= 0.50)
				{
					willowispKillTimer = 0;	
					willowispKillRequested = false;
					PlayerIdentity identity = GetIdentity();
					
					if (identity)
					{
						Print("Player <" + GetIdentity().GetName() + "> has been harmed by Will-o'-Wisp <" + willowispKillName + ">.");
					}
					else
					{
						Print("Player <unknown> has been harmed by Will-o'-Wisp <" + willowispKillName + ">.");
					}
					
					willowispKillName = "";
					
					if (GetAllowDamage())
					{
						float currenthealth = GetHealth("GlobalHealth", "Health");
						currenthealth = currenthealth - Math.RandomFloatInclusive(5, 10);
						SetHealth("GlobalHealth", "Health", currenthealth);
						RequestUnconsciousness(true);
						willowispUnco = true;
					}
				}
			}
			
			if (willowispUnco)
			{
				willowispUncoTimer += timeSlice;
				
				if (willowispUncoTimer >= 10)
				{
					RequestUnconsciousness(false);
					willowispUncoTimer = 0;
					willowispUnco = false;
				}
			}
		}
	}
	
	void RequestWillowispKill(string name)
	{
		willowispKillRequested = true;
		willowispKillName = name;
	}
};