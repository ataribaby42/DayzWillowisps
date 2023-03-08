class ab_willowisp_model_base extends BuildingSuper
{
	protected ref EffectSound 	m_KillFx;
	protected ref EffectSound 	m_AmbientFx;
	protected ref EffectSound 	m_CreepyFx;
	Willowisps_Entity_LIGHT 	m_Light;
	Particle					m_WillowispsParticle; 

	void ab_willowisp_model_base()
	{	
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdateTheLifeTime, 1000, false);
		
		if ( GetGame().IsClient() || !GetGame().IsMultiplayer() )
		{
			m_Light = Willowisps_Entity_LIGHT.Cast( ScriptedLightBase.CreateLight(Willowisps_Entity_LIGHT, "0 1 0", 0.5) );
			m_Light.AttachOnObject(this, "0 1 0", "0 0 0");
			m_WillowispsParticle = Particle.PlayOnObject(ParticleList.Willowisps_Entity, this);
		}
	}
	
	void ~ab_willowisp_model_base()
	{
		if (m_WillowispsParticle) m_WillowispsParticle.Stop();
		if (m_Light) m_Light.FadeOut();
	}
	
	void UpdateTheLifeTime()
    {
        SetLifetime(360000);
    }
	
	void RequestPlayKill()
	{
		if (GetGame() && GetGame().IsServer())
		{
			Param1<string> p = new Param1<string>("kill");
			GetGame().RPCSingleParam(this, abWillowispRPC.RPC_AB_WILLOWISP_SOUND_REQUEST, p, true);
		}
	}
	
	void RequestPlayAmbient()
	{
		if (GetGame() && GetGame().IsServer())
		{
			Param1<string> p = new Param1<string>("ambient");
			GetGame().RPCSingleParam(this, abWillowispRPC.RPC_AB_WILLOWISP_SOUND_REQUEST, p, true);
		}
	}
	
	void RequestPlayCreepy()
	{
		if (GetGame() && GetGame().IsServer())
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

class Willowisps_Entity_LIGHT extends PointLightBase
{		
	void Willowisps_Entity_LIGHT()
	{
		SetVisibleDuringDaylight(true);
		SetRadiusTo(7);
		SetBrightnessTo(0.6);
		SetCastShadow(false);
		SetFadeOutTime(3);
		SetDiffuseColor(0.0, 1, 0.47058823529);
		SetAmbientColor(0.0, 1, 0.47058823529);
		SetFlareVisible(false);
		SetFlickerAmplitude(0.7);
		SetFlickerSpeed(4.0);
		SetDancingShadowsMovementSpeed(0.4);
		SetDancingShadowsAmplitude(0.5);
	}
}