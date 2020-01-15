#include "esmDwellSequenceGen.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Parameters   {
	//Initialize the vectors to store the data of the Parameters used in calculating esmDwellSequence;
	private: 
	//Parameters saved as a single data
	double esmStartFreq_data_entry =  500e6;
	double esmEndFreq_data_entry =  18000e6;
	double esmStepFreq_data_entry =  50e6;
	double esmInstBand_data_entry =  400e6;
	double esmMinDwellTime_data_entry =  10e-3;
	int nEmitter_data_entry = 2;

	//Parameters saved as vectors for further transformation as arrays
	vector<double> freqStart_data_entry = { 4500e6, 500e6 };
	vector<double> freqEnd_data_entry = { 5800e6, 4000e6 };
	vector<double> tScan_data_entry = { 2, 2 };
	vector<double> taoScan_data_entry = { 0.1, 0.1 };
	vector<double> taoDwell_data_entry = { 50e-3, 50e-3 };
	vector<double> pInterceptDesired_data_entry = { 0.78, 0.95 };
	vector<double> tInterceptDesired_data_entry = { 5, 5 };

	public:
		//get values from single parameters
		double getesmStartFreq_data_entry(void) {
			double p = esmStartFreq_data_entry;
			return p;
		}
		double getesmEndFreq_data_entry(void) {
			double p = esmEndFreq_data_entry;
			return p;
		}
		double getesmStepFreq_data_entry(void) {
			double p = esmStepFreq_data_entry;
			return p;
		}
		double getesmInstBand_data_entry(void) {
			double p = esmInstBand_data_entry;
			return p;
		}
		double getesmMinDwellTime_data_entry(void) {
			double p = esmMinDwellTime_data_entry;
			return p;
		}
		double getnEmitter_data_entry(void) {
			return nEmitter_data_entry;
		}
		//Convert vectors to Arrays
		double* freqStart2arr(void)
		{	
			double* p = freqStart_data_entry.data();
			return p;
		}
		double* freqEnd2arr(void)
		{	
			double* p = freqEnd_data_entry.data();
			return p;	
		}
		double* tScan2arr(void)
		{	
			double* p = tScan_data_entry.data();
			return p;
		}
		double* taoScan2arr(void)
		{	
			double* p = taoScan_data_entry.data();
			return p;
		}
		double* taoDwell2arr(void)
		{
			double* p = taoDwell_data_entry.data();
			return p;
		}
		double* pInterceptDesired2arr(void)
		{	
			double* p = pInterceptDesired_data_entry.data();
			return p;
		}
		double* tInterceptDesired2arr(void)
		{	
			double* p = tInterceptDesired_data_entry.data();
			return p;
		}

};

int main()
{
	Parameters entry;
	mclmcrInitialize();
	if (!mclInitializeApplication(NULL, 0)) return -1;
	if (!esmDwellSequenceGenInitialize()) return -1;

	mwArray esmStartFreq(1, 1, mxDOUBLE_CLASS, mxREAL);
	mwArray esmEndFreq(1, 1, mxDOUBLE_CLASS, mxREAL);
	mwArray esmStepFreq(1, 1, mxDOUBLE_CLASS, mxREAL);
	mwArray esmInstBand(1, 1, mxDOUBLE_CLASS, mxREAL);
	mwArray esmMinDwellTime(1, 1, mxDOUBLE_CLASS, mxREAL);

	mwArray nEmitter(1, 1, mxDOUBLE_CLASS, mxREAL);
	mwArray freqStart(1, 2, mxDOUBLE_CLASS, mxREAL);
	mwArray freqEnd(1, 2, mxDOUBLE_CLASS, mxREAL);
	mwArray tScan(1, 2, mxDOUBLE_CLASS, mxREAL);
	mwArray taoScan(1, 2, mxDOUBLE_CLASS, mxREAL);
	mwArray taoDwell(1, 2, mxDOUBLE_CLASS, mxREAL);
	mwArray pInterceptDesired(1, 2, mxDOUBLE_CLASS, mxREAL);
	mwArray tInterceptDesired(1, 2, mxDOUBLE_CLASS, mxREAL);

	double esmStartFreq_data[1] = { entry.getesmStartFreq_data_entry()};
	double esmEndFreq_data[1] = { entry.getesmEndFreq_data_entry() };
	double esmStepFreq_data[1] = { entry.getesmStepFreq_data_entry() };
	double esmInstBand_data[1] = { entry.getesmInstBand_data_entry() };
	double esmMinDwellTime_data[1] = { entry.getesmMinDwellTime_data_entry() };

	esmStartFreq.SetData(esmStartFreq_data, 1);
	esmEndFreq.SetData(esmEndFreq_data, 1);
	esmStepFreq.SetData(esmStepFreq_data, 1);
	esmInstBand.SetData(esmInstBand_data, 1);
	esmMinDwellTime.SetData(esmMinDwellTime_data, 1);
	double num = entry.getnEmitter_data_entry();
	double nEmitter_data[1] = { entry.getnEmitter_data_entry() };
	nEmitter.SetData(nEmitter_data, 1);

	freqStart.SetData(entry.freqStart2arr(), num);
	freqEnd.SetData(entry.freqEnd2arr(), num);
	tScan.SetData(entry.tScan2arr(), num);
	taoScan.SetData(entry.taoScan2arr(), num);
	taoDwell.SetData(entry.taoDwell2arr(), num);
	pInterceptDesired.SetData(entry.pInterceptDesired2arr(), num);
	tInterceptDesired.SetData(entry.tInterceptDesired2arr(), num);

	mwArray dwellSequence;
	mwArray pIntercept;

	esmDwellSequenceGen(2, dwellSequence, pIntercept, esmStartFreq, esmEndFreq, esmStepFreq, esmInstBand, esmMinDwellTime,
		nEmitter, freqStart, freqEnd, tScan, taoScan, taoDwell, pInterceptDesired, tInterceptDesired);

	mwArray dims = dwellSequence.GetDimensions();
	int row = dims.Get(1, 1);
	int col = dims.Get(1, 2);

	double *dwellSequence_pt;
	dwellSequence_pt = new double[row*col];
	dwellSequence.GetData(dwellSequence_pt, row*col);

	std::cout << "The value of dwellSequence matrix is:" << std::endl;

	for (int i = 0; i < row; i++)
	{
		printf("%.4f  %.4f  %.4e  %.4e\n", dwellSequence_pt[i], dwellSequence_pt[row + i],
			dwellSequence_pt[2 * row + i], dwellSequence_pt[3 * row + i]);
	}

	delete dwellSequence_pt;

	esmDwellSequenceGenTerminate();
	mclTerminateApplication();

	system("pause");
	return 0;
}