#pragma once


#include<qvector.h>
#include"APoint.h"
#include "Rada.h"
#include "qpair.h"
#include "Mission_G.h"
#include "qdebug.h"
#include "qfile.h"
class A_STAR 
{

public:
	A_STAR( APoint&,  APoint&,  APoint&,  QVector<Rada*>&,  Mission_G&,float e_w1, float survice_w1, float start_w1, float end_w1,float horizontal_corner1, float verticality_corner1, float hmin1, float hmax1, float StepLength1);
	QVector<APoint*> searchFirstPoints(APoint*, APoint*);
	float getcostG(APoint*p);
	float getcostH(APoint*p, APoint* endpos, int flag);
	float getcostheight(APoint * p);
	QVector<float> getCostS(APoint* ,APoint*);
	float Calc_dist(APoint*, APoint*);
	APoint* buble_poits(QVector<APoint*>);
	QVector<APoint*> searchPoints(APoint* lastPB, int parentIndex, APoint* endpos, int flag);
	void saveresult();
	QVector<float>  smooth_path(QVector<float>,int );

public:
	float StepLength;
	QVector<APoint*> close;
	QVector<APoint*> open;
	QVector<Rada*> radas;
	QVector<APoint*> result_point;
	QVector<QPair<float, float>> result;
	QList<Mission_G*> mission_g;
	APoint * startP;
	APoint * endP;
	float e_w;
	float survice_w;
	float start_w;
	float end_w;
	float horizontal_corner;
	float verticality_corner;
	float h_min;
	float h_max;
	QVector<QVector<float>> zcubic;

	QVector<float> width;
	QVector<float> depth;

	QVector<APoint*> route;


};
