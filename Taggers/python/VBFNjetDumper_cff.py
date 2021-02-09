import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.tagsDumpConfig_cff import tagsDumpConfig

VBFNjetTagDumper = cms.EDAnalyzer('CutBasedVBFNjetTagDumper',
                 **tagsDumpConfig.parameters_())
