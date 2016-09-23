#ifndef Filters_DoubleArmFilter_h
#define Filters_DoubleArmFilter_h

// basic include files
#include "FWCore/Framework/interface/stream/EDFilter.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// user include files
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "DataUtils/Formats/interface/DataTypes.h"
#include <cmath>
#include <utility>
#include <vector>

// class declaration
class DoubleArmFilter : public edm::stream::EDFilter<> {
	
  public:

    // constructors and destructors
    explicit DoubleArmFilter(const edm::ParameterSet&);

  private:

    // functions
    virtual bool filter(edm::Event&, const edm::EventSetup&) override;
    
    // variables
    edm::Handle<DataTypes::VertexCollection> vertices;
    edm::Handle<DataTypes::PPSRecoCollection> ppsreco;
	  
    edm::EDGetTokenT<DataTypes::VertexCollection> vertices_tk;
    edm::EDGetTokenT<DataTypes::PPSRecoCollection> ppsreco_tk;

    unsigned int tofRes;
    bool result;
};

#endif
