import FWCore.ParameterSet.Config as cms

doubleArmFilter = cms.EDFilter( 'DoubleArmFilter'
                               , vertices = cms.InputTag('offlineSlimmedPrimaryVertices')
                               , tofRes = cms.uint32(50) # picoseconds
                              )
