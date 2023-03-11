class ab_Willowisp
{
	private string name;
	private float willowispModelHeightOffset = 1.0;
	private float willowispModelLosCheckOriginHeightOffset = 0;
	private float shaking = 0.1;
	private vector origPosition;
	private float animRange = 300;
	private float transmittingRange = 100;
	private float detectionRange = 50;
	private float teleportRange = 15;
	private float teleportKillRange = 5;
	private const ref array<string> willowispModels = {"ab_willowisp_model"};
	private Object willowispObject;
	private vector areaPosition;
	private float areaResetRadius;
	private ab_willowisp_model_base willowispObjectBase;
	bool InRange = false; // Any player is within detectionRange regardless of LOS check - used for creepy random sounds
	bool IsTransmitting = false; // Any player is within transmittingRange regardless of LOS check - used for ambient random sounds
	bool IsTeleportBlocked = false; // Will-o'-Wisp teleportation is blocked as Will-o'-Wisp wandered outside Will-o'-Wisps area
	private vector movePos = vector.Zero;
	private float speed;
	private PlayerBase targetPlayer;
	private float attackTimeoutTimeslice;
	private bool attackTimeout;
	private vector lastPos;
	bool InAnimRange = false;
	private string mode;
	
	void ab_Willowisp(string Name, vector AreaPosition, float AreaResetRadius)
	{
		if (GetGame())
		{
			mode = "IDLE";
			attackTimeoutTimeslice = 0;
			attackTimeout = false;
			this.name = Name;
			this.areaPosition = AreaPosition;
			this.areaResetRadius = AreaResetRadius;
			willowispObject = GetGame().CreateObject(willowispModels.GetRandomElement(), areaPosition);
			willowispObjectBase = ab_willowisp_model_base.Cast(willowispObject);
			willowispObjectBase.TeleportRange = teleportRange;
			willowispObjectBase.WillowispModelHeightOffset = willowispModelHeightOffset;
			willowispObjectBase.SetMode(mode);
		}
	}
	
	void ~ab_Willowisp()
	{
		targetPlayer = null;
	}
	
	void Setup(vector position)
	{
		if (GetGame())
		{
			float y = GetGame().SurfaceY(position[0], position[2]);
			willowispObject.SetPosition(Vector(position[0], y + willowispModelHeightOffset, position[2]));
			origPosition = willowispObject.GetPosition();
			InRange = false;
			InAnimRange = false;
			IsTransmitting = false;
			IsTeleportBlocked = false;
			mode = "IDLE";
			willowispObjectBase.Reset();
			movePos = vector.Zero;
			targetPlayer = null;
			attackTimeoutTimeslice = 0;
			attackTimeout = false;
		}
	}
	
	void RequestPlayAmbient()
	{
		if (willowispObjectBase)
		{
			willowispObjectBase.RequestPlayAmbient();
		}
	}
	
	void RequestPlayCreepy()
	{
		if (willowispObjectBase)
		{
			willowispObjectBase.RequestPlayCreepy();
		}
	}
	
	bool LosCheck(Object fromObj, PlayerBase toObj)
	{
		if (fromObj && toObj)
		{
			vector fromPos = fromObj.GetPosition();
			vector begPos = Vector(fromPos[0], fromPos[1] + willowispModelLosCheckOriginHeightOffset, fromPos[2]);
			vector endPos = toObj.GetPosition();
			vector contactPos;
			vector contactDir;
			int contactComponent; 
			set<Object> results = new set<Object>;
			Object with = NULL; 
			Object ignore = willowispObject;  
			bool sorted = false; 
			bool ground_only = false;  
			int iType = 2;
			float radius = 0;
			
			int bone_index = toObj.GetBoneIndexByName("Head");
			vector headPosition= toObj.GetBonePositionWS(bone_index);
			bone_index = toObj.GetBoneIndexByName("Pelvis");
			vector pelvisPosition= toObj.GetBonePositionWS(bone_index);
			
			if (DayZPhysics.RaycastRV(begPos, endPos, contactPos, contactDir, contactComponent, results, with, ignore, sorted, ground_only, iType, radius))
			{
				if (results && results.Count() > 0 && results[0] == toObj)
				{
					return true;
				}
			}
			
			if (DayZPhysics.RaycastRV(begPos, pelvisPosition, contactPos, contactDir, contactComponent, results, with, ignore, sorted, ground_only, iType, radius))
			{
				if (results && results.Count() > 0 && results[0] == toObj)
				{
					return true;
				}
			}
			
			if (DayZPhysics.RaycastRV(begPos, headPosition, contactPos, contactDir, contactComponent, results, with, ignore, sorted, ground_only, iType, radius))
			{
				if (results && results.Count() > 0 && results[0] == toObj)
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
	void UpdateAnim(float timeslice)
	{
		if (GetGame() && InAnimRange)
		{
			if (attackTimeout)
			{
				attackTimeoutTimeslice += timeslice;
				
				if (attackTimeoutTimeslice >= 20)
				{
					attackTimeoutTimeslice = 0;
					attackTimeout = false;
				}
			}
			
			if (movePos == vector.Zero)
			{
				willowispObjectBase.MoveTo(Vector(origPosition[0] + Math.RandomFloatInclusive(-shaking, shaking), origPosition[1] + Math.RandomFloatInclusive(-shaking, shaking), origPosition[2] + Math.RandomFloatInclusive(-shaking, shaking)), 6, mode);
			}
			else
			{
				float willowispSpeed = speed;
				float distance;
				
				if (targetPlayer && targetPlayer.IsAlive())
				{
					distance = vector.Distance(targetPlayer.GetPosition(), willowispObject.GetPosition());
					
					if (distance <= teleportKillRange)
					{
						willowispSpeed = 10;	
						mode = "ATTACK";
					}
					else
					{
						mode = "MOVE";
					}
				}
				
				if (!MoveTo(movePos, willowispSpeed, timeslice))
				{
					movePos = vector.Zero;
					origPosition = willowispObject.GetPosition();
					mode = "IDLE";
					willowispObjectBase.SetMode(mode);
				}
				
				if (targetPlayer && targetPlayer.IsAlive())
				{
					vector willowispTestPos = willowispObject.GetPosition();
					willowispTestPos = Vector(willowispTestPos[0], willowispTestPos[1] - willowispModelHeightOffset, willowispTestPos[2]);
					distance = vector.Distance(targetPlayer.GetPosition(), willowispTestPos);
					
					if (distance <= 1.0)
					{
						willowispObjectBase.RequestPlayKill();
						targetPlayer.ab_RequestWillowispKill(name);
						attackTimeoutTimeslice = 0;
						attackTimeout = true;
						movePos = lastPos;
						targetPlayer = null;
					}
				}
			}
		}
		else
		{
			movePos = vector.Zero;
			targetPlayer = null;
		}
	}
	
	bool MoveTo(vector position, float speed, float timeslice)
	{
		if (GetGame())
		{
			if (position != vector.Zero)
			{
				vector willowispPos = willowispObject.GetPosition();
				float distance = vector.Distance(position, willowispPos);
				
				if(distance <= 0.2)
				{
					return false;
				}
				else
				{
					willowispObjectBase.MoveTo(position, speed, mode);
					return true;
				}
			}
		}
		
		return false;
	}
	
	void Update(float timeslice)
	{
		if (GetGame())
		{
			InRange = false;
			InAnimRange = false;
			IsTransmitting = false;
			IsTeleportBlocked = false;
			int i;
			float distance = -1;
			float distanceCheck
			vector angles;
			Object nearestPerson = NULL;
			vector position = willowispObject.GetPosition();
			array<Man> players = new array<Man>; 			
			
			GetGame().GetPlayers(players);
			
			for (i = 0; i < players.Count(); i++)
			{
				PlayerBase playerCheck;
				Class.CastTo(playerCheck, players.Get(i));
				
				if (playerCheck.IsAlive())
				{
					vector playerPos = playerCheck.GetPosition();
					distanceCheck = vector.Distance(playerPos, position);
					
					if (distanceCheck <= animRange)
					{
						InAnimRange = true;
					}
					
					if (distanceCheck <= detectionRange)
					{
						bool losCheck = LosCheck(willowispObject, playerCheck);
						
						if ((distance > distanceCheck || distance == -1) && losCheck)
						{
							distance = distanceCheck;
							nearestPerson = playerCheck;
						}
						
						InRange = true;
					}
					
					if (distanceCheck <= transmittingRange)
					{
						IsTransmitting = true;
					}
				}
			}

			if (nearestPerson)
			{
				PlayerBase playerTest = PlayerBase.Cast(nearestPerson);
				distanceCheck = vector.Distance(playerTest.GetPosition(), areaPosition);
						
				if (distanceCheck > areaResetRadius)
				{
					IsTeleportBlocked = true;
				}
			}

			if (!attackTimeout && !IsTeleportBlocked && nearestPerson)
			{
				lastPos = position;
				PlayerBase player = PlayerBase.Cast(nearestPerson);
				targetPlayer = player;
				movePos = player.GetPosition();
				float y = GetGame().SurfaceY(movePos[0], movePos[2]);
				movePos = Vector(movePos[0], y + willowispModelHeightOffset, movePos[2]);
				speed = Math.RandomFloatInclusive(1, 5);
			}
			else
			{	
				targetPlayer = null;
			}
		}
	}
}