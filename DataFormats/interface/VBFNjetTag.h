#ifndef FLASHgg_VBFNjetTag_h
#define FLASHgg_VBFNjetTag_h

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/VBFDiPhoDiJetMVAResult.h"
#include "flashgg/DataFormats/interface/Jet.h"

namespace flashgg {

  class VBFNjetTag: public DiPhotonTagBase
  {
  public:
    VBFNjetTag();
    VBFNjetTag(edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult>, edm::Ptr<VBFDiPhoDiJetMVAResult>);
    VBFNjetTag(edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult, VBFDiPhoDiJetMVAResult );
    ~VBFNjetTag();
    
    VBFNjetTag *clone() const override { return ( new VBFNjetTag( *this ) ); }
    
    const VBFDiPhoDiJetMVAResult VBFDiPhoDiJetMVA() const;
    const VBFMVAResult VBFMVA() const ;
    const std::vector<edm::Ptr<Jet> > jets() const { return Jets_;}
    
    void setJets( std::vector<edm::Ptr<Jet> > Jets ) { Jets_ = Jets; }
    
  private:
    VBFDiPhoDiJetMVAResult vbfDiPhoDiJet_mva_result_;
    std::vector<edm::Ptr<Jet> > Jets_;
  };
}

#endif 
