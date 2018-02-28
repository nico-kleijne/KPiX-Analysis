/* Code to compare calibration files @DESY 2018-02-28
 * by Mengqing.Wu@desy.de
 */
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TApplication.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TStyle.h"
#include "THStack.h"
#include "TKey.h"
#include "TClass.h"
#include "TObject.h"

class RootCompare {

 protected:
  TFile *hfile; // output root file
  TCanvas c1; // canvas to write to output root file
 public:
  void TGraphStack(TGraph* graph_arr);
  void TH1FStack(TH1F* th1f_arr);
  
}
