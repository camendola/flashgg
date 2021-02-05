import FWCore.ParameterSet.Config as cms
from flashgg.MicroAOD.flashggJets_cfi import flashggUnpackedJets
from flashgg.Taggers.flashggDiPhotonMVA_cfi import flashggDiPhotonMVA
from flashgg.Taggers.flashggPreselectedDiPhotons_cfi import flashggPreselectedDiPhotons
from flashgg.Taggers.flashggDifferentialPhoIdInputsCorrection_cfi import flashggDifferentialPhoIdInputsCorrection, setup_flashggDifferentialPhoIdInputsCorrection
from flashgg.Taggers.flashggVBFNjetTag_cff import *

def flashggPrepareVBFNjetSequence(process, options):
    setup_flashggDifferentialPhoIdInputsCorrection(process, options)

    if "flashggDiPhotonMVA" in options:
        flashggDiPhotonMVA.diphotonMVAweightfile = cms.FileInPath(str(options["flashggDiPhotonMVA"]["weightFile"]))
        flashggDiPhotonMVA.Version = cms.string(str(options["flashggDiPhotonMVA"]["version"]))

    flashggVBFNjetSequence = cms.Sequence(flashggDifferentialPhoIdInputsCorrection
                                      * flashggPreselectedDiPhotons
                                      * flashggDiPhotonMVA
                                      * flashggUnpackedJets
                                      * flashggVBFNjetTag
    )

    return flashggVBFNjetSequence
