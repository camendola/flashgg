#ifndef FLASHgg_VBFNjetTag_h
#define FLASHgg_VBFNjetTag_h

#include "TLorentzVector.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/VBFDiPhoDiJetMVAResult.h"
#include "flashgg/DataFormats/interface/Jet.h"


#ifdef __MAKECINT__ 
#pragma link C++ class pair<double,int>+; 
#endif

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

        // --- getters ---
        const std::vector<edm::Ptr< flashgg::Jet> > jets_bypt()  const { return Jets_bypt_;}
        const std::vector<edm::Ptr< flashgg::Jet> > jets_bymjj() const { return Jets_bymjj_;}

        // di-jet 
        const TLorentzVector jj(std::vector<edm::Ptr<flashgg::Jet> > Jets) const ;
        const TLorentzVector jj_bypt()  const {return jj(Jets_bypt_);}
        const TLorentzVector jj_bymjj() const {return jj(Jets_bymjj_);}        
        
        // gen quantities
        const TLorentzVector jj(edm::Ptr<reco::GenJet> genJet1, edm::Ptr<reco::GenJet> genJet2) const ;
        const TLorentzVector jj(edm::Ptr<reco::GenParticle> genPart1, edm::Ptr<reco::GenParticle> genPart2) const ;


        const edm::Ptr<reco::GenParticle> genPart1()    const {return genPart1_;} 
        const edm::Ptr<reco::GenParticle> genPart2()    const {return genPart2_;} 
        const bool                        hasGenPart1() const {return genPart1_.isNonnull();} 
        const bool                        hasGenPart2() const {return genPart2_.isNonnull();} 
        const TLorentzVector              jj_genPart()  const {return jj(genPart1_, genPart2_);}

        const edm::Ptr<reco::GenJet>      genJet1()     const {return genJet1_;} 
        const edm::Ptr<reco::GenJet>      genJet2()     const {return genJet2_;} 
        const bool                        hasGenJet1()  const {return genJet1_.isNonnull();} 
        const bool                        hasGenJet2()  const {return genJet2_.isNonnull();} 
        const TLorentzVector              jj_genJet()   const {return jj(genJet1_, genJet2_);}
        
        const bool   match_genPart(std::vector<edm::Ptr<flashgg::Jet>> Jets, 
                                   edm::Ptr<reco::GenParticle> genPart1,  
                                   edm::Ptr<reco::GenParticle> genPart2)  const ;
        const bool   jj_bypt_matchGenPart()  const {return match_genPart(Jets_bypt_, genPart1_, genPart2_);}                     
        const bool   jj_bymjj_matchGenPart() const {return match_genPart(Jets_bypt_, genPart1_, genPart2_);}                     

        // eta weight functions
        // null mass
        // compute for existing jets vector
        const std::vector<double> TB_jetvec(std::vector<edm::Ptr<flashgg::Jet> > Jets) const ;      
        const std::vector<double> TB_bypt()  const {return TB_jetvec(Jets_bypt_);}         
        const std::vector<double> TB_bymjj() const {return TB_jetvec(Jets_bymjj_);}
        
        // get TB ordered by value
        const std::vector<double> TB()       const {return TB_;}         

        // --- setters ---

        void setJets_bypt  (std::vector<edm::Ptr<flashgg::Jet> > Jets) { Jets_bypt_  = Jets; }      
        void setJets_bymjj (std::vector<edm::Ptr<flashgg::Jet> > Jets) { Jets_bymjj_ = Jets; }   
        void setTB         (std::vector<edm::Ptr<flashgg::Jet> > Jets) 
        { 
            std::vector <double> TB = TB_jetvec(Jets); 
            std::sort(TB.begin(),TB.end()); 
            TB_ = TB; 
        }    
        
        void setgenJet1 (edm::Ptr<reco::GenJet> genJet) { genJet1_ = genJet; }
        void setgenJet2 (edm::Ptr<reco::GenJet> genJet) { genJet2_ = genJet; }
        void setgenParticle1 (edm::Ptr<reco::GenParticle> genParticle) { genPart1_ = genParticle; }
        void setgenParticle2 (edm::Ptr<reco::GenParticle> genParticle) { genPart2_ = genParticle; }
        
    private:
        //VBFDiPhoDiJetMVAResult vbfDiPhoDiJet_mva_result_;
        std::vector<edm::Ptr<flashgg::Jet> > Jets_bypt_;
        std::vector<edm::Ptr<flashgg::Jet> > Jets_bymjj_;
        std::vector<double> TB_;
        edm::Ptr<reco::GenJet> genJet1_ ; 
        edm::Ptr<reco::GenJet> genJet2_ ; 
        edm::Ptr<reco::GenParticle> genPart1_ ; 
        edm::Ptr<reco::GenParticle> genPart2_ ; 
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
