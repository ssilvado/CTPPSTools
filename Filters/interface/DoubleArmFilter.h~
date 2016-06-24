#ifndef Filters_DoubleArmFilter_h
#define Filters_DoubleArmFilter_h

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include "FWCore/Utilities/interface/StreamID.h"

// CT-PPS
#include "FWCore/Utilities/interface/Exception.h"
#include "FastSimulation/PPSFastObjects/interface/PPSSpectrometer.h"
//#include "FastSimulation/PPSFastObjects/interface/PPSGenData.h"
//#include "FastSimulation/PPSFastObjects/interface/PPSSimData.h"
#include "FastSimulation/PPSFastObjects/interface/PPSRecoData.h"
//#include "FastSimulation/PPSFastObjects/interface/PPSGenVertex.h"
//#include "FastSimulation/PPSFastObjects/interface/PPSRecoVertex.h"

// CMS - Central
//#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include <vector>

// class declaration
class DoubleArmFilter : public edm::stream::EDFilter<> {
	
  public:

    // constructors and destructors
    explicit DoubleArmFilter(const edm::ParameterSet&);

  private:

    // typedefs
    typedef PPSGenData  PPSGen; 
    typedef PPSSimData  PPSSim; 
    typedef PPSRecoData PPSReco;

    // functions
    virtual bool filter(edm::Event&, const edm::EventSetup&) override;
    
    // variables
    edm::Handle<reco::VertexCollection> vertices;
//    edm::Handle<PPSSpectrometer<PPSGen> > ppsGen;
//    edm::Handle<PPSSpectrometer<PPSSim> > ppsSim;
    edm::Handle<PPSSpectrometer<PPSReco> > ppsReco;
	  
    edm::EDGetTokenT<reco::VertexCollection> verticesToken;
    edm::EDGetTokenT<PPSSpectrometer<PPSReco> > ppsRecoToken;

    double tof_resolution;
};

#endif
