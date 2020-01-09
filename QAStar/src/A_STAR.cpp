#include "A_STAR.h"
#define PI 3.1415926
#define MAX_DIST 10000000
A_STAR::A_STAR( APoint& sp,  APoint& tp,  APoint& ep,  QVector<Rada*>& radav,  Mission_G& mg, float e_w1, float survice_w1, float start_w1, float end_w1, float horizontal_corner1, float verticality_corner1, float hmin1, float hmax1, float StepLength1)
{
	APoint *target = &tp;
	close.append(target);

	//参数初始化
		survice_w = survice_w1;// 1.0f;
		start_w = start_w1;// 50;
		end_w = end_w1;//50;
		e_w = e_w1; //0.5;
		survice_w = survice_w1;//1;
		h_min = hmin1; //0.1f;
		h_max = hmax1;//3.0f;
		horizontal_corner = horizontal_corner1;//0.7854f;
		verticality_corner = verticality_corner1;//0.5236f;
		StepLength = StepLength1;//4;
	for (int i = 0; i < 10; i++)
	{
		width << i * 100;
	}
	for (int i = 0; i < 7; i++)
	{
		depth << i * 100;
	}
	for (int i = 0; i < 601; i++)
	{
		QVector<float> c;
		for (int j = 0; j < 901; j++)
		{
			c << 0;
		}
		zcubic.append(c);
	}


	startP = &sp;
	endP = &ep;
	//Rada *rada1 = new Rada(1, 450, 350, 0, 100, 1);
	//Rada *rada2 = new Rada(2, 450, 350, 0, 70, 1);
	//Rada *rada3 = new Rada(3, 450, 350, 0, 70, 1);
	//radas.append(rada1);
	//radas.append(rada2);
	//radas.append(rada3);
	radas = radav;
	mission_g.append(&mg);
	QVector<APoint*> qpoints;
	open = searchFirstPoints(close[0], startP);
	float min_distance = MAX_DIST;
	APoint *temp_dis = new APoint(mission_g[0]->x, mission_g[0]->y, mission_g[0]->z, 0, 0, 0, 0, 0);
	float dist_start_miss = Calc_dist(startP, temp_dis);
	float dist_end_miss = Calc_dist(endP, temp_dis);
	delete(temp_dis);


	//从任务到开始位置的搜索
	while (1)
	{

		APoint* selectPoint = buble_poits(open);
		open.removeOne(selectPoint);
		close.append(selectPoint);
		float distance = Calc_dist(selectPoint, startP);
		float ratio = 1 - (distance) / dist_start_miss;

		qDebug() << "selectpoint:" << selectPoint->X << "  " << selectPoint->Y << " " << selectPoint->Z << endl;
		if (distance < StepLength / 2)
		{
			qDebug() << "finish all APoint from start to target";
			saveresult();
			APoint* lastpoint = result_point.at(result_point.size() - 2);
			APoint * selectpoint = result_point.last();
			lastpoint->TYPE = 0;
			selectpoint->TYPE = 1;
			result_point.removeOne(lastpoint);
			result_point.removeOne(selectpoint);
			open.clear();
			close.clear();
			close.append(lastpoint);
			open.append(selectpoint);
			
			min_distance = MAX_DIST;

			break;

		}
		QVector<APoint*> p = searchPoints(selectPoint, close.size(), startP, 1);
		open.append(p);



	}

	//从任务倒结束位置的搜素
	while (1)
	{
		APoint* selectPoint = buble_poits(open);
		qDebug() << "selectpoint:" << selectPoint->X << "  " << selectPoint->Y << " " << selectPoint->Z << " " << selectPoint->F << endl;
		open.removeOne(selectPoint);
		close.append(selectPoint);
		float distance = Calc_dist(selectPoint, endP);
		float ratio = 1 - (distance) / dist_end_miss;
		if (distance < StepLength / 2)
		{
			for (int i = 0; i < close.size(); i++)
			{
				result_point.append(close[i]);

			}
			qDebug() << "finish route from target to endpoint " << endl;
			break;

		}
		QVector<APoint*> p = searchPoints(selectPoint, close.size(), endP, 2);
		open.append(p);


	}
	QVector<float> x1;
	QVector<float> y1;
	QVector<float> z1;
	for (int i = 0; i < result_point.size(); i++)
	{
		float x = result_point[i]->X;
		float y = result_point[i]->Y;
		float z = result_point[i]->Z;
		x1.append(x);
		y1.append(y);
		z1.append(z);

	}
	x1 = smooth_path(x1, 3);
	y1 = smooth_path(y1, 3);
	z1 = smooth_path(z1, 3);

	QFile file("e:/result.txt");
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		for (int i = 0; i < result_point.size(); i++)
		{
			QString str = QString("%1,%2,%3").arg(x1[i]).arg(y1[i]).arg(z1[i]);
			file.write(str.toLatin1());
			file.write("\n");
			APoint route1(x1[i],y1[i],z1[i],0,0,0,0,0);
			route.append(&route1);
				
		}


	}
	file.close();
	
	

}

