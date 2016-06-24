#include "CTPPSTools/Filters/interface/DoubleArmFilter.h"

// class members definiton
DoubleArmFilter::DoubleArmFilter(const edm::ParameterSet& iConfig):
  verticesToken (consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices")))
, ppsRecoToken (consumes<PPSSpectrometer<PPSReco> >(iConfig.getParameter<edm::InputTag>("ppsReco")))
, tof_resolution (iConfig.getParameter<double>("tof_resolution"))
{

}

bool DoubleArmFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  iEvent.getByToken(ppsRecoToken, ppsReco);
  iEvent.getByToken(verticesToken, vertices); 
   
  // check collections
  if( !ppsReco.isValid()
   || !vertices.isValid()
    ) {
    throw cms::Exception("DoubleArmFilter") << "\n +" << __FILE__ << ":" << __LINE__ << ":"
    << "\n filter(): One of the following collections could not be retrieved" 
    << "\n filter(): from root file:"
    << "\n filter(): jets.isValid()=" << jets.isValid()
    << "\n filter(): Aborting.\n\n";
  }
   
  if (  (ppsReco->Vertices->size()) < 1) result = false;
   
  if (vertices->size() < 1){return false;}
   double oPV_0 = vertices->at(0).z();

   std::vector<size_t> protons_armF;
   std::vector<size_t> protons_armB;

   for (size_t i = 0; i < ppsReco->ArmB.ToFDet.size(); i++) {
	   for (size_t j = 0; j < ppsReco->ArmF.ToFDet.size(); j++) {
		double ZPPS_V = (14.9896229)*(ppsReco->ArmB.ToFDet.at(i).ToF - ppsReco->ArmF.ToFDet.at(j).ToF);
		double zppsmax = ZPPS_V + ppsz_resolution;
		double zppsmin = ZPPS_V - ppsz_resolution;	
		if( oPV_0 < zppsmax && oPV_0 > zppsmin) {
			protons_armB.push_back(i);
			protons_armF.push_back(j);
			}	
	   }
   }

   if (protons_armB.size() > 0) return true;

   return true;
}

DEFINE_FWK_MODULE(DoubleArmFilter);
