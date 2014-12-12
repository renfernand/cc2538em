/**
\brief Applications running on top of the OpenWSN stack.

\author Thomas Watteyne <watteyne@eecs.berkeley.edu>, September 2014.
*/

#include "opendefs.h"

// CoAP
#include "c6t.h"
#include "cinfo.h"
#include "cleds.h"
#include "cwellknown.h"
// TCP
#include "techo.h"
// UDP
#include "uecho.h"
// OSENS
#include "osens_app.h"

#define DAGROOT_ENABLE_ONSTARTUP  0   // RFF
//=========================== variables =======================================

//=========================== prototypes ======================================

//=========================== public ==========================================

//=========================== private =========================================

void openapps_init(void) {
   // CoAP
   c6t_init();
   cinfo_init();
   cleds__init();
   cwellknown_init();
   // TCP
   techo_init();
   // UDP
   uecho_init();
   //sensor board
   osens_app_init();

   //teste rff
#if DAGROOT_ENABLE_ONSTARTUP
   idmanager_setIsDAGroot(TRUE);
   idmanager_setIsBridge(TRUE);
#endif
   //teste rff



}
