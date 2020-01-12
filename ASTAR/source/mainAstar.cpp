#include "mainAstar.h"
namespace AS {

	VAPointPtr Choose_Astar(const APoint& sp, const APoint& tp, const APoint& ep, const VRada& radav, const Mission_G& mg, const float& e_w1, const float& survice_w1, const float& start_w1, const float& end_w1, const float& horizontal_corner1, const float& verticality_corner1, const float& hmin1, const float& hmax1, const float& StepLength1)
	{
		A_STAR MyAstar(sp, tp, ep, radav, mg, e_w1, survice_w1, start_w1, end_w1, horizontal_corner1, verticality_corner1, hmin1, hmax1, StepLength1);

		int MAX_It = int(Calc_dist(sp, ep) / StepLength1) * 2;
		for (int i = 0; i < 10; i++)
		{
			MyAstar.width.push_back(i * 100);
		}
		for (int i = 0; i < 7; i++)
		{
			MyAstar.depth.push_back(i * 100);
		}
		for (int i = 0; i < 601; i++)
		{
			vector<float> c;
			for (int j = 0; j < 901; j++)
			{
				c.push_back(0);
			}
			MyAstar.zcubic.push_back(c);
		}
		auto ptr_tp = make_shared<APoint>(tp);
		auto ptr_ep = make_shared<APoint>(ep);
		auto ptr_sp = make_shared<APoint>(sp);
		MyAstar.close.push_back(ptr_tp);

		VAPointPtr qpoints;
		MyAstar.open = searchFirstPoints(MyAstar);
		float min_distance = MAX_DIST;
		APoint temp_dis(mg.x, mg.y, mg.z, 0, 0, 0, 0, 0);
		float dist_start_miss = Calc_dist(sp, temp_dis);
		float dist_end_miss = Calc_dist(ep, temp_dis);


		//begin astar
		int it = 0;
		while (it++ < MAX_It)
		{

			APointPtr selectPoint = buble_poits(MyAstar.open);
			//open.removeOne(selectPoint);
			removeone(MyAstar.open, selectPoint);
			MyAstar.close.push_back(selectPoint);

			float distance = Calc_dist(selectPoint, ptr_sp);
			float ratio = 1 - (distance) / dist_start_miss;

			cout << it << ": " << "selectpoint:" << selectPoint->X << "  " << selectPoint->Y << " " << selectPoint->Z << endl;
			if (distance < MyAstar.StepLength / 2)
			{
				cout << "finish all APoint from start to target";
				saveresult(MyAstar);
				APointPtr lastpoint = MyAstar.result_point.at(MyAstar.result_point.size() - 2);
				APointPtr selectpoint = MyAstar.result_point.back();
				lastpoint->TYPE = 0;
				selectpoint->TYPE = 1;
				//result_point.removeOne(lastpoint);
				//result_point.removeOne(selectpoint);
				removeone(MyAstar.result_point, lastpoint);
				removeone(MyAstar.result_point, selectpoint);
				MyAstar.open.clear();
				MyAstar.close.clear();
				MyAstar.close.push_back(lastpoint);
				MyAstar.open.push_back(selectpoint);

				min_distance = MAX_DIST;
				break;
			}
			VAPointPtr p = searchPoints(selectPoint, MyAstar.close.size(), ptr_sp, 1, MyAstar);
			for (int i = 0; i < p.size(); i++)
			{
				MyAstar.open.push_back(p[i]);
			}



		}
		it = 0;
		//从任务倒结束位置的搜素
		while (it++ < MAX_It)
		{
			APointPtr selectPoint = buble_poits(MyAstar.open);
			cout << "selectpoint:" << selectPoint->X << "  " << selectPoint->Y << " " << selectPoint->Z << " " << selectPoint->F << endl;
			removeone(MyAstar.open, selectPoint);
			MyAstar.close.push_back(selectPoint);
			float distance = Calc_dist(selectPoint, ptr_ep);
			float ratio = 1 - (distance) / dist_end_miss;
			if (distance < MyAstar.StepLength / 2)
			{
				for (int i = 0; i < MyAstar.close.size(); i++)
				{
					MyAstar.result_point.push_back(MyAstar.close[i]);

				}
				cout << "finish route from target to endpoint " << endl;
				break;

			}
			VAPointPtr p = searchPoints(selectPoint, MyAstar.close.size(), ptr_ep, 2, MyAstar);
			//open.push_back(p);
			for (int i = 0; i < p.size(); i++)
			{
				MyAstar.open.push_back(p[i]);
			}

		}
		//vector<float> x1;
		//vector<float> y1;
		//vector<float> z1;
		//for (int i = 0; i < MyAstar.result_point.size(); i++)
		//{
		//	float x = MyAstar.result_point[i]->X;
		//	float y = MyAstar.result_point[i]->Y;
		//	float z = MyAstar.result_point[i]->Z;
		//	x1.push_back(x);
		//	y1.push_back(y);
		//	z1.push_back(z);

		//}

		return MyAstar.result_point;
	}

