#include "CTPPSTools/Analyzers/interface/DoubleArmAnalyzer.h"

// class members definiton
DoubleArmAnalyzer::DoubleArmAnalyzer(const edm::ParameterSet& cfg) :
  vertices_tk (consumes<DataTypes::VertexCollection>(cfg.getParameter<edm::InputTag>("vertices")))
, ppsreco_tk(consumes<DataTypes::PPSRecoCollection>(edm::InputTag("ppssim","PPSReco")))
, tofRes(cfg.getParameter<double>("tofRes")*pow(10,-12)) // in seconds
{
   //now do what ever initialization is needed
   usesResource("TFileService");
}


void DoubleArmAnalyzer::beginJob()
{
  // initialize
  doubles["pps_fwd_ntracks"] = 99999;
  doubles["pps_bkw_ntracks"] = 99999;
  doubles["pps_fwd_matched_tracks"] = 99999;
  doubles["pps_bkw_matched_tracks"] = 99999;
  doubles["pps_matched_vertices"] = 99999;

  tree = fs->make<TTree>( "Events","Events" );
  tree->Branch("pps_fwd_ntracks",&doubles["pps_fwd_ntracks"],"pps_fwd_ntracks/D");
  tree->Branch("pps_bkw_ntracks",&doubles["pps_bkw_ntracks"],"pps_bkw_ntracks/D");
  tree->Branch("pps_fwd_matched_tracks",&doubles["pps_fwd_matched_tracks"],"pps_fwd_matched_tracks/D");
  tree->Branch("pps_bkw_matched_tracks",&doubles["pps_bkw_matched_tracks"],"pps_bkw_matched_tracks/D");
  tree->Branch("pps_matched_vertices",&doubles["pps_matched_vertices"],"pps_matched_vertices/D");
}

void DoubleArmAnalyzer::endJob() 
{

}


void DoubleArmAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
  // reset variables in each event
  doubles["pps_fwd_ntracks"] = 99999;
  doubles["pps_bkw_ntracks"] = 99999;
  doubles["pps_fwd_matched_tracks"] = 99999;
  doubles["pps_bkw_matched_tracks"] = 99999;
  doubles["pps_matched_vertices"] = 99999;

  event.getByToken(vertices_tk, vertices); 
  event.getByToken(ppsreco_tk, ppsreco);
   
  // check collections
  if( !vertices.isValid()
   || !ppsreco.isValid()
    ) {
    throw cms::Exception("DoubleArmAnalyzer") << "\n +" << __FILE__ << ":" << __LINE__ << ":"
    << "\n analyzer(): One of the following collections could not be retrieved" 
    << "\n analyzer(): from root file:"
    << "\n analyzer(): vertices.isValid()=" << vertices.isValid()
    << "\n analyzer(): ppsreco.isValid()=" << ppsreco.isValid()
    << "\n analyzer(): Aborting.\n\n";
  }
   
  // check primary vertex
  if (!vertices->size()) {
    edm::LogWarning("DoubleArmAnalyzer") << "\n +" << __FILE__ << ":" << __LINE__ << ":"
    << "\n analyzer(): No primary vertex in this event! Skiping event...";
    return;
  }

  // check pps vertex
  if (!ppsreco->Vertices->size()) {
    edm::LogWarning("DoubleArmAnalyzer") << "\n +" << __FILE__ << ":" << __LINE__ << ":"
    << "\n analyzer(): No PPS vertex in this event! Skiping event...";
    return;
  }

  // signal pv_z (cm)
  const double pv_z = vertices->at(0).z();

  // speed of light (m/s)
  const double c = 3.*pow(10,8);

  // pps_z resolution (cm)
  const double ppsz_resolution = ((c/2.)*sqrt(2)*tofRes)*pow(10,2);


  std::vector<std::pair<size_t,size_t> > tracks;
  std::set<size_t> fwd_tracks;
  std::set<size_t> bkw_tracks;

  edm::LogWarning("DoubleArmAnalyzer")
  << "\n\n analyzer(): -----------------PPS vertices-----------------\n"

  for (size_t i=0; i < ppsreco->ArmF.Tracks.size(); i++) {
    for (size_t j=0; j < ppsreco->ArmB.Tracks.size(); j++) {

      // delta ToF (s)
      const double deltaTof = (ppsreco->ArmB.Tracks.at(j).ToF.ToF - ppsreco->ArmF.Tracks.at(i).ToF.ToF)*pow(10,-9);

      // pps z (cm)
      const double pps_z = ((c/2.)*deltaTof)*pow(10,2);

      const double zppsmax = pps_z + ppsz_resolution;
      const double zppsmin = pps_z - ppsz_resolution;	
	
      if(zppsmin < pv_z && pv_z < zppsmax) {
        tracks.push_back(std::make_pair(i,j));
        fwd_tracks.insert(i); // do not duplicate entry
        bkw_tracks.insert(j); // do not duplicate entry

        edm::LogWarning("DoubleArmAnalyzer")
        << "\n analyzer(): PPS vertex matched to PV:\n"
        << "\n analyzer(): track combination: fwd(" << i << "),bkw(" << j << ")"
        << "\n analyzer(): pv_z: " << pv_z
        << "\n analyzer(): pps_vertex_z: " << pps_z
        << "\n analyzer(): pps_vertex_z-pv_z [mm]: " << (pps_z-pv_z)*10
      }
    }
  }


  // total tracks
  doubles["pps_fwd_ntracks"] = ppsreco->ArmF.Tracks.size();
  doubles["pps_bkw_ntracks"] = ppsreco->ArmB.Tracks.size();

  // matched vertices
  doubles["pps_matched_vertices"] = tracks.size();

  // matched tracks
  doubles["pps_fwd_matched_tracks"] = fwd_tracks.size();
  doubles["pps_bkw_matched_tracks"] = bkw_tracks.size();


  edm::LogWarning("DoubleArmAnalyzer")
  << "\n\n analyzer(): -----------------RESULTS-----------------\n"
  << "\n analyzer(): forward tracks  : total   =" << doubles["pps_fwd_ntracks"]
  << "\n analyzer(): backward tracks : total   =" << doubles["pps_bkw_ntracks"]
  << "\n analyzer(): forward tracks  : matched =" << doubles["pps_fwd_matched_tracks"]
  << "\n analyzer(): backward tracks : matched =" << doubles["pps_bkw_matched_tracks"]
  << "\n analyzer(): pps vertices    : matched =" << doubles["pps_matched_vertices"];

  tree->Fill();

}

// define edm plugin
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(DoubleArmAnalyzer);
