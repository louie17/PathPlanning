/****************************************************************
*	Copyright(c)2019-2020 四川大学电子信息学院智能控制研究所
*	All right reserved.
*
*	文件名称：Scenario.hpp
*	功能：提供XML解析数据封装类声明
*	当前版本：1.0
*	作者：韩露
*	完成日期：2019-11-3
******************************************************************/

#ifndef __SCENARIO__
#define __SCENARIO__

#include<stdexcept>
#include<string>
#include<vector>
#include<memory>
#include<type_traits>
#include<cassert>
#include<unordered_map>

namespace sce
{
	class Vertex;
	class Platform;
	class Rf_values;
	class Rf;
	class Pw_values;
	class Pw;
	class Pri_values;
	class Pri;
	class Scan;
	class Erp;
	class Radar_Mode;
	class Emitter;
	class Weapon;
	class Site;
	class Point;
	class Mission;
	class OwnPlatform;
	class Esm;
	class Ecm;
	class WayPoint;
	class Route;
	class Location;
	class DwellSquence;
	class EsmStrategySection;
	class EsmStrategy;
	class EcmStrategySection;
	class EcmStrategy;
	class PlatformSiteRelation;
	class PlatformEmitterRelation;
	class PlatformWeaponRelation;
	class OwnPlatformEsmRelation;
	class EsmEsmStrategyRelation;
	class OwnPlatformEcmRelation;
	class EcmEcmStrategyRelation;
	class OwnPlatformRouteRelation;

	typedef std::shared_ptr<sce::Vertex > Vertex_ptr;
	typedef std::vector<Vertex_ptr> VertexsVector;

	typedef std::shared_ptr<sce::Platform > Platform_ptr;
	typedef std::vector<Platform_ptr> PlatformsVector;

	typedef std::shared_ptr<sce::Emitter> Emitter_ptr;
	typedef std::vector<Emitter_ptr> EmittersVector;

	typedef std::shared_ptr<sce::Weapon> Weapon_ptr;
	typedef std::vector<Weapon_ptr> WeaponsVector;

	typedef std::shared_ptr<sce::Site> Site_ptr;
	typedef std::vector<Site_ptr> SitesVector;

	typedef std::unordered_map<Site_ptr, double> Site_WeaponRange_relation;

	//site-emitter键值对，无序且允许重复关键值
	typedef std::unordered_multimap<Site_ptr, Emitter_ptr> Site_Emitter_relation;

	//site-weapon键值对，无序且允许重复关键值
	typedef std::unordered_multimap<Site_ptr, Weapon_ptr> Site_Weapon_relation;

	typedef std::shared_ptr<sce::OwnPlatform> OwnPlatform_ptr;
	typedef std::vector<OwnPlatform_ptr> OwnPlatformsVector;

	typedef std::shared_ptr<sce::Esm> Esm_ptr;
	typedef std::vector<Esm_ptr> EsmsVector;

	typedef std::shared_ptr<sce::Ecm> Ecm_ptr;
	typedef std::vector<Ecm_ptr> EcmsVector;

	typedef std::shared_ptr<sce::Route> Route_ptr;
	typedef std::vector<Route_ptr> RoutesVector;

	typedef std::shared_ptr<sce::EsmStrategy> EsmStrategy_ptr;
	typedef std::vector<EsmStrategy_ptr> EsmStrategyVector;

	typedef std::shared_ptr<sce::EcmStrategy> EcmStrategy_ptr;
	typedef std::vector<EcmStrategy_ptr> EcmStrategyVector;


	class Error : public std::runtime_error
	{
	public:
		Error(const std::string &msg)
			:std::runtime_error(std::string("Scenario:").append(msg))
		{}

	private:

	};

	class Scenario
	{
	public:
		Scenario(void);
		Scenario(Vertex_ptr, Platform_ptr, Emitter_ptr,	Weapon_ptr,	Site_ptr, OwnPlatform_ptr, Esm_ptr, Ecm_ptr, Route_ptr,	EsmStrategy_ptr, EcmStrategy_ptr,
			PlatformSiteRelation&,
			PlatformEmitterRelation&,
			PlatformWeaponRelation&,
			OwnPlatformEsmRelation&,
			EsmEsmStrategyRelation&,
			OwnPlatformEcmRelation&,
			EcmEcmStrategyRelation&,
			OwnPlatformRouteRelation&);

		Scenario(VertexsVector&, PlatformsVector&, EmittersVector&, WeaponsVector&, SitesVector&, OwnPlatformsVector&, EsmsVector&, EcmsVector&, RoutesVector&,
			EsmStrategyVector&,	EcmStrategyVector&,
			std::vector<PlatformSiteRelation>&,
			std::vector<PlatformEmitterRelation>&,
			std::vector<PlatformWeaponRelation>&,
			std::vector<OwnPlatformEsmRelation>&,
			std::vector<EsmEsmStrategyRelation>&,
			std::vector<OwnPlatformEcmRelation>&,
			std::vector<EcmEcmStrategyRelation>&,
			std::vector<OwnPlatformRouteRelation>&);		

		bool isVertexEmpty(void) { return m_ptrVertex.empty() ? true : false; }
		bool isPlatformEmpty(void) { return m_ptrPlatform.empty() ? true : false; }
		bool isEmitterEmpty(void) { return m_ptrEmitter.empty() ? true : false; }
		bool isWeaponEmpty(void) { return m_ptrWeapon.empty() ? true : false; }
		bool isSiteEmpty(void) { return m_ptrSite.empty() ? true : false; }
		bool isOwnPlatformEmpty(void) { return m_ptrOwnPlatform.empty() ? true : false; }
		bool isEsmEmpty(void) { return m_ptrEsm.empty() ? true : false; }
		bool isEcmEmpty(void) { return m_ptrEcm.empty() ? true : false; }
		bool isRouteEmpty(void) { return m_ptrRoute.empty() ? true : false; }
		bool isEsmStrategyEmpty(void) { return m_ptrEsmStrategy.empty() ? true : false; }
		bool isEcmStrategyEmpty(void) { return m_ptrEcmStrategy.empty() ? true : false; }

		bool isPlatformSiteRelationEmpty(void) { return m_PlatformSiteRelation.empty() ? true : false; }
		bool isPlatformEmitterRelationEmpty(void) { return m_PlatformEmitterRelation.empty() ? true : false; }
		bool isPlatformWeaponRelationEmpty(void) { return m_PlatformWeaponRelation.empty() ? true : false; }
		bool isOwnPlatformEsmRelationEmpty(void) { return m_OwnPlatformEsmRelation.empty() ? true : false; }
		bool isEsmEsmStrategyRelationEmpty(void) { return m_EsmEsmStrategyRelation.empty() ? true : false; }
		bool isOwnPlatformEcmRelationEmpty(void) { return m_OwnPlatformEcmRelation.empty() ? true : false; }
		bool isEcmEcmStrategyRelationEmpty(void) { return m_EcmEcmStrategyRelation.empty() ? true : false; }
		bool isOwnPlatformRouteRelationEmpty(void) { return m_OwnPlatformRouteRelation.empty() ? true : false; }

