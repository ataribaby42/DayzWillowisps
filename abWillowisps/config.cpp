class CfgPatches
{
	class abWillowisps
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Scripts"
		};
	};
};

class CfgMods
{
	class abWillowisps
	{
		type="mod";
		dependencies[] = {"Game", "World", "Mission"};
		
		class defs
		{
			class gameScriptModule
			{
				value="";
				files[]={"abWillowisps/scripts/3_game"};
			};
			class worldScriptModule
			{
				value="";
				files[]={"abWillowisps/scripts/4_world"};
			};
			class missionScriptModule
			{
				value="";
				files[]={"abWillowisps/scripts/5_mission"};
			};
		};
	};
};

class CfgVehicles
{
	class HouseNoDestruct;
	class ab_willowisp_model_base: HouseNoDestruct
	{
	};
	class ab_willowisp_model: ab_willowisp_model_base
	{
		scope=2;
		storageCategory = 10;
		model="abWillowisps\models\empty.p3d";
	};
};

class CfgSoundShaders
{
	class ab_willowisp_kill_SoundShader
	{
		samples[]=
		{
			
			{
				"abWillowisps\sounds\kill.ogg",
				1
			}
		};
		range=150; 
		volume=5;
	};
	
	class ab_willowisp_ambient_SoundShader
	{
		samples[]=
		{
			
			{
				"abWillowisps\sounds\ambient1",
				1
			}
		};
		range=300;
		volume=1.5;
	};
	
	class ab_willowisp_creepy_SoundShader
	{
		samples[]=
		{
			
			{
				"abWillowisps\sounds\creepy1",
				1
			},
			{
				"abWillowisps\sounds\creepy2",
				1
			},
			{
				"abWillowisps\sounds\creepy3",
				1
			},
			{
				"abWillowisps\sounds\creepy4",
				1
			},
			{
				"abWillowisps\sounds\creepy5",
				1
			},
			{
				"abWillowisps\sounds\creepy6",
				1
			}
		};
		range=200;
		volume=1.5;
	};
};

class CfgSoundSets
{
	class ab_willowisp_kill_SoundSet
	{
		soundShaders[]=
		{
			"ab_willowisp_kill_SoundShader"
		};
	};
	
	class ab_willowisp_ambient_SoundSet
	{
		soundShaders[]=
		{
			"ab_willowisp_ambient_SoundShader"
		};
	};
	
	class ab_willowisp_creepy_SoundSet
	{
		soundShaders[]=
		{
			"ab_willowisp_creepy_SoundShader"
		};
	};
};
