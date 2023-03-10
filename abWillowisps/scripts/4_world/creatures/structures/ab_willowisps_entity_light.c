class ab_Willowisps_Entity_Light extends PointLightBase
{		
	void ab_Willowisps_Entity_Light()
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