#include <algorithm>

#include "MyAna/bpkHitFitForExcitedQuark/interface/bpkRunHitFit.h"
#include "TopQuarkAnalysis/TopHitFit/interface/Defaults_Text.h"
#include "MyAna/bprimeKit/interface/format.h"

// Explanation about the MIN/MAX definitions:
//
// For a given number of jets, there is a corresponding number of
// permutations how to assign each jet in the event to the corresponding
// parton-level jet.
// The number of permutations up to 10 jets are given below for Tt and
// TtH events.
//
// NJet         Npermutation (Tt)       Npermutation (TtH)
// 4            24                      --
// 5            120                     --
// 6            360                     360
// 7            840                     2520
// 8            1680                    10080
// 9            3024                    30240
// 10           5040                    75600
//
// The formulas for the number of permutations for Tt and TtH events
// given n jets in the event are
//
//         n!
// Tt:  -------- ; n >= 4
//      (n - 4)!
//
//          n!
// TtH: ---------- ; n >= 6
//      (n - 6)!2!
//
// The current MAX settings are chosen for a maximum number of 8 jets
// Increasing this limit should be done with caution, as it will
// increase the number of permutations rapidly.
//

namespace hitfit{

  bpkRunHitFit::bpkRunHitFit(const LeptonTranslator& lep,
			     const JetTranslator&    jet,
			     const METTranslator&    met,
			     const std::string       default_file,
			     double                  lepw_mass,
			     double                  hadw_mass,
			     double                  top_mass,
                 int                     nu_sol):
    _LeptonTranslator(lep),
    _JetTranslator(jet),
    _METTranslator(met),
    _event(0,0),
    _jetObjRes(false),
    //_Top_Fit(Top_Fit_Args(Defaults_Text(default_file)),lepw_mass,hadw_mass,top_mass)
    _TopGluon_Fit(TopGluon_Fit_Args(Defaults_Text(default_file)),lepw_mass,hadw_mass,top_mass),
    _nu_solution(nu_sol)
  {
    if(_nu_solution<0 || _nu_solution>1) _nu_solution=2;
  }

  bpkRunHitFit::~bpkRunHitFit()
  {
  }

  void bpkRunHitFit::clear()
  {
    _event = Lepjets_Event(0,0);
    _jets.clear();
    _jetObjRes = false;
    _Unfitted_Events.clear();
    _Fit_Results.clear();
  }

  void bpkRunHitFit::AddLepton(const LepInfoBranches& leptons,
			       const int index,
			       bool useObjRes)
  {
    _event.add_lep(_LeptonTranslator(leptons,index,lepton_label,useObjRes));
    return;
  }

  void bpkRunHitFit::AddJet(const int index,
			    bool useObjRes)
  {
    // Only set flag when adding the first jet
    // the additional jets then WILL be treated in the
    // same way like the first jet.
    if (_jets.empty()) {
      _jetObjRes = useObjRes;
    }

    if (_jets.size() < MAX_HITFIT_JET) {
      _jets.push_back(index);
    }
    return;
  }

  void bpkRunHitFit::SetMet(const EvtInfoBranches& evt,
			    bool useObjRes)
  {
    _event.met()    = _METTranslator(evt,useObjRes);
    _event.kt_res() = _METTranslator.KtResolution(evt,useObjRes);
    return;
  }

  void bpkRunHitFit::SetKtResolution(const Resolution& res)
  {
    _event.kt_res() = res;
    return;
  }

  void bpkRunHitFit::SetMETResolution(const Resolution& res)
  {
    SetKtResolution(res);
    return;
  }

  const TopGluon_Fit& bpkRunHitFit::GetTopGluonFit() const
  {
    return _TopGluon_Fit;
  }

/*
  const Top_Fit& bpkRunHitFit::GetTopFit() const
  {
    return _Top_Fit;
  }
  */