		Vertex_ptr getPtr2Vertex(const size_t& pos);
		Platform_ptr getPtr2Platform(const size_t& pos);
		Emitter_ptr getPtr2Emitter(const size_t& pos);
		Weapon_ptr getPtr2Weapon(const size_t& pos);
		Site_ptr getPtr2Site(const size_t& pos);
		OwnPlatform_ptr getPtr2OwnPlatform(const size_t& pos);
		Esm_ptr getPtr2Esm(const size_t& pos);
		Ecm_ptr getPtr2Ecm(const size_t& pos);
		Route_ptr getPtr2Route(const size_t& pos);
		EsmStrategy_ptr getPtr2EsmStrategy(const size_t& pos);
		EcmStrategy_ptr getPtr2EcmStrategy(const size_t& pos);

		PlatformSiteRelation& getPlatformSiteRelation(const size_t& pos);
		PlatformEmitterRelation& getPlatformEmitterRelation(const size_t& pos);
		PlatformWeaponRelation& getPlatformWeaponRelation(const size_t& pos);
		OwnPlatformEsmRelation& getOwnPlatformEsmRelation(const size_t& pos);
		EsmEsmStrategyRelation& getEsmEsmStrategyRelation(const size_t& pos);
		OwnPlatformEcmRelation& getOwnPlatformEcmRelation(const size_t& pos);
		EcmEcmStrategyRelation& getEcmEcmStrategyRelation(const size_t& pos);
		OwnPlatformRouteRelation& getOwnPlatformRouteRelation(const size_t& pos);

		VertexsVector& getAllVertex(void);
		PlatformsVector& getAllPlatform(void);
		EmittersVector& getAllEmitter(void);
		WeaponsVector& getAllWeapon(void);
		SitesVector& getAllSite(void);
		OwnPlatformsVector& getAllOwnPlatform(void);
		EsmsVector& getAllEsm(void);
		EcmsVector& getAllEcm(void);
		RoutesVector& getAllRoute(void);
		EsmStrategyVector& getAllEsmStrategy(void);
		EcmStrategyVector& getAllEcmStrategy(void);

		std::vector<PlatformSiteRelation>& getAllPlatformSiteRelation(void);
		std::vector<PlatformEmitterRelation>& getAllPlatformEmitterRelation(void);
		std::vector<PlatformWeaponRelation>& getAllPlatformWeaponRelation(void);
		std::vector<OwnPlatformEsmRelation>& getAllOwnPlatformEsmRelation(void);
		std::vector<EsmEsmStrategyRelation>& getAllEsmEsmStrategyRelation(void);
		std::vector<OwnPlatformEcmRelation>& getAllOwnPlatformEcmRelation(void);
		std::vector<EcmEcmStrategyRelation>& getAllEcmEcmStrategyRelation(void);
		std::vector<OwnPlatformRouteRelation>& getAllOwnPlatformRouteRelation(void);

		bool setPtr2Vertex(const size_t& pos, Vertex_ptr ptr2Entity);
		bool setPtr2Platform(const size_t& pos, Platform_ptr ptr2Entity);
		bool setPtr2Emitter(const size_t& pos, Emitter_ptr ptr2Entity);
		bool setPtr2Weapon(const size_t& pos, Weapon_ptr ptr2Entity);
		bool setPtr2Site(const size_t& pos, Site_ptr ptr2Entity);
		bool setPtr2OwnPlatform(const size_t& pos, OwnPlatform_ptr ptr2Entity);
		bool setPtr2Esm(const size_t& pos, Esm_ptr ptr2Entity);
		bool setPtr2Ecm(const size_t& pos, Ecm_ptr ptr2Entity);
		bool setPtr2Route(const size_t& pos, Route_ptr ptr2Entity);
		bool setPtr2EsmStrategy(const size_t& pos, EsmStrategy_ptr ptr2Entity);
		bool setPtr2EcmStrategy(const size_t& pos, EcmStrategy_ptr ptr2Entity);

		void addVertex(Vertex_ptr ptr2Entity);
		void addPlatform(Platform_ptr ptr2Entity);
		void addEmitter(Emitter_ptr ptr2Entity);
		void addWeapon(Weapon_ptr ptr2Entity);
		void addSite(Site_ptr ptr2Entity);
		void addOwnPlatform(OwnPlatform_ptr ptr2Entity);
		void addEsm(Esm_ptr ptr2Entity);
		void addEcm(Ecm_ptr ptr2Entity);
		void addRoute(Route_ptr ptr2Entity);
		void addEsmStrategy(EsmStrategy_ptr ptr2Entity);
		void addEcmStrategy(EcmStrategy_ptr ptr2Entity);

		bool insertPtr2Vertex(const size_t& pos, Vertex_ptr ptr2Entity);
		bool insertPtr2Platform(const size_t& pos, Platform_ptr ptr2Entity);
		bool insertPtr2Emitter(const size_t& pos, Emitter_ptr ptr2Entity);
		bool insertPtr2Weapon(const size_t& pos, Weapon_ptr ptr2Entity);
		bool insertPtr2Site(const size_t& pos, Site_ptr ptr2Entity);
		bool insertPtr2OwnPlatform(const size_t& pos, OwnPlatform_ptr ptr2Entity);
		bool insertPtr2Esm(const size_t& pos, Esm_ptr ptr2Entity);
		bool insertPtr2Ecm(const size_t& pos, Ecm_ptr ptr2Entity);
		bool insertPtr2Route(const size_t& pos, Route_ptr ptr2Entity);
		bool insertPtr2EsmStrategy(const size_t& pos, EsmStrategy_ptr ptr2Entity);
		bool insertPtr2EcmStrategy(const size_t& pos, EcmStrategy_ptr ptr2Entity);

		bool deleteVertex(const size_t& pos);
		bool deletePlatform(const size_t& pos);
		bool deleteEmitter(const size_t& pos);
		bool deleteWeapon(const size_t& pos);
		bool deleteSite(const size_t& pos);
		bool deleteOwnPlatform(const size_t& pos);
		bool deleteEsm(const size_t& pos);
		bool deleteEcm(const size_t& pos);
		bool deleteRoute(const size_t& pos);
		bool deleteEsmStrategy(const size_t& pos);
		bool deleteEcmStrategy(const size_t& pos);

		bool setPlatformSiteRelation(const size_t& pos, PlatformSiteRelation& relation);
		bool setPlatformEmitterRelation(const size_t& pos, PlatformEmitterRelation& relation);
		bool setPlatformWeaponRelation(const size_t& pos, PlatformWeaponRelation& relation);
		bool setOwnPlatformEsmRelation(const size_t& pos, OwnPlatformEsmRelation& relation);
		bool setEsmEsmStrategyRelation(const size_t& pos, EsmEsmStrategyRelation& relation);
		bool setOwnPlatformEcmRelation(const size_t& pos, OwnPlatformEcmRelation& relation);
		bool setEcmEcmStrategyRelation(const size_t& pos, EcmEcmStrategyRelation& relation);
		bool setOwnPlatformRouteRelation(const size_t& pos, OwnPlatformRouteRelation& relation);

		void addPlatformSiteRelation(PlatformSiteRelation& relation);
		void addPlatformEmitterRelation(PlatformEmitterRelation& relation);
		void addPlatformWeaponRelation(PlatformWeaponRelation& relation);
		void addOwnPlatformEsmRelation(OwnPlatformEsmRelation& relation);
		void addEsmEsmStrategyRelation(EsmEsmStrategyRelation& relation);
		void addOwnPlatformEcmRelation(OwnPlatformEcmRelation& relation);
		void addEcmEcmStrategyRelation(EcmEcmStrategyRelation& relation);
		void addOwnPlatformRouteRelation(OwnPlatformRouteRelation& relation);

