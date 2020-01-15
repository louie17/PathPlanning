#include "spjaSystem.h"
#include <vector>

using namespace std;





class parameter {
	public:
	parameter(const vector<double >& v1, const vector<double >& v2, const vector<double >& v3) :radarIndex_data_entry(v1), radarERP_data_entry(v2), radarDangerValue_data_entry(v3) {};
	public:
		vector<double> radarIndex_data_entry; // = { 1, 2, 3, 4, 5, 6 };
		vector<double> radarERP_data_entry ;//{ 10e3, 15e3, 30e3, 20e3, 15e3, 15e3 };
		vector<double> radarDangerValue_data_entry;//= { 2, 1, 6, 3, 5, 4 };
		vector<double> lutTecEffect_data = { 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6, 15, 8, 5, 8, 7, 12, 1, 1, 1, 1, 1, 1 };
		double jammerChannel_data =  3 ;
		double jammerERP_data =  1e3 ;
		double platformRCS_data = 10;
		double index;

	public:
		double setIndex(void) {
			index = radarIndex_data_entry.size();
			return 0;
		}
		double getIndex(void) {
			return index;
		}
		double getjammerChannel_data(void){
			double p = jammerChannel_data;
			return p;
		}
		double getjammerERP_data(void) {
			double p = jammerERP_data;
			return p;
		}
		double getplatformRCS_data(void){
			double p = platformRCS_data;
			return p;
		}
		//get pointer of the vectors
		double* getlutTecEffect_data(void) //in total 24 numbers
		{
			double* p = lutTecEffect_data.data();
			return p;
		}
		double* getradarIndex_data_entry(void) {
			double* p = radarIndex_data_entry.data();
			return p;
		}
		double* getradarERP_data_entry(void) {
			double* p = radarERP_data_entry.data();
			return p;
		}
		double* getradarDangerValue_data_entry(void) {
			double* p = radarDangerValue_data_entry.data();
			return p;
		}



};
int main()
{	
	//vector<double> v1,v2,v3;

	//parameter spj(v1,v2,v3);//initialize spj parameter entity
	//mclmcrInitialize();
	//if (!mclInitializeApplication(NULL, 0)) return -1;
	//if (!spjaSystemInitialize()) return -1;

	//spj.setIndex();//acquire the number of radars
	//double num = spj.getIndex();
	//mwArray radarIndex(1, num, mxDOUBLE_CLASS, mxREAL);
	//mwArray radarERP(1, num, mxDOUBLE_CLASS, mxREAL);
	//mwArray radarDangerValue(1, num, mxDOUBLE_CLASS, mxREAL);
	//mwArray jammerChannel(1, 1, mxDOUBLE_CLASS, mxREAL);
	//mwArray jammerERP(1, 1, mxDOUBLE_CLASS, mxREAL);
	//mwArray platformRCS(1, 1, mxDOUBLE_CLASS, mxREAL);
	//mwArray lutTecEffect(6, 4, mxDOUBLE_CLASS, mxREAL);

	//double jammerChannel_data[1] = { spj.getjammerChannel_data() };
	//double jammerERP_data[1] = { spj.getjammerERP_data() };
	//double platformRCS_data[1] = { spj.getplatformRCS_data() };

	//radarIndex.SetData(spj.getradarIndex_data_entry(), num);
	//radarERP.SetData(spj.getradarERP_data_entry(), num);
	//radarDangerValue.SetData(spj.getradarDangerValue_data_entry(), num);
	//jammerChannel.SetData(jammerChannel_data, 1);
	//jammerERP.SetData(jammerERP_data, 1);
	//platformRCS.SetData(platformRCS_data, 1);
	//lutTecEffect.SetData(spj.getlutTecEffect_data(), 24);

	//mwArray jammingAllocation;

	//spjaSystem(1, jammingAllocation, radarIndex, radarERP, radarDangerValue, jammerChannel, jammerERP, platformRCS, lutTecEffect);

	//mwArray dims = jammingAllocation.GetDimensions();
	//int row = dims.Get(1, 1);
	//int col = dims.Get(1, 2);

	//double* jammingAllocation_pt;
	//jammingAllocation_pt = new double[row * col];
	//jammingAllocation.GetData(jammingAllocation_pt, row * col);

	//std::cout << "The value of jammingAllocation matrix is:" << std::endl;

	//std::cout << "被干扰的雷达工作模式	采用的干扰技术 	烧穿距离"<< std::endl;

	//for (int i = 0; i < col; i++)
	//{
	//	printf("%f  %f  %.2e\n", jammingAllocation_pt[i * row + 0], jammingAllocation_pt[i * row + 1],
	//		jammingAllocation_pt[i * row + 2]);
	//}

	//delete jammingAllocation_pt;

	//spjaSystemTerminate();
	//mclTerminateApplication();
	//system("pause");


	return 0;
}
vector<double*> init(const vector<double >& radarIndex_data_entry, const vector<double >& radarERP_data_entry, const vector<double >& radarDangerValue_data_entry)
{
	parameter spj(radarIndex_data_entry, radarERP_data_entry, radarDangerValue_data_entry);
	mclmcrInitialize();
	if (!mclInitializeApplication(NULL, 0)) return -1;
	if (!spjaSystemInitialize()) return -1;

	spj.setIndex();//acquire the number of radars
	double num = spj.getIndex();
	mwArray radarIndex(1, num, mxDOUBLE_CLASS, mxREAL);
	mwArray radarERP(1, num, mxDOUBLE_CLASS, mxREAL);
	mwArray radarDangerValue(1, num, mxDOUBLE_CLASS, mxREAL);
	mwArray jammerChannel(1, 1, mxDOUBLE_CLASS, mxREAL);
	mwArray jammerERP(1, 1, mxDOUBLE_CLASS, mxREAL);
	mwArray platformRCS(1, 1, mxDOUBLE_CLASS, mxREAL);
	mwArray lutTecEffect(6, 4, mxDOUBLE_CLASS, mxREAL);

	double jammerChannel_data[1] = { spj.getjammerChannel_data() };
	double jammerERP_data[1] = { spj.getjammerERP_data() };
	double platformRCS_data[1] = { spj.getplatformRCS_data() };

	radarIndex.SetData(spj.getradarIndex_data_entry(), num);
	radarERP.SetData(spj.getradarERP_data_entry(), num);
	radarDangerValue.SetData(spj.getradarDangerValue_data_entry(), num);
	jammerChannel.SetData(jammerChannel_data, 1);
	jammerERP.SetData(jammerERP_data, 1);
	platformRCS.SetData(platformRCS_data, 1);
	lutTecEffect.SetData(spj.getlutTecEffect_data(), 24);

	mwArray jammingAllocation;

	spjaSystem(1, jammingAllocation, radarIndex, radarERP, radarDangerValue, jammerChannel, jammerERP, platformRCS, lutTecEffect);

	mwArray dims = jammingAllocation.GetDimensions();
	int row = dims.Get(1, 1);
	int col = dims.Get(1, 2);

	double* jammingAllocation_pt;
	jammingAllocation_pt = new double[row * col];
	jammingAllocation.GetData(jammingAllocation_pt, row * col);

	std::cout << "The value of jammingAllocation matrix is:" << std::endl;

	std::cout << "被干扰的雷达工作模式	采用的干扰技术 	烧穿距离" << std::endl;
	vector<double*> jamming_distance;
	for (int i = 0; i < col; i++)
	{
		
		jamming_distance.push_back(&jammingAllocation_pt[i * row + 2]);
		printf("%f  %f  %.2e\n", jammingAllocation_pt[i * row + 0], jammingAllocation_pt[i * row + 1],
			jammingAllocation_pt[i * row + 2]);
	}

	delete jammingAllocation_pt;

	spjaSystemTerminate();
	mclTerminateApplication();
	return jamming_distance;

}
