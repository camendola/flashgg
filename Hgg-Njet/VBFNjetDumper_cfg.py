import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
import os
from flashgg.MetaData.MetaConditionsReader import *

# ========================================================================
# Standard configuration setting
process = cms.Process("FLASHggSyst")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.maxEvents   = cms.untracked.PSet( input  = cms.untracked.int32( 10 ) )
process.options     = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1 )

process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(
'file:/eos/cms/store/group/phys_higgs/cmshgg/alesauva/flashgg/UL2017/10_6_4/VBFHToGG_M125_TuneCP5_13TeV-amcatnlo-pythia8/UL2017-10_6_4-v0-RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6-v2/200514_145023/0000/myMicroAODOutputFile_8.root'
                             )
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test.root"))


from flashgg.MetaData.JobConfig import customize

customize.options.register('ignoreNegR9',
                           True,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'ignoreNegR9'
                           )

# set default options if needed
customize.setDefault("maxEvents",300)
customize.setDefault("targetLumi",1.00e+3)
customize.parse()
customize.metaConditions = MetaConditionsReader(customize.metaConditions)

### Global Tag
from Configuration.AlCa.GlobalTag import GlobalTag
if customize.processId == "Data": process.GlobalTag.globaltag = str(customize.metaConditions['globalTags']['data'])
else: process.GlobalTag.globaltag = str(customize.metaConditions['globalTags']['MC'])


#========================================================================================
### Set VBFNjet Producer
process.load('flashgg.Taggers.flashggVBFNjetSequence_cfi')

from flashgg.Taggers.flashggVBFNjetSequence_cfi import *
process.flashggVBFNjetSequence = flashggPrepareVBFNjetSequence( process, customize.metaConditions )
#process.flashggVBFNjetTag.leadPhoOverMassThreshold = cms.double(0.25)
#process.flashggVBFNjetTag.METThreshold             = cms.double(45)

### Systematics customize
from flashgg.Systematics.SystematicsCustomize import *
#jetSystematicsInputTags = createStandardSystematicsProducers(process,customize)
#modifyTagSequenceForSystematics(process,jetSystematicsInputTags,2)

process.load('flashgg.Systematics.flashggDiPhotonSystematics_cfi')
import flashgg.Systematics.flashggDiPhotonSystematics_cfi as diPhotons_syst
diPhotons_syst.setupDiPhotonSystematics( process, customize )
process.flashggPreselectedDiPhotons.src = cms.InputTag('flashggDiPhotonSystematics')

#Using standard tools
useEGMTools(process)

#==============================================================================================
### VBFNjet Dumper setting
from   flashgg.Taggers.tagsDumpers_cfi   import createTagDumper
process.load('flashgg.Taggers.VBFNjetDumper_cff')
process.VBFNjetTagDumper.className = "CutBasedVBFNjetDumper"
process.VBFNjetTagDumper.dumpTrees     = True
process.VBFNjetTagDumper.dumpHistos    = False
process.VBFNjetTagDumper.dumpWorkspace = False
process.VBFNjetTagDumper.src = "flashggVBFNjetTag"
process.VBFNjetTagDumper.nameTemplate = cms.untracked.string("$PROCESS_$SQRTS_$CLASSNAME_$SUBCAT_$LABEL")

# ===============================================================================================

