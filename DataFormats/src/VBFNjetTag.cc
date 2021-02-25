#include "flashgg/DataFormats/interface/VBFNjetTag.h"
#include <tuple>
#include <utility> 
#include <algorithm>
#include <math.h> 

using namespace flashgg;

VBFNjetTag::VBFNjetTag() : DiPhotonTagBase::DiPhotonTagBase() {}

VBFNjetTag::~VBFNjetTag() {}

//VBFNjetTag::VBFNjetTag(edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes, edm::Ptr<VBFDiPhoDiJetMVAResult> vbfDiPhoDiJet_mvaRes) :
//    VBFNjetTag::VBFNjetTag(diPho, *mvaRes, *vbfDiPhoDiJet_mvaRes) {}
//
//VBFNjetTag::VBFNjetTag(edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares, VBFDiPhoDiJetMVAResult vbfDiPhoDiJet_mvaRes) :
//    DiPhotonTagBase::DiPhotonTagBase(dipho, mvares) {}

VBFNjetTag::VBFNjetTag(edm::Ptr<flashgg::DiPhotonCandidate> diPho, 
                       edm::Ptr<DiPhotonMVAResult> mvaRes) :
    VBFNjetTag::VBFNjetTag(diPho, *mvaRes) {}

VBFNjetTag::VBFNjetTag(edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares) :
    DiPhotonTagBase::DiPhotonTagBase(dipho, mvares) {}

const TLorentzVector VBFNjetTag::jj(std::vector<edm::Ptr<flashgg::Jet> > Jets) const
{
    TLorentzVector tlv_jet1, tlv_jet2;
    tlv_jet1.SetPxPyPzE(Jets.at(0)->px(), 
                        Jets.at(0)->py(), 
                        Jets.at(0)->pz(), 
                        Jets.at(0)->energy());
    tlv_jet2.SetPxPyPzE(Jets.at(1)->px(), 
                        Jets.at(1)->py(), 
                        Jets.at(1)->pz(), 
                        Jets.at(1)->energy());
    TLorentzVector tlv_jj = tlv_jet1 + tlv_jet2;
    return tlv_jj;
}

const TLorentzVector VBFNjetTag::jj(edm::Ptr<reco::GenJet>  genJet1, 
                                    edm::Ptr<reco::GenJet>  genJet2) const
{
    TLorentzVector tlv_jet1, tlv_jet2;
    tlv_jet1.SetPxPyPzE(genJet1->px(), 
                        genJet1->py(), 
                        genJet1->pz(), 
                        genJet1->energy());
    tlv_jet2.SetPxPyPzE(genJet2->px(), 
                        genJet2->py(), 
                        genJet2->pz(), 
                        genJet2->energy());
    TLorentzVector tlv_jj = tlv_jet1 + tlv_jet2;
    return tlv_jj;
}

const TLorentzVector VBFNjetTag::jj(edm::Ptr<reco::GenParticle>  genPart1, 
                                    edm::Ptr<reco::GenParticle>  genPart2) const
{
    TLorentzVector tlv_jet1, tlv_jet2;
    tlv_jet1.SetPxPyPzE(genPart1->px(), 
                        genPart1->py(), 
                        genPart1->pz(), 
                        genPart1->energy());
    tlv_jet2.SetPxPyPzE(genPart2->px(), 
                        genPart2->py(), 
                        genPart2->pz(), 
                        genPart2->energy());
    TLorentzVector tlv_jj = tlv_jet1 + tlv_jet2;
    return tlv_jj;
}

const vector<double> VBFNjetTag::TB_jetvec(std::vector<edm::Ptr<flashgg::Jet> > Jets) const
{
    std::vector<double > v_TB_j;
    for (unsigned int i = 0; i < Jets.size(); i++)
        {
        double TB_j = Jets.at(i)->pt()*exp(-fabs(Jets.at(i)->eta()));
        v_TB_j.push_back(TB_j);
        }
    return v_TB_j;
}

const bool VBFNjetTag::match_genPart(std::vector<edm::Ptr<flashgg::Jet>> Jets, 
                                     edm::Ptr<reco::GenParticle> genPart1,  
                                     edm::Ptr<reco::GenParticle> genPart2)  const 
{
    bool match = false;
    TLorentzVector tlv_jet1, tlv_jet2;
    if (Jets.size() > 0)
        {
            tlv_jet1.SetPxPyPzE(Jets.at(0)->px(), 
                                Jets.at(0)->py(), 
                                Jets.at(0)->pz(), 
                                Jets.at(0)->energy());
        }
    
    if (Jets.size() > 1)
        {
            tlv_jet2.SetPxPyPzE(Jets.at(1)->px(), 
                                Jets.at(1)->py(), 
                                Jets.at(1)->pz(), 
                                Jets.at(1)->energy());
        }    
    TLorentzVector tlv_part1, tlv_part2;
    if (genPart1.isNonnull())
        {
            tlv_part1.SetPxPyPzE(genPart1->px(), 
                                 genPart1->py(), 
                                 genPart1->pz(), 
                                 genPart1->energy());
        }
    if (genPart2.isNonnull())
        { 
            tlv_part2.SetPxPyPzE(genPart2->px(), 
                                 genPart2->py(), 
                                 genPart2->pz(), 
                                 genPart2->energy());
        }
    if (genPart1.isNonnull() && genPart2.isNonnull() && Jets.size() > 1)
        {            
            if ((tlv_jet1.DeltaR(tlv_part1) < 0.5 && tlv_jet2.DeltaR(tlv_part2) < 0.5) 
                || (tlv_jet1.DeltaR(tlv_part2) < 0.5 && tlv_jet2.DeltaR(tlv_part1) < 0.5)) 
                match = true;
        }
    return match;
}

//VBFNjetTag::VBFNjetTag(edm::Ptr<flashgg::DiPhotonCandidate> diPho) :
//    VBFNjetTag::VBFNjetTag(diPho) {}
//
//VBFNjetTag::VBFNjetTag(edm::Ptr<DiPhotonCandidate> dipho) :
//    DiPhotonTagBase::DiPhotonTagBase(dipho) {}



// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
