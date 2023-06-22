//#define AB_WILLOWISP_SMOOTH_MOVEMENT

class ab_willowisp_model_base extends BuildingSuper
{
	protected ref EffectSound 				m_KillFx;
	protected ref EffectSound 				m_AmbientFx;
	protected ref EffectSound 				m_CreepyFx;
	protected ab_Willowisps_Entity_Light	m_Light;
	protected Particle						m_WillowispsParticle; 
	protected vector 						moveTo;
	protected float 						speedTo;
	protected ab_Willowisp_Mode				mode;
	protected vector 						scatter;
	protected float 						maxScatter = 0.5;
	protected float 						scatterTimeslice;
	protected bool							active = false;
	float 									TeleportRange;
	float									WillowispModelHeightOffset;

	void ab_willowisp_model_base()
	{	
		SetEventMask(EntityEvent.SIMULATE);

		moveTo = vector.Zero;
		scatter = vector.Zero;
		mode = ab_Willowisp_Mode.IDLE;
		scatterTimeslice = 0;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdateTheLifeTime, 1000, false);
		
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			m_Light = ab_Willowisps_Entity_Light.Cast(ScriptedLightBase.CreateLight(ab_Willowisps_Entity_Light, "0 1 0", 0.5));
			m_Light.AttachOnObject(this, "0 1 0", "0 0 0");
			
			if (ab_Willowisp_Particle == 1)
			{
				m_WillowispsParticle = Particle.PlayOnObject(ParticleList.Willowisps_Entity_Traces, this);
			}
			else if (ab_Willowisp_Particle == 2)
			{
				m_WillowispsParticle = Particle.PlayOnObject(ParticleList.Willowisps_Entity_Traces_All, this);
			}
			else
			{
				m_WillowispsParticle = Particle.PlayOnObject(ParticleList.Willowisps_Entity, this);
			}
		}
	}
	
	void ~ab_willowisp_model_base()
	{
		if (m_WillowispsParticle) m_WillowispsParticle.Stop();
		if (m_Light) m_Light.FadeOut();
		
		ClearEventMask(EntityEvent.SIMULATE);
	}
	
	override void EOnSimulate( IEntity owner, float dt )
	{
		if (GetGame() && GetGame().IsServer() && active)
		{
			scatterTimeslice += dt;
			
			if (scatterTimeslice >= 0.25)
			{
				scatterTimeslice = 0;
				
				if (mode == ab_Willowisp_Mode.ATTACK)
				{
					scatter = Vector(0, 0, 0);
				}
				else
				{
					scatter = Vector(Math.RandomFloatInclusive(-maxScatter, maxScatter), Math.RandomFloatInclusive(-maxScatter / 2, maxScatter / 2), Math.RandomFloatInclusive(-maxScatter, maxScatter));
				}	
			}
			
			if(moveTo != vector.Zero)
			{
				float x;
				float y;
				float z;
				
				if (mode == ab_Willowisp_Mode.IDLE || mode == ab_Willowisp_Mode.COOLDOWN)
				{
					x = moveTo[0];
					y = moveTo[1];;
					z = moveTo[2];
				}
				else
				{
					vector willowispPos = GetPosition();
					float distance = vector.Distance(moveTo, willowispPos);
					distance = Math.Min(TeleportRange, distance);
					distance = Math.Min(distance, dt * speedTo);
					vector angles = vector.Direction(moveTo, willowispPos).Normalized().VectorToAngles();
					angles[0] = angles[0] + 180;
					x = willowispPos[0] + (distance * Math.Sin(angles[0] * Math.DEG2RAD)) + (scatter[0] * dt * speedTo);
					z = willowispPos[2] + (distance * Math.Cos(angles[0] * Math.DEG2RAD)) + (scatter[2] * dt * speedTo);
					y = GetGame().SurfaceY(x, z) + WillowispModelHeightOffset + (scatter[1] * dt * speedTo);
				}
				
				vector movePos = Vector(x, y, z);
#ifdef AB_WILLOWISP_SMOOTH_MOVEMENT				
				vector transform[4];
				GetTransform(transform);
				transform[3] = movePos;
				MoveInTime(transform, dt);
#else				
				SetPosition(movePos);
#endif			
			}
		}
	}
	
	void Reset()
	{
		moveTo = vector.Zero;
		speedTo = 0;
		mode = ab_Willowisp_Mode.IDLE;
	}
	
	void MoveTo(vector position, float speed, ab_Willowisp_Mode Mode)
	{
		moveTo = position;
		speedTo = speed;
		mode = Mode;
	}
	
	void SetMode(ab_Willowisp_Mode Mode)
	{
		mode = Mode;
	}
	
	void SetActive(bool isActive)
	{
		active = isActive;
	}
	
	void UpdateTheLifeTime()
    {
        SetLifetime(360000);
    }
	
	void RequestPlayKill()
	{
		if (GetGame() && GetGame().IsServer() && active)
		{
			Param1<string> p = new Param1<string>("kill");
			GetGame().RPCSingleParam(this, abWillowispRPC.RPC_AB_WILLOWISP_SOUND_REQUEST, p, true);
		}
	}
	
	void RequestPlayAmbient()
	{
		if (GetGame() && GetGame().IsServer() && active)
		{
			Param1<string> p = new Param1<string>("ambient");
			GetGame().RPCSingleParam(this, abWillowispRPC.RPC_AB_WILLOWISP_SOUND_REQUEST, p, true);
		}
	}
	
	void RequestPlayCreepy()
	{
		if (GetGame() && GetGame().IsServer() && active)
		{
			Param1<string> p = new Param1<string>("creepy");
			GetGame().RPCSingleParam(this, abWillowispRPC.RPC_AB_WILLOWISP_SOUND_REQUEST, p, true);
		}
	}
	
	void PlayKill()
	{
		if(m_KillFx && m_KillFx.IsSoundPlaying()) return;
		
		PlaySoundSet(m_KillFx, "ab_willowisp_kill_SoundSet", 0.1, 0.1);
	}
	
	void PlayAmbient()
	{
		if(m_AmbientFx && m_AmbientFx.IsSoundPlaying()) return;
		
		PlaySoundSet(m_AmbientFx, "ab_willowisp_ambient_SoundSet", 0.1, 0.1);
	}
	
	void PlayCreepy()
	{
		if(m_CreepyFx && m_CreepyFx.IsSoundPlaying()) return;
		
		PlaySoundSet(m_CreepyFx, "ab_willowisp_creepy_SoundSet", 0.1, 0.1);
	}
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		if (GetGame() && (GetGame().IsClient() || !GetGame().IsMultiplayer()))
		{
			switch(rpc_type)
			{
				case abWillowispRPC.RPC_AB_WILLOWISP_SOUND_REQUEST:

					ref Param1<string> soundRequest = new Param1<string>("");
					
					if (ctx.Read(soundRequest))
					{
						if (soundRequest.param1 == "kill")
						{
							PlayKill();
						}
						else if (soundRequest.param1 == "ambient")
						{
							PlayAmbient();
						}
						else if (soundRequest.param1 == "creepy")
						{
							PlayCreepy();
						}
					}
					
				break;
			}
		}
	}
}