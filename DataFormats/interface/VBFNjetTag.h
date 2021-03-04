#ifndef FLASHgg_VBFNjetTag_h
#define FLASHgg_VBFNjetTag_h

#include "TLorentzVector.h"
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

        // --- getters ---
        const std::vector<edm::Ptr< flashgg::Jet> > jets_bypt()  const { return Jets_bypt_;}
        const std::vector<edm::Ptr< flashgg::Jet> > jets_bymjj() const { return Jets_bymjj_;}
        const std::vector<edm::Ptr< reco::GenJet> > genjets()    const {return genJets_; }
        const bool isSameOrdering() const {return isSameOrdering_;}
        // di-jet 
        const TLorentzVector jj(std::vector<edm::Ptr<flashgg::Jet> > Jets) const ;
        const TLorentzVector jj_bypt()  const {return jj(Jets_bypt_);}
        const TLorentzVector jj_bymjj() const {return jj(Jets_bymjj_);}        
        
        // gen quantities
        const TLorentzVector jj(const reco::GenJet * genJet1, const reco::GenJet * genJet2) const ;
        const TLorentzVector jj(edm::Ptr<reco::GenParticle> genPart1, edm::Ptr<reco::GenParticle> genPart2) const ;


        const edm::Ptr<reco::GenParticle> genPart1()    const {return genPart1_;} 
        const edm::Ptr<reco::GenParticle> genPart2()    const {return genPart2_;} 

        //const edm::Ptr<reco::GenParticle> genPart1HS()    const {return genPart1HS_;} 
        //const edm::Ptr<reco::GenParticle> genPart2HS()    const {return genPart2HS_;} 
        //const edm::Ptr<reco::GenParticle> genPart1ISR()    const {return genPart1ISR_;} 
        //const edm::Ptr<reco::GenParticle> genPart2ISR()    const {return genPart2ISR_;} 

        const bool                        hasGenPart1() const {return genPart1_.isNonnull();} 
        const bool                        hasGenPart2() const {return genPart2_.isNonnull();} 
        const TLorentzVector              jj_genPart()  const {return jj(genPart1_, genPart2_);}

        //const edm::Ptr<reco::GenJet>      getGenJet(int genpartidx, std::vector<edm::Ptr<reco::GenJet> > genJets)     const ; 
        const reco::GenJet *  getGenJet(edm::Ptr<flashgg::Jet> jet, std::vector<edm::Ptr<reco::GenJet> > genJets)      const ; 

        const TLorentzVector              jj_genJet_recomatch_bypt()   const {
            return jj(getGenJet(Jets_bypt_.at(0), genJets_), getGenJet(Jets_bypt_.at(1), genJets_));
        }
        const TLorentzVector              jj_genJet_recomatch_bymjj()   const {
            return jj(getGenJet(Jets_bymjj_.at(0), genJets_), getGenJet(Jets_bymjj_.at(1), genJets_));
        }
        //const TLorentzVector              jj_genJet_partmatch()   const {
        //    return jj(getGenJet(genPart1idx_, genJets_), getGenJet(genPart2idx_, genJets_));
        //}
        

        const bool   match_genPart(std::vector<edm::Ptr<flashgg::Jet>> Jets, 
                                   edm::Ptr<reco::GenParticle> genPart1,  
                                   edm::Ptr<reco::GenParticle> genPart2)  const ;

        const bool   jj_bypt_matchGenPart()  const {return match_genPart(Jets_bypt_, genPart1_, genPart2_);}                     
        const bool   jj_bymjj_matchGenPart() const {return match_genPart(Jets_bymjj_, genPart1_, genPart2_);}                     

        // eta weight functions
        // null mass
        // compute for existing jets vector
        const std::vector<double> TB_jetvec(std::vector<edm::Ptr<flashgg::Jet> > Jets) const ;      
        const std::vector<double> TB_jetvec(std::vector<edm::Ptr<reco::GenJet> > Jets) const ;      
        const std::vector<double> TB_bypt()  const {return TB_jetvec(Jets_bypt_);}         
        const std::vector<double> TB_bymjj() const {return TB_jetvec(Jets_bymjj_);}

        const std::vector<double> TC_jetvec(std::vector<edm::Ptr<flashgg::Jet> > Jets) const ;      
        const std::vector<double> TC_jetvec(std::vector<edm::Ptr<reco::GenJet> > Jets) const ;   
        const std::vector<double> TC_bypt()  const {return TC_jetvec(Jets_bypt_);}         
        const std::vector<double> TC_bymjj() const {return TC_jetvec(Jets_bymjj_);}        

        // get Ti ordered by value
        const std::vector<double> TB()       const {return TB_;}         
        const std::vector<double> TC()       const {return TC_;}         
        const std::vector<double> TB_genJets()       const {return TB_genJets_;}         
        const std::vector<double> TC_genJets()       const {return TC_genJets_;}         

        // --- setters ---

        void setJets_bypt      (std::vector<edm::Ptr<flashgg::Jet> > Jets) { Jets_bypt_  = Jets; }   
        void setJets_bymjj     (std::vector<edm::Ptr<flashgg::Jet> > Jets) { Jets_bymjj_ = Jets; }   
        void setIsSameOrdering (bool isSameOrdering) { isSameOrdering_ = isSameOrdering; }
        void setTB         (std::vector<edm::Ptr<flashgg::Jet> > Jets) 
        { 
            std::vector <double> TB = TB_jetvec(Jets); 
            std::sort(TB.begin(),TB.end()); 
            TB_ = TB; 
        }    
        void setTC         (std::vector<edm::Ptr<flashgg::Jet> > Jets) 
        { 
            std::vector <double> TC = TC_jetvec(Jets); 
            std::sort(TC.begin(),TC.end()); 
            TC_ = TC; 
        }    
        void setTB         (std::vector<edm::Ptr<reco::GenJet> > Jets) 
        { 
            std::vector <double> TB = TB_jetvec(Jets); 
            std::sort(TB.begin(),TB.end()); 
            TB_genJets_ = TB; 
        }    
        void setTC         (std::vector<edm::Ptr<reco::GenJet> > Jets) 
        { 
            std::vector <double> TC = TC_jetvec(Jets); 
            std::sort(TC.begin(),TC.end()); 
            TC_genJets_ = TC; 
        }    
        
        void setGenJets (std::vector<edm::Ptr<reco::GenJet>> genJets) { genJets_ = genJets; }
        void setgenParticle1idx (int idx) { genPart1idx_ = idx; }
        void setgenParticle2idx (int idx) { genPart2idx_ = idx; }

        void setgenParticle1 (edm::Ptr<reco::GenParticle> genParticle) { genPart1_ = genParticle; }
        void setgenParticle2 (edm::Ptr<reco::GenParticle> genParticle) { genPart2_ = genParticle; }
        //void setgenParticle1HS (edm::Ptr<reco::GenParticle> genParticle) { genPart1HS_ = genParticle; }
        //void setgenParticle2HS (edm::Ptr<reco::GenParticle> genParticle) { genPart2HS_ = genParticle; }
        //void setgenParticle1ISR (edm::Ptr<reco::GenParticle> genParticle) { genPart1ISR_ = genParticle; }
        //void setgenParticle2ISR (edm::Ptr<reco::GenParticle> genParticle) { genPart2ISR_ = genParticle; }

    private:
        //VBFDiPhoDiJetMVAResult vbfDiPhoDiJet_mva_result_;
        std::vector<edm::Ptr<flashgg::Jet> > Jets_bypt_;
        std::vector<edm::Ptr<flashgg::Jet> > Jets_bymjj_;
        std::vector<edm::Ptr<reco::GenJet> > genJets_;
        bool isSameOrdering_ ; 
        std::vector<double> TB_;
        std::vector<double> TC_;
        std::vector<double> TB_genJets_;
        std::vector<double> TC_genJets_;
        edm::Ptr<reco::GenParticle> genPart1_ ; 
        edm::Ptr<reco::GenParticle> genPart2_ ; 
        int genPart1idx_ ; 
        int genPart2idx_ ; 
        //edm::Ptr<reco::GenParticle> genPart1HS_ ; 
        //edm::Ptr<reco::GenParticle> genPart2HS_ ; 
        //edm::Ptr<reco::GenParticle> genPart1ISR_ ; 
        //edm::Ptr<reco::GenParticle> genPart2ISR_ ; 
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