		bool insertPlatformSiteRelation(const size_t& pos, PlatformSiteRelation& relation);
		bool insertPlatformEmitterRelation(const size_t& pos, PlatformEmitterRelation& relation);
		bool insertPlatformWeaponRelation(const size_t& pos, PlatformWeaponRelation& relation);
		bool insertOwnPlatformEsmRelation(const size_t& pos, OwnPlatformEsmRelation& relation);
		bool insertEsmEsmStrategyRelation(const size_t& pos, EsmEsmStrategyRelation& relation);
		bool insertOwnPlatformEcmRelation(const size_t& pos, OwnPlatformEcmRelation& relation);
		bool insertEcmEcmStrategyRelation(const size_t& pos, EcmEcmStrategyRelation& relation);
		bool insertOwnPlatformRouteRelation(const size_t& pos, OwnPlatformRouteRelation& relation);

		bool deletePlatformSiteRelation(const size_t& pos);
		bool deletePlatformEmitterRelation(const size_t& pos);
		bool deletePlatformWeaponRelation(const size_t& pos);
		bool deleteOwnPlatformEsmRelation(const size_t& pos);
		bool deleteEsmEsmStrategyRelation(const size_t& pos);
		bool deleteOwnPlatformEcmRelation(const size_t& pos);
		bool deleteEcmEcmStrategyRelation(const size_t& pos);
		bool deleteOwnPlatformRouteRelation(const size_t& pos);

		void setAllVertex(VertexsVector& ptr2AllEntities);
		void setAllPlatform(PlatformsVector& ptr2AllEntities);
		void setAllEmitter(EmittersVector& ptr2AllEntities);
		void setAllWeapon(WeaponsVector& ptr2AllEntities);
		void setAllSite(SitesVector& ptr2AllEntities);
		void setAllOwnPlatform(OwnPlatformsVector& ptr2AllEntities);
		void setAllEsm(EsmsVector& ptr2AllEntities);
		void setAllEcm(EcmsVector& ptr2AllEntities);
		void setAllRoute(RoutesVector& ptr2AllEntities);
		void setAllEsmStrategy(EsmStrategyVector& ptr2AllEntities);
		void setAllEcmStrategy(EcmStrategyVector& ptr2AllEntities);

		void setAllPlatformSiteRelation(std::vector<PlatformSiteRelation>& ptr2AllEntities);
		void setAllPlatformEmitterRelation(std::vector<PlatformEmitterRelation>& ptr2AllEntities);
		void setAllPlatformWeaponRelation(std::vector<PlatformWeaponRelation>& ptr2AllEntities);
		void setAllOwnPlatformEsmRelation(std::vector<OwnPlatformEsmRelation>& ptr2AllEntities);
		void setAllEsmEsmStrategyRelation(std::vector<EsmEsmStrategyRelation>& ptr2AllEntities);
		void setAllOwnPlatformEcmRelation(std::vector<OwnPlatformEcmRelation>& ptr2AllEntities);
		void setAllEcmEcmStrategyRelation(std::vector<EcmEcmStrategyRelation>& ptr2AllEntities);
		void setAllOwnPlatformRouteRelation(std::vector<OwnPlatformRouteRelation>& ptr2AllEntities);

	private:
		VertexsVector m_ptrVertex;
		//由于Platform中name数据成员为string这种可变长类型，不应直接在容器中放置类对象，而应该使用指针
		PlatformsVector m_ptrPlatform;
		EmittersVector m_ptrEmitter;
		WeaponsVector m_ptrWeapon;
		SitesVector m_ptrSite;

		OwnPlatformsVector m_ptrOwnPlatform;
		EsmsVector m_ptrEsm;
		EcmsVector m_ptrEcm;
		RoutesVector m_ptrRoute;
		EsmStrategyVector m_ptrEsmStrategy;
		EcmStrategyVector m_ptrEcmStrategy;

		std::vector<PlatformSiteRelation> m_PlatformSiteRelation;
		std::vector<PlatformEmitterRelation> m_PlatformEmitterRelation;
		std::vector<PlatformWeaponRelation> m_PlatformWeaponRelation;
		std::vector<OwnPlatformEsmRelation> m_OwnPlatformEsmRelation;
		std::vector<EsmEsmStrategyRelation> m_EsmEsmStrategyRelation;
		std::vector<OwnPlatformEcmRelation> m_OwnPlatformEcmRelation;
		std::vector<EcmEcmStrategyRelation> m_EcmEcmStrategyRelation;
		std::vector<OwnPlatformRouteRelation> m_OwnPlatformRouteRelation;

		Site_Emitter_relation m_seRelation;
		Site_Weapon_relation m_swRelation;

	public:

		void insertSiteWeaponRelation(Site_ptr siteptr, Weapon_ptr weaponptr) { m_swRelation.insert(std::make_pair(siteptr,weaponptr)); };
		void insertSiteEmitterRelation(Site_ptr siteptr, Emitter_ptr emitterptr) { m_seRelation.insert(std::make_pair(siteptr, emitterptr)); };

		Site_Weapon_relation SiteWeaponRelation() { return m_swRelation; }
		Site_Emitter_relation SiteEmitterRelation() { return m_seRelation; }
	};

	class Vertex
	{
	public:

		Vertex(void);
		Vertex(const double &latitude, const double &longitude);
		Vertex(const double &latitude, const double &longitude, const double &altitude);

		//get latitude and longitude
		const double& getLatitude(void) const;
		const double& getLongitude(void) const;
		const double& getAltitude(void) const;

		//set latitude and longitute
		void setLatitude(const double &);
		void setLongitude(const double &);
		void setAltitude(const double &);

	private:
		double m_latitude, m_longitude,m_altitude;
	};

	enum class PlatformType
	{
		LAND = 0,
		AIR = 1,
		SURFACE = 2
	};


	class Platform
	{
	public:
		//请注意，数据成员中自定义类虽然都有默认构造函数，但本质上没有完全实例化类对象，
		//由于充满不确定性，调用默认构造可能会引起一些未知错误；
		//此构造函数用于编译器隐含调用，使用者请不要使用默认构造函数
		Platform(void);
		Platform(const std::string &name, const PlatformType &type);

		//get PlatformName and PlatformType
		const std::string& getPlatformName(void) const;
		const PlatformType& getPlatformType(void) const;

		//set PlatformName and PlatformType
		void setPlatformName(const std::string &);
		void setPlatformType(const PlatformType &);
	private:
		std::string m_name;
		PlatformType m_type;
	};

	class Rf_values
	{
	public:
		Rf_values(void);

		Rf_values(const size_t &pOrder, const size_t &numOfPulses, const size_t &min, const size_t &max, const size_t &sweepTime);

		//get method
		const size_t& getPhaseOrder(void) const;
		const size_t& getNumberOfPulses(void) const;
		const size_t& getMin(void) const;
		const size_t& getMax(void) const;
		const size_t& getSweepTime(void) const;

		//set method
		void setPhaseOrder(const size_t &);
		void setNumberOfPulses(const size_t &);
		void setMin(const size_t &);
		void setMax(const size_t &);
		void setSweepTime(const size_t &);