QVector<APoint*> A_STAR::searchFirstPoints(APoint *close_point, APoint *endpos)
{
	float x = close_point->X;
	float y = close_point->Y;
	float z = close_point->Z;

	float cornerlength = StepLength*sin(0.25*PI);
	//生产8个点
	QVector<APoint* >points;
	APoint* p1 = new APoint(x + StepLength, y, z, 0, 0, 0, 0, 0);
	APoint* p2 = new APoint(x - StepLength, y, z, 0, 0, 0, 0, 0);
	APoint* p3 = new APoint(x , y+ StepLength, z, 0, 0, 0, 0, 0);
	APoint* p4 = new APoint(x , y- StepLength, z, 0, 0, 0, 0, 0);
	APoint* p5 = new APoint(x + cornerlength, y+ cornerlength, z, 0, 0, 0, 0, 0);
	APoint* p6 = new APoint(x + cornerlength, y- cornerlength, z, 0, 0, 0, 0, 0);
	APoint* p7 = new APoint(x - cornerlength, y+ cornerlength, z, 0, 0, 0, 0, 0);
	APoint* p8 = new APoint(x - cornerlength, y- cornerlength, z, 0, 0, 0, 0, 0);
	points.append(p1);
	points.append(p2);
	points.append(p3);
	points.append(p4);
	points.append(p5);
	points.append(p6);
	points.append(p7);
	points.append(p8);

	for (int i = 0; i < points.size(); i++)
	{
		points[i]->G = getcostG(points[i]);
		points[i]->F = getcostH(points[i], endpos,1) + points[i]->G + getcostheight(points[i]);
		points[i]->TYPE = 1;
	
		QVector<float> a = getCostS(points[i], close_point);
		points[i]->TIME_COST = a[0];
		points[i]->S_COST = a[1];
		points[i]->F += points[i]->TIME_COST + points[i]->S_COST;


	}


	return points;
}

float A_STAR::getcostG(APoint * p)
{
	float f_w = 0.3f;
	float cost_1;
	float cost_g;
	APoint *last_point = NULL;
	int close_m = close.size();
	if (close_m != 0)
	{
		last_point = close.at(close_m-1);
		 cost_1 = StepLength;
	}
	else
	{
		cost_1 = 0;
	}
	cost_g = f_w *cost_1 + last_point->G;

	return cost_g;
}

float A_STAR::getcostH(APoint * p, APoint * endpos, int flag)
{
	float cost_h=0.0f;
	//p当前点
	//endpos 当前路径规划的终点，flag为1表示该点到起点规划
	if (flag == 1)
	{
		cost_h = start_w*sqrt(pow((endpos->X - p->X), 2) + pow((endpos->Y - p->Y), 2) + pow((endpos->Z - p->Z), 2));
	}
	if (flag == 2)
	{
		cost_h = end_w*sqrt(pow((endpos->X - p->X), 2) + pow((endpos->Y - p->Y), 2) + pow((endpos->Z - p->Z), 2));
	}


	return cost_h;
}

float A_STAR::getcostheight(APoint * p)
{
	float cost_height;

	cost_height = e_w *abs(p->Z - 1) * 100;

	return cost_height;
}

