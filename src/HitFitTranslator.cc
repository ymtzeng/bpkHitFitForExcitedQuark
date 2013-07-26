#include <stdio.h>
#include <stdlib.h>

#include "MyAna/bpkHitFitForExcitedQuark/interface/HitFitTranslator.h"
#include "MyAna/bprimeKit/interface/format.h"

namespace hitfit {

  // LeptonTranslator
  LeptonTranslator::LeptonTranslator()
  {

    std::string CMSSW_BASE(getenv("CMSSW_BASE"));
    std::string resolution_filename = CMSSW_BASE +
      std::string("/src/TopQuarkAnalysis/TopHitFit/data/resolution/tqafElectronResolution.txt");
    electronResolution_ = EtaDepResolution(resolution_filename);
    resolution_filename = CMSSW_BASE +
      std::string("/src/TopQuarkAnalysis/TopHitFit/data/resolution/tqafMuonResolution.txt");
    muonResolution_ = EtaDepResolution(resolution_filename);

  } // LeptonTranslator::LeptonTranslator()


  LeptonTranslator::LeptonTranslator(const std::string& elfile, const std::string& mufile)
  {

    std::string CMSSW_BASE(getenv("CMSSW_BASE"));
    std::string resolution_filename;

    if (elfile.empty()) {
      resolution_filename = CMSSW_BASE +
        std::string("/src/TopQuarkAnalysis/TopHitFit/data/resolution/tqafElectronResolution.txt");
    } else {
      resolution_filename = elfile ;
    }
    electronResolution_ = EtaDepResolution(resolution_filename);

    if (mufile.empty()) {
      resolution_filename = CMSSW_BASE +
        std::string("/src/TopQuarkAnalysis/TopHitFit/data/resolution/tqafMuonResolution.txt");
    } else {
      resolution_filename = mufile ;
    }
    muonResolution_ = EtaDepResolution(resolution_filename);

  } // LeptonTranslator::LeptonTranslator(const std::string& elfile, const std::strin& mufile)


  LeptonTranslator::~LeptonTranslator()
  {
  }


  Lepjets_Event_Lep
  LeptonTranslator::operator()(const LepInfoBranches& leptons,
			       const int index,
			       int type /* = hitfit::lepton_label */,
			       bool useObjEmbRes /* = false */)
  {

    Fourvec p(leptons.Px[index],leptons.Py[index],leptons.Pz[index],leptons.Energy[index]);

    double            lepton_eta        = leptons.Eta[index];
    Vector_Resolution lepton_resolution;
    if(leptons.LeptonType[index]==11)
      lepton_resolution = electronResolution_.GetResolution(lepton_eta);
    else if(leptons.LeptonType[index]==13)
      lepton_resolution = muonResolution_.GetResolution(lepton_eta);

    Lepjets_Event_Lep lepton(p,
			     lepton_label,
			     lepton_resolution);
    return lepton;

  } // Lepjets_Event_Lep LeptonTranslator::operator()


  const EtaDepResolution&
  LeptonTranslator::electronResolution() const
  {
    return electronResolution_;
  }

  const EtaDepResolution&
  LeptonTranslator::muonResolution() const
  {
    return muonResolution_;
  }

  bool
  LeptonTranslator::CheckEta(const LepInfoBranches& leptons, const int index) const
  {
    if(leptons.LeptonType[index] == 11)
      return electronResolution_.CheckEta(leptons.Eta[index]);
    else if(leptons.LeptonType[index] == 13)
      return muonResolution_.CheckEta(leptons.Eta[index]);
    else
      return false;
  }

  // JetTranslator
  JetTranslator::JetTranslator()
  {

    std::string CMSSW_BASE(getenv("CMSSW_BASE"));
    std::string resolution_filename = CMSSW_BASE +
      std::string("/src/TopQuarkAnalysis/TopHitFit/data/resolution/tqafUdscJetResolution.txt");
    udscResolution_ = EtaDepResolution(resolution_filename);
    resolution_filename = CMSSW_BASE +
      std::string("/src/TopQuarkAnalysis/TopHitFit/data/resolution/tqafBJetResolution.txt");
    bResolution_    = EtaDepResolution(resolution_filename);
    jetCorrectionLevel_ = "L7Parton";
    jes_  = 1.0;
    jesB_ = 1.0;

  } // JetTranslator::JetTranslator()


  JetTranslator::JetTranslator(const std::string& udscFile,
			       const std::string& bFile)
  {

    std::string CMSSW_BASE(getenv("CMSSW_BASE"));
    std::string udscResolution_filename;
    std::string bResolution_filename;

    if (udscFile.empty()) {
      udscResolution_filename = CMSSW_BASE +
        std::string("/src/TopQuarkAnalysis/TopHitFit/data/resolution/tqafUdscJetResolution.txt");
    } else {
      udscResolution_filename = udscFile;
    }

    if (bFile.empty()) {
      bResolution_filename = CMSSW_BASE +
        std::string("/src/TopQuarkAnalysis/TopHitFit/data/resolution/tqafBJetResolution.txt");
    } else {
      bResolution_filename = bFile;
    }

    udscResolution_ = EtaDepResolution(udscResolution_filename);
    bResolution_    = EtaDepResolution(bResolution_filename);
    jetCorrectionLevel_ = "L7Parton";
    jes_  = 1.0;
    jesB_ = 1.0;

  } // JetTranslator::JetTranslator(const std::string& ifile)