	private:
		size_t m_phaseOrder;
		size_t m_numberOfPulses;
		size_t m_min;
		size_t m_max;
		size_t m_sweepTime;
	};

	enum class RfType
	{
		BURST = 0,
		FIX = 1
	};

	class Rf
	{
	public:
		//请注意，数据成员中自定义类虽然都有默认构造函数，但本质上没有完全实例化类对象，
		//由于充满不确定性，调用默认构造可能会引起一些未知错误；
		//此构造函数用于编译器隐含调用，使用者请不要使用默认构造函数
		Rf(void);
		Rf(const size_t &rfmin, const size_t &rfmax);
		Rf(const size_t &rfmin, const size_t &rfmax, const RfType &type);

		Rf(const size_t &rfmin, const size_t &rfmax, const Rf_values &rfValues, const RfType &type);
		Rf(const size_t &rfmin, const size_t &rfmax, const std::vector<Rf_values> &rfValues, const RfType &type);

		//Rf(const Rf& rf);
		//Rf& operator=(const Rf& rf);

		//get mehtod
		const size_t& getRfMin(void) const;
		const size_t& getRfMax(void) const;
		const RfType& getRfType(void) const;

		//获取容纳Pri_values的容器对象
		std::vector<Rf_values>& getAllRfValues(void);

		//判断Pri_values值不为空
		bool isValuesEmpty();

		//根据values的索引值获取不同values的对象。
		//在获取values前应判断容器是否为空
		Rf_values& getRfValue(const size_t& valueOrder);

		//add Rf_values
		void addRfValues(const Rf_values &);

		//Values值设置/替换
		bool setRfValues(const size_t& pos, const Rf_values &);

		//insert Rf_values
		bool insertRfValues(const size_t& pos, const Rf_values &);

		//delete Rf_values
		bool deleteRfValues(const size_t& pos);

		//set method
		void setRfMin(const size_t &);
		void setRfMax(const size_t &);
		void setRfType(const RfType &);

		//赋值覆盖，可用此成员函数覆盖住默认构造函数初始化的对象
		void setAllRfValues(const std::vector<Rf_values>&);

	private:
		size_t m_min;
		size_t m_max;
		RfType m_type;
		std::vector<Rf_values> m_values;

	public:
		//const Rf_values &operator[](size_t valuesOrder) const;
	};

	class Pw_values
	{
	public:

		Pw_values(void);
		Pw_values(const size_t & pOrder, const size_t & numOfPulses, const size_t & min, const size_t & max, const size_t & sweepTime);

		//get method
		const size_t& getPhaseOrder(void) const;
		const size_t& getNumberOfPulses(void) const;
		const size_t& getMin(void) const;
		const size_t& getMax(void) const;
		const size_t& getSweepTime(void) const;

		//set method
		void setPhaseOrder(const size_t &);
		void setNumberOfPulses(const size_t &);
		void setMin(const size_t &);
		void setMax(const size_t &);
		void setSweepTime(const size_t &);

	private:
		size_t m_phaseOrder;
		size_t m_numberOfPulses;
		size_t m_min;
		size_t m_max;
		size_t m_sweepTime;
	};

	enum class PwType
	{
		FIX = 0
	};

	class Pw
	{
	public:
		//请注意，数据成员中自定义类虽然都有默认构造函数，但本质上没有完全实例化类对象，
		//由于充满不确定性，调用默认构造可能会引起一些未知错误；
		//此构造函数用于编译器隐含调用，使用者请不要使用默认构造函数
		Pw(void);
		Pw(const size_t &pwmin, const size_t &pwmax);
		Pw(const size_t &pwmin, const size_t &pwmax, const PwType &type);

		Pw(const size_t &pwmin, const size_t &pwmax, const Pw_values &pwValues, const PwType &type);
		Pw(const size_t &pwmin, const size_t &pwmax, const std::vector<Pw_values > &pwValues, const PwType &type);

		//get mehtod
		const size_t& getPwMin(void) const;
		const size_t& getPwMax(void) const;
		const PwType& getPwType(void) const;

		//获取容纳Pri_values的容器对象
		std::vector<Pw_values>& getAllPwValues(void);

		//判断Pri_values值不为空
		bool isValuesEmpty();

		//根据values的索引值获取不同values的对象。
		//在获取values前应判断容器是否为空
		Pw_values& getPwValue(size_t valueOrder);

		//add Pw_values
		void addPriValues(Pw_values &);

		//Values值设置/替换
		bool setPwValues(const size_t& pos, const Pw_values &);

		//insert Pw_values
		bool insertPwValues(size_t pos, Pw_values &);

		//delete Pw_values
		bool deletePwValues(size_t pos);

		//set method
		void setPwMin(const size_t &);
		void setPwMax(const size_t &);
		void setPwType(const PwType &);

		//赋值覆盖，可用此成员函数覆盖住默认构造函数初始化的对象
		void setAllPwValues(const std::vector<Pw_values>&);

	private:
		size_t m_min;
		size_t m_max;
		PwType m_type;
		std::vector<Pw_values> m_values;

	public:
		//const Pw_values &operator[](size_t valuesOrder) const;
	};

	class Pri_values
	{
	public:
		Pri_values(void);
		Pri_values(const size_t &pOrder, const size_t &numOfPulses, const size_t &min, const size_t &max, const size_t &sweepTime);

		//get method
		const size_t& getPhaseOrder(void) const;
		const size_t& getNumberOfPulses(void) const;
		const size_t& getMin(void) const;
		const size_t& getMax(void) const;
		const size_t& getSweepTime(void) const;

		//set method
		void setPhaseOrder(const size_t &);
		void setNumberOfPulses(const size_t &);
		void setMin(const size_t &);
		void setMax(const size_t &);
		void setSweepTime(const size_t &);

	private:
		size_t m_phaseOrder;
		size_t m_numberOfPulses;
		size_t m_min;
		size_t m_max;
		size_t m_sweepTime;
	};

	enum class PriType
	{
		STAGGER = 0,
		SWITCHING = 1,
		FIX = 2
	};

	class Pri
	{
	public:
		//请注意，数据成员中自定义类虽然都有默认构造函数，但本质上没有完全实例化类对象，
		//由于充满不确定性，调用默认构造可能会引起一些未知错误；
		//此构造函数用于编译器隐含调用，使用者请不要使用默认构造函数
		Pri(void);
		Pri(const size_t &primin, const size_t &primax);
		Pri(const size_t &primin, const size_t &primax, const PriType &type);
		Pri(const size_t &primin, const size_t &primax, const Pri_values &priValues, const PriType &type);
		Pri(const size_t &primin, const size_t &primax, const std::vector<Pri_values> &priValues, const PriType &type);

		//get mehtod
		const size_t& getPriMin(void) const;
		const size_t& getPriMax(void) const;
		const PriType& getPriType(void) const;

		//获取容纳Pri_values的容器对象
		std::vector<Pri_values>& getAllPriValues(void);

		//判断Pri_values值不为空
		bool isValuesEmpty();

		//根据Pri_value的顺序获取不同Pri_values的对象。
		//在获取Pri_value前应判断容器是否为空
		Pri_values& getPriValue(size_t valueOrder);

		//set method
		void setPriMin(const size_t &);
		void setPriMax(const size_t &);
		void setPriType(const PriType &);

		//add Pri_values
		void addPriValues(Pri_values &);