QVector<float> A_STAR::getCostS(APoint *Par_Pos, APoint * lastPB)
{
	QVector<float > time_surv;
	float inter_base_cost = 3.0;
	float surv = 0.0f;
	float time = 0.0f;
	QVector<int> sensornum;
	//11.25日修改（10e7）
	float sur_k = 10e7;
	for (int i = 0; i < radas.size(); i++)
	{


		float r = radas[i]->r;
		APoint *p1 = new APoint(radas[i]->x, radas[i]->y, radas[i]->z, 0, 0, 0, 0, 0);
		float d = Calc_dist(Par_Pos, p1);
		delete(p1);
		if (d < r)
		{
			sensornum.append(radas[i]->type);

		}
	}
		if (sensornum.isEmpty())
		{
			surv = 0.0f;
		}
		if (sensornum.size() ==1)
		{
			Rada * cur = NULL;
			int seq = sensornum[0];
			for (int i = 0; i < radas.size(); i++)
			{
				if (seq == radas[i]->type)
				{
					cur = radas[i];
				}
			}
		
			APoint *p1 = new APoint(cur->x, cur->y, cur->z, 0, 0, 0, 0, 0);
			float d = Calc_dist(Par_Pos, p1);
			delete(p1);
			surv += sur_k / pow(d, 2);
		}
		if (sensornum.size() > 1)
		{
			surv = inter_base_cost;
			for (int i = 0; i < sensornum.size(); i++)
			{
				Rada * cur = NULL;
				int seq = sensornum[i];
				for (int i = 0; i < radas.size(); i++)
				{
					if (seq == radas[i]->type)
					{
						cur = radas[i];
					}
				}

				APoint *p1 = new APoint(cur->x, cur->y, cur->z, 0, 0, 0, 0, 0);
				float d = Calc_dist(Par_Pos, p1);
				delete(p1);
				surv += sur_k / pow(d, 2);
			}

		}
		time = 0;
		surv = survice_w *surv;
		if (sensornum.size() != 0)
		{
			time = 1.2 *(lastPB->TIME_COST + StepLength / 10.0f);
			surv += time;
		}

	
	time_surv.append(time);
	time_surv.append(surv);


	return time_surv;
}

float A_STAR::Calc_dist( APoint *p1, APoint *p2)
{


	return  float(sqrt(pow( (p1->X - p2->X),2 ) + pow((p1->Y - p2->Y), 2)+ pow((p1->Z - p2->Z), 2)) *1.0f);
}

APoint* A_STAR::buble_poits(QVector<APoint*> p)
{
	float min = 10000000000.0f;

	for (int i = 0; i < p.size(); i++)
	{
		if (min > p[i]->F)
		{
			min = p[i]->F;
		}
	}
	for (int i = 0; i < p.size(); i++)
	{
		if (min == p[i]->F)
		{
			return p[i];
		}
	}

	return NULL;

}

