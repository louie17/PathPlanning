#include "xml_prase.h"

using namespace sce;

bool find_Vertex_data(const QDomElement &qd,sce::Scenario &scenario)
{
	if ((qd.tagName() == "Vertex") && (qd.parentNode().nodeName() == "Scenario")) {
		Vertex vx;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Latitude") {
				vx.setLatitude(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "Longitude") {
				vx.setLongitude(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "Altitude") {
				vx.setAltitude(qdd.toElement().text().toDouble());
			}
		}
		scenario.addVertex(std::make_shared<sce::Vertex>(vx));
		return true;
	}
	return false;
}

bool find_Platform_data(const QDomElement &qd,sce::Scenario &scenario)
{
	if ((qd.tagName() == "Platform") && (qd.parentNode().nodeName() == "Scenario")) {
		Platform pf;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Name") {
				pf.setPlatformName(qdd.toElement().text().toStdString());
			}
			if (qdd.nodeName() == "Type") {
				if (qdd.toElement().text().toStdString() == "Land")
					pf.setPlatformType(PlatformType(0));
				if (qdd.toElement().text().toStdString() == "Air")
					pf.setPlatformType(PlatformType(1));
				if (qdd.toElement().text().toStdString() == "Surface")
					pf.setPlatformType(PlatformType(2));
			}
		}
		scenario.addPlatform(std::make_shared<sce::Platform>(pf));
		return true;
	}
	return false;
}