		//Values值设置/替换
		bool setPriValues(const size_t& pos, const Pri_values &);

		//insert Pri_values
		bool insertPriValues(size_t pos, Pri_values &);

		//delete Pri_values
		bool deletePriValues(size_t pos);

		//赋值覆盖，可用此成员函数覆盖住默认构造函数初始化的对象
		void setAllPriValues(const std::vector<Pri_values>&);

	private:
		size_t m_min;
		size_t m_max;
		PriType m_type;
		std::vector<Pri_values> m_values;

	public:
		//const Pri_values &operator[](size_t valuesOrder) const;
	};

	enum class ScanType
	{
		SECTORIAL = 0
	};

	class Scan
	{
	public:

		Scan(void);
		Scan(const size_t &max, const size_t &min, const ScanType &type);

		//get mehtod
		const size_t& getScanMin(void) const;
		const size_t& getScanMax(void) const;
		const ScanType& getScanType(void) const;

		//set method
		void setScanMin(const size_t &);
		void setScanMax(const size_t &);
		void setScanType(const ScanType &);

	private:
		size_t m_min;
		size_t m_max;
		ScanType m_type;
	};

	class Erp
	{
	public:

		Erp(void);
		Erp(const size_t &min, const size_t &max);

		//get mehtod
		const size_t& getErpMin(void) const;
		const size_t& getErpMax(void) const;

		//set method
		void setErpMin(const size_t &);
		void setErpMax(const size_t &);


	private:
		size_t m_min;
		size_t m_max;
	};

	enum class ModeType
	{
		PULSE = 0
	};

	class Radar_Mode
	{
	public:
		//请注意，数据成员中自定义类虽然都有默认构造函数，但本质上没有完全实例化类对象，
		//由于充满不确定性，调用默认构造可能会引起一些未知错误；
		//此构造函数用于编译器隐含调用，使用者请不要使用默认构造函数
		Radar_Mode(void);

		Radar_Mode(const std::string &modeCode, const ModeType &modeType, const Rf &rf, const Pw &pw, const Pri &pri, const Scan &scan, const Erp &erp);

		//get mehtod
		const std::string& getModeCode(void) const;
		const ModeType& getModeType(void) const;
		Rf& getRf(void);
		Pw& getPw(void);
		Pri& getPri(void);
		Scan& getScan(void);
		Erp& getErp(void);

		//set method
		void setModeCode(const std::string &);
		void setModeType(const ModeType &);
		void setRf(Rf &);
		void setPw(Pw &);
		void setPri(Pri &);
		void setScan(const Scan &);
		void setErp(const Erp &);

	private:
		std::string m_modeCode;//直接初始化
		ModeType m_modeType;
		Rf m_rf;
		Pw m_pw;
		Pri m_pri;
		Scan m_scan;
		Erp m_erp;
	};

	typedef std::shared_ptr<sce::Radar_Mode> radarmode_ptr;
	typedef std::vector<radarmode_ptr> radarmodeVector;

	class Emitter
	{
	public:
		//请注意，数据成员中自定义类虽然都有默认构造函数，但本质上没有完全实例化类对象，
		//由于充满不确定性，调用默认构造可能会引起一些未知错误；
		//此构造函数用于编译器隐含调用，使用者请不要使用默认构造函数
		Emitter(void);
		Emitter(const std::string &name);
		Emitter(const std::string &name, std::shared_ptr<Radar_Mode> ptrRadarMode);
		Emitter(const std::string &name, std::vector<std::shared_ptr<Radar_Mode>> &ptrRadarModes);

		//get method
		const std::string& getName(void) const;

		//获取RadarMode容器对象
		std::vector<std::shared_ptr<Radar_Mode>>& getAllPtr2RadarModes(void);

		bool isRadarModeEmpty(void);

		//按索引获取RadarMode
		const std::shared_ptr<Radar_Mode> getPtr2RadarMode(size_t valueOrder) const;

		//add method
		void addRadarMode(const std::shared_ptr<Radar_Mode>);

		//按照索引设置/修改RadarMode
		bool setPtr2RadarMode(size_t& pos, std::shared_ptr<Radar_Mode>);

		//insert
		bool insertRadarMode(size_t& pos, std::shared_ptr<Radar_Mode>);

		//delete
		bool deleteRadarMode(size_t& pos);

		//set method
		void setName(const std::string& name);

		//整体设置修改RadarMode
		void setAllPtr2RadarModes(std::vector<std::shared_ptr<Radar_Mode>>&);

	private:
		std::string m_name{ "Emitter1" };
		std::vector<std::shared_ptr<Radar_Mode>> m_ptrRadarMode;
	};

	class Weapon
	{
	public:
		//不用使用标准容器类直接装带有string类型成员数据的类对象，而应该是该对象的指针
		Weapon(void);
		Weapon(const std::string& name, const size_t& cepr, const size_t& weaponAreaCoverage);

		const std::string& getName(void) const;
		const size_t& getCEPR(void) const;
		const size_t& getWeaponAreaCoverage(void) const;

		void setName(const std::string&);
		void setCEPR(const size_t&);
		void setWeaponAreaCoverage(const size_t&);


	private:
		std::string m_name;
		size_t m_cepr;
		size_t m_weaponAreaCoverage;
	};

	class Site
	{
	public:
		//不用使用标准容器类直接装带有string类型成员数据的类对象，而应该是该对象的指针
		Site(void);
		Site(const std::string &name, const double &altitude, const double &latitude, const double &longitude);

		//get latitude and longitude
		const std::string& getName(void) const;
		const double& getAltitude(void) const;
		const double& getLatitude(void) const;
		const double& getLongitude(void) const;

		//set latitude and longitute
		void setName(const std::string&);
		void setAltitude(const double &);
		void setLatitude(const double &);
		void setLongitude(const double &);

	private:
		std::string m_name;
		double m_altitude;
		double m_latitude;
		double m_longitude;
	};

	//重载自定义类Site的==比较操作符，默认当altitude,latitude,longitude三者相等时为同一个Site.
	//bool operator==(const Site &lsite, const Site &rsite)
	//{
	//	return lsite.getAltitude() == rsite.getAltitude()
	//		&& lsite.getLatitude() == rsite.getLatitude()
	//		&& lsite.getLongitude() == rsite.getLongitude();
	//}

	class Point
	{
	public:
		Point(void);
		Point(const double &altitude, const double &latitude, const double &longitude);
		Point(const double &altitude, const double &latitude, const double &longitude, const double &tmin, const double &tmax);

		//get latitude and longitude
		const double& getAltitude(void) const;
		const double& getLatitude(void) const;
		const double& getLongitude(void) const;
		const double& getTmin(void) const;
		const double& getTmax(void) const;

		//set latitude and longitute
		void setAltitude(const double &);
		void setLatitude(const double &);
		void setLongitude(const double &);
		void setTmin(const double &);
		void setTmax(const double &);

	private:
		double m_altitude;
		double m_latitude;
		double m_longitude;
		double m_tmin;
		double m_tmax;
	};

	enum class MissionType
	{
		STRIKE = 0,
		SUPPORT = 1
	};

	class Mission
	{
	public:
		//请注意，数据成员中自定义类虽然都有默认构造函数，但本质上没有完全实例化类对象，
		//由于充满不确定性，调用默认构造可能会引起一些未知错误；
		//此构造函数用于编译器隐含调用，使用者请不要使用默认构造函数
		Mission(void);