QVector<APoint*> A_STAR::searchPoints(APoint * lastPB, int parentIndex, APoint * endpos, int flag)
{
	APoint* lastPA = close.at(lastPB->TYPE - 1);
	float l = StepLength / cos(horizontal_corner);
	
	float norm_ab = Calc_dist(lastPB, lastPA);
	float pa_x = lastPA->X;
	float pa_y = lastPA->Y;
	float pb_x = lastPB->X;
	float pb_y = lastPB->Y;
	result.clear();
	 pa_x = float(int(pa_x * 100) / 100.0f);
	 pb_x = float(int(pb_x * 100) / 100.0f);
	if (pa_x != pb_x)
	{
		//解方程
		/*float c = cos(horizontal_corner)*norm_ab*StepLength;
		float d = (c + (pb_y - pa_y)*pb_y + (pb_x - pa_x)*pb_x)/((pb_x - pa_x));
		float m = (pb_y - pa_y)*(pb_y - pa_y) + 1;
		float n = 2 * (pb_y - pa_y)*(d - pb_x) + 2 * pb_y;
		float k = pb_y*pb_y + (d - pb_x)*(d - pb_x) - StepLength*StepLength;

		float y = 0;
		
		float y1 =  (n + sqrt(n*n - 4 * m*k) )/2*m;
		float y2 = (n - sqrt(n*n - 4 * m*k)) / 2 * m;
		float x1 = d - y1*(pb_y - pa_y)/((pb_x - pa_x));
		float x2 = d - y2*(pb_y - pa_y)/ (pb_x - pa_x);

		result.append(qMakePair(int(x1), int(y1)));
		result.append(qMakePair(int(x2), int(y2)));*/
		float x1 = pa_x;
		float x2 = pa_y;
		float x1_0, x2_0;
		float x1_1, x2_1;
		float delte_x1, delte_x2;
		float fun_f, fun_g;
		float f_x1, f_x2;
		float g_x1, g_x2;
		float ERROR = 5e-2;
		float error_out;
		int steps;
		int MAX_STEP = 100000;
		x1_0 = x1;
		x2_0 = x2;
		for (steps = 0; steps < MAX_STEP; steps++)
		{
			//方程组
			fun_f = int(norm_ab)*cos(horizontal_corner)*StepLength - int((pb_x - pa_x)*(x1_0 - pb_x)) - int((pb_y - pa_y)*(x2_0 - pb_y));
			fun_g = int((x1_0 - pb_x)* (x1_0 - pb_x)) + int((x2_0 - pb_y)*(x2_0 - pb_y)) - int(StepLength*StepLength);
			//偏导数
			f_x1 = (-(x1_0)*(pb_x - pa_x));
			f_x2 = (-(x2_0)*(pb_y - pa_y));
			g_x1 = (2 * x1_0 - 2 * pb_x);
			g_x2 = (2 * x2_0 - 2 * pb_y);
			
			
			//迭代
			delte_x1 = -fun_f / (f_x1 + f_x2);
			delte_x2 = -fun_g / (g_x1 + g_x2);

			x1_1 = x1_0 + delte_x1;
			x2_1 = x2_0 + delte_x2;

			x1_0 = x1_1;
			x2_0 = x2_1;
			error_out = fabs(delte_x1) + fabs(delte_x2);
			if (error_out < ERROR)
			{
				
				if(result.isEmpty())
				result.append(qMakePair(int(x1_0), int(x2_0)));
				//result.append(qMakePair(x1_0+StepLength, x2_0+StepLength));
				else 
				{
					if (100== result.size())
						break;
					
					if (int(result.last().first) != int(x1_0) && int(result.last().second) != int(x2_0))
					result.append(qMakePair(int(x1_0), int(x2_0)));
				}
			}
		}
		//debug 使用
		int a=10;
		qDebug() << a;
	}
	else
	{
		float x = StepLength*sin(horizontal_corner);
		float y = StepLength*cos(horizontal_corner);
		if (pa_y < pb_y)
			y = pb_y + y;
		else
			y = pb_y - y;
		result.append(qMakePair(int(pb_x - x), int(y)));
		result.append(qMakePair(int(pb_x + x), int(y)));
	}
	if (result.size() < 2)
	{
		float x = StepLength*sin(horizontal_corner);
		float y = StepLength*cos(horizontal_corner);
		if (pa_y < pb_y)
			y = pb_y + y;
		else
			y = pb_y - y;
		result.append(qMakePair(int(pb_x - x), int(y)));
		result.append(qMakePair(int(pb_x + x), int(y)));
	}


	QVector<QVector<float>> points;
	QVector<float> cur_point;
	QVector<float> C;
	//计算水平面上的直线点
	for (int i = 0; i < result.size(); i++)
	{
		cur_point.clear();
		cur_point << result[i].first << result[i].second << lastPB->Z << l;
	
		points.append(cur_point);
	}



	C.clear();
	C << (lastPB->X - lastPA->X + pb_x) << (lastPB->Y - lastPA->Y + pb_y) << lastPB->Z << StepLength;
	points.append(C);
	//计算不转弯情况下的爬升点
	l = StepLength / cos(verticality_corner);
	float z_top = float(int((l*tan(verticality_corner) + lastPB->Z) * 100) / 100.0f);
	//偏转情况步长
	for (int i = 0; i < 2; i++)
	{
		float step = sqrt((points[i][3 - 1] - z_top)*(points[i][3 - 1] - z_top) + points[i][4 - 1] * points[i][4 - 1]);
		QVector<float> p_new;
		p_new << points[i][1-1] << points[i][2-1] << z_top << step;
		points.append(p_new);
		
	}
	C.clear();
	C << points[3 - 1][1 - 1] << points[3 - 1][2 - 1] << z_top << l;
	points.append(C);
	//计算下降的三个点
	float z_bottom = float(int((l*tan(-verticality_corner) + lastPB->Z) * 100) / 100.0f);
	for(int i=0;i<2;i++)
	{
		float step = sqrt((points[i][3 - 1] - z_bottom)*(points[i][3 - 1] - z_bottom) + points[i][4 - 1] * points[i][4 - 1]);
		QVector<float> p_new;
		p_new << points[i][1 - 1] << points[i][2 - 1] << z_bottom << step;
		points.append(p_new);
	}
	QVector<float> p_new;
	p_new << points[2][1 - 1] << points[2][2 - 1] << z_bottom << l;
	points.append(p_new);

	//筛选掉高度小于0.1的店和超过外界的点
	int index = 0;
	QVector<APoint*> vec_points;

	for(;index<points.size();index++)
	{
		if (index < 0)
			index = 0;
		if (points[index][1 - 1] <= width[0] || points[index][1 - 1] >= width[9] || points[index][2 - 1] <= depth[0] || points[index][2 - 1] >= depth[6])
		{
			points.removeAt(index);
			index--;
			continue;
		}
		int tempx = ceil(points[index][0]);
		int tempy = ceil(points[index][1]);
		if (points[index][3 - 1]<zcubic[tempy][tempx] + h_min || points[index][3 - 1]>zcubic[tempy][tempx] + h_max)
		{
			points.removeAt(index);
			index--;
			continue;
		}


	
	}
	//筛选不合适的解
	for (int i = 0; i < points.size(); i++)
	{
		for (int j = i + 1; j < points.size(); j++)
		{
			if (points[i][0] == points[j][0] && points[i][1] == points[j][1])
			{
				points.remove(i);
				i--;
				break;
			}

		}

	}



	//针对筛选后得到的点加入代价信息
	for (int i = 0; i < points.size(); i++)
	{
		APoint* Pot = new APoint(points[i][0], points[i][1], points[i][2], 0, 0, 0, 0, 0);
		Pot->G = getcostG(Pot);
		Pot->F = getcostH(Pot, endpos, flag) + Pot->G + getcostheight(Pot);
		Pot->TYPE = parentIndex;
		QVector<float> a = getCostS(Pot, lastPB);
		Pot->TIME_COST = a[0];
		Pot->S_COST = a[1];
		Pot->F += Pot->S_COST;
		vec_points.append(Pot);

	}
	//需要筛选一下有些点是在close表里，需要计算代价来判断是否更新该点还是从points中踢出来避免重复加入这个点
	for (int i = 0; i < close.size(); i++)
	{
		float exist_x = float(int((close[i]->X) * 100) / 100.0f);
		float exist_y = float(int((close[i]->Y) * 100) / 100.0f);
		float exist_z = float(int((close[i]->Z) * 100) / 100.0f);
		float exist_c = float(int((close[i]->F) * 100) / 100.0f);


		//需要判断任务状态是否相同
	
		for (int i = 0; i < vec_points.size(); i++)
		{
			if (float(int((vec_points[i]->X) * 100) / 100.0f) == exist_x && float(int((vec_points[i]->F) * 100) / 100.0f) == exist_y && float(int((vec_points[i]->Z) * 100) / 100.0f) == exist_z)
			{
				if (float(int((vec_points[i]->F) * 100) / 100.0f) < exist_c)
				{
					continue;
				}

				vec_points.removeAt(i);
				i--;

			}

		}

	}

	//筛选一下有些点是在open表中的，需要决定是否更新还是踢出来
	for (int i = 0; i < open.size(); i++)
	{
		float exist_x = float(int((open[i]->X) * 100) / 100.0f);
		float exist_y = float(int((open[i]->Y) * 100) / 100.0f);
		float exist_z = float(int((open[i]->Z) * 100) / 100.0f);
		float exist_c = float(int((open[i]->F) * 100) / 100.0f);
		int j = 0;
		while (j < vec_points.size())
		{
			if (float(int((vec_points[j]->X) * 100) / 100.0f) == exist_x && float(int((vec_points[j]->Y) * 100) / 100.0f) == exist_y && float(int((vec_points[j]->Z) * 100) / 100.0f) == exist_z)
			{
				if (float(int((vec_points[j]->F) * 100) / 100.0f) < exist_c)
				{
					
					open[i] = vec_points[j];

				}
				vec_points.removeAt(j);
				j--;

			}
			j++;
		}


	}



	return vec_points;



}

