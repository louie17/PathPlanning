/****************************************************************
*	Copyright(c)2019-2020 四川大学电子信息学院智能控制研究所
*	All right reserved.
*
*	文件名称：Scenario.cpp
*	功能：提供XML解析数据封装类实现程序
*	当前版本：1.0
*	作者：韩露
*	完成日期：2019年11月3日
******************************************************************/

#include "Scenario.hpp"
#include<iostream>
#include<cassert>

namespace sce
{
	/***************************Vertex***********************************/
	Vertex::Vertex(void)
		:m_latitude(0.0), m_longitude(0.0), m_altitude(0.0)
	{
	}

	Vertex::Vertex(const double &la, const double &lo)
		: m_latitude(la), m_longitude(lo), m_altitude(0.0)
	{

	}

	Vertex::Vertex(const double &latitude , const double &longitude, const double &altitude)
		: m_latitude(latitude), m_longitude(longitude), m_altitude(altitude)
	{

	}

	const double& Vertex::getLatitude(void) const
	{
		return m_latitude;
	}

	const double& Vertex::getLongitude(void) const
	{
		return m_longitude;
	}

	const double & Vertex::getAltitude(void) const
	{
		return m_altitude;
	}

	void Vertex::setLatitude(const double & la)
	{
		m_latitude = la;
	}

	void Vertex::setLongitude(const double & lo)
	{
		m_longitude = lo;
	}

	void Vertex::setAltitude(const double &altitude)
	{
		m_altitude = altitude;
	}

	/***************************Platform***********************************/
	Platform::Platform(void)
		:m_name("Platform1"), m_type(PlatformType::LAND)
	{
	}

	Platform::Platform(const std::string &pname, const PlatformType &type)
		: m_name(pname), m_type(type)
	{

	}

	inline const std::string& Platform::getPlatformName(void) const
	{
		return m_name;
	}
	const PlatformType & Platform::getPlatformType(void) const
	{
		return m_type;
	}
	void Platform::setPlatformName(const std::string & name)
	{
		m_name = name;
	}
	void Platform::setPlatformType(const PlatformType & type)
	{
		m_type = type;
	}

	/***************************Rf_values***********************************/
	Rf_values::Rf_values(void)
		: m_phaseOrder(1), m_numberOfPulses(0), m_min(90), m_max(90), m_sweepTime(0)
	{
	}

	Rf_values::Rf_values(const size_t &pOrder, const size_t &numOfPulses, const size_t &min, const size_t &max,	const size_t &sweepTime)
		: m_phaseOrder(pOrder), m_numberOfPulses(numOfPulses), m_min(min), m_max(max), m_sweepTime(sweepTime)
	{
	}

	const size_t & Rf_values::getPhaseOrder(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_phaseOrder;
	}
	const size_t & Rf_values::getNumberOfPulses(void) const
	{
		return m_numberOfPulses;
	}
	const size_t & Rf_values::getMin(void) const
	{
		return m_min;
	}
	const size_t & Rf_values::getMax(void) const
	{
		return m_max;
	}
	const size_t & Rf_values::getSweepTime(void) const
	{
		return m_sweepTime;
	}

	void Rf_values::setPhaseOrder(const size_t & pOrder)
	{
		m_phaseOrder = pOrder;
	}
	void Rf_values::setNumberOfPulses(const size_t & numOfPulses)
	{
		m_numberOfPulses = numOfPulses;
	}
	void Rf_values::setMin(const size_t & min)
	{
		m_min = min;
	}
	void Rf_values::setMax(const size_t & max)
	{
		m_max = max;
	}
	void Rf_values::setSweepTime(const size_t & sweepTime)
	{
		m_sweepTime = sweepTime;
	}

	/************************Rf**********************************/
	//由于默认构造函数尚未进行Values值初始化，所以调用默认构造后应尽快添加Values值
	Rf::Rf(void)
		: m_min(9000), m_max(10000), m_type(RfType::BURST)
	{
	}

	Rf::Rf(const size_t &rfmin, const size_t &rfmax)
		: m_min(rfmin), m_max(rfmax), m_type(RfType::BURST)
	{
	}

	//由于默认构造函数尚未进行Values值初始化，所以调用默认构造后应尽快添加Values值
	Rf::Rf(const size_t &rfmin,	const size_t &rfmax, const RfType &rftype)
		: m_min(rfmin), m_max(rfmax), m_type(rftype)
	{
	}

	Rf::Rf(const size_t &rfmin,	const size_t &rfmax, const Rf_values & rfvalues, const RfType &rftype)
		: m_min(rfmin), m_max(rfmax), m_type(rftype), m_values({ rfvalues })/*function({ arg1, arg2,...})复制列表初始化，以花括号初始化器列表为实参，以列表初始化对函数形参初始化*/
	{
	}

	Rf::Rf(const size_t &rfmin,	const size_t &rfmax, const std::vector<Rf_values>& rfvalues, const RfType &rftype)
		: m_min(rfmin), m_max(rfmax), m_type(rftype), m_values(rfvalues)
	{
	}

	//	Rf::~Rf()
	//	{
	//		/*std::vector<Rf_values *>::iterator it;
	//		for (it = _rfValues.begin(); it != _rfValues.end(); it++)
	//		{
	//			delete *it;
	//			*it = nullptr;
	//		}*/
	//	}

	const size_t & Rf::getRfMin(void) const
	{
		return m_min;
	}

	const size_t & Rf::getRfMax(void) const
	{
		return m_max;
	}

	const RfType & Rf::getRfType(void) const
	{
		return m_type;
	}

	std::vector<Rf_values>& Rf::getAllRfValues(void)
	{
		// TODO: 在此处插入 return 语句
		return m_values;
	}

	bool Rf::isValuesEmpty()
	{
		return m_values.empty() ? true : false;
	}

