import FWCore.ParameterSet.Config as cms
from flashgg.MicroAOD.flashggJets_cfi import UnpackedJetCollectionVInputTag


flashggVBFNjetTag = cms.EDProducer("FlashggVBFNjetTagProducer",
                                   DiPhotonTag                 = cms.InputTag('flashggPreselectedDiPhotons'),
                                   inputTagJets                = UnpackedJetCollectionVInputTag
                               )
