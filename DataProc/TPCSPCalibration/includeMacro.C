#include "RVersion.h"

void includeMacro() {
  #if ROOT_VERSION_CODE < ROOT_VERSION(6,0,0)
  // ROOT 5-only workarounds: set the appropriate include paths and trigger
  // preloading of the required libraries before invoking ACLiC on the
  // processing macro
  gSystem->SetIncludePath("-I$ALICE_PHYSICS/include -I$ALICE_ROOT/include");
  AliTPCDcalibRes *dummyTPCDcalibRes;
  #endif
}