	Rf_values & Rf::getRfValue(const size_t& valueOrder)
	{
		assert(valueOrder < m_values.size());
		if (valueOrder<m_values.size())
		{
			return m_values[valueOrder];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	void Rf::addRfValues(const Rf_values & rfvalues)
	{
		m_values.push_back(rfvalues);
	}

	bool Rf::setRfValues(const size_t & pos, const Rf_values &rfValues)
	{
		assert(pos < m_values.size());
		if (pos < m_values.size())
		{
			m_values[pos] = rfValues;
			return true;
		}
		return false;
	}

	bool Rf::insertRfValues(const size_t& pos, const Rf_values & rfvalues)
	{
		assert(pos <= m_values.size());
		if (pos <= m_values.size())
		{
			m_values.insert(m_values.begin() + pos, rfvalues);
			return true;
		}
		return false;
	}

	bool Rf::deleteRfValues(const size_t& pos)
	{
		assert(pos < m_values.size());
		if (pos<m_values.size())
		{
			//delete *(_rfValues.begin() + pos);
			m_values.erase(m_values.begin() + pos);
			return true;
		}
		return false;
	}

	void Rf::setRfMin(const size_t & rfmin)
	{
		m_min = rfmin;
	}

	void Rf::setRfMax(const size_t & rfmax)
	{
		m_max = rfmax;
	}

	void Rf::setRfType(const RfType & rftype)
	{
		m_type = rftype;
	}

	void Rf::setAllRfValues(const std::vector<Rf_values>&rfValues)
	{
		m_values = rfValues;
	}

	//inline const Rf_values& Rf::operator[](size_t valuesOrder) const
	//{
	//	// TODO: 在此处插入 return 语句
	//	return Rf::getRfValues(valuesOrder);
	//}

	/**************************Pw_Values*************************************/
	Pw_values::Pw_values(void)
		: m_phaseOrder(0), m_numberOfPulses(0), m_min(20), m_max(20), m_sweepTime(0)
	{
	}

	Pw_values::Pw_values(const size_t &pOrder, const size_t &numOfPulses, const size_t &min, const size_t &max, const size_t &sweepTime)
		: m_phaseOrder(pOrder), m_numberOfPulses(numOfPulses), m_min(min), m_max(max), m_sweepTime(sweepTime)
	{
	}

	//	Pw_values::~Pw_values(void)
	//	{
	//	}

	const size_t & Pw_values::getPhaseOrder(void) const
	{
		return m_phaseOrder;
	}
	const size_t & Pw_values::getNumberOfPulses(void) const
	{
		return m_numberOfPulses;
	}
	const size_t & Pw_values::getMin(void) const
	{
		return m_min;
	}
	const size_t & Pw_values::getMax(void) const
	{
		return m_max;
	}
	const size_t & Pw_values::getSweepTime(void) const
	{
		return m_sweepTime;
	}
	void Pw_values::setPhaseOrder(const size_t & pOrder)
	{
		m_phaseOrder = pOrder;
	}
	void Pw_values::setNumberOfPulses(const size_t & numOfPulses)
	{
		m_numberOfPulses = numOfPulses;
	}
	void Pw_values::setMin(const size_t & min)
	{
		m_min = min;
	}
	void Pw_values::setMax(const size_t & max)
	{
		m_max = max;
	}
	void Pw_values::setSweepTime(const size_t & sweepTime)
	{
		m_sweepTime = sweepTime;
	}

	/***************************PW************************************/
	//由于默认构造函数尚未进行Values值初始化，所以调用默认构造后应尽快添加Values值
	Pw::Pw(void)
		:m_min(20), m_max(20), m_type(PwType::FIX)
	{
	}

	Pw::Pw(const size_t &pwmin, const size_t &pwmax)
		:m_min(pwmin),m_max(pwmax),m_type(PwType::FIX)
	{
	}
	//由于默认构造函数尚未进行Values值初始化，所以调用默认构造后应尽快添加Values值
	Pw::Pw(const size_t &pwmin,	const size_t &pwmax, const PwType &pwtype)
		:m_min(pwmin), m_max(pwmax), m_type(pwtype)
	{
	}

	Pw::Pw(const size_t &pwmin, const size_t &pwmax, const Pw_values &pwvalues,	const PwType &pwtype)
		:m_min(pwmin), m_max(pwmax), m_type(pwtype), m_values({ pwvalues })
	{
	}

	Pw::Pw(const size_t &pwmin,	const size_t &pwmax, const std::vector<Pw_values>&pwvalues,	const PwType &pwtype)
		:m_min(pwmin), m_max(pwmax), m_type(pwtype), m_values(pwvalues)
	{
		/*for (auto &it : pwvalues)
		{
		_pwValues.push_back(it);
		}*/
	}

	//	Pw::~Pw(void)
	//	{
	//		//_pwValues.clear();
	//		/*std::vector<std::shared_ptr<Pw_values>>::iterator it;
	//		for (it = _pwValues.begin(); it != _pwValues.end(); it++)
	//		{
	//			delete *it;
	//			*it = nullptr;
	//		}*/
	//	}

	const size_t & Pw::getPwMin(void) const
	{
		return m_min;
	}

	const size_t & Pw::getPwMax(void) const
	{
		return m_max;
	}

	const PwType & Pw::getPwType(void) const
	{
		return m_type;
	}

	std::vector<Pw_values>& Pw::getAllPwValues(void)
	{
		// TODO: 在此处插入 return 语句
		return m_values;
	}

	bool Pw::isValuesEmpty()
	{
		return m_values.empty() ? true : false;
	}

	Pw_values & Pw::getPwValue(size_t valueOrder)
	{
		assert(valueOrder < m_values.size());
		if (valueOrder<m_values.size())
		{
			//return *(_pwValues[valueOrder]);
			return m_values[valueOrder];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	void Pw::addPriValues(Pw_values & pwvalues)
	{
		m_values.push_back(pwvalues);
	}

	bool Pw::setPwValues(const size_t & pos, const Pw_values &pwValues)
	{
		assert(pos < m_values.size());
		if (pos < m_values.size())
		{
			m_values[pos] = pwValues;
			return true;
		}
		return false;
	}

	bool Pw::insertPwValues(size_t pos, Pw_values & pwvalues)
	{
		assert(pos <= m_values.size());
		if (pos <= m_values.size())
		{
			//_pwValues.insert(_pwValues.begin() + pos, &pwvalues);
			m_values.insert(m_values.begin() + pos, pwvalues);
			return true;
		}
		return false;
	}

	bool Pw::deletePwValues(size_t pos)
	{
		assert(pos < m_values.size());
		if (pos<m_values.size())
		{
			//delete *(_pwValues.begin() + pos);
			m_values.erase(m_values.begin() + pos);
			return true;
		}
		return false;
	}

	void Pw::setPwMin(const size_t & pwmin)
	{
		m_min = pwmin;
	}

	void Pw::setPwMax(const size_t & pwmax)
	{
		m_max = pwmax;
	}

	void Pw::setPwType(const PwType & pwtype)
	{
		m_type = pwtype;
	}

	void Pw::setAllPwValues(const std::vector<Pw_values>&pwValues)
	{
		m_values = pwValues;
	}


	//inline const Pw_values & Pw::operator[](size_t valuesOrder) const
	//{
	//	// TODO: 在此处插入 return 语句
	//	return Pw::getPwValues(valuesOrder);
	//}

	/***************************Pri_Values************************************/
	Pri_values::Pri_values(void)
		: m_phaseOrder(1) , m_numberOfPulses(0), m_min(90), m_max(90), m_sweepTime(0)
	{
	}

	Pri_values::Pri_values(const size_t &pOrder, const size_t &numOfPulses, const size_t &min, const size_t &max, const size_t &sweepTime)
		: m_phaseOrder(pOrder), m_numberOfPulses(numOfPulses), m_min(min), m_max(max), m_sweepTime(sweepTime)
	{
	}

	//	Pri_values::~Pri_values(void)
	//	{
	//	}
	const size_t & Pri_values::getPhaseOrder(void) const
	{
		return m_phaseOrder;
	}
	const size_t & Pri_values::getNumberOfPulses(void) const
	{
		return m_numberOfPulses;
	}
	const size_t & Pri_values::getMin(void) const
	{
		return m_min;
	}
	const size_t & Pri_values::getMax(void) const
	{
		return m_max;
	}
	const size_t & Pri_values::getSweepTime(void) const
	{
		return m_sweepTime;
	}
	void Pri_values::setPhaseOrder(const size_t & pOrder)
	{
		m_phaseOrder = pOrder;
	}
	void Pri_values::setNumberOfPulses(const size_t & numOfPulses)
	{
		m_numberOfPulses = numOfPulses;
	}
	void Pri_values::setMin(const size_t & min)
	{
		m_min = min;
	}
	void Pri_values::setMax(const size_t & max)
	{
		m_max = max;
	}
	void Pri_values::setSweepTime(const size_t & sweepTime)
	{
		m_sweepTime = sweepTime;
	}

	/***************************Pri************************************/
	//由于默认构造函数尚未进行Values值初始化，所以调用默认构造后应尽快添加Values值
	Pri::Pri(void) 
		:m_min(90), m_max(100), m_type(PriType::FIX)
	{
	}

	Pri::Pri(const size_t &primin, const size_t &primax)
		: m_min(primin), m_max(primax), m_type(PriType::FIX), m_values({ Pri_values() })
	{
	}

	//由于构造函数尚未进行Values值初始化，所以调用默认构造后应尽快添加Values值
	Pri::Pri(const size_t &primin, const size_t &primax, const PriType &pritype)
		: m_min(primin), m_max(primax), m_type(pritype)
	{
	}

	Pri::Pri(const size_t &primin, const size_t &primax, const Pri_values &privalues, const PriType & pritype)
		:m_min(primin), m_max(primax), m_type(pritype), m_values({ privalues })
	{
	}

	Pri::Pri(const size_t &primin, const size_t &primax, const std::vector<Pri_values> &privalues, const PriType & pritype)
		:m_min(primin), m_max(primax), m_type(pritype), m_values(privalues)
	{
	}

	//	Pri::~Pri(void)
	//	{
	//		/*std::vector<Pri_values *>::iterator it;
	//		for (it = _priValues.begin(); it != _priValues.end(); it++)
	//		{
	//			delete *it;
	//			*it = nullptr;
	//		}*/
	//	}
	const size_t & Pri::getPriMin(void) const
	{
		return m_min;
	}

	const size_t & Pri::getPriMax(void) const
	{
		return m_max;
	}

	const PriType & Pri::getPriType(void) const
	{
		return m_type;
	}

	std::vector<Pri_values>& Pri::getAllPriValues(void)
	{
		// TODO: 在此处插入 return 语句
		return m_values;
	}

	bool Pri::isValuesEmpty()
	{
		return m_values.empty() ? true : false;
	}

	Pri_values & Pri::getPriValue(size_t valueOrder)
	{
		assert(valueOrder<m_values.size());
		if (valueOrder<m_values.size())
		{
			//return *(_priValues[valueOrder]);
			return m_values[valueOrder];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	void Pri::setPriMin(const size_t & primin)
	{
		m_min = primin;
	}

	void Pri::setPriMax(const size_t & primax)
	{
		m_max = primax;
	}

	void Pri::setPriType(const PriType & pritype)
	{
		m_type = pritype;
	}

	void Pri::addPriValues(Pri_values & privalues)
	{
		m_values.push_back(privalues);
	}

	bool Pri::setPriValues(const size_t & pos, const Pri_values &priValues)
	{
		assert(pos < m_values.size());
		if (pos < m_values.size())
		{
			m_values[pos] = priValues;
			return true;
		}
		return false;
	}

	bool Pri::insertPriValues(size_t pos, Pri_values & privalues)
	{
		assert(pos > m_values.size());
		if (pos <= m_values.size())
		{
			m_values.insert(m_values.begin() + pos, privalues);
			return true;
		}
		return false;
	}

	bool Pri::deletePriValues(size_t pos)
	{
		assert(pos < m_values.size());
		if (pos<m_values.size())
		{
			//delete *(_priValues.begin() + pos);
			m_values.erase(m_values.begin() + pos);
			return true;
		}
		return false;
	}

	void Pri::setAllPriValues(const std::vector<Pri_values>&priValues)
	{
		m_values = priValues;
	}

	//inline const Pri_values & Pri::operator[](size_t valuesOrder) const
	//{
	//	// TODO: 在此处插入 return 语句
	//	return Pri::getPriValues(valuesOrder);
	//}

	/**************************Scan*************************************/
	Scan::Scan(void)
		: m_min(2), m_max(2), m_type(ScanType::SECTORIAL)
	{
	}

	Scan::Scan(const size_t &max, const size_t &min, const ScanType &type)
		: m_min(min), m_max(max), m_type(type)
	{
	}

	//	Scan::~Scan(void)
	//	{
	//	}

	const size_t & Scan::getScanMin(void) const
	{
		return m_min;
	}

	const size_t & Scan::getScanMax(void) const
	{
		return m_max;
	}

	const ScanType & Scan::getScanType(void) const
	{
		return m_type;
	}

	void Scan::setScanMin(const size_t & min)
	{
		m_min = min;
	}

	void Scan::setScanType(const ScanType & type)
	{
		m_type = type;
	}

	void Scan::setScanMax(const size_t & max)
	{
		m_max = max;
	}

	/**************************Erp*************************************/
	Erp::Erp(void)
		:m_min(50000), m_max(50000)
	{
	}

	Erp::Erp(const size_t &min,	const size_t &max)
		:m_min(min), m_max(max)
	{
	}

	//	Erp::~Erp(void)
	//	{
	//	}
	const size_t & Erp::getErpMin(void) const
	{
		return m_min;
	}
	const size_t & Erp::getErpMax(void) const
	{
		return m_max;
	}
	void Erp::setErpMin(const size_t & min)
	{
		m_min = min;
	}
	void Erp::setErpMax(const size_t & max)
	{
		m_max = max;
	}

	/**************************Radar_Mode**********************************/
	//由于默认构造函数尚未进行Values值初始化，所以调用默认构造后应尽快添加Values值
	Radar_Mode::Radar_Mode(void)
		: m_modeCode("001"), m_modeType(ModeType::PULSE)
	{
	}

	Radar_Mode::Radar_Mode(const std::string &modeCode,	const ModeType &modeType, const Rf &rf, const Pw &pw, const Pri &pri, const Scan &scan, const Erp &erp)
		: m_modeCode(modeCode), m_modeType(modeType), m_rf(rf), m_pw(pw), m_pri(pri), m_scan(scan), m_erp(erp)
	{
	}

	//	Radar_Mode::~Radar_Mode(void)
	//	{
	//	}
	const std::string & Radar_Mode::getModeCode(void) const
	{
		return m_modeCode;
	}
	const ModeType & Radar_Mode::getModeType(void) const
	{
		return m_modeType;
	}

	Rf & Radar_Mode::getRf(void)
	{
		return m_rf;
	}

	Pw & Radar_Mode::getPw(void)
	{
		return m_pw;
	}

	Pri & Radar_Mode::getPri(void)
	{
		return m_pri;
	}

	Scan & Radar_Mode::getScan(void)
	{
		return m_scan;
	}

	Erp & Radar_Mode::getErp(void)
	{
		return m_erp;
	}

	void Radar_Mode::setModeCode(const std::string & modeCode)
	{
		m_modeCode = modeCode;
	}

	void Radar_Mode::setModeType(const ModeType & modeType)
	{
		m_modeType = modeType;
	}

	//这里Rf,Pw,Pri的方法中不在用const修饰是考虑到其类成员函数中的
	//getValues(void)返回值为非常量应用，这里用const修饰Rf类对象
	//的应用会发生常量值与非常量属性值不匹配的情况，后续在深入整理
	void Radar_Mode::setRf(Rf & rf)
	{
		m_rf = rf;
		// m_rf.setRfValues(rf.getRfValues());

	}
	void Radar_Mode::setPw(Pw & pw)
	{
		m_pw = pw;
	}
	void Radar_Mode::setPri(Pri & pri)
	{
		m_pri = pri;
	}

	void Radar_Mode::setScan(const Scan & scan)
	{
		m_scan = scan;
	}

	void Radar_Mode::setErp(const Erp & erp)
	{
		m_erp = erp;
	}

	/***************************Emitter*********************************/
	//由于默认构造函数尚未进行RadarMode值初始化，所以调用默认构造后应尽快完成RadarMode值初始化操作
	Emitter::Emitter(void)
		:m_name("Emitter1")
	{
	}

	Emitter::Emitter(const std::string & name)
		:m_name(name)
	{
	}

	Emitter::Emitter(const std::string &name, std::shared_ptr<Radar_Mode> ptrRadarMode)
		:m_name(name), m_ptrRadarMode({ ptrRadarMode })
	{
	}

	Emitter::Emitter(const std::string& name, std::vector<std::shared_ptr<Radar_Mode>>& ptrRadarModes)
		:m_name(name), m_ptrRadarMode(ptrRadarModes)
	{
	}

	const std::string& Emitter::getName(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_name;
	}

	std::vector<std::shared_ptr<Radar_Mode>>& Emitter::getAllPtr2RadarModes(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrRadarMode;
	}

	bool Emitter::isRadarModeEmpty(void)
	{
		return m_ptrRadarMode.empty() ? true : false;
	}

	const std::shared_ptr<Radar_Mode> Emitter::getPtr2RadarMode(size_t valueOrder) const
	{
		// TODO: 在此处插入 return 语句
		assert(valueOrder < m_ptrRadarMode.size());
		if (valueOrder<m_ptrRadarMode.size())
		{
			return m_ptrRadarMode[valueOrder];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	void Emitter::addRadarMode(const std::shared_ptr<Radar_Mode> ptr_radarmode)
	{
		m_ptrRadarMode.push_back(ptr_radarmode);
	}

	bool Emitter::setPtr2RadarMode(size_t & pos, std::shared_ptr<Radar_Mode> ptr_radarmode)
	{
		assert(pos<m_ptrRadarMode.size());
		if (pos < m_ptrRadarMode.size())
		{
			m_ptrRadarMode[pos] = ptr_radarmode;
			return true;
		}
		return false;
	}

	bool Emitter::insertRadarMode(size_t& pos, std::shared_ptr<Radar_Mode> ptr_radarmode)
	{
		assert(pos <= m_ptrRadarMode.size());
		if (pos <= m_ptrRadarMode.size())
		{
			m_ptrRadarMode.insert(m_ptrRadarMode.begin() + pos, ptr_radarmode);
			return true;
		}
		return false;
	}

	bool Emitter::deleteRadarMode(size_t& pos)
	{
		assert(pos<m_ptrRadarMode.size());
		if (pos < m_ptrRadarMode.size())
		{
			m_ptrRadarMode.erase(m_ptrRadarMode.begin() + pos);
			return true;
		}
		return false;
	}

	void Emitter::setName(const std::string & name)
	{
		m_name = name;
	}

	void Emitter::setAllPtr2RadarModes(std::vector<std::shared_ptr<Radar_Mode>>& ptrRadarModes)
	{
		m_ptrRadarMode = ptrRadarModes;
	}

	/*********************Weapon*********************************/
	Weapon::Weapon(void)
		:m_name("Weapon 1"), m_cepr(50000), m_weaponAreaCoverage(100000)
	{
	}

	Weapon::Weapon(const std::string& name,	const size_t& cepr,	const size_t& weaponAreaCoverage)
		:m_name(name), m_cepr(cepr), m_weaponAreaCoverage(weaponAreaCoverage)
	{
	}

	const std::string & Weapon::getName(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_name;
	}

	const size_t & Weapon::getCEPR(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_cepr;
	}

	const size_t & Weapon::getWeaponAreaCoverage(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_weaponAreaCoverage;
	}

	void Weapon::setName(const std::string & name)
	{
		m_name = name;
	}

	void Weapon::setCEPR(const size_t &cepr)
	{
		m_cepr = cepr;
	}

	void Weapon::setWeaponAreaCoverage(const size_t &weaponAreaCoverage)
	{
		m_weaponAreaCoverage = weaponAreaCoverage;
	}

	/***********************Site*********************************/
	Site::Site(void)
		:m_name("Site1"), m_altitude(100.0), m_latitude(0.0), m_longitude(0.0)
	{
	}

	Site::Site(const std::string &name, const double &altitude, const double &latitude, const double &longitude)
		:m_name(name), m_altitude(altitude), m_latitude(latitude), m_longitude(longitude)
	{
	}

	const std::string & Site::getName(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_name;
	}

	const double & Site::getAltitude(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_altitude;
	}

	const double & Site::getLatitude(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_latitude;
	}

	const double & Site::getLongitude(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_longitude;
	}

	void Site::setName(const std::string &name)
	{
		m_name = name;
	}

	void Site::setAltitude(const double &altitude)
	{
		m_altitude = altitude;
	}

	void Site::setLatitude(const double &latitude)
	{
		m_latitude = latitude;
	}

	void Site::setLongitude(const double &longitude)
	{
		m_longitude = longitude;
	}

	/*******************Point*******************************/
	Point::Point(void)
		:m_altitude(0.0),m_latitude(0.0),m_longitude(0.0),m_tmin(0.0),m_tmax(0.0)
	{
	}

	Point::Point(const double &altitude, const double &latitude, const double &longitude)
		:m_altitude(altitude), m_latitude(latitude), m_longitude(longitude), m_tmin(0.0), m_tmax(0.0)
	{
	}

	Point::Point(const double &altitude, const double &latitude, const double &longitude, const double &tmin, const double &tmax)
		:m_altitude(altitude), m_latitude(latitude), m_longitude(longitude), m_tmin(tmin), m_tmax(tmax)
	{
	}

	const double & Point::getAltitude(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_altitude;
	}

	const double & Point::getLatitude(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_latitude;
	}

	const double & Point::getLongitude(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_longitude;
	}

	const double & Point::getTmin(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_tmin;
	}

	const double & Point::getTmax(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_tmax;
	}

	void Point::setAltitude(const double &altitude)
	{
		m_altitude = altitude;
	}

	void Point::setLatitude(const double &latitude)
	{
		m_latitude = latitude;
	}

	void Point::setLongitude(const double &longitude)
	{
		m_longitude = longitude;
	}

	void Point::setTmin(const double &tmin)
	{
		m_tmin = tmin;
	}

	void Point::setTmax(const double &tmax)
	{
		m_tmax = tmax;
	}

	/*******************Mission********************************/
	Mission::Mission(void)
	{
	}

	Mission::Mission(const MissionType &missionType, const Point &startPoint, const Point &endPoint, const std::vector<Point>&targetPoint)
		:m_type(missionType), m_startPoint(startPoint), m_endPoint(endPoint), m_targetPoints(targetPoint)
	{
	}

	Mission::Mission(const MissionType &missionType, const Point &startPoint, const Point &endPoint, const Point &targetPoint)
		:m_type(missionType), m_startPoint(startPoint), m_endPoint(endPoint), m_targetPoints({ targetPoint })
	{
	}

	//此构造函数应该在MissionType为Support时才用此构造函数，但一如前面所说，不保证安全性
	Mission::Mission(const MissionType &missionType, const Point &startPoint, const Point &endPoint)
		:m_type(missionType), m_startPoint(startPoint), m_endPoint(endPoint)
	{
	}

	const MissionType & Mission::getMissionType(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_type;
	}

	Point & Mission::getStartPoint(void)
	{
		// TODO: 在此处插入 return 语句
		return m_startPoint;
	}

	Point & Mission::getEndPoint(void)
	{
		// TODO: 在此处插入 return 语句
		return m_endPoint;
	}

	std::vector<Point>& Mission::getAllTargetPoints(void)
	{
		// TODO: 在此处插入 return 语句
		return m_targetPoints;
	}

	bool Mission::isTargetPointEmpty(void)
	{
		return m_targetPoints.empty() ? true : false;
	}

	Point & Mission::getTargetPoint(const size_t& pos)
	{
		// TODO: 在此处插入 return 语句
		assert(pos < m_targetPoints.size());
		if (pos<m_targetPoints.size())
		{
			return m_targetPoints[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	//请注意，当使用默认构造函数时，添加第一个类对象时，不应该直接使用add函数，
	//因为默认构造已经初始化了一个随机的对象在容器中，此时应该对这个初始化对象进行编辑使其成为待添加的对象。
	void Mission::addTargetPoint(const Point &targetPoint)
	{
		m_targetPoints.push_back(targetPoint);
	}

	bool Mission::setTargetPoint(const size_t& pos, const Point &targetPoint)
	{
		assert(pos < m_targetPoints.size());
		if (pos < m_targetPoints.size())
		{
			m_targetPoints[pos] = targetPoint;
			return true;
		}
		return false;
	}

	bool Mission::insertTargetPoint(const size_t& pos, const Point &targetPoint)
	{
		assert(pos <= m_targetPoints.size());
		if (pos <= m_targetPoints.size())
		{
			m_targetPoints.insert(m_targetPoints.begin() + pos, targetPoint);
			return true;
		}
		return false;
	}

	bool Mission::deleteTargetPoint(const size_t& pos)
	{
		assert(pos<m_targetPoints.size());
		if (pos<m_targetPoints.size())
		{
			m_targetPoints.erase(m_targetPoints.begin() + pos);
			return true;
		}
		return false;
	}

	void Mission::setMissionType(const MissionType &missionType)
	{
		m_type = missionType;
	}

	void Mission::setStartPoint(const Point &startPoint)
	{
		m_startPoint = startPoint;
	}

	void Mission::setEndPoint(const Point &endPoint)
	{
		m_endPoint = endPoint;
	}

	void Mission::setAllTargetPoints(const std::vector<Point>& targetPoints)
	{
		m_targetPoints = targetPoints;
	}

	/***********************OwnPlatform************************************/
	OwnPlatform::OwnPlatform(void)
		:m_name("OwnPlatform 1")
		,m_type(OwnPlatformType::AIR)
		,m_maxAcceleration(9.8)
		,m_maxDeceleration(9.8)
		,m_maxClimbRate(30.0)
		,m_maxDiveRate(30.0)
		,m_maxSpeed(340.0)
		,m_maxTurnRadius(10000)
	{
	}

	OwnPlatform::OwnPlatform(const std::string &name,
		const OwnPlatformType &ownPlatformType,
		const double &maxAcceleration,
		const double &maxDeceleration,
		const double &maxClimbRate,
		const double &maxDiveRate,
		const double &maxSpeed,
		const double &maxTurnRadius)
		:m_name(name)
		,m_type(ownPlatformType)
		,m_maxAcceleration(maxAcceleration)
		,m_maxDeceleration(maxDeceleration)
		,m_maxClimbRate(maxClimbRate)
		,m_maxDiveRate(maxDiveRate)
		,m_maxSpeed(maxSpeed)
		,m_maxTurnRadius(maxTurnRadius)
	{
	}

	OwnPlatform::OwnPlatform(const std::string &name,
		const OwnPlatformType &ownPlatformType,
		const double &maxAcceleration,
		const double &maxDeceleration,
		const double &maxClimbRate,
		const double &maxDiveRate,
		const double &maxSpeed,
		const double &maxTurnRadius,
		const Mission &mission)
		:m_name(name)
		,m_type(ownPlatformType)
		,m_maxAcceleration(maxAcceleration)
		,m_maxDeceleration(maxDeceleration)
		,m_maxClimbRate(maxClimbRate)
		,m_maxDiveRate(maxDiveRate)
		,m_maxSpeed(maxSpeed)
		,m_maxTurnRadius(maxTurnRadius)
		,m_mission(mission)
	{
	}

	const std::string & OwnPlatform::getName(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_name;
	}

	const OwnPlatformType & OwnPlatform::getType(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_type;
	}

	const double & OwnPlatform::getMaxAcceleration(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_maxAcceleration;
	}

	const double & OwnPlatform::getMaxDeceleration(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_maxDeceleration;
	}

	const double & OwnPlatform::getMaxClimbRate(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_maxClimbRate;
	}

	const double & OwnPlatform::getMaxDiveRate(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_maxDiveRate;
	}

	const double & OwnPlatform::getMaxSpeed(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_maxSpeed;
	}

	const double & OwnPlatform::getMaxTurnRadius(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_maxTurnRadius;
	}

	Mission & OwnPlatform::getMission(void)
	{
		// TODO: 在此处插入 return 语句
		return m_mission;
	}

	void OwnPlatform::setName(const std::string &name)
	{
		m_name = name;
	}

	void OwnPlatform::setType(const OwnPlatformType &ownPlatformType)
	{
		m_type = ownPlatformType;
	}

	void OwnPlatform::setMaxAcceleration(const double &maxAcceleration)
	{
		m_maxAcceleration = maxAcceleration;
	}

	void OwnPlatform::setMaxDeceleration(const double &maxDeceleration)
	{
		m_maxDeceleration = maxDeceleration;
	}

	void OwnPlatform::setMaxClimbRate(const double &maxClimbRate)
	{
		m_maxClimbRate = maxClimbRate;
	}

	void OwnPlatform::setMaxDiveRate(const double &maxDiveRate)
	{
		m_maxDiveRate = maxDiveRate;
	}

	void OwnPlatform::setMaxSpeed(const double &maxSpeed)
	{
		m_maxSpeed = maxSpeed;
	}

	void OwnPlatform::setMaxTurnRadius(const double &maxTurnRadius)
	{
		m_maxTurnRadius = maxTurnRadius;
	}

	void OwnPlatform::setMission(const Mission &mission)
	{
		m_mission = mission;
	}

	/****************************Esm***************************/
	Esm::Esm(void)
		:m_name("Esm1")
		,m_dwellFreqResolution(100.0)
		,m_tuningStep(20)
		,m_rfCovMin(500)
		,m_rfCovMax(12000)
		,m_numPulsesAcquisition(200)
		,m_numPulsesAlarm(100)
		,m_esmMinDwellTime(0.01)
		,m_nEmitter(2)
		,m_taoScan(1.0)
		,m_Pmin(1.0)
		,m_Aeff(1.0)
	{
	}

	Esm::Esm(const std::string &name,
		const double &dwellFreqResolution,
		const size_t &tuningStep,
		const size_t &rfCovMin,
		const size_t &rfCovMax,
		const size_t &numPulsesAcquisition,
		const size_t &numPulsesAlarm)
		:m_name(name)
		, m_dwellFreqResolution(dwellFreqResolution)
		, m_tuningStep(tuningStep)
		, m_rfCovMin(rfCovMin)
		, m_rfCovMax(rfCovMax)
		, m_numPulsesAcquisition(numPulsesAcquisition)
		, m_numPulsesAlarm(numPulsesAlarm)
		, m_esmMinDwellTime(0.01)
		, m_nEmitter(2)
		, m_taoScan(1.0)
		, m_Pmin(1.0)
		, m_Aeff(1.0)
	{
	}

	Esm::Esm(const std::string &name,
		const double &dwellFreqResolution,
		const size_t &tuningStep,
		const size_t &rfCovMin,
		const size_t &rfCovMax,
		const size_t &numPulsesAcquisition,
		const size_t &numPulsesAlarm,
		const double &esmMinDwellTime,
		const size_t &nEmitter,
		const double &taoScan,
		const double &Pmin,
		const double &Aeff)
		:m_name(name)
		, m_dwellFreqResolution(dwellFreqResolution)
		, m_tuningStep(tuningStep)
		, m_rfCovMin(rfCovMin)
		, m_rfCovMax(rfCovMax)
		, m_numPulsesAcquisition(numPulsesAcquisition)
		, m_numPulsesAlarm(numPulsesAlarm)
		, m_esmMinDwellTime(esmMinDwellTime)
		, m_nEmitter(nEmitter)
		, m_taoScan(taoScan)
		, m_Pmin(Pmin)
		, m_Aeff(Aeff)
	{
	}

	const std::string & Esm::getName(void)
	{
		// TODO: 在此处插入 return 语句
		return m_name;
	}

	const double & Esm::getDwellFreqResolution(void)
	{
		// TODO: 在此处插入 return 语句
		return m_dwellFreqResolution;
	}

	const size_t & Esm::getTuningStep(void)
	{
		// TODO: 在此处插入 return 语句
		return m_tuningStep;
	}

	const size_t & Esm::getRfCovMin(void)
	{
		// TODO: 在此处插入 return 语句
		return m_rfCovMin;
	}

	const size_t & Esm::getRfCovMax(void)
	{
		// TODO: 在此处插入 return 语句
		return m_rfCovMax;
	}

	const size_t & Esm::getNumPulsesAcquisition(void)
	{
		// TODO: 在此处插入 return 语句
		return m_numPulsesAcquisition;
	}

	const size_t & Esm::getNumPulsesAlarm(void)
	{
		// TODO: 在此处插入 return 语句
		return m_numPulsesAlarm;
	}

	void Esm::setName(const std::string &name)
	{
		m_name = name;
	}

	void Esm::setDwellFreqResolution(const double &dwellFreqResolution)
	{
		m_dwellFreqResolution = dwellFreqResolution;
	}

	void Esm::setTuningStep(const size_t &tuningStep)
	{
		m_tuningStep = tuningStep;
	}

	void Esm::setRfCovMin(const size_t &rfCovMin)
	{
		m_rfCovMin = rfCovMin;
	}

	void Esm::setRfCovMax(const size_t &rfCovMax)
	{
		m_rfCovMax = rfCovMax;
	}

	void Esm::setNumPulsesAcquisition(const size_t &numPulsesAcquisition)
	{
		m_numPulsesAcquisition = numPulsesAcquisition;
	}

	void Esm::setNumPulsesAlarm(const size_t &numPulsesAlarm)
	{
		m_numPulsesAlarm = numPulsesAlarm;
	}

	void Esm::setesmMinDwellTime(const double &esmMinDwellTime)
	{
		m_esmMinDwellTime = esmMinDwellTime;
	}

	void Esm::setnEmitter(const int &nEmitter)
	{
		m_nEmitter = m_nEmitter;
	}

	void Esm::settaoScan(const double &taoScan)
	{
		m_taoScan = taoScan;
	}

	void Esm::setPmin(const double &Pmin)
	{
		m_Pmin = Pmin;
	}

	void Esm::setAeff(const double &aeff)
	{
		m_Aeff = aeff;
	}

	/********************Ecm********************************/
	Ecm::Ecm(void)
		:m_name("Ecm1")
		,m_pt(20000)
		,m_gain(10)
		,m_rfMin(9000)
		,m_rfMax(9500)
		,m_techName({ Tech::NOISE })
		,m_radarMSR(1.0)
		,m_jammerERP_support(1.0)
		,m_platformRCS(1.0)
		,m_radarDangerValue(1.0)
		,m_jammerERP_attack(1.0)
	{
	}

	Ecm::Ecm(const std::string &name, const size_t &pt, const size_t &gain, const size_t &rfMin, const size_t &rfMax)
		:m_name(name)
		,m_pt(pt)
		,m_gain(gain)
		,m_rfMin(rfMin)
		,m_rfMax(rfMax)
		,m_techName({ Tech::NOISE })
		,m_radarMSR(1.0)
		,m_jammerERP_support(1.0)
		,m_platformRCS(1.0)
		,m_radarDangerValue(1.0)
		,m_jammerERP_attack(1.0)
	{
	}

	Ecm::Ecm(const std::string &name,
		const size_t &pt,
		const size_t &gain,
		const size_t &rfmin,
		const size_t &rfmax,
		const Tech& techName)
		:m_name(name)
		, m_pt(pt)
		, m_gain(gain)
		, m_rfMin(rfmin)
		, m_rfMax(rfmax)
		, m_techName({ techName })
		, m_radarMSR(1.0)
		, m_jammerERP_support(1.0)
		, m_platformRCS(1.0)
		, m_radarDangerValue(1.0)
		, m_jammerERP_attack(1.0)
	{
	}

	Ecm::Ecm(const std::string &name,
		const size_t &pt,
		const size_t &gain,
		const size_t &rfmin,
		const size_t &rfmax,
		const std::vector<Tech> &techName)
		:m_name(name)
		, m_pt(pt)
		, m_gain(gain)
		, m_rfMin(rfmin)
		, m_rfMax(rfmax)
		, m_techName(techName)
		, m_radarMSR(1.0)
		, m_jammerERP_support(1.0)
		, m_platformRCS(1.0)
		, m_radarDangerValue(1.0)
		, m_jammerERP_attack(1.0)
	{
	}

	Ecm::Ecm(const std::string &name, 
		const size_t &pt, 
		const size_t &gain, 
		const size_t &rfmin, 
		const size_t &rfmax, 
		const std::vector<Tech> &techName,
		const double &radarMSR,
		const double &jammerERP_support,
		const double &platformRCS,
		const double &radarDangerValue,
		const double &jammerERP_attack)
		:m_name(name)
		, m_pt(pt)
		, m_gain(gain)
		, m_rfMin(rfmin)
		, m_rfMax(rfmax)
		, m_techName(techName)
		, m_radarMSR(radarMSR)
		, m_jammerERP_support(jammerERP_support)
		, m_platformRCS(platformRCS)
		, m_radarDangerValue(radarDangerValue)
		, m_jammerERP_attack(jammerERP_attack)
	{
	}

	const std::string & Ecm::getName(void)
	{
		// TODO: 在此处插入 return 语句
		return m_name;
	}

	const size_t & Ecm::getPt(void)
	{
		// TODO: 在此处插入 return 语句
		return m_pt;
	}

	const size_t & Ecm::getGain(void)
	{
		// TODO: 在此处插入 return 语句
		return m_gain;
	}

	const size_t & Ecm::getRfMin(void)
	{
		// TODO: 在此处插入 return 语句
		return m_rfMin;
	}

	const size_t & Ecm::getRfMax(void)
	{
		// TODO: 在此处插入 return 语句
		return m_rfMax;
	}

	std::vector<Tech>& Ecm::getAllTechs(void)
	{
		// TODO: 在此处插入 return 语句
		return m_techName;
	}

	bool Ecm::isTechEmpty(void)
	{
		return m_techName.size() ? true : false;
	}

	const Tech & Ecm::getTech(const size_t & order)
	{
		// TODO: 在此处插入 return 语句
		assert(order<m_techName.size());
		if (order<m_techName.size())
		{
			return m_techName[order];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	void Ecm::addTech(const Tech &tech)
	{
		m_techName.push_back(tech);
	}

	bool Ecm::setTech(const size_t & pos, const Tech &tech)
	{
		assert(pos<m_techName.size());
		if (pos<m_techName.size())
		{
			m_techName[pos] = tech;
			return true;
		}
		return false;
	}

	bool Ecm::insertTech(const size_t & pos, const Tech &tech)
	{
		assert(pos <= m_techName.size());
		if (pos <= m_techName.size())
		{
			m_techName.insert(m_techName.begin() + pos, tech);
			return true;
		}
		return false;
	}

	bool Ecm::deleteTech(const size_t & pos)
	{
		assert(pos < m_techName.size());
		if (pos<m_techName.size())
		{
			m_techName.erase(m_techName.begin() + pos);
			return true;
		}
		return false;
	}

	void Ecm::setName(const std::string &name)
	{
		m_name = name;
	}

	void Ecm::setPt(const size_t &pt)
	{
		m_pt = pt;
	}

	void Ecm::setGain(const size_t &gain)
	{
		m_gain = gain;
	}

	void Ecm::setRfMin(const size_t &rfMin)
	{
		m_rfMin = rfMin;
	}

	void Ecm::setRfMax(const size_t &rfMax)
	{
		m_rfMax = rfMax;
	}

	void Ecm::setradarMSR(const double & radarMSR)
	{
		m_radarMSR = radarMSR;
	}

	void Ecm::setjammerERP_support(const double & jammerERP_support)
	{
		m_jammerERP_support = jammerERP_support;
	}

	void Ecm::setplatformRCS(const double & platformRCS)
	{
		m_platformRCS = platformRCS;
	}

	void Ecm::setradarDangerValue(const double & radarDangerValue)
	{
		m_radarDangerValue = radarDangerValue;
	}

	void Ecm::setjammerERP_attack(const double & jammerERP_attack)
	{
		m_jammerERP_attack = jammerERP_attack;
	}

	void Ecm::setAllTechs(const std::vector<Tech>& techs)
	{
		m_techName = techs;
	}

	/*************************WarPoint********************************/
	WayPoint::WayPoint(void)
		:m_index(1)
		,m_altitude(0.0)
		,m_latitude(0.0)
		,m_longitude(0.0)
		,m_time(0.0)
		,m_velocity(200.0)
		,m_acceleration(0.0)
	{
	}

	WayPoint::WayPoint(const double &longitude, const double &latitude, const double &altitude)
		:m_index(1)
		,m_altitude(altitude)
		,m_latitude(latitude)
		,m_longitude(longitude)
		,m_time(0.0)
		,m_velocity(200.0)
		,m_acceleration(0.0)
	{
	}

	WayPoint::WayPoint(const size_t &index, const double &longitude, const double &latitude, const double &altitude)
		:m_index(index)
		,m_altitude(altitude)
		,m_latitude(latitude)
		,m_longitude(longitude)
		,m_time(0.0)
		,m_velocity(200.0)
		,m_acceleration(0.0)
	{
	}

	WayPoint::WayPoint(const size_t &index,
		const double &altitude,
		const double &latitude,
		const double &longitude,
		const double &time,
		const double &velocity,
		const double &acceleration)
		:m_index(index)
		,m_altitude(altitude)
		,m_latitude(latitude)
		,m_longitude(longitude)
		,m_time(time)
		,m_velocity(velocity)
		,m_acceleration(acceleration)
	{
	}

	const size_t & WayPoint::getIndex(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_index;
	}

	const double & WayPoint::getAltitude(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_altitude;
	}

	const double & WayPoint::getLatitude(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_latitude;
	}

	const double & WayPoint::getLongitude(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_longitude;
	}

	const double & WayPoint::getTime(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_time;
	}

	const double & WayPoint::getVelocity(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_velocity;
	}

	const double & WayPoint::getAcceleration(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_acceleration;
	}

	void WayPoint::setIndex(const size_t &index)
	{
		m_index = index;
	}

	void WayPoint::setAltitude(const double &altitude)
	{
		m_altitude = altitude;
	}

	void WayPoint::setLatitude(const double &latitude)
	{
		m_latitude = latitude;
	}

	void WayPoint::setLongitude(const double &longitude)
	{
		m_longitude = longitude;
	}

	void WayPoint::setTime(const double &time)
	{
		m_time = time;
	}

	void WayPoint::setVelocity(const double &velocity)
	{
		m_velocity = velocity;
	}

	void WayPoint::setAcceleration(const double &acceleration)
	{
		m_acceleration = acceleration;
	}

	/************************Route*****************************/
	Route::Route(void)
		:m_name("Route1")
	{

	}

	Route::Route(const std::string &name)
		:m_name(name)
	{

	}

	Route::Route(const std::string &name, const WayPoint &wayPoint)
		:m_name(name)
	{
	}

	Route::Route(const std::string &name, const std::vector<WayPoint>&wayPoints)
		: m_name(name), m_wayPoints(wayPoints)
	{
	}

	const std::string & Route::getName(void)
	{
		// TODO: 在此处插入 return 语句
		return m_name;
	}

	bool Route::isWayPointEmpty(void)
	{
		return m_wayPoints.empty() ? true : false;
	}

	WayPoint & Route::getWayPoint(const size_t &index)
	{
		// TODO: 在此处插入 return 语句
		assert(index < m_wayPoints.size());
		if (index<m_wayPoints.size())
		{
			return m_wayPoints[index];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	std::vector<WayPoint> & Route::getAllWayPoints(void)
	{
		// TODO: 在此处插入 return 语句
		return m_wayPoints;
	}

	void Route::addWayPoint(const WayPoint &wayPoint)
	{
		m_wayPoints.push_back(wayPoint);
	}

	bool Route::insertWayPoint(const size_t &pos, const WayPoint &wayPoint)
	{
		assert(pos <= m_wayPoints.size());
		if (pos <= m_wayPoints.size())
		{
			m_wayPoints.insert(m_wayPoints.begin() + pos, wayPoint);
			return true;
		}
		return false;
	}

	bool Route::setWayPoint(const size_t &index, const WayPoint &wayPoint)
	{
		assert(index < m_wayPoints.size());
		if (index < m_wayPoints.size())
		{
			m_wayPoints[index] = wayPoint;
			return true;
		}
		return false;
	}

	bool Route::deleteWayPoint(const size_t &index)
	{
		assert(index < m_wayPoints.size());
		if (index < m_wayPoints.size())
		{
			m_wayPoints.erase(m_wayPoints.begin() + index);
			return true;
		}
		return false;
	}

	void Route::setName(const std::string &name)
	{
		m_name = name;
	}

	void Route::setAllWayPoints(const std::vector<WayPoint>& wayPoints)
	{
		m_wayPoints = wayPoints;
	}

	/***************************Location********************************/
	Location::Location(void)
		:m_altitude(0.0)
		,m_latitude(0.0)
		,m_longitude(0.0)
	{
	}

	Location::Location(const double &longitude,
		const double &latitude,
		const double &altitude)
		:m_altitude(altitude)
		,m_latitude(latitude)
		,m_longitude(longitude)
	{
	}

	const double & Location::getAltitude(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_altitude;
	}

	const double & Location::getLatitude(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_latitude;
	}

	const double & Location::getLongitude(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_longitude;
	}

	void Location::setAltitude(const double &altitude)
	{
		m_altitude = altitude;
	}

	void Location::setLatitude(const double &latitude)
	{
		m_latitude = latitude;
	}

	void Location::setLongitude(const double &longitude)
	{
		m_longitude = longitude;
	}

	/*********************DwellSquence***********************************/
	DwellSquence::DwellSquence(void)
		:m_index(1), m_minFreq(100), m_maxFreq(200), m_startTime(0.0), m_endTime(30.0)
	{
	}

	DwellSquence::DwellSquence(const size_t &index, const size_t &minFreq, const size_t &maxFreq, const double &startTime, const double &endTime)
		:m_index(index), m_minFreq(minFreq), m_maxFreq(maxFreq), m_startTime(startTime), m_endTime(endTime)
	{
	}

	const size_t & DwellSquence::getIndex(void)
	{
		// TODO: 在此处插入 return 语句
		return m_index;
	}

	const size_t & DwellSquence::getMinFreq(void)
	{
		// TODO: 在此处插入 return 语句
		return m_minFreq;
	}

	const size_t & DwellSquence::getMaxFreq(void)
	{
		// TODO: 在此处插入 return 语句
		return m_maxFreq;
	}

	const double & DwellSquence::getStartTime(void)
	{
		// TODO: 在此处插入 return 语句
		return m_startTime;
	}

	const double & DwellSquence::getEndTime(void)
	{
		// TODO: 在此处插入 return 语句
		return m_endTime;
	}

	void DwellSquence::setIndex(const size_t &index)
	{
		m_index = index;
	}

	void DwellSquence::setMinFreq(const size_t &minFreq)
	{
		m_minFreq = minFreq;
	}

	void DwellSquence::setMaxFreq(const size_t &maxFreq)
	{
		m_maxFreq = maxFreq;
	}

	void DwellSquence::setStartTime(const double &startTime)
	{
		m_startTime = startTime;
	}

	void DwellSquence::setEndTime(const double &endTime)
	{
		m_endTime = endTime;
	}

	/***********************EsmStrategySection*********************************/
	EsmStrategySection::EsmStrategySection(void)
		:m_startTime(0)
		, m_endTime(100)
		, m_startLocation(Location())
		, m_endLocation(Location())
	{
	}

	EsmStrategySection::EsmStrategySection(const double &startTime,	const double &endTime, const Location &startLocation, const Location &endLocation)
		:m_startTime(startTime)
		,m_endTime(endTime)
		,m_startLocation(startLocation)
		,m_endLocation(endLocation)
	{
	}

	EsmStrategySection::EsmStrategySection(const double &startTime,
		const double &endTime,
		const Location &startLocation,
		const Location &endLocation,
		const DwellSquence &dwellsquence)
		:m_startTime(startTime)
		,m_endTime(endTime)
		,m_startLocation(startLocation)
		,m_endLocation(endLocation)
		,m_dwellSquences({ dwellsquence })
	{
	}

	EsmStrategySection::EsmStrategySection(const double &startTime,
		const double &endTime,
		const Location &startLocation,
		const Location &endLocation,
		const std::vector<DwellSquence> &dwellsquences)
		:m_startTime(startTime)
		, m_endTime(endTime)
		, m_startLocation(startLocation)
		, m_endLocation(endLocation)
		, m_dwellSquences(dwellsquences)
	{
	}

	const double & EsmStrategySection::getStartTime(void)
	{
		// TODO: 在此处插入 return 语句
		return m_startTime;
	}

	const double & EsmStrategySection::getEndTime(void)
	{
		// TODO: 在此处插入 return 语句
		return m_endTime;
	}

	Location & EsmStrategySection::getStartLocation(void)
	{
		// TODO: 在此处插入 return 语句
		return m_startLocation;
	}

	Location & EsmStrategySection::getEndLocation(void)
	{
		// TODO: 在此处插入 return 语句
		return m_endLocation;
	}

	bool EsmStrategySection::isDwellSquenceEmpty(void)
	{
		return m_dwellSquences.empty() ? true : false;
	}

	DwellSquence & EsmStrategySection::getDwellSquence(const size_t & index)
	{
		// TODO: 在此处插入 return 语句
		assert(index < m_dwellSquences.size());
		if (index<m_dwellSquences.size())
		{
			return m_dwellSquences[index];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	std::vector<DwellSquence>& EsmStrategySection::getAllDwellSquences(void)
	{
		// TODO: 在此处插入 return 语句
		return m_dwellSquences;
	}

	void EsmStrategySection::addDwellSquence(const DwellSquence &dwellsquence)
	{
		m_dwellSquences.push_back(dwellsquence);
	}

	bool EsmStrategySection::insertDwellSquence(const size_t & pos, const DwellSquence &dwellsquence)
	{
		assert(pos <= m_dwellSquences.size());
		if (pos <= m_dwellSquences.size())
		{
			m_dwellSquences.insert(m_dwellSquences.begin() + pos, dwellsquence);
			return true;
		}
		return false;
	}

	bool EsmStrategySection::setDwellSquence(const size_t & pos, const DwellSquence &dwellsquence)
	{
		assert(pos < m_dwellSquences.size());
		if (pos < m_dwellSquences.size())
		{
			m_dwellSquences[pos] = dwellsquence;
			return true;
		}
		return false;
	}

	bool EsmStrategySection::deleteDwellSquence(const size_t & index)
	{
		assert(index < m_dwellSquences.size());
		if (index < m_dwellSquences.size())
		{
			m_dwellSquences.erase(m_dwellSquences.begin() + index);
			return true;
		}
		return false;
	}

	void EsmStrategySection::setDwellSquences(const std::vector<DwellSquence>& dwellsquences)
	{
		m_dwellSquences = dwellsquences;
	}

	void EsmStrategySection::setStartTime(const double &startTime)
	{
		m_startTime = startTime;
	}

	void EsmStrategySection::setEndTime(const double &endTime)
	{
		m_endTime = endTime;
	}

	void EsmStrategySection::setStartLocation(const Location &startLocation)
	{
		m_startLocation = startLocation;
	}

	void EsmStrategySection::setEndLocation(const Location &endLocation)
	{
		m_endLocation = endLocation;
	}

	/********************EsmStrategy*********************************/
	EsmStrategy::EsmStrategy(void)
		:m_name("ESM Strategy 1")
	{
	}

	EsmStrategy::EsmStrategy(const std::string &name)
		: m_name(name)
	{
	}

	EsmStrategy::EsmStrategy(const std::string &name,
		const std::shared_ptr<EsmStrategySection>&ptrEsmStrategySection)
		: m_name(name)
		, m_ptrSections({ ptrEsmStrategySection })
	{
	}

	EsmStrategy::EsmStrategy(const std::string &name,
		const std::vector<std::shared_ptr<EsmStrategySection>>& ptrEsmStrategySections)
		: m_name(name)
		, m_ptrSections(ptrEsmStrategySections)
	{
	}

	const std::string & EsmStrategy::getName(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_name;
	}

	std::vector<std::shared_ptr<EsmStrategySection>>& EsmStrategy::getAllPtr2Sections(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrSections;
	}

	bool EsmStrategy::isSectionEmpty(void)
	{
		return m_ptrSections.empty() ? true : false;
	}

	const std::shared_ptr<EsmStrategySection> EsmStrategy::getPtr2Section(size_t & pos) const
	{
		assert(pos < m_ptrSections.size());
		if (pos<m_ptrSections.size())
		{
			return m_ptrSections[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	void EsmStrategy::addSection(const std::shared_ptr<EsmStrategySection> ptrSection)
	{
		m_ptrSections.push_back(ptrSection);
	}

	bool EsmStrategy::setPtr2Section(size_t & pos, std::shared_ptr<EsmStrategySection> ptrSection)
	{
		assert(pos < m_ptrSections.size());
		if (pos<m_ptrSections.size())
		{
			m_ptrSections[pos] = ptrSection;
			return true;
		}
		return false;
	}

	bool EsmStrategy::insertSection(size_t & pos, std::shared_ptr<EsmStrategySection> ptrSection)
	{
		assert(pos <= m_ptrSections.size());
		if (pos <= m_ptrSections.size())
		{
			m_ptrSections.insert(m_ptrSections.begin() + pos, ptrSection);
			return true;
		}
		return false;
	}

	bool EsmStrategy::deleteSection(size_t & pos)
	{
		assert(pos < m_ptrSections.size());
		if (pos < m_ptrSections.size())
		{
			m_ptrSections.erase(m_ptrSections.begin() + pos);
			return true;
		}
		return false;
	}

	void EsmStrategy::setName(const std::string & name)
	{
		m_name = name;
	}

	void EsmStrategy::setAllPtr2Sections(std::vector<std::shared_ptr<EsmStrategySection>>& ptrAllSections)
	{
		m_ptrSections = ptrAllSections;
	}

	/*************************EcmStrategySection**********************************/
	EcmStrategySection::EcmStrategySection(void)
		:m_startTime(0.0), m_endTime(100.0), m_startLocation(Location()), m_endLocation(Location()), m_tech(Tech::NOISE)
	{
	}

	EcmStrategySection::EcmStrategySection(const double &startTime,	const double &endTime, const Location &startLocation, const Location &endLocation)
		:m_startTime(startTime), m_endTime(endTime), m_startLocation(startLocation), m_endLocation(endLocation), m_tech(Tech::NOISE)
	{
	}

	EcmStrategySection::EcmStrategySection(const double &startTime,	const double &endTime, const Location &startLocation, const Location &endLocation, const Tech &techName)
		:m_startTime(startTime), m_endTime(endTime), m_startLocation(startLocation), m_endLocation(endLocation), m_tech(techName)
	{
	}

	const double & EcmStrategySection::getStartTime(void)
	{
		// TODO: 在此处插入 return 语句
		return m_startTime;
	}

	const double & EcmStrategySection::getEndTime(void)
	{
		// TODO: 在此处插入 return 语句
		return m_endTime;
	}

	Location & EcmStrategySection::getStartLocation(void)
	{
		// TODO: 在此处插入 return 语句
		return m_startLocation;
	}

	Location & EcmStrategySection::getEndLocation(void)
	{
		// TODO: 在此处插入 return 语句
		return m_endLocation;
	}

	const Tech & EcmStrategySection::getTechName(void)
	{
		// TODO: 在此处插入 return 语句
		return m_tech;
	}

	void EcmStrategySection::setStartTime(const double &startTime)
	{
		m_startTime = startTime;
	}

	void EcmStrategySection::setEndTime(const double &endTime)
	{
		m_endTime = endTime;
	}

	void EcmStrategySection::setStartLocation(const Location &startLocation)
	{
		m_startLocation = startLocation;
	}

	void EcmStrategySection::setEndLocation(const Location &endLocation)
	{
		m_endLocation = endLocation;
	}

	void EcmStrategySection::setTechName(const Tech &techName)
	{
		m_tech = techName;
	}

	/***************************EcmStrategy******************************/
	EcmStrategy::EcmStrategy(void)
		:m_name("ECM Strategy 1")
	{
	}

	EcmStrategy::EcmStrategy(const std::string &name)
		: m_name(name)
	{
	}

	EcmStrategy::EcmStrategy(const std::string &name, const std::shared_ptr<EcmStrategySection>&ptrEcmStrategySection)
		: m_name(name)
		, m_ptrSections({ ptrEcmStrategySection })
	{
	}

	EcmStrategy::EcmStrategy(const std::string &name, const std::vector<std::shared_ptr<EcmStrategySection>>& ptrEcmStrategySections)
		: m_name(name)
		, m_ptrSections(ptrEcmStrategySections)
	{
	}

	const std::string & EcmStrategy::getName(void) const
	{
		// TODO: 在此处插入 return 语句
		return m_name;
	}

	std::vector<std::shared_ptr<EcmStrategySection>>& EcmStrategy::getAllPtr2Sections(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrSections;
	}

	bool EcmStrategy::isSectionEmpty(void)
	{
		return m_ptrSections.empty() ? true : false;
	}

	const std::shared_ptr<EcmStrategySection> EcmStrategy::getPtr2Section(size_t & pos) const
	{
		assert(pos < m_ptrSections.size());
		if (pos<m_ptrSections.size())
		{
			return m_ptrSections[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	void EcmStrategy::addSection(const std::shared_ptr<EcmStrategySection> ptrSection)
	{
		m_ptrSections.push_back(ptrSection);
	}

	bool EcmStrategy::setPtr2Section(size_t & pos, std::shared_ptr<EcmStrategySection> ptrSection)
	{
		assert(pos < m_ptrSections.size());
		if (pos<m_ptrSections.size())
		{
			m_ptrSections[pos] = ptrSection;
			return true;
		}
		return false;
	}

	bool EcmStrategy::insertSection(size_t & pos, std::shared_ptr<EcmStrategySection> ptrSection)
	{
		assert(pos <= m_ptrSections.size());
		if (pos <= m_ptrSections.size())
		{
			m_ptrSections.insert(m_ptrSections.begin() + pos, ptrSection);
			return true;
		}
		return false;
	}

	bool EcmStrategy::deleteSection(size_t & pos)
	{
		assert(pos < m_ptrSections.size());
		if (pos < m_ptrSections.size())
		{
			m_ptrSections.erase(m_ptrSections.begin() + pos);
			return true;
		}
		return false;
	}

	void EcmStrategy::setName(const std::string & name)
	{
		m_name = name;
	}

	void EcmStrategy::setAllPtr2Sections(std::vector<std::shared_ptr<EcmStrategySection>>& ptrAllSections)
	{
		m_ptrSections = ptrAllSections;
	}

	/**************************PlatformSiteRelation*****************************/
	PlatformSiteRelation::PlatformSiteRelation(void)
		:m_ptrPlatform(std::make_shared<Platform>(Platform()))
		,m_ptrSite(std::make_shared<Site>(Site()))
	{
	}

	PlatformSiteRelation::PlatformSiteRelation(const Platform_ptr ptrPlatform,
		const Site_ptr ptrSite)
		:m_ptrPlatform(ptrPlatform)
		,m_ptrSite(ptrSite)
	{
	}
	//	PlatformSiteRelation::~PlatformSiteRelation(void)
	//	{
	//	}

	const std::string & PlatformSiteRelation::getPlatformName(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrPlatform->getPlatformName();
	}
	const std::string & PlatformSiteRelation::getSiteName(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrSite->getName();
	}
	Site_ptr PlatformSiteRelation::getSite(void)
	{
		return m_ptrSite;
	}
	Platform_ptr PlatformSiteRelation::getPlatform(void)
	{
		return m_ptrPlatform;
	}
	void PlatformSiteRelation::setPlatform(const Platform_ptr ptrPlatform)
	{
		m_ptrPlatform = ptrPlatform;
	}
	void PlatformSiteRelation::setSite(const Site_ptr ptrSite)
	{
		m_ptrSite = ptrSite;
	}

	/************************PlatformEmitterRelation*************************/
	PlatformEmitterRelation::PlatformEmitterRelation(void)
		:m_ptrPlatform(std::make_shared<Platform>(Platform()))
		,m_ptrEmitter(std::make_shared<Emitter>(Emitter()))
	{
	}
	PlatformEmitterRelation::PlatformEmitterRelation(const Platform_ptr ptrPlatform,
		const Emitter_ptr ptrEmitter)
		:m_ptrPlatform(ptrPlatform)
		,m_ptrEmitter(ptrEmitter)
	{
	}
	//	PlatformEmitterRelation::~PlatformEmitterRelation(void)
	//	{
	//	}
	const std::string & PlatformEmitterRelation::getPlatformName(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrPlatform->getPlatformName();
	}
	const std::string & PlatformEmitterRelation::getEmitterName(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrEmitter->getName();
	}
	Platform_ptr PlatformEmitterRelation::getPlatform(void)
	{
		return m_ptrPlatform;
	}
	Emitter_ptr PlatformEmitterRelation::getEmitter(void)
	{
		return m_ptrEmitter;
	}
	void PlatformEmitterRelation::setPlatform(const Platform_ptr ptrPlatform)
	{
		m_ptrPlatform = ptrPlatform;
	}
	void PlatformEmitterRelation::setEmitter(const Emitter_ptr ptrEmitter)
	{
		m_ptrEmitter = ptrEmitter;
	}

	/*************************PlatformWeaponRelation*********************************/
	PlatformWeaponRelation::PlatformWeaponRelation(void)
		:m_ptrPlatform(std::make_shared<Platform>(Platform()))
		,m_ptrWeapon(std::make_shared<Weapon>(Weapon()))
	{
	}
	PlatformWeaponRelation::PlatformWeaponRelation(const Platform_ptr ptrPlatform,
		const Weapon_ptr ptrWeapon)
		:m_ptrPlatform(ptrPlatform)
		,m_ptrWeapon(ptrWeapon)
	{
	}
	//	PlatformWeaponRelation::~PlatformWeaponRelation(void)
	//	{
	//	}
	const std::string & PlatformWeaponRelation::getPlatformName(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrPlatform->getPlatformName();
	}
	const std::string & PlatformWeaponRelation::getWeaponName(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrWeapon->getName();
	}
	Platform_ptr PlatformWeaponRelation::getPlatform(void)
	{
		return m_ptrPlatform;
	}
	Weapon_ptr PlatformWeaponRelation::getWeapon(void)
	{
		return m_ptrWeapon;
	}
	void PlatformWeaponRelation::setPlatform(const Platform_ptr ptrPlatform)
	{
		m_ptrPlatform = ptrPlatform;
	}
	void PlatformWeaponRelation::setWeapon(const Weapon_ptr ptrWeapon)
	{
		m_ptrWeapon = ptrWeapon;
	}

	/************************OwnPlatformEsmRelation********************************/
	OwnPlatformEsmRelation::OwnPlatformEsmRelation(void)
		:m_ptrOwnPlatform(std::make_shared<OwnPlatform>(OwnPlatform()))
		,m_ptrEsm(std::make_shared<Esm>(Esm()))
	{
	}
	OwnPlatformEsmRelation::OwnPlatformEsmRelation(const OwnPlatform_ptr ptrOwnPlatform,
		const Esm_ptr ptrEsm)
		:m_ptrOwnPlatform(ptrOwnPlatform)
		, m_ptrEsm(ptrEsm)
	{
	}
	//	OwnPlatformEsmRelation::~OwnPlatformEsmRelation(void)
	//	{
	//	}
	const std::string & OwnPlatformEsmRelation::getOwnPlatformName(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrOwnPlatform->getName();
	}
	const std::string & OwnPlatformEsmRelation::getEsmName(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrEsm->getName();
	}
	OwnPlatform_ptr OwnPlatformEsmRelation::getOwnPlatform(void)
	{
		return m_ptrOwnPlatform;
	}
	Esm_ptr OwnPlatformEsmRelation::getEsm(void)
	{
		return m_ptrEsm;
	}
	void OwnPlatformEsmRelation::setOwnPlatform(const OwnPlatform_ptr ptrOwnPlatform)
	{
		m_ptrOwnPlatform = ptrOwnPlatform;
	}
	void OwnPlatformEsmRelation::setEsm(const Esm_ptr ptrEsm)
	{
		m_ptrEsm = ptrEsm;
	}

	/**********************EsmEsmStrategyRelation*************************/
	EsmEsmStrategyRelation::EsmEsmStrategyRelation(void)
		:m_ptrEsm(std::make_shared<Esm>(Esm()))
		,m_ptrEsmStrategy(std::make_shared<EsmStrategy>(EsmStrategy()))
	{
	}
	EsmEsmStrategyRelation::EsmEsmStrategyRelation(const Esm_ptr ptrEsm,
		const EsmStrategy_ptr ptrEsmStrategy)
		:m_ptrEsm(ptrEsm)
		, m_ptrEsmStrategy(ptrEsmStrategy)
	{
	}
	//	EsmEsmStrategyRelation::~EsmEsmStrategyRelation(void)
	//	{
	//	}
	const std::string & EsmEsmStrategyRelation::getEsmName(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrEsm->getName();
	}
	const std::string & EsmEsmStrategyRelation::getEsmStrategyName(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrEsmStrategy->getName();
	}
	EsmStrategy_ptr EsmEsmStrategyRelation::getEsmStrategy(void)
	{
		return m_ptrEsmStrategy;
	}
	Esm_ptr EsmEsmStrategyRelation::getEsm(void)
	{
		return m_ptrEsm;
	}
	void EsmEsmStrategyRelation::setEsm(const Esm_ptr ptrEsm)
	{
		m_ptrEsm = ptrEsm;
	}
	void EsmEsmStrategyRelation::setEsmStrategy(const EsmStrategy_ptr ptrEsmStrategy)
	{
		m_ptrEsmStrategy = ptrEsmStrategy;
	}

	/************************OwnPlatformEcmRelation**********************/
	OwnPlatformEcmRelation::OwnPlatformEcmRelation(void)
		:m_ptrOwnPlatform(std::make_shared<OwnPlatform>(OwnPlatform()))
		,m_ptrEcm(std::make_shared<Ecm>(Ecm()))
	{
	}
	OwnPlatformEcmRelation::OwnPlatformEcmRelation(const OwnPlatform_ptr ptrOwnPlatform,
		const Ecm_ptr ptrEcm)
		:m_ptrOwnPlatform(ptrOwnPlatform)
		,m_ptrEcm(ptrEcm)
	{
	}
	//	OwnPlatformEcmRelation::~OwnPlatformEcmRelation(void)
	//	{
	//	}
	const std::string & OwnPlatformEcmRelation::getOwnPlatformName(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrOwnPlatform->getName();
	}
	const std::string & OwnPlatformEcmRelation::getEcmName(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrEcm->getName();
	}
	OwnPlatform_ptr OwnPlatformEcmRelation::getOwnPlatform(void)
	{
		return m_ptrOwnPlatform;
	}
	Ecm_ptr OwnPlatformEcmRelation::getEcm(void)
	{
		return m_ptrEcm;
	}
	void OwnPlatformEcmRelation::setOwnPlatform(const OwnPlatform_ptr ptrOwnPlatform)
	{
		m_ptrOwnPlatform = ptrOwnPlatform;
	}
	void OwnPlatformEcmRelation::setEcm(const Ecm_ptr ptrEcm)
	{
		m_ptrEcm = ptrEcm;
	}

	/************************EcmEcmStrategyRelation**********************/
	EcmEcmStrategyRelation::EcmEcmStrategyRelation(void)
		:m_ptrEcm(std::make_shared<Ecm>(Ecm()))
		,m_ptrEcmStrategy(std::make_shared<EcmStrategy>(EcmStrategy()))
	{
	}
	EcmEcmStrategyRelation::EcmEcmStrategyRelation(const Ecm_ptr ptrEcm, const EcmStrategy_ptr ptrEcmStrategy)
		:m_ptrEcm(ptrEcm), m_ptrEcmStrategy(ptrEcmStrategy)
	{
	}
	//	EcmEcmStrategyRelation::~EcmEcmStrategyRelation(void)
	//	{
	//	}
	const std::string & EcmEcmStrategyRelation::getEcmName(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrEcm->getName();
	}
	const std::string & EcmEcmStrategyRelation::getEcmStrategyName(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrEcmStrategy->getName();
	}
	EcmStrategy_ptr EcmEcmStrategyRelation::getEcmStrategy(void)
	{
		return m_ptrEcmStrategy;
	}
	Ecm_ptr EcmEcmStrategyRelation::getEcm(void)
	{
		return m_ptrEcm;
	}
	void EcmEcmStrategyRelation::setEcm(const Ecm_ptr ptrEcm)
	{
		m_ptrEcm = ptrEcm;
	}
	void EcmEcmStrategyRelation::setEcmStrategy(const EcmStrategy_ptr ptrEcmStrategy)
	{
		m_ptrEcmStrategy = ptrEcmStrategy;
	}

	/************************OwnPlatformRouteRelation**********************/
	OwnPlatformRouteRelation::OwnPlatformRouteRelation(void)
		:m_ptrOwnPlatform(std::make_shared<OwnPlatform>(OwnPlatform()))
		,m_ptrRoute(std::make_shared<Route>(Route()))
	{
	}
	OwnPlatformRouteRelation::OwnPlatformRouteRelation(const OwnPlatform_ptr ptrOwnPlatform,
		const Route_ptr ptrRoute)
		:m_ptrOwnPlatform(ptrOwnPlatform)
		,m_ptrRoute(ptrRoute)
	{
	}
	//	OwnPlatformRouteRelation::~OwnPlatformRouteRelation(void)
	//	{
	//	}
	const std::string & OwnPlatformRouteRelation::getOwnPlatformName(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrOwnPlatform->getName();
	}
	const std::string & OwnPlatformRouteRelation::getRouteName(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrRoute->getName();
	}
	OwnPlatform_ptr OwnPlatformRouteRelation::getOwnPlatform(void)
	{
		return m_ptrOwnPlatform;
	}
	Route_ptr OwnPlatformRouteRelation::getRoute(void)
	{
		return m_ptrRoute;
	}
	void OwnPlatformRouteRelation::setOwnPlatform(const OwnPlatform_ptr ptrOwnPlatform)
	{
		m_ptrOwnPlatform = ptrOwnPlatform;
	}
	void OwnPlatformRouteRelation::setRoute(const Route_ptr ptrRoute)
	{
		m_ptrRoute = ptrRoute;
	}

	/******************************Scenario***********************************/
	Scenario::Scenario(void)
	{
	}

	Scenario::Scenario(Vertex_ptr vertexs,
		Platform_ptr ptrPlatforms,
		Emitter_ptr ptrEmitters,
		Weapon_ptr ptrWeapons,
		Site_ptr ptrSites,
		OwnPlatform_ptr ptrOwnPlatforms,
		Esm_ptr ptrEsms,
		Ecm_ptr ptrEcms,
		Route_ptr ptrRoutes,
		EsmStrategy_ptr ptrEsmStrategy,
		EcmStrategy_ptr ptrEcmStrategy,
		PlatformSiteRelation& platformSiteRelations,
		PlatformEmitterRelation& platformEmitterRelations,
		PlatformWeaponRelation& platformWeaponRelations,
		OwnPlatformEsmRelation& ownPlatformEsmRelations,
		EsmEsmStrategyRelation& esmEsmStrategyRelations,
		OwnPlatformEcmRelation& ownPlatformEcmRelations,
		EcmEcmStrategyRelation& ecmEcmStrategyRelations,
		OwnPlatformRouteRelation& ownPlatformRouteRelations)
		: m_ptrVertex({ vertexs })
		, m_ptrPlatform({ ptrPlatforms })
		, m_ptrEmitter({ ptrEmitters })
		, m_ptrWeapon({ ptrWeapons })
		, m_ptrSite({ ptrSites })
		, m_ptrOwnPlatform({ ptrOwnPlatforms })
		, m_ptrEsm({ ptrEsms })
		, m_ptrEcm({ ptrEcms })
		, m_ptrRoute({ ptrRoutes })
		, m_ptrEsmStrategy({ ptrEsmStrategy })
		, m_ptrEcmStrategy({ ptrEcmStrategy })
		, m_PlatformSiteRelation({ platformSiteRelations })
		, m_PlatformEmitterRelation({ platformEmitterRelations })
		, m_PlatformWeaponRelation({ platformWeaponRelations })
		, m_OwnPlatformEsmRelation({ ownPlatformEsmRelations })
		, m_EsmEsmStrategyRelation({ esmEsmStrategyRelations })
		, m_OwnPlatformEcmRelation({ ownPlatformEcmRelations })
		, m_EcmEcmStrategyRelation({ ecmEcmStrategyRelations })
		, m_OwnPlatformRouteRelation({ ownPlatformRouteRelations })
	{
	}
	Scenario::Scenario(VertexsVector& vertexs,
		PlatformsVector& ptrPlatforms,
		EmittersVector& ptrEmitters,
		WeaponsVector& ptrWeapons,
		SitesVector& ptrSites,
		OwnPlatformsVector& ptrOwnPlatforms,
		EsmsVector& ptrEsms,
		EcmsVector& ptrEcms,
		RoutesVector& ptrRoutes,
		EsmStrategyVector& ptrEsmStrategy,
		EcmStrategyVector& ptrEcmStrategy,
		std::vector<PlatformSiteRelation>& platformSiteRelations,
		std::vector<PlatformEmitterRelation>& platformEmitterRelations,
		std::vector<PlatformWeaponRelation>& platformWeaponRelations,
		std::vector<OwnPlatformEsmRelation>& ownPlatformEsmRelations,
		std::vector<EsmEsmStrategyRelation>& esmEsmStrategyRelations,
		std::vector<OwnPlatformEcmRelation>& ownPlatformEcmRelations,
		std::vector<EcmEcmStrategyRelation>& ecmEcmStrategyRelations,
		std::vector<OwnPlatformRouteRelation>& ownPlatformRouteRelations)
		: m_ptrVertex(vertexs)
		, m_ptrPlatform(ptrPlatforms)
		, m_ptrEmitter(ptrEmitters)
		, m_ptrWeapon(ptrWeapons)
		, m_ptrSite(ptrSites)
		, m_ptrOwnPlatform(ptrOwnPlatforms)
		, m_ptrEsm(ptrEsms)
		, m_ptrEcm(ptrEcms)
		, m_ptrRoute(ptrRoutes)
		, m_ptrEsmStrategy(ptrEsmStrategy)
		, m_ptrEcmStrategy(ptrEcmStrategy)
		, m_PlatformSiteRelation(platformSiteRelations)
		, m_PlatformEmitterRelation(platformEmitterRelations)
		, m_PlatformWeaponRelation(platformWeaponRelations)
		, m_OwnPlatformEsmRelation(ownPlatformEsmRelations)
		, m_EsmEsmStrategyRelation(esmEsmStrategyRelations)
		, m_OwnPlatformEcmRelation(ownPlatformEcmRelations)
		, m_EcmEcmStrategyRelation(ecmEcmStrategyRelations)
		, m_OwnPlatformRouteRelation(ownPlatformRouteRelations)
	{
	}

	Vertex_ptr Scenario::getPtr2Vertex(const size_t & pos)
	{
		assert(pos<m_ptrVertex.size());
		if (pos<m_ptrVertex.size())
		{
			return m_ptrVertex[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}
	Platform_ptr Scenario::getPtr2Platform(const size_t & pos)
	{
		assert(pos < m_ptrPlatform.size());
		if (pos < m_ptrPlatform.size())
		{
			return m_ptrPlatform[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}
	Emitter_ptr Scenario::getPtr2Emitter(const size_t & pos)
	{
		assert(pos < m_ptrEmitter.size());
		if (pos < m_ptrEmitter.size())
		{
			return m_ptrEmitter[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}
	Weapon_ptr Scenario::getPtr2Weapon(const size_t & pos)
	{
		assert(pos < m_ptrWeapon.size());
		if (pos < m_ptrWeapon.size())
		{
			return m_ptrWeapon[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	Site_ptr Scenario::getPtr2Site(const size_t & pos)
	{
		assert(pos < m_ptrSite.size());
		if (pos < m_ptrSite.size())
		{
			return m_ptrSite[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	OwnPlatform_ptr Scenario::getPtr2OwnPlatform(const size_t & pos)
	{
		assert(pos < m_ptrOwnPlatform.size());
		if (pos < m_ptrOwnPlatform.size())
		{
			return m_ptrOwnPlatform[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	Esm_ptr Scenario::getPtr2Esm(const size_t & pos)
	{
		assert(pos < m_ptrEsm.size());
		if (pos < m_ptrEsm.size())
		{
			return m_ptrEsm[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	Ecm_ptr Scenario::getPtr2Ecm(const size_t & pos)
	{
		assert(pos < m_ptrEcm.size());
		if (pos < m_ptrEcm.size())
		{
			return m_ptrEcm[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	Route_ptr Scenario::getPtr2Route(const size_t & pos)
	{
		assert(pos < m_ptrRoute.size());
		if (pos < m_ptrRoute.size())
		{
			return m_ptrRoute[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	EsmStrategy_ptr Scenario::getPtr2EsmStrategy(const size_t & pos)
	{
		assert(pos < m_ptrEsmStrategy.size());
		if (pos < m_ptrEsmStrategy.size())
		{
			return m_ptrEsmStrategy[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	EcmStrategy_ptr Scenario::getPtr2EcmStrategy(const size_t & pos)
	{
		assert(pos < m_ptrEcmStrategy.size());
		if (pos < m_ptrEcmStrategy.size())
		{
			return m_ptrEcmStrategy[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	PlatformSiteRelation & Scenario::getPlatformSiteRelation(const size_t & pos)
	{
		assert(pos < m_PlatformSiteRelation.size());
		if (pos < m_PlatformSiteRelation.size())
		{
			return m_PlatformSiteRelation[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	PlatformEmitterRelation & Scenario::getPlatformEmitterRelation(const size_t & pos)
	{
		assert(pos < m_PlatformEmitterRelation.size());
		if (pos < m_PlatformEmitterRelation.size())
		{
			return m_PlatformEmitterRelation[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	PlatformWeaponRelation & Scenario::getPlatformWeaponRelation(const size_t & pos)
	{
		assert(pos < m_PlatformWeaponRelation.size());
		if (pos < m_PlatformWeaponRelation.size())
		{
			return m_PlatformWeaponRelation[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	OwnPlatformEsmRelation & Scenario::getOwnPlatformEsmRelation(const size_t & pos)
	{
		assert(pos < m_OwnPlatformEsmRelation.size());
		if (pos < m_OwnPlatformEsmRelation.size())
		{
			return m_OwnPlatformEsmRelation[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	EsmEsmStrategyRelation & Scenario::getEsmEsmStrategyRelation(const size_t & pos)
	{
		assert(pos < m_EsmEsmStrategyRelation.size());
		if (pos < m_EsmEsmStrategyRelation.size())
		{
			return m_EsmEsmStrategyRelation[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	OwnPlatformEcmRelation & Scenario::getOwnPlatformEcmRelation(const size_t & pos)
	{
		assert(pos < m_OwnPlatformEcmRelation.size());
		if (pos < m_OwnPlatformEcmRelation.size())
		{
			return m_OwnPlatformEcmRelation[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	EcmEcmStrategyRelation & Scenario::getEcmEcmStrategyRelation(const size_t & pos)
	{
		assert(pos < m_EcmEcmStrategyRelation.size());
		if (pos < m_EcmEcmStrategyRelation.size())
		{
			return m_EcmEcmStrategyRelation[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	OwnPlatformRouteRelation & Scenario::getOwnPlatformRouteRelation(const size_t & pos)
	{
		assert(pos < m_OwnPlatformRouteRelation.size());
		if (pos < m_OwnPlatformRouteRelation.size())
		{
			return m_OwnPlatformRouteRelation[pos];
		}
		throw Error("can't return this value (doesn't exist)");
	}

	VertexsVector& Scenario::getAllVertex(void)
	{
		return m_ptrVertex;
	}

	PlatformsVector& Scenario::getAllPlatform(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrPlatform;
	}

	EmittersVector& Scenario::getAllEmitter(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrEmitter;
	}

	WeaponsVector& Scenario::getAllWeapon(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrWeapon;
	}

	SitesVector& Scenario::getAllSite(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrSite;
	}

	OwnPlatformsVector& Scenario::getAllOwnPlatform(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrOwnPlatform;
	}

	EsmsVector& Scenario::getAllEsm(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrEsm;
	}

	EcmsVector& Scenario::getAllEcm(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrEcm;
	}

	RoutesVector& Scenario::getAllRoute(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrRoute;
	}

	EsmStrategyVector& Scenario::getAllEsmStrategy(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrEsmStrategy;
	}

	EcmStrategyVector& Scenario::getAllEcmStrategy(void)
	{
		// TODO: 在此处插入 return 语句
		return m_ptrEcmStrategy;
	}

	std::vector<PlatformSiteRelation>& Scenario::getAllPlatformSiteRelation(void)
	{
		// TODO: 在此处插入 return 语句
		return m_PlatformSiteRelation;
	}

	std::vector<PlatformEmitterRelation>& Scenario::getAllPlatformEmitterRelation(void)
	{
		// TODO: 在此处插入 return 语句
		return m_PlatformEmitterRelation;
	}

	std::vector<PlatformWeaponRelation>& Scenario::getAllPlatformWeaponRelation(void)
	{
		// TODO: 在此处插入 return 语句
		return m_PlatformWeaponRelation;
	}

	std::vector<OwnPlatformEsmRelation>& Scenario::getAllOwnPlatformEsmRelation(void)
	{
		// TODO: 在此处插入 return 语句
		return m_OwnPlatformEsmRelation;
	}

	std::vector<EsmEsmStrategyRelation>& Scenario::getAllEsmEsmStrategyRelation(void)
	{
		// TODO: 在此处插入 return 语句
		return m_EsmEsmStrategyRelation;
	}

	std::vector<OwnPlatformEcmRelation>& Scenario::getAllOwnPlatformEcmRelation(void)
	{
		// TODO: 在此处插入 return 语句
		return m_OwnPlatformEcmRelation;
	}

	std::vector<EcmEcmStrategyRelation>& Scenario::getAllEcmEcmStrategyRelation(void)
	{
		// TODO: 在此处插入 return 语句
		return m_EcmEcmStrategyRelation;
	}

	std::vector<OwnPlatformRouteRelation>& Scenario::getAllOwnPlatformRouteRelation(void)
	{
		// TODO: 在此处插入 return 语句
		return m_OwnPlatformRouteRelation;
	}

	bool Scenario::setPtr2Vertex(const size_t & pos, Vertex_ptr ptr2Entity)
	{
		assert(pos < m_ptrVertex.size());
		if (pos < m_ptrVertex.size())
		{
			m_ptrVertex[pos] = ptr2Entity;
			return true;
		}
		return false;
	}

	bool Scenario::setPtr2Platform(const size_t & pos, Platform_ptr ptr2Entity)
	{
		assert(pos < m_ptrPlatform.size());
		if (pos < m_ptrPlatform.size())
		{
			m_ptrPlatform[pos] = ptr2Entity;
			return true;
		}
		return false;
	}

	bool Scenario::setPtr2Emitter(const size_t & pos, Emitter_ptr ptr2Entity)
	{
		assert(pos < m_ptrEmitter.size());
		if (pos < m_ptrEmitter.size())
		{
			m_ptrEmitter[pos] = ptr2Entity;
			return true;
		}
		return false;
	}

	bool Scenario::setPtr2Weapon(const size_t & pos, Weapon_ptr ptr2Entity)
	{
		assert(pos < m_ptrWeapon.size());
		if (pos < m_ptrWeapon.size())
		{
			m_ptrWeapon[pos] = ptr2Entity;
			return true;
		}
		return false;
	}

	bool Scenario::setPtr2Site(const size_t & pos, Site_ptr ptr2Entity)
	{
		assert(pos < m_ptrSite.size());
		if (pos < m_ptrSite.size())
		{
			m_ptrSite[pos] = ptr2Entity;
			return true;
		}
		return false;
	}

	bool Scenario::setPtr2OwnPlatform(const size_t & pos, OwnPlatform_ptr ptr2Entity)
	{
		assert(pos < m_ptrOwnPlatform.size());
		if (pos < m_ptrOwnPlatform.size())
		{
			m_ptrOwnPlatform[pos] = ptr2Entity;
			return true;
		}
		return false;
	}

	bool Scenario::setPtr2Esm(const size_t & pos, Esm_ptr ptr2Entity)
	{
		assert(pos < m_ptrEsm.size());
		if (pos < m_ptrEsm.size())
		{
			m_ptrEsm[pos] = ptr2Entity;
			return true;
		}
		return false;
	}

	bool Scenario::setPtr2Ecm(const size_t & pos, Ecm_ptr ptr2Entity)
	{
		assert(pos < m_ptrEcm.size());
		if (pos < m_ptrEcm.size())
		{
			m_ptrEcm[pos] = ptr2Entity;
			return true;
		}
		return false;
	}

	bool Scenario::setPtr2Route(const size_t & pos, Route_ptr ptr2Entity)
	{
		assert(pos < m_ptrRoute.size());
		if (pos < m_ptrRoute.size())
		{
			m_ptrRoute[pos] = ptr2Entity;
			return true;
		}
		return false;
	}

	bool Scenario::setPtr2EsmStrategy(const size_t & pos, EsmStrategy_ptr ptr2Entity)
	{
		assert(pos < m_ptrEsmStrategy.size());
		if (pos < m_ptrEsmStrategy.size())
		{
			m_ptrEsmStrategy[pos] = ptr2Entity;
			return true;
		}
		return false;
	}

	bool Scenario::setPtr2EcmStrategy(const size_t & pos, EcmStrategy_ptr ptr2Entity)
	{
		assert(pos < m_ptrEcmStrategy.size());
		if (pos < m_ptrEcmStrategy.size())
		{
			m_ptrEcmStrategy[pos] = ptr2Entity;
			return true;
		}
		return false;
	}

	void Scenario::addVertex(Vertex_ptr ptr2Entity)
	{
		m_ptrVertex.push_back(ptr2Entity);
	}

	void Scenario::addPlatform(Platform_ptr ptr2Entity)
	{
		m_ptrPlatform.push_back(ptr2Entity);
	}

	void Scenario::addEmitter(Emitter_ptr ptr2Entity)
	{
		m_ptrEmitter.push_back(ptr2Entity);
	}

	void Scenario::addWeapon(Weapon_ptr ptr2Entity)
	{
		m_ptrWeapon.push_back(ptr2Entity);
	}

	void Scenario::addSite(Site_ptr ptr2Entity)
	{
		m_ptrSite.push_back(ptr2Entity);
	}

	void Scenario::addOwnPlatform(OwnPlatform_ptr ptr2Entity)
	{
		m_ptrOwnPlatform.push_back(ptr2Entity);
	}

	void Scenario::addEsm(Esm_ptr ptr2Entity)
	{
		m_ptrEsm.push_back(ptr2Entity);
	}

	void Scenario::addEcm(Ecm_ptr ptr2Entity)
	{
		m_ptrEcm.push_back(ptr2Entity);
	}

	void Scenario::addRoute(Route_ptr ptr2Entity)
	{
		m_ptrRoute.push_back(ptr2Entity);
	}

	void Scenario::addEsmStrategy(EsmStrategy_ptr ptr2Entity)
	{
		m_ptrEsmStrategy.push_back(ptr2Entity);
	}

	void Scenario::addEcmStrategy(EcmStrategy_ptr ptr2Entity)
	{
		m_ptrEcmStrategy.push_back(ptr2Entity);
	}

	bool Scenario::insertPtr2Vertex(const size_t & pos, Vertex_ptr ptr2Entity)
	{
		assert(pos <= m_ptrVertex.size());
		if (pos <= m_ptrVertex.size())
		{
			m_ptrVertex.insert(m_ptrVertex.begin() + pos, ptr2Entity);;
			return true;
		}
		return false;
	}

	bool Scenario::insertPtr2Platform(const size_t & pos, Platform_ptr ptr2Entity)
	{
		assert(pos <= m_ptrPlatform.size());
		if (pos <= m_ptrPlatform.size())
		{
			m_ptrPlatform.insert(m_ptrPlatform.begin() + pos, ptr2Entity);
			return true;
		}
		return false;
	}

	bool Scenario::insertPtr2Emitter(const size_t & pos, Emitter_ptr ptr2Entity)
	{
		assert(pos <= m_ptrEmitter.size());
		if (pos <= m_ptrEmitter.size())
		{
			m_ptrEmitter.insert(m_ptrEmitter.begin() + pos, ptr2Entity);
			return true;
		}
		return false;
	}

	bool Scenario::insertPtr2Weapon(const size_t & pos, Weapon_ptr ptr2Entity)
	{
		assert(pos <= m_ptrWeapon.size());
		if (pos <= m_ptrWeapon.size())
		{
			m_ptrWeapon.insert(m_ptrWeapon.begin() + pos, ptr2Entity);
			return true;
		}
		return false;
	}

	bool Scenario::insertPtr2Site(const size_t & pos, Site_ptr ptr2Entity)
	{
		assert(pos <= m_ptrSite.size());
		if (pos <= m_ptrSite.size())
		{
			m_ptrSite.insert(m_ptrSite.begin() + pos, ptr2Entity);
			return true;
		}
		return false;
	}

	bool Scenario::insertPtr2OwnPlatform(const size_t & pos, OwnPlatform_ptr ptr2Entity)
	{
		assert(pos <= m_ptrOwnPlatform.size());
		if (pos <= m_ptrOwnPlatform.size())
		{
			m_ptrOwnPlatform.insert(m_ptrOwnPlatform.begin() + pos, ptr2Entity);
			return true;
		}
		return false;
	}

	bool Scenario::insertPtr2Esm(const size_t & pos, Esm_ptr ptr2Entity)
	{
		assert(pos <= m_ptrEsm.size());
		if (pos <= m_ptrEsm.size())
		{
			m_ptrEsm.insert(m_ptrEsm.begin() + pos, ptr2Entity);
			return true;
		}
		return false;
	}

	bool Scenario::insertPtr2Ecm(const size_t & pos, Ecm_ptr ptr2Entity)
	{
		assert(pos <= m_ptrEcm.size());
		if (pos <= m_ptrEcm.size())
		{
			m_ptrEcm.insert(m_ptrEcm.begin() + pos, ptr2Entity);
			return true;
		}
		return false;
	}

	bool Scenario::insertPtr2Route(const size_t & pos, Route_ptr ptr2Entity)
	{
		assert(pos <= m_ptrRoute.size());
		if (pos <= m_ptrRoute.size())
		{
			m_ptrRoute.insert(m_ptrRoute.begin() + pos, ptr2Entity);
			return true;
		}
		return false;
	}

	bool Scenario::insertPtr2EsmStrategy(const size_t & pos, EsmStrategy_ptr ptr2Entity)
	{
		assert(pos <= m_ptrEsmStrategy.size());
		if (pos <= m_ptrEsmStrategy.size())
		{
			m_ptrEsmStrategy.insert(m_ptrEsmStrategy.begin() + pos, ptr2Entity);
			return true;
		}
		return false;
	}

	bool Scenario::insertPtr2EcmStrategy(const size_t & pos, EcmStrategy_ptr ptr2Entity)
	{
		assert(pos <= m_ptrEcmStrategy.size());
		if (pos <= m_ptrEcmStrategy.size())
		{
			m_ptrEcmStrategy.insert(m_ptrEcmStrategy.begin() + pos, ptr2Entity);
			return true;
		}
		return false;
	}

	bool Scenario::deleteVertex(const size_t & pos)
	{
		assert(pos < m_ptrVertex.size());
		if (pos < m_ptrVertex.size())
		{
			m_ptrVertex.erase(m_ptrVertex.begin() + pos);
			return true;
		}
		return false;
	}

	bool Scenario::deletePlatform(const size_t & pos)
	{
		assert(pos < m_ptrPlatform.size());
		if (pos < m_ptrPlatform.size())
		{
			m_ptrPlatform.erase(m_ptrPlatform.begin() + pos);
			return true;
		}
		return false;
	}

	bool Scenario::deleteEmitter(const size_t & pos)
	{
		assert(pos < m_ptrEmitter.size());
		if (pos < m_ptrEmitter.size())
		{
			m_ptrEmitter.erase(m_ptrEmitter.begin() + pos);
			return true;
		}
		return false;
	}

	bool Scenario::deleteWeapon(const size_t & pos)
	{
		assert(pos < m_ptrWeapon.size());
		if (pos < m_ptrWeapon.size())
		{
			m_ptrWeapon.erase(m_ptrWeapon.begin() + pos);
			return true;
		}
		return false;
	}

	bool Scenario::deleteSite(const size_t & pos)
	{
		assert(pos < m_ptrSite.size());
		if (pos < m_ptrSite.size())
		{
			m_ptrSite.erase(m_ptrSite.begin() + pos);
			return true;
		}
		return false;
	}

	bool Scenario::deleteOwnPlatform(const size_t & pos)
	{
		assert(pos < m_ptrOwnPlatform.size());
		if (pos < m_ptrOwnPlatform.size())
		{
			m_ptrOwnPlatform.erase(m_ptrOwnPlatform.begin() + pos);
			return true;
		}
		return false;
	}

	bool Scenario::deleteEsm(const size_t & pos)
	{
		assert(pos < m_ptrEsm.size());
		if (pos < m_ptrEsm.size())
		{
			m_ptrEsm.erase(m_ptrEsm.begin() + pos);
			return true;
		}
		return false;
	}

	bool Scenario::deleteEcm(const size_t & pos)
	{
		assert(pos < m_ptrEcm.size());
		if (pos < m_ptrEcm.size())
		{
			m_ptrEcm.erase(m_ptrEcm.begin() + pos);
			return true;
		}
		return false;
	}

	bool Scenario::deleteRoute(const size_t & pos)
	{
		assert(pos < m_ptrRoute.size());
		if (pos < m_ptrRoute.size())
		{
			m_ptrRoute.erase(m_ptrRoute.begin() + pos);
			return true;
		}
		return false;
	}

	bool Scenario::deleteEsmStrategy(const size_t & pos)
	{
		assert(pos < m_ptrEsmStrategy.size());
		if (pos < m_ptrEsmStrategy.size())
		{
			m_ptrEsmStrategy.erase(m_ptrEsmStrategy.begin() + pos);
			return true;
		}
		return false;
	}

	bool Scenario::deleteEcmStrategy(const size_t & pos)
	{
		assert(pos < m_ptrEcmStrategy.size());
		if (pos < m_ptrEcmStrategy.size())
		{
			m_ptrEcmStrategy.erase(m_ptrEcmStrategy.begin() + pos);
			return true;
		}
		return false;
	}

	bool Scenario::setPlatformSiteRelation(const size_t & pos, PlatformSiteRelation & relation)
	{
		assert(pos < m_PlatformSiteRelation.size());
		if (pos < m_PlatformSiteRelation.size())
		{
			m_PlatformSiteRelation[pos] = relation;
			return true;
		}
		return false;
	}

	bool Scenario::setPlatformEmitterRelation(const size_t & pos, PlatformEmitterRelation & relation)
	{
		assert(pos < m_PlatformEmitterRelation.size());
		if (pos < m_PlatformEmitterRelation.size())
		{
			m_PlatformEmitterRelation[pos] = relation;
			return true;
		}
		return false;
	}

	bool Scenario::setPlatformWeaponRelation(const size_t & pos, PlatformWeaponRelation & relation)
	{
		assert(pos < m_PlatformWeaponRelation.size());
		if (pos < m_PlatformWeaponRelation.size())
		{
			m_PlatformWeaponRelation[pos] = relation;
			return true;
		}
		return false;
	}

	bool Scenario::setOwnPlatformEsmRelation(const size_t & pos, OwnPlatformEsmRelation & relation)
	{
		assert(pos < m_OwnPlatformEsmRelation.size());
		if (pos < m_OwnPlatformEsmRelation.size())
		{
			m_OwnPlatformEsmRelation[pos] = relation;
			return true;
		}
		return false;
	}

	bool Scenario::setEsmEsmStrategyRelation(const size_t & pos, EsmEsmStrategyRelation & relation)
	{
		assert(pos < m_EsmEsmStrategyRelation.size());
		if (pos < m_EsmEsmStrategyRelation.size())
		{
			m_EsmEsmStrategyRelation[pos] = relation;
			return true;
		}
		return false;
	}

	bool Scenario::setOwnPlatformEcmRelation(const size_t & pos, OwnPlatformEcmRelation & relation)
	{
		assert(pos < m_OwnPlatformEcmRelation.size());
		if (pos < m_OwnPlatformEcmRelation.size())
		{
			m_OwnPlatformEcmRelation[pos] = relation;
			return true;
		}
		return false;
	}

	bool Scenario::setEcmEcmStrategyRelation(const size_t & pos, EcmEcmStrategyRelation & relation)
	{
		assert(pos < m_EcmEcmStrategyRelation.size());
		if (pos < m_EcmEcmStrategyRelation.size())
		{
			m_EcmEcmStrategyRelation[pos] = relation;
			return true;
		}
		return false;
	}

	bool Scenario::setOwnPlatformRouteRelation(const size_t & pos, OwnPlatformRouteRelation & relation)
	{
		assert(pos < m_OwnPlatformRouteRelation.size());
		if (pos < m_OwnPlatformRouteRelation.size())
		{
			m_OwnPlatformRouteRelation[pos] = relation;
			return true;
		}
		return false;
	}

	void Scenario::addPlatformSiteRelation(PlatformSiteRelation & relation)
	{
		m_PlatformSiteRelation.push_back(relation);
	}

	void Scenario::addPlatformEmitterRelation(PlatformEmitterRelation & relation)
	{
		m_PlatformEmitterRelation.push_back(relation);
	}

	void Scenario::addPlatformWeaponRelation(PlatformWeaponRelation & relation)
	{
		m_PlatformWeaponRelation.push_back(relation);
	}

	void Scenario::addOwnPlatformEsmRelation(OwnPlatformEsmRelation & relation)
	{
		m_OwnPlatformEsmRelation.push_back(relation);
	}

	void Scenario::addEsmEsmStrategyRelation(EsmEsmStrategyRelation & relation)
	{
		m_EsmEsmStrategyRelation.push_back(relation);
	}

	void Scenario::addOwnPlatformEcmRelation(OwnPlatformEcmRelation & relation)
	{
		m_OwnPlatformEcmRelation.push_back(relation);
	}

	void Scenario::addEcmEcmStrategyRelation(EcmEcmStrategyRelation & relation)
	{
		m_EcmEcmStrategyRelation.push_back(relation);
	}

	void Scenario::addOwnPlatformRouteRelation(OwnPlatformRouteRelation & relation)
	{
		m_OwnPlatformRouteRelation.push_back(relation);
	}

	bool Scenario::insertPlatformSiteRelation(const size_t & pos, PlatformSiteRelation & relation)
	{
		assert(pos <= m_PlatformSiteRelation.size());
		if (pos <= m_PlatformSiteRelation.size())
		{
			m_PlatformSiteRelation.insert(m_PlatformSiteRelation.begin() + pos, relation);
			return true;
		}
		return false;
	}

	bool Scenario::insertPlatformEmitterRelation(const size_t & pos, PlatformEmitterRelation & relation)
	{
		assert(pos <= m_PlatformEmitterRelation.size());
		if (pos <= m_PlatformEmitterRelation.size())
		{
			m_PlatformEmitterRelation.insert(m_PlatformEmitterRelation.begin() + pos, relation);
			return true;
		}
		return false;
	}

	bool Scenario::insertPlatformWeaponRelation(const size_t & pos, PlatformWeaponRelation & relation)
	{
		assert(pos <= m_PlatformWeaponRelation.size());
		if (pos <= m_PlatformWeaponRelation.size())
		{
			m_PlatformWeaponRelation.insert(m_PlatformWeaponRelation.begin() + pos, relation);
			return true;
		}
		return false;
	}

	bool Scenario::insertOwnPlatformEsmRelation(const size_t & pos, OwnPlatformEsmRelation & relation)
	{
		assert(pos <= m_OwnPlatformEsmRelation.size());
		if (pos <= m_OwnPlatformEsmRelation.size())
		{
			m_OwnPlatformEsmRelation.insert(m_OwnPlatformEsmRelation.begin() + pos, relation);
			return true;
		}
		return false;
	}

	bool Scenario::insertEsmEsmStrategyRelation(const size_t & pos, EsmEsmStrategyRelation & relation)
	{
		assert(pos <= m_EsmEsmStrategyRelation.size());
		if (pos <= m_EsmEsmStrategyRelation.size())
		{
			m_EsmEsmStrategyRelation.insert(m_EsmEsmStrategyRelation.begin() + pos, relation);
			return true;
		}
		return false;
	}

	bool Scenario::insertOwnPlatformEcmRelation(const size_t & pos, OwnPlatformEcmRelation & relation)
	{
		assert(pos <= m_OwnPlatformEcmRelation.size());
		if (pos <= m_OwnPlatformEcmRelation.size())
		{
			m_OwnPlatformEcmRelation.insert(m_OwnPlatformEcmRelation.begin() + pos, relation);
			return true;
		}
		return false;
	}

	bool Scenario::insertEcmEcmStrategyRelation(const size_t & pos, EcmEcmStrategyRelation & relation)
	{
		assert(pos <= m_EcmEcmStrategyRelation.size());
		if (pos <= m_EcmEcmStrategyRelation.size())
		{
			m_EcmEcmStrategyRelation.insert(m_EcmEcmStrategyRelation.begin() + pos, relation);
			return true;
		}
		return false;
	}

	bool Scenario::insertOwnPlatformRouteRelation(const size_t & pos, OwnPlatformRouteRelation & relation)
	{
		assert(pos <= m_OwnPlatformRouteRelation.size());
		if (pos <= m_OwnPlatformRouteRelation.size())
		{
			m_OwnPlatformRouteRelation.insert(m_OwnPlatformRouteRelation.begin() + pos, relation);
			return true;
		}
		return false;
	}

	bool Scenario::deletePlatformSiteRelation(const size_t & pos)
	{
		assert(pos < m_PlatformSiteRelation.size());
		if (pos < m_PlatformSiteRelation.size())
		{
			m_PlatformSiteRelation.erase(m_PlatformSiteRelation.begin() + pos);
			return true;
		}
		return false;
	}

	bool Scenario::deletePlatformEmitterRelation(const size_t & pos)
	{
		assert(pos < m_PlatformEmitterRelation.size());
		if (pos < m_PlatformEmitterRelation.size())
		{
			m_PlatformEmitterRelation.erase(m_PlatformEmitterRelation.begin() + pos);
			return true;
		}
		return false;
	}

	bool Scenario::deletePlatformWeaponRelation(const size_t & pos)
	{
		assert(pos < m_PlatformWeaponRelation.size());
		if (pos < m_PlatformWeaponRelation.size())
		{
			m_PlatformWeaponRelation.erase(m_PlatformWeaponRelation.begin() + pos);
			return true;
		}
		return false;
	}

	bool Scenario::deleteOwnPlatformEsmRelation(const size_t & pos)
	{
		assert(pos < m_OwnPlatformEsmRelation.size());
		if (pos < m_OwnPlatformEsmRelation.size())
		{
			m_OwnPlatformEsmRelation.erase(m_OwnPlatformEsmRelation.begin() + pos);
			return true;
		}
		return false;
	}

	bool Scenario::deleteEsmEsmStrategyRelation(const size_t & pos)
	{
		assert(pos < m_EsmEsmStrategyRelation.size());
		if (pos < m_EsmEsmStrategyRelation.size())
		{
			m_EsmEsmStrategyRelation.erase(m_EsmEsmStrategyRelation.begin() + pos);
			return true;
		}
		return false;
	}

	bool Scenario::deleteOwnPlatformEcmRelation(const size_t & pos)
	{
		assert(pos < m_OwnPlatformEcmRelation.size());
		if (pos < m_OwnPlatformEcmRelation.size())
		{
			m_OwnPlatformEcmRelation.erase(m_OwnPlatformEcmRelation.begin() + pos);
			return true;
		}
		return false;
	}

	bool Scenario::deleteEcmEcmStrategyRelation(const size_t & pos)
	{
		assert(pos < m_EcmEcmStrategyRelation.size());
		if (pos < m_EcmEcmStrategyRelation.size())
		{
			m_EcmEcmStrategyRelation.erase(m_EcmEcmStrategyRelation.begin() + pos);
			return true;
		}
		return false;
	}

	bool Scenario::deleteOwnPlatformRouteRelation(const size_t & pos)
	{
		assert(pos < m_OwnPlatformRouteRelation.size());
		if (pos < m_OwnPlatformRouteRelation.size())
		{
			m_OwnPlatformRouteRelation.erase(m_OwnPlatformRouteRelation.begin() + pos);
			return true;
		}
		return false;
	}

	void Scenario::setAllVertex(VertexsVector& ptr2AllEntities)
	{
		m_ptrVertex = ptr2AllEntities;
	}

	void Scenario::setAllPlatform(PlatformsVector& ptr2AllEntities)
	{
		m_ptrPlatform = ptr2AllEntities;
	}

	void Scenario::setAllEmitter(EmittersVector& ptr2AllEntities)
	{
		m_ptrEmitter = ptr2AllEntities;
	}

	void Scenario::setAllWeapon(WeaponsVector& ptr2AllEntities)
	{
		m_ptrWeapon = ptr2AllEntities;
	}

	void Scenario::setAllSite(SitesVector& ptr2AllEntities)
	{
		m_ptrSite = ptr2AllEntities;
	}

	void Scenario::setAllOwnPlatform(OwnPlatformsVector& ptr2AllEntities)
	{
		m_ptrOwnPlatform = ptr2AllEntities;
	}

	void Scenario::setAllEsm(EsmsVector& ptr2AllEntities)
	{
		m_ptrEsm = ptr2AllEntities;
	}

	void Scenario::setAllEcm(EcmsVector& ptr2AllEntities)
	{
		m_ptrEcm = ptr2AllEntities;
	}

	void Scenario::setAllRoute(RoutesVector& ptr2AllEntities)
	{
		m_ptrRoute = ptr2AllEntities;
	}

	void Scenario::setAllEsmStrategy(EsmStrategyVector& ptr2AllEntities)
	{
		m_ptrEsmStrategy = ptr2AllEntities;
	}

	void Scenario::setAllEcmStrategy(EcmStrategyVector& ptr2AllEntities)
	{
		m_ptrEcmStrategy = ptr2AllEntities;
	}

	void Scenario::setAllPlatformSiteRelation(std::vector<PlatformSiteRelation>& ptr2AllEntities)
	{
		m_PlatformSiteRelation = ptr2AllEntities;
	}

	void Scenario::setAllPlatformEmitterRelation(std::vector<PlatformEmitterRelation>& ptr2AllEntities)
	{
		m_PlatformEmitterRelation = ptr2AllEntities;
	}

	void Scenario::setAllPlatformWeaponRelation(std::vector<PlatformWeaponRelation>& ptr2AllEntities)
	{
		m_PlatformWeaponRelation = ptr2AllEntities;
	}

	void Scenario::setAllOwnPlatformEsmRelation(std::vector<OwnPlatformEsmRelation>& ptr2AllEntities)
	{
		m_OwnPlatformEsmRelation = ptr2AllEntities;
	}

	void Scenario::setAllEsmEsmStrategyRelation(std::vector<EsmEsmStrategyRelation>& ptr2AllEntities)
	{
		m_EsmEsmStrategyRelation = ptr2AllEntities;
	}

	void Scenario::setAllOwnPlatformEcmRelation(std::vector<OwnPlatformEcmRelation>& ptr2AllEntities)
	{
		m_OwnPlatformEcmRelation = ptr2AllEntities;
	}

	void Scenario::setAllEcmEcmStrategyRelation(std::vector<EcmEcmStrategyRelation>& ptr2AllEntities)
	{
		m_EcmEcmStrategyRelation = ptr2AllEntities;
	}

	void Scenario::setAllOwnPlatformRouteRelation(std::vector<OwnPlatformRouteRelation>& ptr2AllEntities)
	{
		m_OwnPlatformRouteRelation = ptr2AllEntities;
	}

}
