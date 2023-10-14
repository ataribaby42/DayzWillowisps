ref ab_WillowispManager g_ab_WillowispManager;
ab_WillowispManager Get_ab_WillowispManager()
{
	if (!g_ab_WillowispManager) 
	{
		g_ab_WillowispManager = new ab_WillowispManager;
	}
	return g_ab_WillowispManager;
}

int ab_Willowisp_Particle = 1; // 0 - no traces, 1 - star traces, 2 - star and ball traces

class ab_WillowispManager
{
	private ref array<ref ab_WillowispController> controllers = new array<ref ab_WillowispController>();
	private float ab_WillowispsTimeslice = 0;
	private float ab_WillowispsSoundTimeslice = 0;
	private float ab_WillowispsAnimTimeslice = 0;
	
	void ab_WillowispManager()
	{
		Print("Will-o'-Wisp Manager started.");
	}

	void ~ab_WillowispManager()
	{
		Print("Will-o'-Wisp Manager stopped.");
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