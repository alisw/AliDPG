#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AliTPCDcalibRes.h"
#include <TF3.h>
#include <TString.h>
#endif

// This macro access 2 AliTPCDcalibRes objects from indicated paths and recreates ChebCorr and ChebDist
// objects by morphing these parameterizations of the Exta object into those of the Main object using
// provided weight function (see weighFunction below)
// The binning of the new parameterizations will correspond to those in Main.
// The Main AliTPCDcalibRes updated with new params is saved locally 

AliTPCDcalibRes* morhpMaps(const char* pathMain, // path to the alitpdcalibres file for the map to which
			   const char* pathExtra  // the map from this path will be merged
			   )
{
  TString path0 = pathMain;
  if (!gGrid && !TGrid::Connect("alien://") && !gGrid) {
    printf("Failed to connect to the GRID\n");
    return 0;
  }
  AliTPCDcalibRes* map0 = AliTPCDcalibRes::Load(pathMain);
  AliTPCDcalibRes* map1 = AliTPCDcalibRes::Load(pathExtra);
  if (!map0 || !map1) {
    printf("At least one of the maps is not found: %p %p\n", map0, map1);
    return 0;
  }
  if (!map0->LoadDataFromResTree(pathMain) || !map1->LoadDataFromResTree(pathExtra)) {
    printf("Failed to load data from the residual control trees (needed only for new control tree\n");
    return 0;    
  }
  
  // We need to provide a function to calculate weight between the main and extra source maps
  // The final parametrization will be produced for the source
  // w*map0 + (1.-w)*map1, with w = weighFunction(x, z2x, y2x)
  // The weighFunction can be TF1(x), TF2(x,z2x) or TF3(x,z2x,y2x) returning the value 0:1
  // for the standard map coordinates 85<x<250, -1<z2x<1,  -tan(pi/18)<y2x<tan(pi/18)

  // I use sigmoid function acting on the C-side only: below X~110 cm the Main map has weight~1, above it the Extra
  // map will dominated (as well as on the A side). The binnint of the Chebyshev param will be tailored for the Main map.
  TF2 weighFunction("w","y<0 ? 1.-1./(1.+exp(-(x-110.)/1.)) : 0.",70,250,-1,1);
  map0->SetUsedInstanceExt(map1);
  map0->SetMorphingFunctionXZY(&weighFunction);

  map0->SetUseChebForTraining(kTRUE);
  
  map0->CreateCorrectionObject();
  map0->CreateDistortionObject();

  map0->Save();
  
  map0->WriteResTree();
  
  return map0;
}
