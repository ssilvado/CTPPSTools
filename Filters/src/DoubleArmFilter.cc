#include "CTPPSTools/Filters/interface/DoubleArmFilter.h"

// class members definiton
DoubleArmFilter::DoubleArmFilter(const edm::ParameterSet& cfg):
  vertices_tk (consumes<DataTypes::VertexCollection>(cfg.getParameter<edm::InputTag>("vertices")))
, ppsreco_tk(consumes<DataTypes::PPSRecoCollection>(edm::InputTag("ppssim","PPSReco")))
, tofRes(cfg.getParameter<unsigned int>("tofRes"))
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
   
  if (!vertices->size()) return result;

  if (!ppsreco->Vertices->size()) {
    edm::LogVerbatim("DoubleArmFilter")
    << "\n filter(): No PPS vertex for this event."
    << "\n filter(): skiping..."; 
    return result;
  }
   
  // signal vertex
  const double vertex = vertices->at(0).z();

  // convert to seconds
  tofRes *= pow(10,-12);

  // speed of light (m/s)
  const double c = 3.*pow(10,8);

  // z resolution (cm)
  const double ppsz_resolution = ((c/2.)*sqrt(2)*tofRes)*pow(10,2);


  std::vector<std::pair<size_t,size_t> > tracks;

  for (size_t i=0; i < ppsreco->ArmF.Tracks.size(); i++) {
    for (size_t j=0; j < ppsreco->ArmB.Tracks.size(); j++) {

      // delta ToF (s)
      const double deltaTof = (ppsreco->ArmB.Tracks.at(j).ToF.ToF - ppsreco->ArmF.Tracks.at(i).ToF.ToF)*pow(10,-9);

      // pps vertex z (cm)
		  const double ZPPS_V = ((c/2.)*deltaTof)*pow(10,2);

		  const double zppsmax = ZPPS_V + ppsz_resolution;
		  const double zppsmin = ZPPS_V - ppsz_resolution;	
	
      if(zppsmin < vertex && vertex < zppsmax) tracks.push_back(std::make_pair(i,j));
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