bool find_Esm_data(const QDomElement &qd,sce::Scenario &scenario)
{
	if ((qd.tagName() == "Esm") && (qd.parentNode().nodeName() == "Scenario")) {
		Esm es;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Name") {
				es.setName(qdd.toElement().text().toStdString());
			}
			if (qdd.nodeName() == "Dwell_Freq_Resolution") {
				es.setDwellFreqResolution(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "Tuning_Step") {
				es.setTuningStep(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "RfCovmin") {
				es.setRfCovMin(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "RfCovmax") {
				es.setRfCovMax(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "Num_Pulses_Acquisition") {
				es.setNumPulsesAcquisition(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "Num_Pulses_Alarm") {
				es.setNumPulsesAlarm(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "Pmin") {
				es.setPmin(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "Aeff") {
				es.setAeff(qdd.toElement().text().toDouble());
			}

		}
		scenario.addEsm(std::make_shared<sce::Esm>(es));
		return true;
	}
	return false;
}

bool find_Ecm_data(const QDomElement &qd,sce::Scenario &scenario)
{
	if ((qd.tagName() == "Ecm") && (qd.parentNode().nodeName() == "Scenario")) {
		Ecm ec;
		std::vector<Tech> vtech;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Name") {
				ec.setName(qdd.toElement().text().toStdString());
			}
			if (qdd.nodeName() == "Pt") {
				ec.setPt(qdd.toElement().text().toInt());
			}
			if (qdd.nodeName() == "Gain") {
				ec.setGain(qdd.toElement().text().toInt());
			}
			if (qdd.nodeName() == "Rfmin") {
				ec.setRfMin(qdd.toElement().text().toInt());
			}
			if (qdd.nodeName() == "Rfmax") {
				ec.setRfMax(qdd.toElement().text().toInt());
			}
			if (qdd.nodeName() == "tech") {
				Tech temp;
				if (qdd.firstChildElement().text() == "Noise")
					temp = Tech(0);
				if (qdd.firstChildElement().text() == "VGPO")
					temp = Tech(1);
				if (qdd.firstChildElement().text() == "FT")
					temp = Tech(2);
				if (qdd.firstChildElement().text() == "RGPO")
					temp = Tech(3);
				vtech.push_back(temp);
			}
			if (qdd.nodeName() == "jammerERP") {
				ec.setjammerERP(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "jammerChannel") {
				ec.setjammerChannel(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "jammerCovRange") {
				ec.setjammerCoVRange(qdd.toElement().text().toDouble());
			}
			ec.setAllTechs(vtech);
		}
		scenario.addEcm(std::make_shared<sce::Ecm>(ec));
		return true;
	}
	return false;
}

bool find_Weapon_data(const QDomElement &qd,sce::Scenario &scenario)
{
	if ((qd.tagName() == "Weapon") && (qd.parentNode().nodeName() == "Scenario")) {
		Weapon wp;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Name") {
				wp.setName(qdd.toElement().text().toStdString());
			}
			if (qdd.nodeName() == "CEPR") {
				wp.setCEPR(qdd.toElement().text().toInt());
			}
			if (qdd.nodeName() == "WEAPONAREACOVERAGE") {
				wp.setWeaponAreaCoverage(qdd.toElement().text().toInt());
			}
		}
		scenario.addWeapon(std::make_shared<sce::Weapon>(wp));
		return true;
	}
	return false;
}

bool find_Site_data(const QDomElement &qd,sce::Scenario &scenario)
{
	if ((qd.tagName() == "Site") && (qd.parentNode().nodeName() == "Scenario")) {
		Site st;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Name") {
				st.setName(qdd.toElement().text().toStdString());
			}
			if (qdd.nodeName() == "Altitude") {
				st.setAltitude(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "Latitude") {
				st.setLatitude(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "Longitude") {
				st.setLongitude(qdd.toElement().text().toDouble());
			}
		}
		scenario.addSite(std::make_shared<sce::Site>(st));
		return true;
	}
	return false;
}

bool find_OwnPlatform_data(const QDomElement &qd,sce::Scenario &scenario)
{
	if ((qd.tagName() == "OwnPlatform") && (qd.parentNode().nodeName() == "Scenario")) {
		OwnPlatform op;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Name") {
				op.setName(qdd.toElement().text().toStdString());
			}
			if (qdd.nodeName() == "Type")
			{
				if (qdd.toElement().text().toStdString() == "Air")
					op.setType(OwnPlatformType(0));
			}
			if (qdd.nodeName() == "MaxAcceleration") {
				op.setMaxAcceleration(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "MaxDeceleration") {
				op.setMaxDeceleration(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "MaxClimbRate") {
				op.setMaxClimbRate(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "MaxDiveRate") {
				op.setMaxDiveRate(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "MaxSpeed") {
				op.setMaxSpeed(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "MaxTurnRadius") {
				op.setMaxTurnRadius(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "Mission")
			{
				Mission ms;
				std::vector<Point> vpoint;
				for (QDomNode mission_son = qdd.firstChild(); !mission_son.isNull(); mission_son = mission_son.nextSibling()) {
					if (mission_son.nodeName() == "Type") {
						if (mission_son.toElement().text().toStdString() == "Strike")
							ms.setMissionType(MissionType(0));
						if (mission_son.toElement().text().toStdString() == "Support")
							ms.setMissionType(MissionType(1));
					}
					if (mission_son.nodeName() == "StartPoint") {
						Point pt;
						for (QDomNode sp_son = mission_son.firstChild(); !sp_son.isNull(); sp_son = sp_son.nextSibling()) {
							if (sp_son.nodeName() == "Altitude")
								pt.setAltitude(sp_son.toElement().text().toDouble());
							if (sp_son.nodeName() == "Latitude")
								pt.setLatitude(sp_son.toElement().text().toDouble());
							if (sp_son.nodeName() == "Longitude")
								pt.setLongitude(sp_son.toElement().text().toDouble());
							if (sp_son.nodeName() == "Tmin")
								pt.setTmin(sp_son.toElement().text().toDouble());
							if (sp_son.nodeName() == "Tmax")
								pt.setTmax(sp_son.toElement().text().toDouble());
						}
						ms.setStartPoint(pt);
					}
					if (mission_son.nodeName() == "EndPoint") {
						Point pt;
						for (QDomNode ep_son = mission_son.firstChild(); !ep_son.isNull(); ep_son = ep_son.nextSibling()) {
							if (ep_son.nodeName() == "Altitude")
								pt.setAltitude(ep_son.toElement().text().toDouble());
							if (ep_son.nodeName() == "Latitude")
								pt.setLatitude(ep_son.toElement().text().toDouble());
							if (ep_son.nodeName() == "Longitude")
								pt.setLongitude(ep_son.toElement().text().toDouble());
							if (ep_son.nodeName() == "Tmin")
								pt.setTmin(ep_son.toElement().text().toDouble());
							if (ep_son.nodeName() == "Tmax")
								pt.setTmax(ep_son.toElement().text().toDouble());
						}
						ms.setEndPoint(pt);
					}
					if (mission_son.nodeName() == "TargetPoint") {
						Point pt;
						for (QDomNode ep_son = mission_son.firstChild(); !ep_son.isNull(); ep_son = ep_son.nextSibling()) {
							if (ep_son.nodeName() == "Altitude")
								pt.setAltitude(ep_son.toElement().text().toDouble());
							if (ep_son.nodeName() == "Latitude")
								pt.setLatitude(ep_son.toElement().text().toDouble());
							if (ep_son.nodeName() == "Longitude")
								pt.setLongitude(ep_son.toElement().text().toDouble());
							if (ep_son.nodeName() == "Tmin")
								pt.setTmin(ep_son.toElement().text().toDouble());
							if (ep_son.nodeName() == "Tmax")
								pt.setTmax(ep_son.toElement().text().toDouble());
						}
						vpoint.push_back(pt);
					}
				}
				ms.setAllTargetPoints(vpoint);
				op.setMission(ms);
			}
			if (qdd.nodeName() == "RCS") {
				op.setplatformRCS(qdd.toElement().text().toDouble());
			}
		}
		scenario.addOwnPlatform(std::make_shared<sce::OwnPlatform>(op));
		return true;
	}
	return false;
}

bool find_Emitter_data(const QDomElement &qd,sce::Scenario &scenario)
{
	if ((qd.tagName() == "Emitter") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::Emitter emitter;
		std::vector<std::shared_ptr<sce::Radar_Mode>> v_radar;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Name")
			{
				emitter.setName(qdd.toElement().text().toStdString());
			}
			if (qdd.nodeName() == "MSR")
			{
				emitter.setradarMSR(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "DangerValues")
			{
				emitter.setradarDangerValue(qdd.toElement().text().toDouble());
			}
			if (qdd.nodeName() == "Radar_Mode")
			{
				sce::Radar_Mode radar;
				for (QDomNode qds_son = qdd.firstChild(); !qds_son.isNull(); qds_son = qds_son.nextSibling())
				{
					if (qds_son.nodeName() == "ModeCode")
					{
						radar.setModeCode(qds_son.toElement().text().toStdString());
					}
					if (qds_son.nodeName() == "ModeType")
					{
						if (qds_son.toElement().text().toStdString() == "Pulse")
							radar.setModeType(sce::ModeType(0));
					}
					if (qds_son.nodeName() == "Rf")
					{
						Rf rf;
						std::vector<Rf_values> vrfValues;
						for (QDomNode qds_son_son = qds_son.firstChild(); !qds_son_son.isNull(); qds_son_son = qds_son_son.nextSibling()) {
							if (qds_son_son.nodeName() == "Rfmin") {
								rf.setRfMin(qds_son_son.toElement().text().toInt());
							}
							if (qds_son_son.nodeName() == "Rfmax") {
								rf.setRfMax(qds_son_son.toElement().text().toInt());
							}
							if (qds_son_son.nodeName() == "RfType") {
								sce::RfType;
								if (qds_son_son.toElement().text().toStdString() == "Burst") {
									rf.setRfType(RfType(0));
								}if (qds_son_son.toElement().text().toStdString() == "Fix") {
									rf.setRfType(RfType(1));
								}
							}
							if (qds_son_son.nodeName() == "Rf_Values") {
								Rf_values temp;
								for (QDomNode rf_values_son = qds_son_son.firstChild(); !rf_values_son.isNull(); rf_values_son = rf_values_son.nextSibling())
								{
									if (rf_values_son.nodeName() == "PhaseOrder")
									{
										temp.setPhaseOrder(rf_values_son.toElement().text().toInt());
									}
									if (rf_values_son.nodeName() == "NumberOfPulses")
									{
										temp.setNumberOfPulses(rf_values_son.toElement().text().toInt());
									}
									if (rf_values_son.nodeName() == "Rfmin")
									{
										temp.setMin(rf_values_son.toElement().text().toInt());
									}
									if (rf_values_son.nodeName() == "Rfmax")
									{
										temp.setMax(rf_values_son.toElement().text().toInt());
									}
									if (rf_values_son.nodeName() == "SweepTime")
									{
										temp.setSweepTime(rf_values_son.toElement().text().toInt());
									}
								}
								vrfValues.push_back(temp);
							}
							rf.setAllRfValues(vrfValues);
						}
						radar.setRf(rf);
					}
					if (qds_son.nodeName() == "Pw")
					{
						Pw pw;
						std::vector<Pw_values> vpwValues;
						for (QDomNode qds_son_son = qds_son.firstChild(); !qds_son_son.isNull(); qds_son_son = qds_son_son.nextSibling()) {
							if (qds_son_son.nodeName() == "Pwmin") {
								pw.setPwMin(qds_son_son.toElement().text().toInt());
							}
							if (qds_son_son.nodeName() == "Pwmax") {
								pw.setPwMax(qds_son_son.toElement().text().toInt());
							}
							if (qds_son_son.nodeName() == "PwType") {
								if (qds_son_son.toElement().text().toStdString() == "Fix") {
									pw.setPwType(PwType(0));
								}
							}
							if (qds_son_son.nodeName() == "Pw_Values") {
								sce::Pw_values temp;
								for (QDomNode rf_values_son = qds_son_son.firstChild(); !rf_values_son.isNull(); rf_values_son = rf_values_son.nextSibling())
								{
									if (rf_values_son.nodeName() == "PhaseOrder")
									{
										temp.setPhaseOrder(rf_values_son.toElement().text().toInt());
									}
									if (rf_values_son.nodeName() == "NumberOfPulses")
									{
										temp.setNumberOfPulses(rf_values_son.toElement().text().toInt());
									}
									if (rf_values_son.nodeName() == "Pwmin")
									{
										temp.setMin(rf_values_son.toElement().text().toInt());
									}
									if (rf_values_son.nodeName() == "Pwmax")
									{
										temp.setMax(rf_values_son.toElement().text().toInt());
									}
									if (rf_values_son.nodeName() == "SweepTime")
									{
										temp.setSweepTime(rf_values_son.toElement().text().toInt());
									}
								}
								vpwValues.push_back(temp);
							}
							pw.setAllPwValues(vpwValues);
						}
						radar.setPw(pw);
					}
					if (qds_son.nodeName() == "Pri")
					{
						Pri pri;
						std::vector<Pri_values> vpriValues;
						for (QDomNode qds_son_son = qds_son.firstChild(); !qds_son_son.isNull(); qds_son_son = qds_son_son.nextSibling()) {
							if (qds_son_son.nodeName() == "Primin") {
								pri.setPriMin(qds_son_son.toElement().text().toInt());
							}
							if (qds_son_son.nodeName() == "Primax") {
								pri.setPriMax(qds_son_son.toElement().text().toInt());
							}
							if (qds_son_son.nodeName() == "PriType") {
								sce::PriType;
								if (qds_son_son.toElement().text().toStdString() == "Stagger") {
									pri.setPriType(PriType(0));
								}if (qds_son_son.toElement().text().toStdString() == "Switching") {
									pri.setPriType(PriType(1));
								}if (qds_son_son.toElement().text().toStdString() == "Fix") {
									pri.setPriType(PriType(2));
								}
							}
							if (qds_son_son.nodeName() == "Pri_Values") {
								Pri_values temp;
								for (QDomNode rf_values_son = qds_son_son.firstChild(); !rf_values_son.isNull(); rf_values_son = rf_values_son.nextSibling())
								{
									if (rf_values_son.nodeName() == "PhaseOrder")
									{
										temp.setPhaseOrder(rf_values_son.toElement().text().toInt());
									}
									if (rf_values_son.nodeName() == "NumberOfPulses")
									{
										temp.setNumberOfPulses(rf_values_son.toElement().text().toInt());
									}
									if (rf_values_son.nodeName() == "Primin")
									{
										temp.setMin(rf_values_son.toElement().text().toInt());
									}
									if (rf_values_son.nodeName() == "Primax")
									{
										temp.setMax(rf_values_son.toElement().text().toInt());
									}
									if (rf_values_son.nodeName() == "SweepTime")
									{
										temp.setSweepTime(rf_values_son.toElement().text().toInt());
									}
								}
								vpriValues.push_back(temp);
							}
							pri.setAllPriValues(vpriValues);
						}
						radar.setPri(pri);
					}
					if (qds_son.nodeName() == "Scan")
					{
						Scan scan;
						for (QDomNode scan_son = qds_son.firstChild(); !scan_son.isNull(); scan_son = scan_son.nextSibling()) {
							if (scan_son.nodeName().toStdString() == "ScanType") {
								if (scan_son.toElement().text().toStdString() == "Sectorial")
									scan.setScanType(sce::ScanType(0));
							}
							if (scan_son.nodeName().toStdString() == "Spmax") {
								scan.setScanMax(scan_son.toElement().text().toInt());
							}
							if (scan_son.nodeName().toStdString() == "Spmin") {
								scan.setScanMin(scan_son.toElement().text().toInt());
							}
						}
						radar.setScan(scan);
					}
					if (qds_son.nodeName() == "Erp")
					{
						Erp erp;
						for (QDomNode erp_son = qds_son.firstChild(); !erp_son.isNull(); erp_son = erp_son.nextSibling()) {
							if (erp_son.nodeName().toStdString() == "Erpmax") {
								erp.setErpMax(erp_son.toElement().text().toInt());
							}
							if (erp_son.nodeName().toStdString() == "Erpmin") {
								erp.setErpMin(erp_son.toElement().text().toInt());
							}
						}
						radar.setErp(erp);
					}
				}
				v_radar.push_back(std::make_shared<sce::Radar_Mode>(radar));
			}
			emitter.setAllPtr2RadarModes(v_radar);
		}
		scenario.addEmitter(std::make_shared<sce::Emitter>(emitter));
		return true;
	}
	return false;
}

bool find_Route_data(const QDomElement &qd,sce::Scenario &scenario)
{
	if ((qd.tagName() == "Route") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::Route rot;
		std::vector<sce::WayPoint> wpoint;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Name")
			{
				rot.setName(qdd.toElement().text().toStdString());
			}
			if (qdd.nodeName() == "WayPoints")
			{
				sce::WayPoint point;
				for (QDomNode qds_son = qdd.firstChild(); !qds_son.isNull(); qds_son = qds_son.nextSibling())
				{
					if (qds_son.nodeName() == "Index")
					{
						point.setIndex(qds_son.toElement().text().toInt());
					}
					if (qds_son.nodeName() == "Altitude")
					{
						point.setAltitude(qds_son.toElement().text().toDouble());
					}
					if (qds_son.nodeName() == "Latitude")
					{
						point.setLatitude(qds_son.toElement().text().toDouble());
					}
					if (qds_son.nodeName() == "Longitude")
					{
						point.setLongitude(qds_son.toElement().text().toDouble());
					}
					if (qds_son.nodeName() == "Time")
					{
						point.setTime(qds_son.toElement().text().toDouble());
					}
					if (qds_son.nodeName() == "Velocity")
					{
						point.setVelocity(qds_son.toElement().text().toDouble());
					}
					if (qds_son.nodeName() == "Acceleration")
					{
						point.setAcceleration(qds_son.toElement().text().toDouble());
					}
				}
				wpoint.push_back(point);
			}
			rot.setAllWayPoints(wpoint);
		}
		scenario.addRoute(std::make_shared<sce::Route>(rot));
		return true;
	}
	return false;
}

bool find_EsmStrategy_data(const QDomElement &qd,sce::Scenario &scenario)
{
	if ((qd.tagName() == "EsmStrategy") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::EsmStrategy es;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Name") {
				es.setName(qdd.toElement().text().toStdString());
			}
			if (qdd.nodeName() == "Section")
			{
				sce::EsmStrategySection sec;
				std::vector<sce::DwellSquence> dwellsquences;
				for (QDomNode qds_son = qdd.firstChild(); !qds_son.isNull(); qds_son = qds_son.nextSibling())
				{
					if (qds_son.nodeName() == "StartTime") {
						sec.setStartTime(qds_son.toElement().text().toDouble());
					}
					if (qds_son.nodeName() == "EndTime") {
						sec.setEndTime(qds_son.toElement().text().toDouble());
					}
					if (qds_son.nodeName() == "StartLocation")
					{
						sce::Location loca_st;
						for (QDomNode qdds_son = qds_son.firstChild(); !qdds_son.isNull(); qdds_son = qdds_son.nextSibling())
						{
							if (qdds_son.nodeName() == "Altitude") {
								loca_st.setAltitude(qdds_son.toElement().text().toDouble());
							}if (qdds_son.nodeName() == "Latitude") {
								loca_st.setLatitude(qdds_son.toElement().text().toDouble());
							}if (qdds_son.nodeName() == "Longitude") {
								loca_st.setLongitude(qdds_son.toElement().text().toDouble());
							}
						}
						sec.setStartLocation(loca_st);
					}
					if (qds_son.nodeName() == "EndLocation")
					{
						sce::Location loca_end;
						for (QDomNode qdds_son = qds_son.firstChild(); !qdds_son.isNull(); qdds_son = qdds_son.nextSibling())
						{
							if (qdds_son.nodeName() == "Altitude") {
								loca_end.setAltitude(qdds_son.toElement().text().toDouble());
							}if (qdds_son.nodeName() == "Latitude") {
								loca_end.setLatitude(qdds_son.toElement().text().toDouble());
							}if (qdds_son.nodeName() == "Longitude") {
								loca_end.setLongitude(qdds_son.toElement().text().toDouble());
							}
						}
						sec.setEndLocation(loca_end);
					}
					if (qds_son.nodeName() == "DwellSquence")
					{
						sce::DwellSquence ds;
						for (QDomNode qdds_son = qds_son.firstChild(); !qdds_son.isNull(); qdds_son = qdds_son.nextSibling())
						{
							if (qdds_son.nodeName() == "Index") {
								ds.setIndex(qdds_son.toElement().text().toInt());
							}if (qdds_son.nodeName() == "MinFreq") {
								ds.setMinFreq(qdds_son.toElement().text().toDouble());
							}if (qdds_son.nodeName() == "MaxFreq") {
								ds.setMaxFreq(qdds_son.toElement().text().toDouble());
							}if (qdds_son.nodeName() == "StartTime") {
								ds.setStartTime(qdds_son.toElement().text().toDouble());
							}if (qdds_son.nodeName() == "EndTime") {
								ds.setEndTime(qdds_son.toElement().text().toDouble());
							}
						}
						dwellsquences.push_back(ds);
					}
				}
				sec.setDwellSquences(dwellsquences);
				es.addSection(std::make_shared<sce::EsmStrategySection>(sec));
			}			
		}
		scenario.addEsmStrategy(std::make_shared<sce::EsmStrategy>(es));
		return true;
	}
	return false;
}

bool find_EcmStrategy_data(const QDomElement &qd,sce::Scenario &scenario)
{
	if ((qd.tagName() == "EcmStrategy") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::EcmStrategy ecs;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Name")
			{
				ecs.setName(qdd.toElement().text().toStdString());
			}
			if (qdd.nodeName() == "Section")
			{
				sce::EcmStrategySection sec;
				for (QDomNode qds_son = qdd.firstChild(); !qds_son.isNull(); qds_son = qds_son.nextSibling())
				{
					if (qds_son.nodeName() == "StartTime")
					{
						sec.setStartTime(qds_son.toElement().text().toDouble());
					}
					if (qds_son.nodeName() == "EndTime")
					{
						sec.setEndTime(qds_son.toElement().text().toDouble());
					}
					if (qds_son.nodeName() == "StartLocation")
					{
						sce::Location loc;
						for (QDomNode qdds_son = qds_son.firstChild(); !qdds_son.isNull(); qdds_son = qdds_son.nextSibling())
						{
							if (qdds_son.nodeName() == "Altitude")
							{
								loc.setAltitude(qdds_son.toElement().text().toDouble());
							}
							if (qdds_son.nodeName() == "Latitude")
							{
								loc.setLatitude(qdds_son.toElement().text().toDouble());
							}
							if (qdds_son.nodeName() == "Longitude")
							{
								loc.setLongitude(qdds_son.toElement().text().toDouble());
							}
						}
						sec.setStartLocation(loc);
					}
					if (qds_son.nodeName() == "EndLocation") {
						sce::Location loc;
						for (QDomNode qdds_son = qds_son.firstChild(); !qdds_son.isNull(); qdds_son = qdds_son.nextSibling()) {
							if (qdds_son.nodeName() == "Altitude") {
								loc.setAltitude(qdds_son.toElement().text().toDouble());
							}
							if (qdds_son.nodeName() == "Latitude") {
								loc.setLatitude(qdds_son.toElement().text().toDouble());
							}
							if (qdds_son.nodeName() == "Longitude") {
								loc.setLongitude(qdds_son.toElement().text().toDouble());
							}
							sec.setEndLocation(loc);
						}
					}
					if (qds_son.nodeName() == "tech") {
						sce::Tech tec;
						for (QDomNode qdds_son = qds_son.firstChild(); !qdds_son.isNull(); qdds_son = qdds_son.nextSibling()) {
							if (qdds_son.toElement().text().toStdString() == "Noise") {
								tec = sce::Tech(0);
							}
							if (qdds_son.toElement().text().toStdString() == "VGPO") {
								tec = sce::Tech(1);
							}
							if (qdds_son.toElement().text().toStdString() == "FT") {
								tec = sce::Tech(2);
							}
							if (qdds_son.toElement().text().toStdString() == "RGPO") {
								tec = sce::Tech(3);
							}
							sec.setTechName(tec);
						}
					}
				}
				ecs.addSection(std::make_shared<sce::EcmStrategySection>(sec));
			}
		}
		scenario.addEcmStrategy(std::make_shared<sce::EcmStrategy>(ecs));
		return true;
	}
	return false;
}

bool find_PlatformSiteRelation_data(const QDomElement &qd,sce::Scenario &scenario)
{
	if ((qd.tagName() == "PlatformSiteRelation") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::PlatformSiteRelation psr;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Platform")
			{
				for (int i = 0; i < scenario.getAllPlatform().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllPlatform()[i]->getPlatformName()) {
						psr.setPlatform(scenario.getAllPlatform()[i]);
						break;
					}
				}
			}
			if (qdd.nodeName() == "Site")
			{
				for (int i = 0; i < scenario.getAllSite().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllSite()[i]->getName()) {
						psr.setSite(scenario.getAllSite()[i]);
						break;
					}
				}
			}
		}
		scenario.addPlatformSiteRelation(psr);
		return true;
	}
	return false;
}

