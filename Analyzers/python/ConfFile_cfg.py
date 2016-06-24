import FWCore.ParameterSet.Config as cms

ppsreco          = 'ppssim:PPSReco'
vertices         = 'offlinePrimaryVertices' # offlineSlimmedPrimaryVertices if miniAOD
ppsz_resolution  = cms.double(0.42)

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.options=cms.untracked.PSet(wantSummary=cms.untracked.bool(True))
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:myfile.root'
    )
)

process.filter1 = cms.EDFilter( 'first_step'
        , ppsReco   		 = cms.InputTag(ppsreco)
        , vertices 		 = cms.InputTag(vertices)
	, ppsz_resolution	 = ppsz_resolution
)

process.demo = cms.EDAnalyzer('basic_analyzer'
)


process.p = cms.Path(process.filter1*process.demo)