  std::vector<Fit_Result>::size_type bpkRunHitFit::FitAllPermutation(const JetInfoBranches& jet, std::vector<bool> jetisbtag)
  {
    if (_jets.size() < MIN_HITFIT_JET) {
      // For ttbar lepton+jets, a minimum of MIN_HITFIT_JETS jets
      // is required
      return 0;
    }

    if (_jets.size() > MAX_HITFIT_JET) {
      // Restrict the maximum number of jets in the fit
      // to prevent loop overflow
      return 0;
    }

    _Unfitted_Events.clear();
    _Fit_Results.clear();

    // Prepare the array of jet types for permutation
    std::vector<int> jet_types (_jets.size(), unknown_label);
    jet_types[0] = lepb_label;
    jet_types[1] = hadb_label;
    jet_types[2] = hadw1_label;
    jet_types[3] = hadw1_label;

    jet_types[4] = gluon1_label;
    jet_types[5] = gluon2_label;

    std::stable_sort(jet_types.begin(),jet_types.end());

    const int nustart = (_nu_solution==1) ? _nu_solution : 0;//

    int Npermutation_ = 0;
    int NpermutationB4Reduced_ = 0;

	// Find out the max number of btagged jets
	int Nbtag_jets = 0;
    int jet_size_ = _jets.size();
	for(int i = 0; i < jet_size_; i ++){
		//if(jet.CombinedSVBJetTags[_jets[i]] > 0.679) Nbtag_jets++;
		if(jetisbtag[_jets[i]]) Nbtag_jets++;
	}
	//std::cout << "Number of btag jets : " << Nbtag_jets << std::endl;
    do {
      NpermutationB4Reduced_++;
      // require at least one bjet
      bool EventWithAtLeastOneBtagJet = false;
      int Nbtag_ = 0;
      for (int j = 0 ; j != jet_size_; j++) {
		if(jet_types[j] == lepb_label || jet_types[j] == hadb_label){
      	  //if(jet.CombinedSVBJetTags[_jets[j]] > 0.679){
      	  if(jetisbtag[_jets[j]]){
				EventWithAtLeastOneBtagJet = true;
		  		Nbtag_++;
		  }
	    }
      }
      if(Nbtag_jets <= 1) if(!EventWithAtLeastOneBtagJet) continue;
	  if(Nbtag_jets > 1) if(Nbtag_ < 2) continue;
	  //if(!EventWithAtLeastOneBtagJet) continue;
      //if(Nbtag_!=2) continue;	// require 2 bjets

/*
      bool GluonWithoutPassingBtag = true;
      for (int j = 0 ; j != jet_size_; j++) {
	if(jet_types[j] == gluon1_label || jet_types[j] == gluon2_label){
          if(jet.CombinedSVBJetTags[_jets[j]] > 0.679) GluonWithoutPassingBtag = false;
	}
      }
      */
      //if(!GluonWithoutPassingBtag) continue;

      Npermutation_++;
      // begin loop over all jet permutation
      for (int nusol = nustart ; nusol != 2 ; nusol++) {
    if(nusol > _nu_solution) break;
	// loop over two neutrino solution
	bool nuz = bool(nusol);

	// Copy the event
	Lepjets_Event fev = _event;

	// Add jets into the event, with the assumed type
	// in accord with the permutation.
	// The translator _JetTranslator will correctly
	// return object of Lepjets_Event_Jet with
	// jet energy correction applied in accord with
	// the assumed jet type (b or light).
	for (size_t j = 0 ; j != _jets.size(); j++) {
	  fev.add_jet(_JetTranslator(jet,_jets[j],jet_types[j],_jetObjRes));
	}

	// Clone fev (intended to be fitted event)
	// to ufev (intended to be unfitted event)
	Lepjets_Event ufev = fev;

	// Set jet types.
	fev.set_jet_types(jet_types);
	ufev.set_jet_types(jet_types);

	// Store the unfitted event
	_Unfitted_Events.push_back(ufev);

	// Prepare the placeholder for various kinematic quantities
	double umwhad;
	double utmass;
	double mt;
	double sigmt;
	Column_Vector pullx;
	Column_Vector pully;

	// Do the fit
	double chisq= _TopGluon_Fit.fit_one_perm(fev,
	//double chisq= _Top_Fit.fit_one_perm(fev,
					    nuz,
					    umwhad,
					    utmass,
					    mt,
					    sigmt,
					    pullx,
					    pully);

	//std::cout<<"mt "<<mt<<" utmass "<<utmass<<std::endl;
	// Store output of the fit
	_Fit_Results.push_back(Fit_Result(chisq,
					  fev,
					  pullx,
					  pully,
					  umwhad,
					  utmass,
					  mt,
					  sigmt));

      } // end loop over two neutrino solution

    } while (std::next_permutation (jet_types.begin(), jet_types.end()));
    // end loop over all jet permutations

    std::cout<<"reduced permutations (b4)  : "<<Npermutation_<<" ( "<<NpermutationB4Reduced_<<" ) ; _jets.size() : "<<_jets.size()
    <<std::endl;

    return _Fit_Results.size();

  }

  std::vector<Lepjets_Event> bpkRunHitFit::GetUnfittedEvent()
  {
    return _Unfitted_Events;
  }

  std::vector<Fit_Result> bpkRunHitFit::GetFitAllPermutation()
  {
    return _Fit_Results;
  }

} // namespace hitfit
