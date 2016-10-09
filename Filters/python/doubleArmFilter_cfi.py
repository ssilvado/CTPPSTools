import FWCore.ParameterSet.Config as cms

doubleArmFilter = cms.EDFilter( 'DoubleArmFilter'
                               , vertices = cms.InputTag('offlineSlimmedPrimaryVertices')
                               , tofRes = cms.double(20) # picoseconds
                              )

#MessageLogger = cms.Service("MessageLogger")
