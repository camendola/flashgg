import FWCore.ParameterSet.Config as cms
from flashgg.MicroAOD.flashggJets_cfi import UnpackedJetCollectionVInputTag


flashggVBFNjetTag = cms.EDProducer("FlashggVBFNjetTagProducer",
                                   DiPhotonTag                 = cms.InputTag('flashggPreselectedDiPhotons'),
                                   VBFDiPhoDiJetMVAResultTag   = cms.InputTag('flashggVBFMVA'),
                                   MVAResultTag                = cms.InputTag('flashggDiPhotonMVA'),
                                   inputTagJets                = UnpackedJetCollectionVInputTag,
                                   GenParticleTag              = cms.InputTag( "flashggPrunedGenParticles" ),
                                   GenJetTag                   = cms.InputTag("slimmedGenJets"),
                                   debug                       = cms.bool(False)
                               )
