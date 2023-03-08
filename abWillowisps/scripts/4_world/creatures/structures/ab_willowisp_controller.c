class ab_WillowispController
{
	private string name;
	private float resetRadius;
	private float minTimeBetweenCreepySound = 15;
	private float creepySoundChance = 0.5;
	private float lastCreepySoundTimeslice;
	private vector position;
	private float radius;
	private int willowispCount;
	private ref array<ref ab_Willowisp> willowisps = new array<ref ab_Willowisp>();
	private ref array<string> willowispPositions = new array<string>();
	private bool active = false;
	
	void ab_WillowispController(string Name, vector Position, float ResetRadius, float Radius, int WillowispCount, ref array<string> WillowispPositions)
	{
		if (!WillowispPositions)
		{
			Print("Random Will-o'-Wisps <" + Name + "> created at " + Position + ".");
		}
		else
		{
			Print("Predefined Will-o'-Wisps <" + Name + "> created at " + Position + ".");
		}
		
		lastCreepySoundTimeslice = 0;
		this.name = Name;
		this.position = Position;
		this.radius = Radius;
		this.willowispCount = WillowispCount;
		this.willowispPositions = WillowispPositions;
		this.resetRadius = ResetRadius;
		
		for (int i = 0; i < willowispCount; i++)
		{
			willowisps.Insert(new ref ab_Willowisp(name, position, resetRadius));
		}
		
		SetupWillowisps();
	}

	void ~ab_WillowispController()
	{
		
	}
	
	void SetupWillowisps()
	{
		if (willowisps)
		{
			int i;
			vector pos;
			
			if (!willowispPositions)
			{
				for (i = 0; i < willowispCount; i++)
				{
					if (willowisps && willowisps[i])
					{
						Vector2 randomPos = GetRandomPointInCircle(radius);
						pos = Vector(position[0] + randomPos.x, position[1], position[2] + randomPos.y);
						willowisps[i].Setup(pos);
					}
				}
			}
			else
			{
				for (i = 0; i < willowispPositions.Count(); i++)
				{
					if (willowisps && willowisps[i])
					{
						pos = willowispPositions[i].ToVector();
						willowisps[i].Setup(pos);
					}
				}
			}
			
			Print("Will-o'-Wisps <" + name + "> reset.");
		}
	}
	
	void Update(float timeslice)
	{
		if (GetGame() && willowisps)
		{
			int i;
			bool nobodyNear = true;
			
			array<Man> players = new array<Man>; 																				
			GetGame().GetPlayers(players);
			
			for (i = 0; i < players.Count(); i++)
			{
				PlayerBase player;
				Class.CastTo(player, players.Get(i));
				
				if (player.IsAlive())
				{
					vector playerPos = player.GetPosition();
					float distance = vector.Distance(playerPos, position);
					
					if (distance <= resetRadius * 1.5)
					{
						if (!active)
						{
							Print("Will-o'-Wisps <" + name + "> active at " + position + ".");
						}
					
						nobodyNear = false;
						active = true;
						break;
					}
				}
			}
			
			if (nobodyNear && active)
			{
				SetupWillowisps();
				active = false;
				Print("Will-o'-Wisps <" + name + "> inactive at " + position + ".");
			}
			
			for (i = 0; i < willowisps.Count(); i++)
			{
				if (willowisps && willowisps[i])
				{
					willowisps[i].Update(timeslice);
					
					if (active && willowisps[i].InRange)
					{
						willowisps[i].RequestPlayAmbient();
					}
				}
			}
		}
	}
	
	void UpdateSounds(float timeslice)
	{	
		if (GetGame() && willowisps)
		{
			int i;
			array<ab_Willowisp> willowispsTransmitting = new array<ab_Willowisp>();
			array<ab_Willowisp> willowispsInRange = new array<ab_Willowisp>();
			lastCreepySoundTimeslice += timeslice;
			
			if (lastCreepySoundTimeslice >= minTimeBetweenCreepySound)
			{
				lastCreepySoundTimeslice = 0;	
				
				for (i = 0; i < willowisps.Count(); i++)
				{
					if (willowisps && willowisps[i] && willowisps[i].InRange)
					{
						willowispsInRange.Insert(willowisps[i]);
					}
				}
				
				if (willowispsInRange.Count() > 0 && Math.RandomFloat01() < creepySoundChance)
				{
					willowispsInRange.GetRandomElement().RequestPlayCreepy();
				}
			}
			
			for (i = 0; i < willowisps.Count(); i++)
			{
				if (willowisps && willowisps[i] && willowisps[i].IsTransmitting)
				{
					willowispsTransmitting.Insert(willowisps[i]);
				}
			}
			
			for (i = 0; i < Math.RandomIntInclusive(1, 4); i++)
			{
				if (willowispsTransmitting.Count() > 0)
				{
					ab_Willowisp willowispPlay = willowispsTransmitting.GetRandomElement();
					willowispPlay.RequestPlayAmbient();
					willowispsTransmitting.RemoveItem(willowispPlay);
				}
			}
		}
	}
	
	void UpdateAnim(float timeslice)
	{	
		if (GetGame() && willowisps)
		{
			int i;
			
			for (i = 0; i < willowisps.Count(); i++)
			{
				if (willowisps && willowisps[i])
				{
					willowisps[i].UpdateAnim(timeslice);
				}
			}
		}
	}
	
	Vector2 GetRandomPointInCircle(float Radius)
	{
		float a = Math.RandomFloat01() * 2 * Math.PI;
		float r = Radius * Math.Sqrt(Math.RandomFloat01());
		float x = r * Math.Cos(a);
		float y = r * Math.Sin(a);
		return new Vector2(x, y);
	}
}