bool find_PlatformEmitterRelation_data(const QDomElement &qd,sce::Scenario &scenario)
{
	if ((qd.tagName() == "PlatformEmitterRelation") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::PlatformEmitterRelation per;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Platform")
			{
				for (int i = 0; i < scenario.getAllPlatform().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllPlatform()[i]->getPlatformName()) {
						per.setPlatform(scenario.getAllPlatform()[i]);
						break;
					}
				}
			}
			if (qdd.nodeName() == "Emitter")
			{
				for (int i = 0; i < scenario.getAllEmitter().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllEmitter()[i]->getName()) {
						per.setEmitter(scenario.getAllEmitter()[i]);
						break;
					}
				}
			}
		}
		scenario.addPlatformEmitterRelation(per);
		return true;
	}
	return false;
}

bool find_PlatformWeaponRelation_data(const QDomElement &qd,sce::Scenario &scenario)
{
	if ((qd.tagName() == "PlatformWeaponRelation") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::PlatformWeaponRelation pwr;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Platform")
			{
				for (int i = 0; i < scenario.getAllPlatform().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllPlatform()[i]->getPlatformName()) {
						pwr.setPlatform(scenario.getAllPlatform()[i]);
						break;
					}
				}
			}
			if (qdd.nodeName() == "Weapon")
			{
				for (int i = 0; i < scenario.getAllWeapon().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllWeapon()[i]->getName()) {
						pwr.setWeapon(scenario.getAllWeapon()[i]);
						break;
					}
				}
			}
		}
		scenario.addPlatformWeaponRelation(pwr);
		return true;
	}
	return false;
}

