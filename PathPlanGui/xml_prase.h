#pragma once
#include "MainWindow.h"
#include <iostream>
#include <vector>
#include <memory>
#include "Scenario.hpp"

bool find_Vertex_data(const QDomElement &qd,sce::Scenario &scenario);
bool find_Platform_data(const QDomElement &qd, sce::Scenario &scenario);
bool find_Esm_data(const QDomElement &qd, sce::Scenario &scenario);
bool find_Ecm_data(const QDomElement &qd, sce::Scenario &scenario);
bool find_Weapon_data(const QDomElement &qd,sce::Scenario &scenario);
bool find_Site_data(const QDomElement &qd,sce::Scenario &scenario);
bool find_OwnPlatform_data(const QDomElement &qd,sce::Scenario &scenario);
bool find_Emitter_data(const QDomElement &qd,sce::Scenario &scenario);
bool find_Route_data(const QDomElement &qd,sce::Scenario &scenario);
bool find_EsmStrategy_data(const QDomElement &qd,sce::Scenario &scenario);
bool find_EcmStrategy_data(const QDomElement &qd,sce::Scenario &scenario);
bool find_PlatformSiteRelation_data(const QDomElement &qd,sce::Scenario &scenario);
bool find_PlatformEmitterRelation_data(const QDomElement &qd,sce::Scenario &scenario);
bool find_PlatformWeaponRelation_data(const QDomElement &qd,sce::Scenario &scenario);
bool find_OwnPlatformEsmRelation_data(const QDomElement &qd,sce::Scenario &scenario);
bool find_EsmEsmStrategyRelation_data(const QDomElement &qd,sce::Scenario &scenario);
bool find_OwnPlatformEcmRelation_data(const QDomElement &qd,sce::Scenario &scenario);
bool find_EcmEcmStrategyRelation_data(const QDomElement &qd,sce::Scenario &scenario);
bool find_OwnPlatformRouteRelation_data(const QDomElement &qd,sce::Scenario &scenario);
