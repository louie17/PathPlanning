//
// MATLAB Compiler: 6.1 (R2015b)
// Date: Thu Jun 27 16:44:13 2019
// Arguments: "-B" "macro_default" "-W" "cpplib:esmDwellSequenceGen" "-T"
// "link:lib" "-d" "D:\Projects Matlab\dllPoI\esmDwellSequenceGen\for_testing"
// "-v" "D:\Projects Matlab\dllPoI\esmDwellSequenceGen.m" 
//

#ifndef __esmDwellSequenceGen_h
#define __esmDwellSequenceGen_h 1

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

#ifdef EXPORTING_esmDwellSequenceGen
#define PUBLIC_esmDwellSequenceGen_C_API __global
#else
#define PUBLIC_esmDwellSequenceGen_C_API /* No import statement needed. */
#endif

#define LIB_esmDwellSequenceGen_C_API PUBLIC_esmDwellSequenceGen_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_esmDwellSequenceGen
#define PUBLIC_esmDwellSequenceGen_C_API __declspec(dllexport)
#else
#define PUBLIC_esmDwellSequenceGen_C_API __declspec(dllimport)
#endif

#define LIB_esmDwellSequenceGen_C_API PUBLIC_esmDwellSequenceGen_C_API


#else

#define LIB_esmDwellSequenceGen_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_esmDwellSequenceGen_C_API 
#define LIB_esmDwellSequenceGen_C_API /* No special import/export declaration */
#endif

extern LIB_esmDwellSequenceGen_C_API 
bool MW_CALL_CONV esmDwellSequenceGenInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_esmDwellSequenceGen_C_API 
bool MW_CALL_CONV esmDwellSequenceGenInitialize(void);

extern LIB_esmDwellSequenceGen_C_API 
void MW_CALL_CONV esmDwellSequenceGenTerminate(void);



extern LIB_esmDwellSequenceGen_C_API 
void MW_CALL_CONV esmDwellSequenceGenPrintStackTrace(void);

extern LIB_esmDwellSequenceGen_C_API 
bool MW_CALL_CONV mlxEsmDwellSequenceGen(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                         *prhs[]);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_esmDwellSequenceGen
#define PUBLIC_esmDwellSequenceGen_CPP_API __declspec(dllexport)
#else
#define PUBLIC_esmDwellSequenceGen_CPP_API __declspec(dllimport)
#endif

#define LIB_esmDwellSequenceGen_CPP_API PUBLIC_esmDwellSequenceGen_CPP_API

#else

#if !defined(LIB_esmDwellSequenceGen_CPP_API)
#if defined(LIB_esmDwellSequenceGen_C_API)
#define LIB_esmDwellSequenceGen_CPP_API LIB_esmDwellSequenceGen_C_API
#else
#define LIB_esmDwellSequenceGen_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_esmDwellSequenceGen_CPP_API void MW_CALL_CONV esmDwellSequenceGen(int nargout, mwArray& dwellSequence, mwArray& pIntercept, const mwArray& esmStartFreq, const mwArray& esmEndFreq, const mwArray& esmStepFreq, const mwArray& esmInstBand, const mwArray& esmMinDwellTime, const mwArray& nEmitter, const mwArray& freqStart, const mwArray& freqEnd, const mwArray& tScan, const mwArray& taoScan, const mwArray& taoDwell, const mwArray& pInterceptDesired, const mwArray& tInterceptDesired);

#endif
#endif