bool find_OwnPlatformEsmRelation_data(const QDomElement &qd,sce::Scenario &scenario)
{
	if ((qd.tagName() == "OwnPlatformEsmRelation") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::OwnPlatformEsmRelation temp;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "OwnPlatform")

			{
				for (int i = 0; i < scenario.getAllOwnPlatform().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllOwnPlatform()[i]->getName()) {
						temp.setOwnPlatform(scenario.getAllOwnPlatform()[i]);
						break;
					}
				}
			}
			if (qdd.nodeName() == "Esm")
			{
				for (int i = 0; i < scenario.getAllEsm().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllEsm()[i]->getName()) {
						temp.setEsm(scenario.getAllEsm()[i]);
						break;
					}
				}
			}
		}
		scenario.addOwnPlatformEsmRelation(temp);
		return true;
	}
	return false;
}

bool find_EsmEsmStrategyRelation_data(const QDomElement &qd,sce::Scenario &scenario)
{
	if ((qd.tagName() == "EsmEsmStrategyRelation") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::EsmEsmStrategyRelation temp;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Esm")
			{
				for (int i = 0; i < scenario.getAllEsm().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllEsm()[i]->getName()) {
						temp.setEsm(scenario.getAllEsm()[i]);
						break;
					}
				}
			}
			if (qdd.nodeName() == "EsmStrategy")
			{
				for (int i = 0; i < scenario.getAllEsmStrategy().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllEsmStrategy()[i]->getName()) {
						temp.setEsmStrategy(scenario.getAllEsmStrategy()[i]);
						break;
					}
				}
			}
		}
		scenario.addEsmEsmStrategyRelation(temp);
		return true;
	}
	return false;
}