# Set variables in N-tuples
vbf_variables = [
    "dipho_mass             := diPhoton.mass",
    "dipho_leadPt           := diPhoton.leadingPhoton.pt",
    "dipho_leadEta          := diPhoton.leadingPhoton.eta",
    "dipho_leadPhi          := diPhoton.leadingPhoton.phi",
    "dipho_leadE            := diPhoton.leadingPhoton.energy",
    "dipho_leadIDMVA        := diPhoton.leadingView.phoIdMvaWrtChosenVtx",
    "dipho_subleadPt        := diPhoton.subLeadingPhoton.pt",
    "dipho_subleadEta       := diPhoton.subLeadingPhoton.eta",
    "dipho_subleadPhi       := diPhoton.subLeadingPhoton.phi",
    "dipho_subleadE         := diPhoton.subLeadingPhoton.energy",
    "dipho_subleadIDMVA     := diPhoton.subLeadingView.phoIdMvaWrtChosenVtx",

    "n_jet                  := jets_bypt.size",

    "jet1_Pt                := ?(jets_bypt.size>0)? jets_bypt.at(0).pt               : -999",
    "jet1_Eta               := ?(jets_bypt.size>0)? jets_bypt.at(0).eta              : -999",
    "jet1_Phi               := ?(jets_bypt.size>0)? jets_bypt.at(0).phi              : -999",
    "jet1_E                 := ?(jets_bypt.size>0)? jets_bypt.at(0).energy           : -999",
    "jet2_Pt                := ?(jets_bypt.size>1)? jets_bypt.at(1).pt               : -999",
    "jet2_Eta               := ?(jets_bypt.size>1)? jets_bypt.at(1).eta              : -999",
    "jet2_Phi               := ?(jets_bypt.size>1)? jets_bypt.at(1).phi              : -999",
    "jet2_E                 := ?(jets_bypt.size>1)? jets_bypt.at(1).energy           : -999",
    "jet3_Pt                := ?(jets_bypt.size>2)? jets_bypt.at(2).pt               : -999",
    "jet3_Eta               := ?(jets_bypt.size>2)? jets_bypt.at(2).eta              : -999",
    "jet3_Phi               := ?(jets_bypt.size>2)? jets_bypt.at(2).phi              : -999",
    "jet3_E                 := ?(jets_bypt.size>2)? jets_bypt.at(2).energy           : -999",

    "jet1_bymjj_Pt          := ?(jets_bymjj.size>0)? jets_bymjj.at(0).pt             : -999",
    "jet1_bymjj_Eta         := ?(jets_bymjj.size>0)? jets_bymjj.at(0).eta            : -999",
    "jet1_bymjj_Phi         := ?(jets_bymjj.size>0)? jets_bymjj.at(0).phi            : -999",
    "jet1_bymjj_E           := ?(jets_bymjj.size>0)? jets_bymjj.at(0).energy         : -999",
    "jet2_bymjj_Pt          := ?(jets_bymjj.size>1)? jets_bymjj.at(1).pt             : -999",
    "jet2_bymjj_Eta         := ?(jets_bymjj.size>1)? jets_bymjj.at(1).eta            : -999",
    "jet2_bymjj_Phi         := ?(jets_bymjj.size>1)? jets_bymjj.at(1).phi            : -999",
    "jet2_bymjj_E           := ?(jets_bymjj.size>1)? jets_bymjj.at(1).energy         : -999",
    "jet3_bymjj_Pt          := ?(jets_bymjj.size>2)? jets_bymjj.at(2).pt             : -999",
    "jet3_bymjj_Eta         := ?(jets_bymjj.size>2)? jets_bymjj.at(2).eta            : -999",
    "jet3_bymjj_Phi         := ?(jets_bymjj.size>2)? jets_bymjj.at(2).phi            : -999",
    "jet3_bymjj_E           := ?(jets_bymjj.size>2)? jets_bymjj.at(2).energy         : -999",

]

#wh_variables += [
#    "n_jets      := jets.size"
#]
#
#njet = 3
#for i in range(njet):
#    wh_variables.append( "jet%d_Pt  := ?(jets.size>%d)? jets.at(%d).pt      : -999" % (i+1, i, i))
#    wh_variables.append( "jet%d_Eta := ?(jets.size>%d)? jets.at(%d).eta     : -999" % (i+1, i, i))
#    wh_variables.append( "jet%d_Phi := ?(jets.size>%d)? jets.at(%d).phi     : -999" % (i+1, i, i))
#    wh_variables.append( "jet%d_E   := ?(jets.size>%d)? jets.at(%d).energy  : -999" % (i+1, i, i))
#    wh_variables.append( "jet%d_deepbtag   := ?(jets.size>%d)? jets.at(%d).bDiscriminator('pfDeepCSVJetTags:probb') + jets.at(%d).bDiscriminator('pfDeepCSVJetTags:probbb') : -999" % (i+1, i, i, i) )

# Set category
cats = [
    ("VBFNjetTag","1",0)
]

import flashgg.Taggers.dumperConfigTools as cfgTools

cfgTools.addCategories(process.VBFNjetTagDumper,
                       cats,
                       variables  = vbf_variables,
                       histograms = []
)

### Final path
process.p = cms.Path(  process.flashggDifferentialPhoIdInputsCorrection
                     * process.flashggDiPhotonSystematics
                     * process.flashggUnpackedJets
                     * process.flashggVBFNjetSequence
                     * process.VBFNjetTagDumper
                     )


# call the customization
customize(process)
