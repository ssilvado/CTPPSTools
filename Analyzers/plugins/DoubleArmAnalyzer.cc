#include "CTPPSTools/Analyzers/interface/DoubleArmAnalyzer.h"

// class members definiton
DoubleArmAnalyzer::DoubleArmAnalyzer(const edm::ParameterSet& cfg) :
  ppsreco_tk(consumes<DataTypes::PPSRecoCollection>(edm::InputTag("ppssim","PPSReco")))
{
   //now do what ever initialization is needed
   usesResource("TFileService");
}


void DoubleArmAnalyzer::beginJob()
{
  // initialize
  vecdoubles["pps_fwd_t"] = new std::vector<double>();
  vecdoubles["pps_fwd_xi"] = new std::vector<double>();
  vecdoubles["pps_bkw_t"] = new std::vector<double>();
  vecdoubles["pps_bkw_xi"] = new std::vector<double>();

  tree = fs->make<TTree>( "Events","Events" );
  tree->Branch("pps_fwd_t","std::vector<double>",&vecdoubles["pps_fwd_t"]);
  tree->Branch("pps_fwd_xi","std::vector<double>",&vecdoubles["pps_fwd_xi"]);
  tree->Branch("pps_bkw_t","std::vector<double>",&vecdoubles["pps_bkw_t"]);
  tree->Branch("pps_bkw_xi","std::vector<double>",&vecdoubles["pps_bkw_xi"]);
}

void DoubleArmAnalyzer::endJob() 
{

}


void DoubleArmAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
  // reset variables in each event
  vecdoubles["pps_fwd_t"]->clear();
  vecdoubles["pps_fwd_xi"]->clear();
  vecdoubles["pps_bkw_t"]->clear();
  vecdoubles["pps_bkw_xi"]->clear();

  event.getByToken(ppsreco_tk, ppsreco);
   
  // check collections
  if( !ppsreco.isValid()
    ) {
    throw cms::Exception("DoubleArmAnalyzer") << "\n +" << __FILE__ << ":" << __LINE__ << ":"
    << "\n analyzer(): One of the following collections could not be retrieved" 
    << "\n analyzer(): from root file:"
    << "\n analyzer(): ppsreco.isValid()=" << ppsreco.isValid()
    << "\n analyzer(): Aborting.\n\n";
  }


  for (size_t i=0; i < ppsreco->ArmF.Tracks.size(); i++) {
    if(ppsreco->ArmF.Tracks.at(i).ToF.ToF==0) continue;
    vecdoubles["pps_fwd_t"]->push_back(ppsreco->ArmF.Tracks.at(i).t);
    vecdoubles["pps_fwd_xi"]->push_back(ppsreco->ArmF.Tracks.at(i).xi);
  }
  for (size_t i=0; i < ppsreco->ArmB.Tracks.size(); i++) {
    if(ppsreco->ArmB.Tracks.at(i).ToF.ToF==0) continue;
    vecdoubles["pps_bkw_t"]->push_back(ppsreco->ArmB.Tracks.at(i).t);
    vecdoubles["pps_bkw_xi"]->push_back(ppsreco->ArmB.Tracks.at(i).xi);
  }

  tree->Fill();

}

// define edm plugin
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(DoubleArmAnalyzer);
