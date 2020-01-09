#pragma once
class Mission_G
{
public:
	Mission_G();
	Mission_G(int,float, float, float, float, float);
	~Mission_G();


public:
	int type;
	float x;
	float y;
	float z;
	float step;
	float para;


};

