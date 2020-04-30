#pragma once
#include <math.h>

// 3D data for each player
// Memory addresses within "PlayerData" 

    class Vect3d
    {
	public:
        float x;
        float y;
        float z;

        Vect3d(float _x, float _y, float _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        Vect3d()
        {
        }

        float length()
        {
			return (float)sqrt(x * x + y * y + z * z);
        }

        float dotproduct(Vect3d dot)
        {
            return (x * dot.x + y * dot.y + z * dot.z);
        }

    };

		struct Color
	{
		public:
		short R;
		short G;
		short B;

		Color()
		{

		}
		Color(short r, short g, short b)
		{
			R = r;
			G = g;
			B = b;
		}

	};

    class PlayerDataVec
    {
		public:
        float xMouse;
        float yMouse;
        int isValid;
        float xPos;
        float yPos;
        float zPos;
        int isAlive;
        int clientNum;
        Color color;
        char name[16];
        int pose;
        int team;
        bool visible;
        int isInGame;
		int health;

        Vect3d VecCoords()
        {
			Vect3d vec(xPos, zPos, yPos);
            return vec;
        }
    };
