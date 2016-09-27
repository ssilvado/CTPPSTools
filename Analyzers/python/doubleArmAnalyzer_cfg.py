import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.MessageLogger = cms.Service("MessageLogger"
                                   , destinations = cms.untracked.vstring('cout')
                                   , cout = cms.untracked.PSet(
                                           threshold = cms.untracked.string('WARNING') 
                                            )
)

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))

process.source = cms.Source( "PoolSource"
                            , fileNames = cms.untracked.vstring('file:file.root')
                            , duplicateCheckMode = cms.untracked.string("noDuplicateCheck")
                           )

process.analyzer = cms.EDAnalyzer( 'DoubleArmAnalyzer'
                                  , vertices = cms.InputTag('offlineSlimmedPrimaryVertices')
                                  , tofRes = cms.double(20) # picoseconds
                                 )

process.p = cms.Path(process.analyzer)

process.TFileService = cms.Service("TFileService", fileName = cms.string('out.root') )
