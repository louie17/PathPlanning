#pragma once
class APoint
{
public:
	APoint();
	APoint(float x, float y, float z, float g, float f, int t,float time, float s);
	~APoint();


public:
	float X;
	float Y;
	float Z;
	float G;
	float F;
	float TIME_COST;
	float S_COST;
	int   TYPE;



};