void A_STAR::saveresult()
{
	APoint* temp = close.last();
	int index = temp->TYPE;
	result_point.append(temp);
	result_point.append(close.at(index));

	while (index)
	{
		index = close.at(index)->TYPE-1;
		qDebug() << index << endl;
		result_point.append(close.at(index));

	}

	
}

QVector<float> A_STAR::smooth_path(QVector<float> x, int m)
{
	int n = x.size()-1;
	QVector<float> a,b;
	a = x;
	b = x;
	for (int k = 0; k <m; k++)
	{
		b[0] = (69 * a[0] + 4 * (a[1] + a[3]) - 6 * a[2] - a[4]) / 70;
		b[1] = (2 * (a[0] + a[4]) + 27 * a[1] + 12 * a[2] - 8 * a[3]) / 35;
		for (int j = 2; j < n-3; j++)
		{
			b[j] = (-3 * (a[j-2] + a[j+2]) + 12 * (a[j-1]+ a[j+1]) + 17 * a[j]) / 35;
		}
		b[n-1]= (2 * (a[n] + a[n-4]) + 27 * a[n-1] + 12 * a[n-2] - 8 * a[n-3]) / 35;
		b[n] = (69 * a[n] + 4 * (a[n-1] + a[n-3]) - 6 * a[n-2] - a[n-4]) / 70;
		a = b;
	}

	return a;

}


