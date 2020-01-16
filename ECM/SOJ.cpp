#include "sojaSystem.h"
#include <vector>

using namespace std;

class parameter {
	private:
		vector<double> radarIndex_data_entry = { 1, 2, 3, 4, 5, 6 };
		vector<double> radarERP_data_entry = { 10e3, 15e3, 30e3, 20e3, 15e3, 15e3 };
		vector<double> radarMSR_data = { 15, 15, 15, 15, 15, 15 };
		vector<double> lutTecEffect_data = { 1, 2, 3, 4, 5, 6, 1, 1, 1, 1, 1, 1, 15, 8, 5, 8, 7, 12, 1, 1, 1, 1, 1, 1 };
		vector<double> radarSojRange_data= { 100e3, 100e3, 100e3, 100e3, 100e3, 100e3 };
		double jammerERP_data =  200e3 ;
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
		double getjammerERP_data(void) {
			double p = jammerERP_data;
			return p;
		}
		double getplatformRCS_data(void) {
			double p = platformRCS_data;
			return p;
		}
		//get pointer of vectors
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
		double* getradarMSR_data_entry(void) {
			double* p = radarMSR_data.data();
			return p;
		}
		double* getradarSojRange_data_entry(void) {
			double* p = radarSojRange_data.data();
			return p;
		}
};

int main()
{	
	parameter soj;//initialize soj parameter entity
	mclmcrInitialize();
	if (!mclInitializeApplication(NULL, 0)) return -1;
	if (!sojaSystemInitialize()) return -1;

	soj.setIndex();//acquire the number of radars
	double num = soj.getIndex();
	mwArray radarIndex(1, 6, mxDOUBLE_CLASS, mxREAL);
	mwArray radarERP(1, 6, mxDOUBLE_CLASS, mxREAL);
	mwArray radarMSR(1, 6, mxDOUBLE_CLASS, mxREAL);
	mwArray radarSojRange(1, 6, mxDOUBLE_CLASS, mxREAL);
	mwArray jammerERP(1, 1, mxDOUBLE_CLASS, mxREAL);
	mwArray platformRCS(1, 1, mxDOUBLE_CLASS, mxREAL);
	mwArray lutTecEffect(6, 4, mxDOUBLE_CLASS, mxREAL);

	double jammerERP_data[1] = { soj.getjammerERP_data() };
	double platformRCS_data[1] = { soj.getplatformRCS_data() };
	
	radarIndex.SetData(soj.getradarIndex_data_entry(), num);
	radarERP.SetData(soj.getradarERP_data_entry(), num);
	radarMSR.SetData(soj.getradarMSR_data_entry(), num);
	radarSojRange.SetData(soj.getradarSojRange_data_entry(), num);
	jammerERP.SetData(jammerERP_data, 1);
	platformRCS.SetData(platformRCS_data, 1);
	lutTecEffect.SetData(soj.getlutTecEffect_data(), 24);

	mwArray jammingAllocation;

	sojaSystem(1, jammingAllocation, radarIndex, radarERP, radarMSR, radarSojRange, jammerERP, platformRCS, lutTecEffect);

	mwArray dims = jammingAllocation.GetDimensions();
	int row = dims.Get(1, 1);
	int col = dims.Get(1, 2);

	double *jammingAllocation_pt;
	jammingAllocation_pt = new double[row*col];
	jammingAllocation.GetData(jammingAllocation_pt, row*col);

	std::cout << "The value of jammingAllocation matrix is:" << std::endl;

	printf("被干扰的雷达工作模式	采用的干扰技术 	烧穿距离\n");


	for (int i = 0; i < col; i++)
	{
		printf("%f  %f  %.2e\n", jammingAllocation_pt[i*row + 0], jammingAllocation_pt[i*row + 1],
			jammingAllocation_pt[i*row + 2]);
	}

	delete jammingAllocation_pt;

	sojaSystemTerminate();
	mclTerminateApplication();

	system("pause");
	return 0;
}