// for online server mode
modded class MissionServer extends MissionBase
{
	private float ab_WillowispsTimeslice = 0;
	private float ab_WillowispsSoundTimeslice = 0;
	private float ab_WillowispsAnimTimeslice = 0;
	
	void MissionServer() 
	{
		
	}

	void ~MissionServer() 
	{
		
	}

	override void OnInit()
	{
		super.OnInit();
	}
	
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		
		if (GetGame() && GetGame().IsServer())	
		{
			ab_WillowispsTimeslice += timeslice;
			
			if (ab_WillowispsTimeslice >= 2.0)
			{
				Get_ab_WillowispManager().Update(ab_WillowispsTimeslice);
				ab_WillowispsTimeslice = 0;	
			}
			
			ab_WillowispsAnimTimeslice += timeslice;
			
			if (ab_WillowispsAnimTimeslice >= 0.25)
			{
				Get_ab_WillowispManager().UpdateAnim(ab_WillowispsAnimTimeslice);
				ab_WillowispsAnimTimeslice = 0;	
			}
			
			ab_WillowispsSoundTimeslice += timeslice;
			
			if (ab_WillowispsSoundTimeslice >= 1.0)
			{
				Get_ab_WillowispManager().UpdateSounds(ab_WillowispsSoundTimeslice);
				ab_WillowispsSoundTimeslice = 0;	
			}
		}
	}
}