		//Mission Type:Strike
		Mission(const MissionType &missionType, const Point &startPoint, const Point &endPoint, const std::vector<Point>&targetPoint);
		Mission(const MissionType &missionType, const Point &startPoint, const Point &endPoint, const Point&targetPoint);

		//Mission Type:Support
		//此构造函数应该在MissionType为Support时才用此构造函数，但一如前面所说，不保证安全性
		Mission(const MissionType &missionType, const Point &startPoint, const Point &endPoint);

		const MissionType& getMissionType(void) const;
		Point& getStartPoint(void);
		Point& getEndPoint(void);

		//获取容纳所有TargetPoints的容器对象
		std::vector<Point>& getAllTargetPoints(void);

		bool isTargetPointEmpty(void);

		//返回非常量引用可以连续调用成员变量类的成员函数
		Point& getTargetPoint(const size_t& pos);

		void addTargetPoint(const Point&);

		bool setTargetPoint(const size_t& pos, const Point&);

		bool insertTargetPoint(const size_t& pos, const Point&);

		bool deleteTargetPoint(const size_t& pos);

		void setMissionType(const MissionType&);
		void setStartPoint(const Point&);
		void setEndPoint(const Point&);
		void setAllTargetPoints(const std::vector<Point>&);


	private:
		MissionType m_type;
		Point m_startPoint;
		Point m_endPoint;
		std::vector<Point> m_targetPoints;
	};

	enum class OwnPlatformType
	{
		AIR = 0

	};

	class OwnPlatform
	{
	public:
		OwnPlatform(void);
		OwnPlatform(const std::string &name,
			const OwnPlatformType &ownPlatformType,
			const double &maxAcceleration,
			const double &maxDeceleration,
			const double &maxClimbRate,
			const double &maxDiveRate,
			const double &maxSpeed,
			const double &maxTurnRadius);

		OwnPlatform(const std::string &name,
			const OwnPlatformType &ownPlatformType,
			const double &maxAcceleration,
			const double &maxDeceleration,
			const double &maxClimbRate,
			const double &maxDiveRate,
			const double &maxSpeed,
			const double &maxTurnRadius,
			const Mission &mission);

		const std::string& getName(void) const;
		const OwnPlatformType& getType(void) const;
		const double& getMaxAcceleration(void)const;
		const double& getMaxDeceleration(void)const;
		const double& getMaxClimbRate(void)const;
		const double& getMaxDiveRate(void)const;
		const double& getMaxSpeed(void)const;
		const double& getMaxTurnRadius(void)const;

		Mission& getMission(void);

		void setName(const std::string&);
		void setType(const OwnPlatformType&);
		void setMaxAcceleration(const double&);
		void setMaxDeceleration(const double&);
		void setMaxClimbRate(const double&);
		void setMaxDiveRate(const double&);
		void setMaxSpeed(const double&);
		void setMaxTurnRadius(const double&);
		void setMission(const Mission&);


	private:
		std::string m_name;
		OwnPlatformType m_type;
		double m_maxAcceleration;
		double m_maxDeceleration;
		double m_maxClimbRate;
		double m_maxDiveRate;
		double m_maxSpeed;
		double m_maxTurnRadius;
		Mission m_mission;
	};

	class Esm
	{
	public:
		Esm(void);
		Esm(const std::string &name,
			const double &dwellFreqResolution,
			const size_t &tuningStep,
			const size_t &rfCovMin,
			const size_t &rfCovMax,
			const size_t &numPulsesAcquisition,
			const size_t &numPulsesAlarm);

		Esm(const std::string &name,
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
			const double &Aeff);

		const std::string& getName(void);
		const double& getDwellFreqResolution(void);
		const size_t& getTuningStep(void);
		const size_t& getRfCovMin(void);
		const size_t& getRfCovMax(void);
		const size_t& getNumPulsesAcquisition(void);
		const size_t& getNumPulsesAlarm(void);
		const double& getesmMinDwellTime(void) { return m_esmMinDwellTime; }
		const size_t& getnEmitter(void) { return m_nEmitter; }
		const double& gettaoScan(void) { return m_taoScan; }
		const double& getPmin(void) { return m_Pmin; }
		const double& getAeff(void) { return m_Aeff; }

		void setName(const std::string&);
		void setDwellFreqResolution(const double&);
		void setTuningStep(const size_t&);
		void setRfCovMin(const size_t&);
		void setRfCovMax(const size_t&);
		void setNumPulsesAcquisition(const size_t&);
		void setNumPulsesAlarm(const size_t&);
		void setesmMinDwellTime(const double&);
		void setnEmitter(const int&);
		void settaoScan(const double&);
		void setPmin(const double&);
		void setAeff(const double&);

	private:
		std::string m_name;
		double m_dwellFreqResolution;
		size_t m_tuningStep;
		size_t m_rfCovMin;
		size_t m_rfCovMax;
		size_t m_numPulsesAcquisition;
		size_t m_numPulsesAlarm;
		double m_esmMinDwellTime;
		size_t m_nEmitter;
		double m_taoScan;
		double m_Pmin;
		double m_Aeff;
	};
	
	enum class Tech
	{
		NOISE = 0,
		VGPO = 1,
		FT = 2,
		RGPO = 3
	};

	class Ecm
	{
	public:
		Ecm(void);
		Ecm(const std::string &name, const size_t &pt, const size_t &gain, const size_t &rfMin, const size_t &rfMax);
		Ecm(const std::string &name, const size_t &pt, const size_t &gain, const size_t &rfmin, const size_t &rfmax, const Tech& techName);
		Ecm(const std::string &name, const size_t &pt, const size_t &gain, const size_t &rfmin, const size_t &rfmax, const std::vector<Tech> &techName);
		Ecm(const std::string &name, const size_t &pt, const size_t &gain, const size_t &rfmin, const size_t &rfmax, const std::vector<Tech> &techName,	const double &radarMSR, const double &jammerERP_support, const double &platformRCS, const double &radarDangerValue, const double &jammerERP_attack);
		const std::string& getName(void);
		const size_t& getPt(void);
		const size_t& getGain(void);
		const size_t& getRfMin(void);
		const size_t& getRfMax(void);
		const double& getjammerERP_attack(void) { return m_jammerERP_attack; }
		const double& getradarDangerValue(void) { return m_radarDangerValue; }
		const double& getplatformRCS(void) { return m_platformRCS; }
		const double& getjammerERP_support(void) { return m_jammerERP_support; }
		const double & getradarMSR(void) { return m_radarMSR; }

		//获取装填所有Tech的容器对象
		std::vector<Tech>& getAllTechs(void);

		//判断装填所有Tech的容器中是否为空
		bool isTechEmpty(void);

		//根据索引值找寻对应的Tech
		const Tech& getTech(const size_t& pos);

		//add Tech
		void addTech(const Tech& techname);

		//对单个Tech进行设置，更改
		bool setTech(const size_t& pos, const Tech& techname);

		//insert Tech
		bool insertTech(const size_t& pos, const Tech& techname);

		//delete Tech
		bool deleteTech(const size_t& pos);

