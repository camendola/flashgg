#include "flashgg/DataFormats/interface/VBFNjetTag.h"
#include <algorithm>

using namespace flashgg;

VBFNjetTag::VBFNjetTag() : DiPhotonTagBase::DiPhotonTagBase() {}

VBFNjetTag::~VBFNjetTag() {}

VBFNjetTag::VBFNjetTag(edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes, edm::Ptr<VBFDiPhoDiJetMVAResult> vbfDiPhoDiJet_mvaRes) :
    VBFNjetTag::VBFNjetTag(diPho, *mvaRes, *vbfDiPhoDiJet_mvaRes) {}

VBFNjetTag::VBFNjetTag(edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares, VBFDiPhoDiJetMVAResult vbfDiPhoDiJet_mvaRes) :
    DiPhotonTagBase::DiPhotonTagBase(dipho, mvares) {}

//VBFNjetTag::VBFNjetTag( edm::Ptr<DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvares ) : DiPhotonTagBase::DiPhotonTagBase( diPho, *mvares ) {}
//VBFNjetTag::VBFNjetTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares ) : DiPhotonTagBase::DiPhotonTagBase( dipho, mvares ) {}


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
