void SpawnWillowisp()
{
	ab_Willowisp_Stealable_Items.Clear();
	ab_Willowisp_Stealable_Items.Insert("NVGoggles");
	ab_Willowisp_Stealable_Items.Insert("GasMask_Filter");
	
	Get_ab_WillowispManager().CreateRandomWillowispController("Sebjan Mine Woods", "4802.379883 88.361198 8559.879883", 500, 40, Math.RandomIntInclusive(1, 3));
	
	array<string> willowispPositions = {
	"4507.180176 87.886299 8202.490234",
	"4504.870117 87.929703 8228.429688",
	"4231.950195 78.514603 8081.220215",
	"4236.709961 78.568398 8123.149902",
	"4311.850098 79.753601 8049.379883"

	};
	Get_ab_WillowispManager().CreatePredefinedWillowispController("Athena Research Institute", "4507.180176 87.886299 8202.490234", 500, willowispPositions );
}

void main()
{
	/*
	
		Your original main() content here
	
	*/
	
	SpawnWillowisp();
};