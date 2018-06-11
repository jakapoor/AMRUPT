#include "__cf_outdoorAngleFinder.h"
#if 0
This file is not available for use in any application other than as a MATLAB
( R ) MEX file for use with the Simulink ( R ) product . If you do not have
the Simulink Coder licensed , this file contains encrypted block names , etc
. If you purchase the Simulink Coder , this file will contain full block
descriptions and improved variable names .
#endif
#include <math.h>
#include "outdoorAngleFinder_acc.h"
#include "outdoorAngleFinder_acc_private.h"
#include <stdio.h>
#include "slexec_vm_simstruct_bridge.h"
#include "slexec_vm_zc_functions.h"
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
#include "simtarget/slAccSfcnBridge.h"
static void mdlOutputs ( SimStruct * S , int_T tid ) { char_T * sErr ; void *
audio ; real_T o342p14vsq ; real_T kyqglkv40w ; real_T byjxw5aijc ;
mkz4gnsvgt * _rtB ; h1j5haitu2 * _rtP ; dwym33kjnc * _rtZCE ; b5dwuj4txq *
_rtDW ; _rtDW = ( ( b5dwuj4txq * ) ssGetRootDWork ( S ) ) ; _rtZCE = ( (
dwym33kjnc * ) _ssGetPrevZCSigState ( S ) ) ; _rtP = ( ( h1j5haitu2 * )
ssGetDefaultParam ( S ) ) ; _rtB = ( ( mkz4gnsvgt * ) _ssGetBlockIO ( S ) ) ;
sErr = GetErrorBuffer ( & _rtDW -> a3qklxpb5m [ 0U ] ) ; audio = ( void * ) &
_rtB -> f32osfkuew ; LibOutputs_FromMMFile ( & _rtDW -> a3qklxpb5m [ 0U ] ,
GetNullPointer ( ) , audio , GetNullPointer ( ) , GetNullPointer ( ) ,
GetNullPointer ( ) ) ; if ( * sErr != 0 ) { ssSetErrorStatus ( S , sErr ) ;
ssSetStopRequested ( S , 1 ) ; } sErr = GetErrorBuffer ( & _rtDW ->
nq45ujvx1i [ 0U ] ) ; audio = ( void * ) & _rtB -> kdf2vzq50u ;
LibOutputs_FromMMFile ( & _rtDW -> nq45ujvx1i [ 0U ] , GetNullPointer ( ) ,
audio , GetNullPointer ( ) , GetNullPointer ( ) , GetNullPointer ( ) ) ; if (
* sErr != 0 ) { ssSetErrorStatus ( S , sErr ) ; ssSetStopRequested ( S , 1 )
; } sErr = GetErrorBuffer ( & _rtDW -> b40brqdfyw [ 0U ] ) ; audio = ( void *
) & _rtB -> jgztesp4me ; LibOutputs_FromMMFile ( & _rtDW -> b40brqdfyw [ 0U ]
, GetNullPointer ( ) , audio , GetNullPointer ( ) , GetNullPointer ( ) ,
GetNullPointer ( ) ) ; if ( * sErr != 0 ) { ssSetErrorStatus ( S , sErr ) ;
ssSetStopRequested ( S , 1 ) ; } sErr = GetErrorBuffer ( & _rtDW ->
mw3i20uptm [ 0U ] ) ; audio = ( void * ) & _rtB -> m4m0tszph0 ;
LibOutputs_FromMMFile ( & _rtDW -> mw3i20uptm [ 0U ] , GetNullPointer ( ) ,
audio , GetNullPointer ( ) , GetNullPointer ( ) , GetNullPointer ( ) ) ; if (
* sErr != 0 ) { ssSetErrorStatus ( S , sErr ) ; ssSetStopRequested ( S , 1 )
; } if ( ( ( _rtZCE -> mzk0mvtd0j == POS_ZCSIG ) != _rtB -> ezdepaihaz ) && (
_rtZCE -> mzk0mvtd0j != UNINITIALIZED_ZCSIG ) ) { o342p14vsq = _rtB ->
f32osfkuew * _rtB -> jgztesp4me ; kyqglkv40w = _rtB -> kdf2vzq50u * _rtB ->
m4m0tszph0 ; o342p14vsq += kyqglkv40w ; kyqglkv40w = _rtB -> kdf2vzq50u *
_rtB -> jgztesp4me ; byjxw5aijc = _rtB -> f32osfkuew * _rtB -> m4m0tszph0 ;
kyqglkv40w -= byjxw5aijc ; o342p14vsq = muDoubleScalarAtan2 ( kyqglkv40w ,
o342p14vsq ) ; o342p14vsq *= _rtP -> P_1 ; if ( o342p14vsq > 1.0 ) {
o342p14vsq = 1.0 ; } else { if ( o342p14vsq < - 1.0 ) { o342p14vsq = - 1.0 ;
} } _rtB -> f51zpey2ix = _rtP -> P_2 * muDoubleScalarAsin ( o342p14vsq ) ;
_rtDW -> kklrpm2yty = 4 ; } _rtZCE -> mzk0mvtd0j = _rtB -> ezdepaihaz ;
ssCallAccelRunBlock ( S , 1 , 6 , SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock
( S , 1 , 7 , SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 1 , 8 ,
SS_CALL_MDL_OUTPUTS ) ; UNUSED_PARAMETER ( tid ) ; } static void
mdlOutputsTID1 ( SimStruct * S , int_T tid ) { ZCEventType zcEvent ;
mkz4gnsvgt * _rtB ; h1j5haitu2 * _rtP ; dwym33kjnc * _rtZCE ; b5dwuj4txq *
_rtDW ; _rtDW = ( ( b5dwuj4txq * ) ssGetRootDWork ( S ) ) ; _rtZCE = ( (
dwym33kjnc * ) _ssGetPrevZCSigState ( S ) ) ; _rtP = ( ( h1j5haitu2 * )
ssGetDefaultParam ( S ) ) ; _rtB = ( ( mkz4gnsvgt * ) _ssGetBlockIO ( S ) ) ;
zcEvent = rt_ZCFcn ( ANY_ZERO_CROSSING , & _rtZCE -> nv2xsrvxxh , ( 0.0 -
_rtP -> P_3 ) ) ; if ( _rtDW -> ac5hzcliph == 0 ) { if ( zcEvent !=
NO_ZCEVENT ) { _rtB -> ezdepaihaz = ! _rtB -> ezdepaihaz ; _rtDW ->
ac5hzcliph = 1 ; } else if ( _rtB -> ezdepaihaz ) { if ( 0.0 != _rtP -> P_3 )
{ _rtB -> ezdepaihaz = false ; } } else { if ( 0.0 == _rtP -> P_3 ) { _rtB ->
ezdepaihaz = true ; } } } else { if ( 0.0 != _rtP -> P_3 ) { _rtB ->
ezdepaihaz = false ; } _rtDW -> ac5hzcliph = 0 ; } UNUSED_PARAMETER ( tid ) ;
}
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { UNUSED_PARAMETER ( tid
) ; }
#define MDL_UPDATE
static void mdlUpdateTID1 ( SimStruct * S , int_T tid ) { UNUSED_PARAMETER (
tid ) ; } static void mdlInitializeSizes ( SimStruct * S ) { ssSetChecksumVal
( S , 0 , 3011126588U ) ; ssSetChecksumVal ( S , 1 , 1438754209U ) ;
ssSetChecksumVal ( S , 2 , 615490741U ) ; ssSetChecksumVal ( S , 3 ,
2178444624U ) ; { mxArray * slVerStructMat = NULL ; mxArray * slStrMat =
mxCreateString ( "simulink" ) ; char slVerChar [ 10 ] ; int status =
mexCallMATLAB ( 1 , & slVerStructMat , 1 , & slStrMat , "ver" ) ; if ( status
== 0 ) { mxArray * slVerMat = mxGetField ( slVerStructMat , 0 , "Version" ) ;
if ( slVerMat == NULL ) { status = 1 ; } else { status = mxGetString (
slVerMat , slVerChar , 10 ) ; } } mxDestroyArray ( slStrMat ) ;
mxDestroyArray ( slVerStructMat ) ; if ( ( status == 1 ) || ( strcmp (
slVerChar , "8.6" ) != 0 ) ) { return ; } } ssSetOptions ( S ,
SS_OPTION_EXCEPTION_FREE_CODE ) ; if ( ssGetSizeofDWork ( S ) != sizeof (
b5dwuj4txq ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal DWork sizes do "
"not match for accelerator mex file." ) ; } if ( ssGetSizeofGlobalBlockIO ( S
) != sizeof ( mkz4gnsvgt ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal BlockIO sizes do "
"not match for accelerator mex file." ) ; } { int ssSizeofParams ;
ssGetSizeofParams ( S , & ssSizeofParams ) ; if ( ssSizeofParams != sizeof (
h1j5haitu2 ) ) { static char msg [ 256 ] ; sprintf ( msg ,
"Unexpected error: Internal Parameters sizes do "
"not match for accelerator mex file." ) ; } } _ssSetDefaultParam ( S , (
real_T * ) & bvo14jvwsn ) ; } static void mdlInitializeSampleTimes (
SimStruct * S ) { slAccRegPrmChangeFcn ( S , mdlOutputsTID1 ) ; } static void
mdlTerminate ( SimStruct * S ) { }
#include "simulink.c"