	VAPointPtr searchFirstPoints(A_STAR& A_STAR)
	{
		auto close_point = A_STAR.close[0];
		float x = close_point->X;
		float y = close_point->Y;
		float z = close_point->Z;
		float StepLength = A_STAR.StepLength;
		float cornerlength = StepLength * sin(0.25 * PI);
		//生产8个点
		VAPointPtr points;
		APointPtr p1 = make_shared<APoint>(x + StepLength, y, z, 0, 0, 0, 0, 0);
		APointPtr p2 = make_shared<APoint>(x - StepLength, y, z, 0, 0, 0, 0, 0);
		APointPtr p3 = make_shared<APoint>(x, y + StepLength, z, 0, 0, 0, 0, 0);
		APointPtr p4 = make_shared<APoint>(x, y - StepLength, z, 0, 0, 0, 0, 0);
		APointPtr p5 = make_shared<APoint>(x + cornerlength, y + cornerlength, z, 0, 0, 0, 0, 0);
		APointPtr p6 = make_shared<APoint>(x + cornerlength, y - cornerlength, z, 0, 0, 0, 0, 0);
		APointPtr p7 = make_shared<APoint>(x - cornerlength, y + cornerlength, z, 0, 0, 0, 0, 0);
		APointPtr p8 = make_shared<APoint>(x - cornerlength, y - cornerlength, z, 0, 0, 0, 0, 0);
		points.push_back(p1);
		points.push_back(p2);
		points.push_back(p3);
		points.push_back(p4);
		points.push_back(p5);
		points.push_back(p6);
		points.push_back(p7);
		points.push_back(p8);

		for (int i = 0; i < points.size(); i++)
		{
			points[i]->G = getcostG(A_STAR);
			points[i]->F = getcostH(points[i], A_STAR, 1) + points[i]->G + getcostheight(points[i], A_STAR.e_w);
			points[i]->TYPE = 1;

			vector<float> a = getCostS(points[i], close_point, A_STAR);
			points[i]->TIME_COST = a[0];
			points[i]->S_COST = a[1];
			points[i]->F += points[i]->TIME_COST + points[i]->S_COST;

		}
		return points;
	}

	float Calc_dist(const APoint& p1, const APoint& p2)
	{
		return  float(sqrt(pow((p1.X - p2.X), 2) + pow((p1.Y - p2.Y), 2) + pow((p1.Z - p2.Z), 2)) * 1.0f);

	}
	float Calc_dist(APointPtr p1, const APointPtr p2)
	{
		return  float(sqrt(pow((p1->X - p2->X), 2) + pow((p1->Y - p2->Y), 2) + pow((p1->Z - p2->Z), 2)) * 1.0f);
	}
	float getcostG(A_STAR& A_STAR)
	{
		float f_w = 0.3f;
		float cost_1;
		float cost_g;
		APointPtr last_point = make_shared<APoint>();
		int close_m = A_STAR.close.size();
		if (close_m != 0)
		{
			last_point = A_STAR.close.at(close_m - 1);
			cost_1 = A_STAR.StepLength;
		}
		else
		{
			cost_1 = 0;
		}
		cost_g = f_w * cost_1 + last_point->G;

		return cost_g;
	}

