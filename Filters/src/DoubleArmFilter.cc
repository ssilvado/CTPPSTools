#include "CTPPSTools/Filters/interface/DoubleArmFilter.h"

// class members definiton
DoubleArmFilter::DoubleArmFilter(const edm::ParameterSet& cfg):
  verticesToken (consumes<DataTypes::VertexCollection>(cfg.getParameter<edm::InputTag>("vertices")))
, ppsRecoToken(consumes<DataTypes::PPSRecoCollection>(edm::InputTag("ppssim","PPSReco")))
, tofRes(cfg.getParameter<unsigned int>("tofRes"))
{
  produces<std::vector<std::pair<size_t,size_t> > >("protons");
}

bool DoubleArmFilter::filter(edm::Event& event, const edm::EventSetup& setup)
{

  // initialize filter result
  result = false;

  event.getByToken(verticesToken, vertices); 
  event.getByToken(ppsRecoToken, ppsReco);
   
  // check collections
  if( !vertices.isValid()
   || !ppsReco.isValid()
    ) {
    throw cms::Exception("DoubleArmFilter") << "\n +" << __FILE__ << ":" << __LINE__ << ":"
    << "\n filter(): One of the following collections could not be retrieved" 
    << "\n filter(): from root file:"
    << "\n filter(): vertices.isValid()=" << vertices.isValid()
    << "\n filter(): ppsReco.isValid()=" << ppsReco.isValid()
    << "\n filter(): Aborting.\n\n";
  }
   
  if (!vertices->size()) return result;

  if (!ppsReco->Vertices->size()) return result;
   
  const double oPV_0 = vertices->at(0).z();

  // convert to seconds
  tofRes *= pow(10,-12);

  // speed of light (m/s)
  const double c = 3.*pow(10,8);

  // z resolution (cm)
  const double ppsz_resolution = ((c/2.)*sqrt(2)*tofRes)*pow(10,2);


  std::vector<std::pair<size_t,size_t> > protons;

  for (size_t i = 0; i < ppsReco->ArmF.ToFDet.size(); i++) {
    for (size_t j = 0; j < ppsReco->ArmB.ToFDet.size(); j++) {

      // delta ToF (s)
      const double deltaTof = (ppsReco->ArmB.ToFDet.at(j).ToF - ppsReco->ArmF.ToFDet.at(i).ToF)*pow(10,-9);

      // pps vertex z (cm)
      const double ZPPS_V = ((c/2.)*deltaTof)*pow(10,2);

      const double zppsmax = ZPPS_V + ppsz_resolution;
      const double zppsmin = ZPPS_V - ppsz_resolution;	
	
      std::cout << "\n filter(): zppsmax = " << zppsmax;
      std::cout << "\n filter(): zppsmin = " << zppsmin;
      std::cout << "\n filter(): ZPPS_V = " << ZPPS_V;
      
      if(zppsmin < oPV_0 && oPV_0 < zppsmax) protons.push_back(std::make_pair(i,j));
    }
  }

  // at least 1 pps vertex matched to cms
  if(protons.size()) {

    // passed filter
    result = true;

    std::auto_ptr<std::vector<std::pair<size_t,size_t> > > pprotons(new std::vector<std::pair<size_t,size_t> >);
//    pprotons->reserve(protons.size());
    *pprotons = protons;

    event.put(pprotons,"protons");
  }


  return result;
}

// define edm plugin
DEFINE_FWK_MODULE(DoubleArmFilter);
