#include <Windows.h>
#include <iostream>
#include "main.h"
#include <vector>
#include <algorithm>
using namespace std;


CHackProcess fProcess;
using namespace std;

#define F6_Key 0x75
#define RIGHT_MOUSE 0x02

int NumOfPlayers = 32;

//Relative offsets:
const DWORD dw_PlayerCountOffs = 0x5EF6BC;//Engine.dll
const DWORD Player_Base = 0x4C6708;
const DWORD dw_m_angRotation = 0x47F1B4; //ViewAngles - find by moving our mouse around, look for changed/unchanged value, or use cl_pdump 1

// Entity offsets
const DWORD dw_mTeamOffset = 0x9C;
const DWORD dw_Health = 0x94;
const DWORD dw_Pos = 0x260;

const DWORD EntityPlayer_Base = 0x4D3904;//Entitylist relative offset
const DWORD EntityLoopDistance = 0x10; //Distance in bytes between each ent

struct MyPlayer_t
{
	DWORD CLocalPlayer; //Address of our ent
	int Team;
	int Health;
	float Position[3];
	void ReadInformation()
	{
		//Get address of entity
		ReadProcessMemory(fProcess.__HandleProcess, (BYTE*)(fProcess.__dwordClient + Player_Base), &CLocalPlayer, sizeof(CLocalPlayer), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (BYTE*)(CLocalPlayer + dw_mTeamOffset), &Team, sizeof(Team), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (BYTE*)(CLocalPlayer + dw_Health), &Health, sizeof(Health), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (BYTE*)(CLocalPlayer + dw_Pos), &Position, sizeof(float[3]), 0);
		//Get Number of players
		ReadProcessMemory(fProcess.__HandleProcess, (BYTE*)(fProcess.__dwordEngine + dw_PlayerCountOffs), &NumOfPlayers, sizeof(int), 0);
	}
}MyPlayer;

//struct for targets
struct TargetList_t
{
	float Distance;
	float AimbotAngle[3];

	TargetList_t() {} //default contructor

	TargetList_t(float aimbotAngle[], float myCoords[], float enemyCoords[])
	{
		Distance = Get3dDistance(myCoords[0], myCoords[1], myCoords[2],
			enemyCoords[0], enemyCoords[1], enemyCoords[2]);

		//set aimbot angles for the ent
		AimbotAngle[0] = aimbotAngle[0];
		AimbotAngle[1] = aimbotAngle[1];
		AimbotAngle[2] = aimbotAngle[2];
	}

	float Get3dDistance(float myCoordsX, float myCoordsZ, float myCoordsY,
		float eNx, float eNz, float eNy)
	{
		return (float)sqrt(
			pow(double(eNx - myCoordsX), 2.0) +
			pow(double(eNy - myCoordsY), 2.0) +
			pow(double(eNz - myCoordsZ), 2.0));
	}
};

//Struct for other players
struct PlayerList_t
{
	DWORD CBaseEntity;
	int Team;
	int Health;
	float Position[3];
	float AimbotAngle[3];
	char Name[39];

	void ReadInformation(int Player)
	{
		//Get Address of Entity
		ReadProcessMemory(fProcess.__HandleProcess, (BYTE*)(fProcess.__dwordClient + EntityPlayer_Base + (Player * EntityLoopDistance)), &CBaseEntity, sizeof(DWORD), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (BYTE*)(CBaseEntity + dw_mTeamOffset), &Team, sizeof(int), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (BYTE*)(CBaseEntity + dw_Health), &Health, sizeof(int), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (BYTE*)(CBaseEntity + dw_Pos), &Position, sizeof(float[3]), 0);
	}
}PlayerList[32];

// Compare distance when sorting the array of Target Enemies "sort predicate"
struct CompareTargetEnArray
{
	bool operator() (TargetList_t & lhs, TargetList_t & rhs)
	{
		return lhs.Distance < rhs.Distance;
	}
};

void CalcAngle(float *src, float *dst, float *angles)
{
	double delta[3] = { (src[0] - dst[0]), (src[1] - dst[1]), (src[2] - dst[2]) };
	double hyp = sqrt(delta[0] * delta[0] + delta[1] * delta[1]);
	angles[0] = (float)(asinf(delta[2] / hyp) * 57.295779513082f);
	angles[1] = (float)(atanf(delta[1] / delta[0]) * 57.295779513082f);
	angles[2] = 0.0f;

	// Normalize angle
	if (delta[0] >= 0.0)
	{
		angles[1] += 180.0f;
	}
}

void Aimbot()
{
	// Declare our target list to define our victims through a dynamic array
	TargetList_t* TargetList = new TargetList_t[NumOfPlayers];

	// Loop through all our players and retrieve their information
	int targetLoop = 0;
	for (int i = 0; i < NumOfPlayers; i++)
	{
		PlayerList[i].ReadInformation(i);

		// Skip if they're my teammates.
		if (PlayerList[i].Team == MyPlayer.Team) continue;

		// Skip players without health such as bad ents
		if (PlayerList[i].Health < 2) continue;

		// PlayerList[i].Position[2] -= 10;
		CalcAngle(MyPlayer.Position, PlayerList[i].Position, PlayerList[i].AimbotAngle);

		// Populate array of targets with only good targets
		TargetList[targetLoop] = TargetList_t(PlayerList[i].AimbotAngle, MyPlayer.Position, PlayerList[i].Position);

		// Increment to advance the array for the next iteration
		targetLoop++;
	}

	//Aim only if we have any enemies
	if (targetLoop > 0)
	{
		//SORT ENEMIES ARRAY BY DISTANCE by using our sort predicate
		std::sort(TargetList, TargetList + targetLoop, CompareTargetEnArray());

		//AIM at the closest ent, by default aim at ALL times, if you right click hold it switches it off
		if (!GetAsyncKeyState(0x2))
		{
			WriteProcessMemory(fProcess.__HandleProcess, (BYTE*)(fProcess.__dwordEngine + dw_m_angRotation), TargetList[0].AimbotAngle, 12, 0);
		}

	}
	// Reset the loop counter
	targetLoop = 0;

	delete[] TargetList; //DELETE OUR ARRAY and clear memory
}

int main()
{
	fProcess.RunProcess(); // Waiting for CSS......
	cout << "Game found! Running b00m h3adsh0t aimbot." << endl;

	//Exit if the F6 key is pressed
	while (!GetAsyncKeyState(F6_Key))
	{
		MyPlayer.ReadInformation();
		Aimbot();
	}
}
