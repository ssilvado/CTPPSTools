#ifndef Analyzers_DoubleArmAnalyzer_h
#define Analyzers_DoubleArmAnalyzer_h

// basic include files
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


// user include files
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataUtils/Formats/interface/DataTypes.h"
#include <cmath>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include "TTree.h"

// class declaration
class DoubleArmAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
	
  public:

    // constructors and destructors
    explicit DoubleArmAnalyzer(const edm::ParameterSet&);

  private:

    // functions
    virtual void beginJob() override;
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

    edm::Service<TFileService> fs;

    // variables
    edm::Handle<DataTypes::PPSRecoCollection> ppsreco;
	  
    edm::EDGetTokenT<DataTypes::PPSRecoCollection> ppsreco_tk;

    TTree * tree;
    std::map<std::string,std::vector<double>*> vecdoubles;
};

#endif