		//set method
		void setName(const std::string& name);
		void setPt(const size_t& pt);
		void setGain(const size_t& gain);
		void setRfMin(const size_t& rfMin);
		void setRfMax(const size_t& rfMax);
		void setradarMSR(const double &radarMSR);
		void setjammerERP_support(const double &jammerERP_support);
		void setplatformRCS(const double &platformRCS);
		void setradarDangerValue(const double&radarDangerValue);
		void setjammerERP_attack(const double &jammerERP_attack);

		//以容器为单位，整体赋值修改
		void setAllTechs(const std::vector<Tech>& techs);

	private:
		std::string m_name;
		size_t m_pt;
		size_t m_gain;
		size_t m_rfMin;
		size_t m_rfMax;
		std::vector<Tech> m_techName;
		double m_radarMSR;
		double m_jammerERP_support;
		double m_platformRCS;
		double m_radarDangerValue;
		double m_jammerERP_attack;
	};

	class WayPoint
	{
	public:
		WayPoint(void);
		WayPoint(const double &longitude, const double &latitude, const double &altitude);
		WayPoint(const size_t&index, const double &longitude, const double &latitude, const double &altitude);
		WayPoint(const size_t&index, const double &longitude, const double &latitude, const double &altitude, const double &time, const double&velocity, const double &acceleration);

		//get latitude and longitude
		const size_t& getIndex(void) const;
		const double& getAltitude(void) const;
		const double& getLatitude(void) const;
		const double& getLongitude(void) const;
		const double& getTime(void) const;
		const double& getVelocity(void) const;
		const double& getAcceleration(void) const;

		//set latitude and longitute
		void setIndex(const size_t &);
		void setAltitude(const double &);
		void setLatitude(const double &);
		void setLongitude(const double &);
		void setTime(const double &);
		void setVelocity(const double&);
		void setAcceleration(const double&);

	private:
		double m_altitude;
		double m_latitude;
		double m_longitude;
		double m_time;
		double m_velocity;
		double m_acceleration;
		size_t m_index;
	};

	class Route
	{
	public:
		Route(void);
		Route(const std::string& name);
		Route(const std::string&name, const WayPoint& wayPoint);
		Route(const std::string&name, const std::vector<WayPoint>& wayPoints);

		//get name
		const std::string& getName(void);

		bool isWayPointEmpty(void);

		//get waypoint by index
		WayPoint& getWayPoint(const size_t&);

		//get all waypoints
		std::vector<WayPoint>& getAllWayPoints(void);

		//add waypoint
		void addWayPoint(const WayPoint&);

		//insert waypoint by position
		bool insertWayPoint(const size_t&, const WayPoint&);

		//set/moditify waypoint by index
		bool setWayPoint(const size_t&, const WayPoint&);

		//detele waypoint by index
		bool deleteWayPoint(const size_t&);

		//set name
		void setName(const std::string&);

		//set/moditify WayPoints
		void setAllWayPoints(const std::vector<WayPoint>&);

	private:
		std::string m_name;
		std::vector<WayPoint> m_wayPoints;
	};

	class Location
	{
	public:
		Location(void);
		Location(const double& longitude, const double& latitude, const double& altitude);

		//get latitude and longitude
		const double& getAltitude(void) const;
		const double& getLatitude(void) const;
		const double& getLongitude(void) const;

		//set latitude and longitute
		void setAltitude(const double &);
		void setLatitude(const double &);
		void setLongitude(const double &);

	private:
		double m_altitude;
		double m_latitude;
		double m_longitude;
	};

	class DwellSquence
	{
	public:
		DwellSquence(void);
		DwellSquence(const size_t &index, const size_t &minFreq, const size_t &maxFreq, const double &startTime, const double &endTime);

		//get method;
		const size_t& getIndex(void);
		const size_t& getMinFreq(void);
		const size_t& getMaxFreq(void);
		const double& getStartTime(void);
		const double& getEndTime(void);

		//set method;
		void setIndex(const size_t&);
		void setMinFreq(const size_t&);
		void setMaxFreq(const size_t&);
		void setStartTime(const double&);
		void setEndTime(const double&);

	private:
		size_t m_index;
		size_t m_minFreq;
		size_t m_maxFreq;
		double m_startTime;
		double m_endTime;
	};

	class EsmStrategySection
	{
	public:
		EsmStrategySection(void);
		EsmStrategySection(const double &startTime, const double &endTime, const Location &startLocation, const Location &endLocation);
		EsmStrategySection(const double &startTime,	const double &endTime, const Location &startLocation, const Location &endLocation, const DwellSquence &dwellsquence);
		EsmStrategySection(const double &startTime, const double &endTime, const Location &startLocation, const Location &endLocation, const std::vector<DwellSquence> &dwellsquences);

		//get method
		const double& getStartTime(void);
		const double& getEndTime(void);
		Location& getStartLocation(void);
		Location& getEndLocation(void);

		bool isDwellSquenceEmpty(void);

		//get DwellSquence by index
		DwellSquence& getDwellSquence(const size_t& index);

		//get all DwellSquences
		std::vector<DwellSquence>& getAllDwellSquences(void);

		//add DwellSquence
		void addDwellSquence(const DwellSquence&);

		//insert DwellSquence
		bool insertDwellSquence(const size_t& pos, const DwellSquence&);

		//set/moditify DwellSquence
		bool setDwellSquence(const size_t& pos, const DwellSquence&);

		//delete DwellSquence
		bool deleteDwellSquence(const size_t& index);

		//set/moditify all DwellSquence
		void setDwellSquences(const std::vector<DwellSquence>&);

		void setStartTime(const double&);
		void setEndTime(const double&);
		void setStartLocation(const Location&);
		void setEndLocation(const Location&);

	private:
		double m_startTime;
		double m_endTime;
		Location m_startLocation;
		Location m_endLocation;
		std::vector<DwellSquence> m_dwellSquences;
	};

	class EsmStrategy
	{
	public:
		EsmStrategy(void);
		EsmStrategy(const std::string &name);
		EsmStrategy(const std::string &name, const std::shared_ptr<EsmStrategySection>&);
		EsmStrategy(const std::string &name, const std::vector<std::shared_ptr<EsmStrategySection>>&);

		//get method
		const std::string& getName(void) const;

		//获取EsmStrategySection指针容器对象
		std::vector<std::shared_ptr<EsmStrategySection>>& getAllPtr2Sections(void);

		bool isSectionEmpty(void);

		//按索引获取EsmStrategySection
		const std::shared_ptr<EsmStrategySection> getPtr2Section(size_t& pos) const;

		//add EsmStrategySection
		void addSection(const std::shared_ptr<EsmStrategySection>);

		//按照索引设置/修改EsmStrategySection
		bool setPtr2Section(size_t& pos, std::shared_ptr<EsmStrategySection>);

		//insert EsmStrategySection
		bool insertSection(size_t& pos, std::shared_ptr<EsmStrategySection>);

		//delete EsmStrategySection
		bool deleteSection(size_t& pos);

		//set name
		void setName(const std::string& name);

		//整体设置修改EsmStrategySection指针容器对象
		void setAllPtr2Sections(std::vector<std::shared_ptr<EsmStrategySection>>&);

	private:
		std::string m_name;
		std::vector<std::shared_ptr<EsmStrategySection>> m_ptrSections;

	};

	class EcmStrategySection
	{
	public:
		EcmStrategySection(void);
		EcmStrategySection(const double &startTime, const double &endTime, const Location &startLocation, const Location &endLocation);
		EcmStrategySection(const double &startTime, const double &endTime, const Location &startLocation, const Location &endLocation, const Tech &tech);

