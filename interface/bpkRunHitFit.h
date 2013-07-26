#ifndef BPKRUNHITFIT
#define BPKRUNHITFIT

/* #include <algorithm> */

/* #include "TopQuarkAnalysis/HitFit/interface/Defaults_Text.hpp" */
#include "MyAna/bpkHitFitForExcitedQuark/interface/HitFitTranslator.h"
#include "MyAna/bpkHitFitForExcitedQuark/interface/Lepjets_Event.h"
#include "MyAna/bpkHitFitForExcitedQuark/interface/TopGluon_Fit.h"
//#include "TopQuarkAnalysis/TopHitFit/interface/Top_Fit.h"

#include "TopQuarkAnalysis/TopHitFit/interface/Fit_Result.h"

static const unsigned int MIN_HITFIT_JET =   4 ;
static const unsigned int MIN_HITFIT_TTH =   6 ;
static const unsigned int MAX_HITFIT_JET =   8 ;
static const unsigned int MAX_HITFIT     = 1680;
static const unsigned int MAX_HITFIT_VAR =  32 ;

namespace hitfit{

  class bpkRunHitFit {

  private:

    LeptonTranslator     _LeptonTranslator;
    JetTranslator        _JetTranslator;
    METTranslator        _METTranslator;

    Lepjets_Event                       _event;

    std::vector<int>                   _jets; //index of jet

    bool                                _jetObjRes;

    TopGluon_Fit                             _TopGluon_Fit;
    //Top_Fit                             _Top_Fit;

    std::vector<Lepjets_Event>          _Unfitted_Events;

    std::vector<Fit_Result>             _Fit_Results;

    int                  _nu_solution;

  public:

    bpkRunHitFit(const LeptonTranslator& lep,
		 const JetTranslator&    jet,
		 const METTranslator&    met,
		 const std::string       default_file,
		 double                  lepw_mass,
		 double                  hadw_mass,
		 double                  top_mass,
         int                     nu_sol=-1);


    ~bpkRunHitFit();

    void clear();

    void AddLepton(const LepInfoBranches& leptons, const int index, bool useObjRes = false);

    void AddJet(const int index, bool useObjRes = false);
 
    void SetMet(const EvtInfoBranches& evt, bool useObjRes = false);

    void SetKtResolution(const Resolution& res);

    void SetMETResolution(const Resolution& res);

    const TopGluon_Fit& GetTopGluonFit() const;
    //const Top_Fit& GetTopFit() const;

    std::vector<Fit_Result>::size_type FitAllPermutation(const JetInfoBranches& jet, std::vector<bool> jetisbtag);

    std::vector<Lepjets_Event> GetUnfittedEvent();

    std::vector<Fit_Result> GetFitAllPermutation();

  };

} // namespace hitfit

#endif // #ifndef BPKRUNHITFIT
