//

#ifndef __sojaSystem_h
#define __sojaSystem_h 1
#ifndef __spjaSystem_h
#define __spjaSystem_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_sojaSystem
#define PUBLIC_sojaSystem_C_API __global
#else
#define PUBLIC_sojaSystem_C_API /* No import statement needed. */
#endif
#ifdef EXPORTING_spjaSystem
#define PUBLIC_spjaSystem_C_API __global
#else
#define PUBLIC_spjaSystem_C_API /* No import statement needed. */
#endif

#define LIB_sojaSystem_C_API PUBLIC_sojaSystem_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_sojaSystem
#define PUBLIC_sojaSystem_C_API __declspec(dllexport)
#else
#define PUBLIC_sojaSystem_C_API __declspec(dllimport)
#endif

#define LIB_sojaSystem_C_API PUBLIC_sojaSystem_C_API


#else

#define LIB_sojaSystem_C_API
#ifdef EXPORTING_spjaSystem
#define PUBLIC_spjaSystem_C_API __declspec(dllexport)
#else
#define PUBLIC_spjaSystem_C_API __declspec(dllimport)
#endif

#define LIB_spjaSystem_C_API PUBLIC_spjaSystem_C_API


//#else

#define LIB_spjaSystem_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_sojaSystem_C_API 
#define LIB_sojaSystem_C_API /* No special import/export declaration */
#endif

extern LIB_sojaSystem_C_API 
bool MW_CALL_CONV sojaSystemInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_sojaSystem_C_API 
bool MW_CALL_CONV sojaSystemInitialize(void);

extern LIB_sojaSystem_C_API 
void MW_CALL_CONV sojaSystemTerminate(void);



extern LIB_sojaSystem_C_API 
void MW_CALL_CONV sojaSystemPrintStackTrace(void);

extern LIB_sojaSystem_C_API 
bool MW_CALL_CONV mlxSojaSystem(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

#ifndef LIB_spjaSystem_C_API 
#define LIB_spjaSystem_C_API /* No special import/export declaration */
#endif

extern LIB_spjaSystem_C_API 
bool MW_CALL_CONV spjaSystemInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_spjaSystem_C_API 
bool MW_CALL_CONV spjaSystemInitialize(void);

extern LIB_spjaSystem_C_API 
void MW_CALL_CONV spjaSystemTerminate(void);



extern LIB_spjaSystem_C_API 
void MW_CALL_CONV spjaSystemPrintStackTrace(void);

extern LIB_spjaSystem_C_API 
bool MW_CALL_CONV mlxSpjaSystem(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);



#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_sojaSystem
#define PUBLIC_sojaSystem_CPP_API __declspec(dllexport)
#else
#define PUBLIC_sojaSystem_CPP_API __declspec(dllimport)
#endif

#define LIB_sojaSystem_CPP_API PUBLIC_sojaSystem_CPP_API

#else

#if !defined(LIB_sojaSystem_CPP_API)
#if defined(LIB_sojaSystem_C_API)
#define LIB_sojaSystem_CPP_API LIB_sojaSystem_C_API
#else
#define LIB_sojaSystem_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_sojaSystem_CPP_API void MW_CALL_CONV sojaSystem(int nargout, mwArray& jammingAllocation, const mwArray& radarIndex, const mwArray& radarERP, const mwArray& radarMSR, const mwArray& radarSojRange, const mwArray& jammerERP, const mwArray& platformRCS, const mwArray& lutTecEffect);

#ifdef EXPORTING_spjaSystem
#define PUBLIC_spjaSystem_CPP_API __declspec(dllexport)
#else
#define PUBLIC_spjaSystem_CPP_API __declspec(dllimport)
#endif

#define LIB_spjaSystem_CPP_API PUBLIC_spjaSystem_CPP_API

#else

#if !defined(LIB_spjaSystem_CPP_API)
#if defined(LIB_spjaSystem_C_API)
#define LIB_spjaSystem_CPP_API LIB_spjaSystem_C_API
#else
#define LIB_spjaSystem_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_spjaSystem_CPP_API void MW_CALL_CONV spjaSystem(int nargout, mwArray& jammingAllocation, const mwArray& radarIndex, const mwArray& radarERP, const mwArray& radarDangerValue, const mwArray& jammerChannel, const mwArray& jammerERP, const mwArray& platformRCS, const mwArray& lutTecEffect);


#endif
#endif