	float getcostH(APointPtr p, A_STAR& A_STAR, const size_t& flag)
	{
		float cost_h = 0.0f;
		//p当前点
		//endpos 当前路径规划的终点，flag为1表示该点到起点规划

		float start_w = A_STAR.start_w;
		float end_w = A_STAR.end_w;
		if (flag == 1)
		{
			APointPtr endpos = make_shared<APoint>(A_STAR.startP);
			cost_h = start_w * sqrt(pow((endpos->X - p->X), 2) + pow((endpos->Y - p->Y), 2) + pow((endpos->Z - p->Z), 2));
		}
		if (flag == 2)
		{
			APointPtr endpos = make_shared<APoint>(A_STAR.endP);
			cost_h = end_w * sqrt(pow((endpos->X - p->X), 2) + pow((endpos->Y - p->Y), 2) + pow((endpos->Z - p->Z), 2));
		}


		return cost_h;
	}
	float getcostheight(APointPtr p, float e_w)
	{
		float cost_height;

		cost_height = e_w * abs(p->Z - 1) * 100;

		return cost_height;
	}

	vector<float> getCostS(APointPtr Par_Pos, APointPtr  lastPB, A_STAR& A_STAR)
	{
		vector<float > time_surv;
		float inter_base_cost = 3.0;
		float surv = 0.0f;
		float time = 0.0f;
		vector<int> sensornum;
		auto radas = A_STAR.radas;
		//11.25日修改（10e7）
		float sur_k = 10e7;
		for (int i = 0; i < radas.size(); i++)
		{


			float r = A_STAR.radas[i].r;
			APointPtr p1 = make_shared<APoint>(radas[i].x, radas[i].y, radas[i].z, 0, 0, 0, 0, 0);
			float d = Calc_dist(Par_Pos, p1);

			if (d < r)
			{
				sensornum.push_back(radas[i].type);

			}
		}
		if (sensornum.empty())
		{
			surv = 0.0f;
		}
		if (sensornum.size() == 1)
		{
			Rada cur;
			int seq = sensornum[0];
			for (int i = 0; i < radas.size(); i++)
			{
				if (seq == radas[i].type)
				{
					cur = radas[i];
				}
			}

			APointPtr p1 = make_shared<APoint>(cur.x, cur.y, cur.z, 0, 0, 0, 0, 0);
			float d = Calc_dist(Par_Pos, p1);

			surv += sur_k / pow(d, 2);
		}
		if (sensornum.size() > 1)
		{
			surv = inter_base_cost;
			for (int i = 0; i < sensornum.size(); i++)
			{
				Rada cur;
				int seq = sensornum[i];
				for (int i = 0; i < radas.size(); i++)
				{
					if (seq == radas[i].type)
					{
						cur = radas[i];
					}
				}

				APointPtr p1 = make_shared<APoint>(cur.x, cur.y, cur.z, 0, 0, 0, 0, 0);
				float d = Calc_dist(Par_Pos, p1);

				surv += sur_k / pow(d, 2);
			}

		}
		time = 0;
		surv = A_STAR.survice_w * surv;
		if (sensornum.size() != 0)
		{
			time = 1.2 * (lastPB->TIME_COST + A_STAR.StepLength / 10.0f);
			surv += time;
		}


		time_surv.push_back(time);
		time_surv.push_back(surv);


		return time_surv;
	}
	APointPtr buble_poits(VAPointPtr p)
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
	void saveresult(A_STAR& A_STAR)
	{
		APointPtr temp = A_STAR.close.back();
		int index = temp->TYPE;
		A_STAR.result_point.push_back(temp);
		A_STAR.result_point.push_back(A_STAR.close.at(index));

		while (index)
		{
			index = A_STAR.close.at(index)->TYPE - 1;
			A_STAR.result_point.push_back(A_STAR.close.at(index));

		}

	}
	bool removeone(VAPointPtr& open, APointPtr selectPoint)
	{
		for (int i = 0; i < open.size(); i++)
		{
			if (selectPoint == open[i])
			{
				open.erase(open.begin() + i);
				return true;
			}
		}
		return false;
	}

