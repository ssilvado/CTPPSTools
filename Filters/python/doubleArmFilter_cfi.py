import FWCore.ParameterSet.Config as cms

doubleArmFilter = cms.EDFilter( 'DoubleArmFilter'
                               , vertices = cms.InputTag('offlineSlimmedPrimaryVertices')
                               , ppsReco = cms.InputTag('PPSReco')
                               , tofRes = cms.uint32(50)
                              )
