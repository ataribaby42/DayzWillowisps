ab_WillowispManager g_ab_WillowispManager;
ab_WillowispManager Get_ab_WillowispManager()
{
	if (!g_ab_WillowispManager) 
	{
		g_ab_WillowispManager = ab_WillowispManager.Cast(GetGame().CreateObject("ab_WillowispManager", "0 0 0"));
	}
	return g_ab_WillowispManager;
}

int ab_Willowisp_Particle = 1; // 0 - no traces, 1 - star traces, 2 - star and ball traces

class ab_WillowispManager extends ScriptedEntity
{
	private ref array<ref ab_WillowispController> controllers = new array<ref ab_WillowispController>();
	private float ab_WillowispsTimeslice = 0;
	private float ab_WillowispsSoundTimeslice = 0;
	private float ab_WillowispsAnimTimeslice = 0;
	
	void ab_WillowispManager()
	{
		Print("Will-o'-Wisp Manager started.");
		SetEventMask(EntityEvent.FRAME);
	}

	void ~ab_WillowispManager()
	{
		Print("Will-o'-Wisp Manager stopped.");
		ClearEventMask(EntityEvent.FRAME);
	}
	
	override void EOnFrame(IEntity other, float timeSlice)
    {
		if (GetGame() && GetGame().IsServer())	
		{
			ab_WillowispsTimeslice += timeSlice;
			
			if (ab_WillowispsTimeslice >= 2.0)
			{
				Get_ab_WillowispManager().Update(ab_WillowispsTimeslice);
				ab_WillowispsTimeslice = 0;	
			}
			
			ab_WillowispsAnimTimeslice += timeSlice;
			
			if (ab_WillowispsAnimTimeslice >= 0.25)
			{
				Get_ab_WillowispManager().UpdateAnim(ab_WillowispsAnimTimeslice);
				ab_WillowispsAnimTimeslice = 0;	
			}
			
			ab_WillowispsSoundTimeslice += timeSlice;
			
			if (ab_WillowispsSoundTimeslice >= 1.0)
			{
				Get_ab_WillowispManager().UpdateSounds(ab_WillowispsSoundTimeslice);
				ab_WillowispsSoundTimeslice = 0;	
			}
		}
    }

	void CreateRandomWillowispController(string name, vector position, float resetRadius, float radius, int willowispCount)
	{
		if (controllers)
		{
			controllers.Insert(new ref ab_WillowispController(name, position, resetRadius, radius, willowispCount, NULL));
		}
	}
	
	void CreatePredefinedWillowispController(string name, vector position, float resetRadius, ref array<string> willowispPositions)
	{
		if (controllers)
		{
			controllers.Insert(new ref ab_WillowispController(name, position, resetRadius, 0, willowispPositions.Count(), willowispPositions));
		}
	}
	
	void Update(float timeslice)
	{
		if (controllers)
		{
			for (int i = 0; i < controllers.Count(); i++)
			{
				if (controllers && controllers[i])
				{
					controllers[i].Update(timeslice);
				}
			}
		}
	}
	
	void UpdateSounds(float timeslice)
	{
		if (controllers)
		{
			for (int i = 0; i < controllers.Count(); i++)
			{
				if (controllers && controllers[i])
				{
					controllers[i].UpdateSounds(timeslice);
				}
			}
		}
	}
	
	void UpdateAnim(float timeslice)
	{
		if (controllers)
		{
			for (int i = 0; i < controllers.Count(); i++)
			{
				if (controllers && controllers[i])
				{
					controllers[i].UpdateAnim(timeslice);
				}
			}
		}
	}
}