  JetTranslator::JetTranslator(const std::string& udscFile,
			       const std::string& bFile,
                               const std::string& jetCorrectionLevel,
			       double jes,
			       double jesB)
  {

    std::string CMSSW_BASE(getenv("CMSSW_BASE"));
    std::string udscResolution_filename;
    std::string bResolution_filename;

    if (udscFile.empty()) {
      udscResolution_filename = CMSSW_BASE +
        std::string("/src/TopQuarkAnalysis/TopHitFit/data/resolution/tqafUdscJetResolution.txt");
    } else {
      udscResolution_filename = udscFile;
    }

    if (bFile.empty()) {
      bResolution_filename = CMSSW_BASE +
        std::string("/src/TopQuarkAnalysis/TopHitFit/data/resolution/tqafBJetResolution.txt");
    } else {
      bResolution_filename = bFile;
    }

    udscResolution_ = EtaDepResolution(udscResolution_filename);
    bResolution_    = EtaDepResolution(bResolution_filename);
    jetCorrectionLevel_ = jetCorrectionLevel;
    jes_  = jes;
    jesB_ = jesB;

  } // JetTranslator::JetTranslator(const std::string& ifile)

  JetTranslator::~JetTranslator()
  {
  } // JetTranslator::~JetTranslator()


  Lepjets_Event_Jet
  JetTranslator::operator()(const JetInfoBranches& jets,
			    const int index,
			    int type /*= hitfit::unknown_label */,
			    bool useObjEmbRes /* = false */)
  {

    Fourvec p;

    double            jet_eta        = jets.Eta[index];

    Vector_Resolution jet_resolution;

    if (type == hitfit::hadb_label || type == hitfit::lepb_label || type == hitfit::higgs_label) {
      jet_resolution = bResolution_.GetResolution(jet_eta);

      //float scale = jets.Pt[index]>0. ? jesB_*jets.PtCorrL7b[index] / jets.Pt[index] : 0.;
      float scale = jesB_;
      if(jets.Pt[index]>0.) {
	if(jetCorrectionLevel_.find("L7")!=std::string::npos) scale*=jets.PtCorrL7b[index] / jets.Pt[index];
	else if(jetCorrectionLevel_.find("L3")!=std::string::npos) scale*=jets.PtCorrL3[index] / jets.Pt[index];
      }

      p = Fourvec(jets.Px[index]*scale,jets.Py[index]*scale,jets.Pz[index]*scale,jets.Energy[index]*scale);

    } else {
      jet_resolution = udscResolution_.GetResolution(jet_eta);

      //float scale = jets.Pt[index]>0. ? jes_*jets.PtCorrL7uds[index] / jets.Pt[index] : 0.;
      float scale = jes_;
      if(jets.Pt[index]>0.) {
	if(jetCorrectionLevel_.find("L7")!=std::string::npos) scale*=jets.PtCorrL7uds[index] / jets.Pt[index];
	else if(jetCorrectionLevel_.find("L3")!=std::string::npos) scale*=jets.PtCorrL3[index] / jets.Pt[index];
      }

      p = Fourvec(jets.Px[index]*scale,jets.Py[index]*scale,jets.Pz[index]*scale,jets.Energy[index]*scale);
    }

    Lepjets_Event_Jet retjet(p,
                             type,
                             jet_resolution);
    return retjet;

  } // Lepjets_Event_Jet JetTranslator::operator()(const pat::Jet& j,int type)


  const EtaDepResolution&
  JetTranslator::udscResolution() const
  {
    return udscResolution_;
  }


  const EtaDepResolution&
  JetTranslator::bResolution() const
  {
    return bResolution_;
  }


  bool
  JetTranslator::CheckEta(const JetInfoBranches& jets, const int index) const
  {
    double jet_eta = jets.Eta[index];
    return bResolution_.CheckEta(jet_eta) && udscResolution_.CheckEta(jet_eta);
  }

  // METTranslator
  METTranslator::METTranslator()
  {
    resolution_ = Resolution(std::string("0,0,12"));
  } // METTranslator::METTranslator()


  METTranslator::METTranslator(const std::string& ifile)
  {
    const Defaults_Text defs(ifile);
    std::string resolution_string(defs.get_string("met_resolution"));
    resolution_ = Resolution(resolution_string);

  } // METTranslator::METTranslator(const std::string& ifile)


  METTranslator::~METTranslator()
  {
  } // METTranslator::~METTranslator()


  Fourvec
  METTranslator::operator()(const EvtInfoBranches& evt,
			    bool useObjEmbRes /* = false */)
  {
    return Fourvec (evt.PFMETx,evt.PFMETy,0.0,evt.PFMET);
  } // Fourvec METTranslator::operator()(const EvtInfoBranches& evt)


  Resolution
  METTranslator::KtResolution(const EvtInfoBranches& evt,
			      bool useObjEmbRes /* = false */) const
  {
    return resolution_;
  } // Resolution METTranslator::KtResolution(const EvtInfoBranches& evt)


  Resolution
  METTranslator::METResolution(const EvtInfoBranches& evt,
			       bool useObjEmbRes /* = false */) const
  {
    return KtResolution(evt,useObjEmbRes);
  } // Resolution METTranslator::METResolution(const EvtInfoBranches& evt)

}