		//get prop
		const double& getStartTime(void);
		const double& getEndTime(void);
		Location& getStartLocation(void);
		Location& getEndLocation(void);
		const Tech& getTechName(void);

		//set prop
		void setStartTime(const double&);
		void setEndTime(const double&);
		void setStartLocation(const Location&);
		void setEndLocation(const Location&);
		void setTechName(const Tech&);

	private:
		double m_startTime;
		double m_endTime;
		Location m_startLocation;
		Location m_endLocation;
		Tech m_tech;
	};

	class EcmStrategy
	{
	public:
		EcmStrategy(void);
		EcmStrategy(const std::string&);
		EcmStrategy(const std::string &name, const std::shared_ptr<EcmStrategySection>&ptrEcmStrategySection);
		EcmStrategy(const std::string &name, const std::vector<std::shared_ptr<EcmStrategySection>>& ptrEcmStrategySections);

		//get method
		const std::string& getName(void) const;

		//获取EcmStrategySection指针容器对象
		std::vector<std::shared_ptr<EcmStrategySection>>& getAllPtr2Sections(void);

		//判断EcmStrategySection指针容器对象是否为空
		bool isSectionEmpty(void);

		//按索引获取EsmStrategySection
		const std::shared_ptr<EcmStrategySection> getPtr2Section(size_t& pos) const;

		//add EsmStrategySection
		void addSection(const std::shared_ptr<EcmStrategySection>);

		//按照索引设置/修改EsmStrategySection
		bool setPtr2Section(size_t& pos, std::shared_ptr<EcmStrategySection>);

		//insert EsmStrategySection
		bool insertSection(size_t& pos, std::shared_ptr<EcmStrategySection>);

		//delete EsmStrategySection
		bool deleteSection(size_t& pos);

		//set name
		void setName(const std::string& name);

		//整体设置修改EsmStrategySection指针容器对象
		void setAllPtr2Sections(std::vector<std::shared_ptr<EcmStrategySection>>&);

	private:
		std::string m_name;
		std::vector<std::shared_ptr<EcmStrategySection>> m_ptrSections;
	};

	class PlatformSiteRelation
	{
	public:
		//关联关系不应存在默认值
		PlatformSiteRelation(void);
		PlatformSiteRelation(const Platform_ptr, const Site_ptr);

		//get name;
		const std::string& getPlatformName(void);
		const std::string& getSiteName(void);

		Site_ptr getSite(void);
		Platform_ptr getPlatform(void);

		//set/moditify pointer of Object
		void setPlatform(const Platform_ptr);
		void setSite(const Site_ptr);


	private:
		Platform_ptr m_ptrPlatform;
		Site_ptr m_ptrSite;
	};

	class PlatformEmitterRelation
	{
	public:
		PlatformEmitterRelation(void);
		PlatformEmitterRelation(const Platform_ptr, const Emitter_ptr);

		//get name;
		const std::string& getPlatformName(void);
		const std::string& getEmitterName(void);

		Platform_ptr getPlatform(void);
		Emitter_ptr getEmitter(void);

		//set/moditify pointer of Object
		void setPlatform(const Platform_ptr);
		void setEmitter(const Emitter_ptr);

	private:
		Platform_ptr m_ptrPlatform;
		Emitter_ptr m_ptrEmitter;
	};

	class PlatformWeaponRelation
	{
	public:
		PlatformWeaponRelation(void);
		PlatformWeaponRelation(const Platform_ptr, const Weapon_ptr);

		//get name;
		const std::string& getPlatformName(void);
		const std::string& getWeaponName(void);

		Platform_ptr getPlatform(void);
		Weapon_ptr getWeapon(void);

		//set/moditify pointer of Object
		void setPlatform(const Platform_ptr);
		void setWeapon(const Weapon_ptr);

	private:
		std::shared_ptr<Platform> m_ptrPlatform;
		Weapon_ptr m_ptrWeapon;
	};

	class OwnPlatformEsmRelation
	{
	public:
		OwnPlatformEsmRelation(void);
		OwnPlatformEsmRelation(const OwnPlatform_ptr, const Esm_ptr);

		//get name;
		const std::string& getOwnPlatformName(void);
		const std::string& getEsmName(void);

		OwnPlatform_ptr getOwnPlatform(void);
		Esm_ptr getEsm(void);

		//set/moditify pointer of Object
		void setOwnPlatform(const OwnPlatform_ptr);
		void setEsm(const Esm_ptr);

	private:
		OwnPlatform_ptr m_ptrOwnPlatform;
		Esm_ptr m_ptrEsm;
	};

	class EsmEsmStrategyRelation
	{
	public:
		EsmEsmStrategyRelation(void);
		EsmEsmStrategyRelation(const Esm_ptr, const EsmStrategy_ptr);

		//get name;
		const std::string& getEsmName(void);
		const std::string& getEsmStrategyName(void);

		EsmStrategy_ptr getEsmStrategy(void);
		Esm_ptr getEsm(void);

		//set/moditify pointer of Object
		void setEsm(const Esm_ptr);
		void setEsmStrategy(const EsmStrategy_ptr);

	private:
		Esm_ptr m_ptrEsm;
		EsmStrategy_ptr m_ptrEsmStrategy;
	};

	class OwnPlatformEcmRelation
	{
	public:
		OwnPlatformEcmRelation(void);
		OwnPlatformEcmRelation(const OwnPlatform_ptr, const Ecm_ptr);

		//get name;
		const std::string& getOwnPlatformName(void);
		const std::string& getEcmName(void);

		OwnPlatform_ptr getOwnPlatform(void);
		Ecm_ptr getEcm(void);

		//set/moditify pointer of Object
		void setOwnPlatform(const OwnPlatform_ptr);
		void setEcm(const Ecm_ptr);

	private:
		OwnPlatform_ptr m_ptrOwnPlatform;
		Ecm_ptr m_ptrEcm;
	};

	class EcmEcmStrategyRelation
	{
	public:
		EcmEcmStrategyRelation(void);
		EcmEcmStrategyRelation(const Ecm_ptr, const EcmStrategy_ptr);

		//get name;
		const std::string& getEcmName(void);
		const std::string& getEcmStrategyName(void);

		EcmStrategy_ptr getEcmStrategy(void);
		Ecm_ptr getEcm(void);

		//set/moditify pointer of Object
		void setEcm(const Ecm_ptr);
		void setEcmStrategy(const EcmStrategy_ptr);

	private:
		Ecm_ptr m_ptrEcm;
		EcmStrategy_ptr m_ptrEcmStrategy;
	};

	class OwnPlatformRouteRelation
	{
	public:
		OwnPlatformRouteRelation(void);
		OwnPlatformRouteRelation(const OwnPlatform_ptr, const Route_ptr);

		//get name;
		const std::string& getOwnPlatformName(void);
		const std::string& getRouteName(void);

		OwnPlatform_ptr getOwnPlatform(void);
		Route_ptr getRoute(void);

		//set/moditify pointer of Object
		void setOwnPlatform(const OwnPlatform_ptr);
		void setRoute(const Route_ptr);

	private:
		OwnPlatform_ptr m_ptrOwnPlatform;
		Route_ptr m_ptrRoute;
	};

}

#endif // !__SCENARIO__
