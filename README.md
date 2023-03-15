# Will-o'-the-Wisp
<p>Will-o'-the-Wisp creature mod for Dayz. Will-o'-the-Wisp tries to catch player and charge into him causing unconscious state for 10 seconds, small random damage and optionally steals one random item from player. After charge, Will-o'-the-Wisp is idle for 20 seconds giving 10s for recovery and escape. Will-o'-the-Wisp always goes to last seen player position. It is possible to hide from it or just run away.</p>
<p>https://youtu.be/9eOUwGqYvGM</p>
<p>Will-o'-the-Wisp code has been written reusing some parts from my Designators mod. All sounds and textures are from free sources. Respective authors are noted in credits.</p>

### INSTALLATION AND SETUP
This is both server and client mod. Client needs just subscribe to mod or adds it manually. 

At server side open your mission Init.c and find void main(). Put following code at the end of main function.

```
Get_ab_WillowispManager().CreateRandomWillowispController("Shachovka Forrest", "9089.700195 315.509003 7558.310059", 500, 50, Math.RandomIntInclusive(1, 3));

array<string> willowispPositions = {
"4464.140137 315.640991 8250.669922",
"4456.279785 315.337006 8264.139648",
"4424.439941 314.873993 8269.299805",
"4472.290039 315.575012 8286.809570",
"4456.529785 315.414001 8315.379883",
"4491.459961 317.023987 8273.509766",
"4486.479980 316.333008 8326.230469",
"4504.129883 317.686005 8297.540039",
"4532.600098 318.393005 8298.469727",
"4547.910156 319.480988 8345.040039",
"4566.259766 319.893005 8382.780273",
"4574.899902 319.507996 8330.860352",
"4572.330078 319.339996 8351.759766",
"4545.560059 319.109009 8316.230469",
"4531.149902 317.933990 8278.950195",
"4561.479980 319.721008 8249.519531",
"4591.509766 320.345001 8242.500000",
"4575.390137 319.764008 8292.790039",
"4571.290039 319.854004 8221.280273",
"4553.089844 319.207001 8287.849609",
"4471.229980 315.656006 8301.559570"
};
Get_ab_WillowispManager().CreatePredefinedWillowispController("Veresnik Military Base", "4537.540039 318.433014 8297.910156", 500, willowispPositions );
```

### API
**_Get_ab_WillowispManager().CreateRandomWillowispController(string name, vector position, float resetRadius, float radius, int willowispCount)_**

Creates circular Will-o'-the-Wisp infested area with size of radius at position and randomly fills it with willowispCount Will-o'-the-Wisps. Area resets everytime no players are inside resetRadius. You can name area with name parameter to see it in logs.

**_Get_ab_WillowispManager().CreatePredefinedWillowispController(string name, vector position, float resetRadius, ref array<string> willowispPositions)_**
  
Creates predefined Will-o'-the-Wisp infested area with logical center position and fills it with Will-o'-the-Wisps from willowispPositions string array that contains coords in vector format. Area resets everytime no players are inside resetRadius. You can name area with name parameter to see it in logs.

**_ab_Willowisp_Stealable_Items_**
Static string array that defines stealable items that Will-o'-the-Wisp is allowed to steal from knocked dow player after successful attack. Array is initialized and empty by default.
In your init.c before spawning any Will-o'-the-Wisps set items with following code:

```
ab_Willowisp_Stealable_Items.Clear();
ab_Willowisp_Stealable_Items.Insert("NVGoggles");
ab_Willowisp_Stealable_Items.Insert("BandageDressing");
ab_Willowisp_Stealable_Items.Insert("DisinfectantSpray");
ab_Willowisp_Stealable_Items.Insert("DisinfectantAlcohol");
```

### CREDITS
<ul>
  <li>Code by ataribaby.</li>
  <li>Special thanks for Heckler_CZ and geordie_pv from PigeonplayRP for valuable help.</li>
  <li>HunterCZ for valuable help.</li>
  <li>Sounds:
	<p>mrs_lenka for voiced random line</p>
    <p>https://freesound.org/people/stevenmaertens/sounds/676466/</p>
	<p>https://freesound.org/people/monosfera/sounds/319365/</p>
	<p>https://freesound.org/people/monosfera/sounds/319346/</p>
	<p>https://freesound.org/people/monosfera/sounds/319347/</p>
	<p>https://freesound.org/people/Robinhood76/sounds/268942/</p>
	<p>https://freesound.org/people/jvmyka@gmail.com/sounds/580775/</p>
	<p>https://freesound.org/people/fury12/sounds/496719/</p>
  </li>
</ul>

### LICENSE
Mod is open source and you are free to modify, repack and publish. If you fix or enhance it please consider push it to me for intergration into official version.

Steam Workshop link: https://steamcommunity.com/sharedfiles/filedetails/?id=2943755582
