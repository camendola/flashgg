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
        //VBFNjetTag(edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult>, edm::Ptr<VBFDiPhoDiJetMVAResult> );
        //VBFNjetTag(edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult, VBFDiPhoDiJetMVAResult );
        VBFNjetTag(edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult>);
        VBFNjetTag(edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult);
        ~VBFNjetTag();
        
        VBFNjetTag *clone() const override { return ( new VBFNjetTag( *this ) ); }
        
        //const VBFDiPhoDiJetMVAResult VBFDiPhoDiJetMVA() const;
        //const VBFMVAResult VBFMVA() const ;
        const std::vector<edm::Ptr< flashgg::Jet> > jets_bypt()  const { return Jets_bypt_;}
        const std::vector<edm::Ptr< flashgg::Jet> > jets_bymjj() const { return Jets_bymjj_;}
        
        void setJets_bypt (std::vector<edm::Ptr<flashgg::Jet> > Jets) { Jets_bypt_  = Jets;}                
        void setJets_bymjj(std::vector<edm::Ptr<flashgg::Jet> > Jets) { Jets_bymjj_ = Jets;}                
        
        
    private:
        //VBFDiPhoDiJetMVAResult vbfDiPhoDiJet_mva_result_;
        std::vector<edm::Ptr<flashgg::Jet> > Jets_bypt_;
        std::vector<edm::Ptr<flashgg::Jet> > Jets_bymjj_;
    };
}

#endif 

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
