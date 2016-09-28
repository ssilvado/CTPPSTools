#include "CTPPSTools/Filters/interface/DoubleArmFilter.h"

// class members definiton
DoubleArmFilter::DoubleArmFilter(const edm::ParameterSet& cfg):
  vertices_tk (consumes<DataTypes::VertexCollection>(cfg.getParameter<edm::InputTag>("vertices")))
, ppsreco_tk(consumes<DataTypes::PPSRecoCollection>(edm::InputTag("ppssim","PPSReco")))
, tofRes(cfg.getParameter<double>("tofRes")*pow(10,-12)) // in seconds
{
  produces<std::vector<std::pair<size_t,size_t> > >("tracks");
}

bool DoubleArmFilter::filter(edm::Event& event, const edm::EventSetup& setup)
{

  // initialize filter result
  result = false;

  event.getByToken(vertices_tk, vertices); 
  event.getByToken(ppsreco_tk, ppsreco);
   
  // check collections
  if( !vertices.isValid()
   || !ppsreco.isValid()
    ) {
    throw cms::Exception("DoubleArmFilter") << "\n +" << __FILE__ << ":" << __LINE__ << ":"
    << "\n filter(): One of the following collections could not be retrieved" 
    << "\n filter(): from root file:"
    << "\n filter(): vertices.isValid()=" << vertices.isValid()
    << "\n filter(): ppsreco.isValid()=" << ppsreco.isValid()
    << "\n filter(): Aborting.\n\n";
  }
   
  // check primary vertex
  if (!vertices->size()) return result;

  // check pps vertex
  if (!ppsreco->Vertices->size()) return result;

  // signal pv_z (cm)
  const double pv_z = vertices->at(0).z();

  // speed of light (m/s)
  const double c = 3.*pow(10,8);

  // pps_z resolution (cm)
  const double ppsz_resolution = ((c/2.)*sqrt(2)*tofRes)*pow(10,2);


  std::vector<std::pair<size_t,size_t> > tracks;

  for (size_t i=0; i < ppsreco->ArmF.Tracks.size(); i++) {
    for (size_t j=0; j < ppsreco->ArmB.Tracks.size(); j++) {

      double fwd_tof = ppsreco->ArmF.Tracks.at(i).ToF.ToF;
      double bkw_tof = ppsreco->ArmB.Tracks.at(j).ToF.ToF;

      if(fwd_tof==0 || bkw_tof==0) continue;
	    
      // delta ToF (s)
      const double deltaTof = (bkw_tof-fwd_tof)*pow(10,-9);

      // pps z (cm)
      const double pps_z = ((c/2.)*deltaTof)*pow(10,2);

      const double zppsmax = pps_z + ppsz_resolution;
      const double zppsmin = pps_z - ppsz_resolution;	
	
      if(zppsmin < pv_z && pv_z < zppsmax) tracks.push_back(std::make_pair(i,j));
    }
  }

  // at least 1 pps vertex matched to cms
  if(tracks.size()) {

    // passed filter
    result = true;

    std::auto_ptr<std::vector<std::pair<size_t,size_t> > > ptracks(new std::vector<std::pair<size_t,size_t> >);
//    ptracks->reserve(tracks.size());
    *ptracks = tracks;

    event.put(ptracks,"tracks");
  }


  return result;
}

// define edm plugin
DEFINE_FWK_MODULE(DoubleArmFilter);
