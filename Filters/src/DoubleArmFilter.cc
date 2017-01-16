#include "CTPPSTools/Filters/interface/DoubleArmFilter.h"

// class members definiton
DoubleArmFilter::DoubleArmFilter(const edm::ParameterSet& cfg):
  vertices_tk (consumes<DataTypes::VertexCollection>(cfg.getParameter<edm::InputTag>("vertices")))
, ppsreco_tk(consumes<DataTypes::PPSRecoCollection>(edm::InputTag("ppssim","PPSReco")))
, tofRes(cfg.getParameter<double>("tofRes")*pow(10,-12)) // convert to seconds
{
  produces<std::vector<size_t>("goldenVertices");
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
   
  // at least 1 primary vertex
  if(!vertices->size()) return result;

  // signal pv_z (cm)
  const double pv_z = vertices->at(0).z();

  // at least 1 pps vertex
  if(!ppsreco->Vertices->size()) return result;

  // select combinations using tracks with no double hit
  std::vector<size_t> gvertices;
  for(size_t i=0; i<ppsreco->Vertices->size(); i++) {
    if(ppsreco->Vertices->at(i).Flag==1) gvertices.push_back(i);
  }

  // match PPS vertex to CMS one
  std::vector<size_t> golden_vertices;
  for(size_t i=0; i<gvertices.size(); i++) {

    size_t gindex = gvertices[i];

    // delta ToF (ns)
    const double deltaTof = ppsreco->ArmB.Tracks.at(ppsreco->Vertices->at(gindex).idxTrkB).ToF.ToF - ppsreco->ArmF.Tracks.at(ppsReco->Vertices->at(gindex).idxTrkF).ToF.ToF;
    deltaTof *= pow(10,-9);// (s)

    // speed of light (m/s)
    const double c = 3.*pow(10,8);

    // pps_z resolution (cm)
    const double ppsz_resolution = ((c/2.)*sqrt(2)*tofRes)*pow(10,2);

    // pps z (cm)
    const double pps_z = ((c/2.)*deltaTof)*pow(10,2);

    const double zppsmax = pps_z + ppsz_resolution;
    const double zppsmin = pps_z - ppsz_resolution;	
	
    if(zppsmin < pv_z && pv_z < zppsmax) golden_vertices.push_back(gindex);
  }

  // at least 1 match
  if(golden_vertices.size()) {

    // passed filter
    result = true;

    std::auto_ptr<std::vector<size_t> > pvertices(new std::vector<size_t>);
    // pvertices->reserve(golden_vertices.size());
    *pvertices = golden_vertices;

    event.put(pvertices,"tracks");
  }


  return result;
}

// define edm plugin
DEFINE_FWK_MODULE(DoubleArmFilter);