bool find_OwnPlatformEcmRelation_data(const QDomElement &qd,sce::Scenario &scenario)
{
	if ((qd.tagName() == "OwnPlatformEcmRelation") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::OwnPlatformEcmRelation temp;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "OwnPlatform")
			{
				for (int i = 0; i < scenario.getAllOwnPlatform().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllOwnPlatform()[i]->getName()) {
						temp.setOwnPlatform(scenario.getAllOwnPlatform()[i]);
						break;
					}
				}
			}
			if (qdd.nodeName() == "Ecm")
			{
				for (int i = 0; i < scenario.getAllEcm().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllEcm()[i]->getName()) {
						temp.setEcm(scenario.getAllEcm()[i]);
						break;
					}
				}
			}
		}
		scenario.addOwnPlatformEcmRelation(temp);
		return true;
	}
	return false;
}

bool find_EcmEcmStrategyRelation_data(const QDomElement &qd,sce::Scenario &scenario)
{
	if ((qd.tagName() == "EcmEcmStrategyRelation") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::EcmEcmStrategyRelation temp;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Ecm")
			{
				for (int i = 0; i < scenario.getAllEcm().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllEcm()[i]->getName()) {
						temp.setEcm(scenario.getAllEcm()[i]);
						break;
					}
				}
			}
			if (qdd.nodeName() == "EcmStrategy")
			{
				for (int i = 0; i < scenario.getAllEcmStrategy().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllEcmStrategy()[i]->getName()) {
						temp.setEcmStrategy(scenario.getAllEcmStrategy()[i]);
						break;
					}
				}
			}
		}
		scenario.addEcmEcmStrategyRelation(temp);
		return true;
	}
	return false;
}

bool find_OwnPlatformRouteRelation_data(const QDomElement &qd,sce::Scenario &scenario)
{
	if ((qd.tagName() == "OwnPlatformRouteRelation") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::OwnPlatformRouteRelation temp;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "OwnPlatform")
			{
				for (int i = 0; i < scenario.getAllOwnPlatform().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllOwnPlatform()[i]->getName()) {
						temp.setOwnPlatform(scenario.getAllOwnPlatform()[i]);
						break;
					}
				}
			}
			if (qdd.nodeName() == "Route")
			{
				for (int i = 0; i < scenario.getAllRoute().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllRoute()[i]->getName()) {
						temp.setRoute(scenario.getAllRoute()[i]);
						break;
					}
				}
			}
		}
		scenario.addOwnPlatformRouteRelation(temp);
		return true;
	}
	return false;
}