	VAPointPtr searchPoints(APointPtr lastPB, size_t parentIndex, APointPtr endpos, size_t flag, A_STAR& A_STAR)
	{

		APointPtr lastPA = A_STAR.close.at(lastPB->TYPE - 1);
		float l = A_STAR.StepLength / cos(A_STAR.horizontal_corner);

		float norm_ab = Calc_dist(lastPB, lastPA);
		float pa_x = lastPA->X;
		float pa_y = lastPA->Y;
		float pb_x = lastPB->X;
		float pb_y = lastPB->Y;
		A_STAR.result.clear();
		pa_x = float(int(pa_x * 100) / 100.0f);
		pb_x = float(int(pb_x * 100) / 100.0f);
		if (pa_x != pb_x)
		{
			//解方程
			/*float c = cos(A_STAR.horizontal_corner)*norm_ab*StepLength;
			float d = (c + (pb_y - pa_y)*pb_y + (pb_x - pa_x)*pb_x)/((pb_x - pa_x));
			float m = (pb_y - pa_y)*(pb_y - pa_y) + 1;
			float n = 2 * (pb_y - pa_y)*(d - pb_x) + 2 * pb_y;
			float k = pb_y*pb_y + (d - pb_x)*(d - pb_x) - StepLength*StepLength;

			float y = 0;

			float y1 =  (n + sqrt(n*n - 4 * m*k) )/2*m;
			float y2 = (n - sqrt(n*n - 4 * m*k)) / 2 * m;
			float x1 = d - y1*(pb_y - pa_y)/((pb_x - pa_x));
			float x2 = d - y2*(pb_y - pa_y)/ (pb_x - pa_x);

			A_STAR.result.push_back(make_pair(int(x1), int(y1)));
			A_STAR.result.push_back(make_pair(int(x2), int(y2)));*/
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
				fun_f = int(norm_ab) * cos(A_STAR.horizontal_corner) * A_STAR.StepLength - int((pb_x - pa_x) * (x1_0 - pb_x)) - int((pb_y - pa_y) * (x2_0 - pb_y));
				fun_g = int((x1_0 - pb_x) * (x1_0 - pb_x)) + int((x2_0 - pb_y) * (x2_0 - pb_y)) - int(A_STAR.StepLength * A_STAR.StepLength);
				//偏导数
				f_x1 = (-(x1_0) * (pb_x - pa_x));
				f_x2 = (-(x2_0) * (pb_y - pa_y));
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

					if (A_STAR.result.empty())
						A_STAR.result.push_back(make_pair(int(x1_0), int(x2_0)));
					//A_STAR.result.push_back(make_pair(x1_0+StepLength, x2_0+StepLength));
					else
					{
						if (100 == A_STAR.result.size())
							break;

						if (int(A_STAR.result.back().first) != int(x1_0) && int(A_STAR.result.back().second) != int(x2_0))
							A_STAR.result.push_back(make_pair(int(x1_0), int(x2_0)));
					}
				}
			}
			//debug 使用
			int a = 10;
			//cout << a;
		}
		else
		{
			float x = A_STAR.StepLength * sin(A_STAR.horizontal_corner);
			float y = A_STAR.StepLength * cos(A_STAR.horizontal_corner);
			if (pa_y < pb_y)
				y = pb_y + y;
			else
				y = pb_y - y;
			A_STAR.result.push_back(make_pair(int(pb_x - x), int(y)));
			A_STAR.result.push_back(make_pair(int(pb_x + x), int(y)));
		}
		if (A_STAR.result.size() < 2)
		{
			float x = A_STAR.StepLength * sin(A_STAR.horizontal_corner);
			float y = A_STAR.StepLength * cos(A_STAR.horizontal_corner);
			if (pa_y < pb_y)
				y = pb_y + y;
			else
				y = pb_y - y;
			A_STAR.result.push_back(make_pair(int(pb_x - x), int(y)));
			A_STAR.result.push_back(make_pair(int(pb_x + x), int(y)));
		}


		vector<vector<float>> points;
		vector<float> cur_point;
		vector<float> C;
		//计算水平面上的直线点
		for (int i = 0; i < A_STAR.result.size(); i++)
		{
			cur_point.clear();
			cur_point.push_back(A_STAR.result[i].first);
			cur_point.push_back(A_STAR.result[i].second);
			cur_point.push_back(lastPB->Z);
			cur_point.push_back(l);
			points.push_back(cur_point);
		}



		C.clear();
		//C << (lastPB->X - lastPA->X + pb_x) << (lastPB->Y - lastPA->Y + pb_y) << lastPB->Z << StepLength;
		C.push_back((lastPB->X - lastPA->X + pb_x));
		C.push_back((lastPB->Y - lastPA->Y + pb_y));
		C.push_back(lastPB->Z);
		C.push_back(A_STAR.StepLength);
		points.push_back(C);
		//计算不转弯情况下的爬升点
		l = A_STAR.StepLength / cos(A_STAR.verticality_corner);
		float z_top = float(int((l * tan(A_STAR.verticality_corner) + lastPB->Z) * 100) / 100.0f);
		//偏转情况步长
		for (int i = 0; i < 2; i++)
		{
			float step = sqrt((points[i][3 - 1] - z_top) * (points[i][3 - 1] - z_top) + points[i][4 - 1] * points[i][4 - 1]);
			vector<float> p_new;
			//p_new << points[i][1 - 1] << points[i][2 - 1] << z_top << step;
			p_new.push_back(points[i][1 - 1]);
			p_new.push_back(points[i][2 - 1]);
			p_new.push_back(z_top);
			p_new.push_back(step);
			points.push_back(p_new);

		}
		C.clear();
		//C << points[3 - 1][1 - 1] << points[3 - 1][2 - 1] << z_top << l;
		C.push_back(points[3 - 1][1 - 1]);
		C.push_back(points[3 - 1][2 - 1]);
		C.push_back(z_top);
		C.push_back(l);
		points.push_back(C);
		//计算下降的三个点
		float z_bottom = float(int((l * tan(-A_STAR.verticality_corner) + lastPB->Z) * 100) / 100.0f);
		for (int i = 0; i < 2; i++)
		{
			float step = sqrt((points[i][3 - 1] - z_bottom) * (points[i][3 - 1] - z_bottom) + points[i][4 - 1] * points[i][4 - 1]);
			vector<float> p_new;
			//p_new << points[i][1 - 1] << points[i][2 - 1] << z_bottom << step;
			p_new.push_back(points[i][1 - 1]);
			p_new.push_back(points[i][2 - 1]);
			p_new.push_back(z_bottom);
			p_new.push_back(step);
			points.push_back(p_new);
		}
		vector<float> p_new;
		//p_new << points[2][1 - 1] << points[2][2 - 1] << z_bottom << l;
		p_new.push_back(points[2][1 - 1]);
		p_new.push_back(points[2][2 - 1]);
		p_new.push_back(z_bottom);
		p_new.push_back(l);
		points.push_back(p_new);

		//筛选掉高度小于0.1的店和超过外界的点
		int index = 0;
		VAPointPtr vec_points;

		for (; index < points.size(); index++)
		{
			if (index < 0)
				index = 0;
			if (points[index][1 - 1] <= A_STAR.width[0] || points[index][1 - 1] >= A_STAR.width[9] || points[index][2 - 1] <= A_STAR.depth[0] || points[index][2 - 1] >= A_STAR.depth[6])
			{
				//points.removeAt(index);
				points.erase(points.begin() + index);
				index--;
				continue;
			}
			int tempx = ceil(points[index][0]);
			int tempy = ceil(points[index][1]);
			if (points[index][3 - 1]<A_STAR.zcubic[tempy][tempx] + A_STAR.h_min || points[index][3 - 1]>A_STAR.zcubic[tempy][tempx] + A_STAR.h_max)
			{
				//points.removeAt(index);
				points.erase(points.begin() + index);
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
					//points.remove(i);
					points.erase(points.begin() + i);
					i--;
					break;
				}

			}

		}



		//针对筛选后得到的点加入代价信息
		for (int i = 0; i < points.size(); i++)
		{
			APointPtr Pot = make_shared<APoint>(points[i][0], points[i][1], points[i][2], 0, 0, 0, 0, 0);
			Pot->G = getcostG(A_STAR);
			Pot->F = getcostH(Pot, A_STAR, flag) + Pot->G + getcostheight(Pot, A_STAR.e_w);
			Pot->TYPE = parentIndex;
			vector<float> a = getCostS(Pot, lastPB, A_STAR);
			Pot->TIME_COST = a[0];
			Pot->S_COST = a[1];
			Pot->F += Pot->S_COST;
			vec_points.push_back(Pot);

		}
		//需要筛选一下有些点是在close表里，需要计算代价来判断是否更新该点还是从points中踢出来避免重复加入这个点
		for (int i = 0; i < A_STAR.close.size(); i++)
		{
			float exist_x = float(int((A_STAR.close[i]->X) * 100) / 100.0f);
			float exist_y = float(int((A_STAR.close[i]->Y) * 100) / 100.0f);
			float exist_z = float(int((A_STAR.close[i]->Z) * 100) / 100.0f);
			float exist_c = float(int((A_STAR.close[i]->F) * 100) / 100.0f);


			//需要判断任务状态是否相同

			for (int i = 0; i < vec_points.size(); i++)
			{
				if (float(int((vec_points[i]->X) * 100) / 100.0f) == exist_x && float(int((vec_points[i]->F) * 100) / 100.0f) == exist_y && float(int((vec_points[i]->Z) * 100) / 100.0f) == exist_z)
				{
					if (float(int((vec_points[i]->F) * 100) / 100.0f) < exist_c)
					{
						continue;
					}

					//vec_points.removeAt(i);
					vec_points.erase(vec_points.begin() + i);
					i--;

				}

			}

		}

		//筛选一下有些点是在open表中的，需要决定是否更新还是踢出来
		for (int i = 0; i < A_STAR.open.size(); i++)
		{
			float exist_x = float(int((A_STAR.open[i]->X) * 100) / 100.0f);
			float exist_y = float(int((A_STAR.open[i]->Y) * 100) / 100.0f);
			float exist_z = float(int((A_STAR.open[i]->Z) * 100) / 100.0f);
			float exist_c = float(int((A_STAR.open[i]->F) * 100) / 100.0f);
			int j = 0;
			while (j < vec_points.size())
			{
				if (float(int((vec_points[j]->X) * 100) / 100.0f) == exist_x && float(int((vec_points[j]->Y) * 100) / 100.0f) == exist_y && float(int((vec_points[j]->Z) * 100) / 100.0f) == exist_z)
				{
					if (float(int((vec_points[j]->F) * 100) / 100.0f) < exist_c)
					{

						A_STAR.open[i] = vec_points[j];

					}
					//vec_points.removeAt(j);
					vec_points.erase(vec_points.begin() + j);
					j--;

				}
				j++;
			}


		}



		return vec_points;



	}
}