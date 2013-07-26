#ifndef HITFITTRANSLATOR
#define HITFITTRANSLATOR

#include "TopQuarkAnalysis/TopHitFit/interface/EtaDepResolution.h"
#include "TopQuarkAnalysis/TopHitFit/interface/Lepjets_Event_Lep.h"
#include "MyAna/bpkHitFitForExcitedQuark/interface/Lepjets_Event_Jet.h"
#include "TopQuarkAnalysis/TopHitFit/interface/fourvec.h"

class LepInfoBranches;
class JetInfoBranches;
class EvtInfoBranches;

namespace hitfit {

  class LeptonTranslator {

  public:

    LeptonTranslator();
    LeptonTranslator(const std::string& elfile,
		     const std::string& mufile);
    ~LeptonTranslator();

    Lepjets_Event_Lep operator()(const LepInfoBranches& leptons,
				 const int index,
				 int type = hitfit::lepton_label,
				 bool useObjEmbRes = false);

    const EtaDepResolution& electronResolution() const;
    const EtaDepResolution& muonResolution() const;

    bool CheckEta(const LepInfoBranches& leptons, const int index) const;


  private:

    EtaDepResolution electronResolution_;
    EtaDepResolution muonResolution_;

  }; //class LeptonTranslator


  class JetTranslator {

  public:

    JetTranslator();
    JetTranslator(const std::string& udscFile,
		  const std::string& bFile);
    JetTranslator(const std::string& udscFile,
		  const std::string& bFile,
		  const std::string& jetCorrectionLevel,
		  double jes,
		  double jesB);
    ~JetTranslator();

    Lepjets_Event_Jet operator()(const JetInfoBranches& jets,
				 const int index,
				 int type = hitfit::unknown_label,
				 bool useObjEmbRes = false);


    const EtaDepResolution& udscResolution() const;
    const EtaDepResolution& bResolution() const;

    bool CheckEta(const JetInfoBranches& jets, const int index) const;


  private:

    EtaDepResolution udscResolution_;
    EtaDepResolution bResolution_;

    std::string jetCorrectionLevel_;
    double jes_;
    double jesB_;

  }; //class JetTranslator


  class METTranslator {

  public:

    METTranslator();
    METTranslator(const std::string& ifile);
    ~METTranslator();

    Fourvec operator() (const EvtInfoBranches& evt,
			bool useObjEmbRes = false);

    Resolution KtResolution(const EvtInfoBranches& evt,
			    bool useObjEmbRes = false) const;
    Resolution METResolution(const EvtInfoBranches& evt,
			     bool useObjEmbRes = false) const;

  private:

    Resolution resolution_;

  }; //class METTranslator

} // namespace hitfit

#endif // #ifndef HitFitTranslator
