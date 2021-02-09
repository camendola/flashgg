#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/VBFNjetTag.h"
#include "flashgg/DataFormats/interface/VBFTagTruth.h"
#include "flashgg/DataFormats/interface/PDFWeightObject.h"

#include <TLorentzVector.h>
#include <vector>
#include <algorithm>
#include <typeinfo>

using namespace std;
using namespace edm;

template <typename t> void move(std::vector<t>& v, size_t prev_idx, size_t new_idx)
{
    if (prev_idx > new_idx)
        std::rotate(v.rend() - prev_idx - 1, v.rend() - prev_idx, v.rend() - new_idx);
    else        
        std::rotate(v.begin() + prev_idx, v.begin() + prev_idx + 1, v.begin() + new_idx + 1);
}


namespace flashgg {

    class VBFNjetTagProducer : public EDProducer 
    {

    public:
        VBFNjetTagProducer( const ParameterSet & );
        
    private:
        void produce( Event &, const EventSetup & ) override;
        
        EDGetTokenT<View<DiPhotonCandidate> >                       diPhotonToken_;
        //EDGetTokenT<View<VBFDiPhoDiJetMVAResult> >                  vbfDiPhoDiJetMvaResultToken_;
        EDGetTokenT<View<DiPhotonMVAResult> >                       mvaResultToken_;
        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > >         tokenJets_;
        std::vector<edm::InputTag>                                  inputTagJets_;
        bool                                                        debug_;
        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;
    };
    
    VBFNjetTagProducer::VBFNjetTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_ (consumes< View< flashgg::DiPhotonCandidate > >  (iConfig.getParameter < InputTag >  ( "DiPhotonTag" ))),
        //vbfDiPhoDiJetMvaResultToken_( consumes < View < flashgg::VBFDiPhoDiJetMVAResult > >( iConfig.getParameter < InputTag > ( "VBFDiPhoDiJetMVAResultTag" ))),
        mvaResultToken_(consumes< View < flashgg::DiPhotonMVAResult > > (iConfig.getParameter < InputTag > ( "MVAResultTag" ))),   
        inputTagJets_  (iConfig.getParameter < std::vector < edm::InputTag > >( "inputTagJets" )), 
        debug_         (iConfig.getParameter <bool>( "debug" ))
    {    
        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }
        produces<vector< VBFNjetTag > >(); 
    }
    
    
    void VBFNjetTagProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        
        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );
        
        //Handle<View<flashgg::VBFDiPhoDiJetMVAResult> > vbfDiPhoDiJetMvaResults;
        //evt.getByToken( vbfDiPhoDiJetMvaResultToken_, vbfDiPhoDiJetMvaResults );
        
        JetCollectionVector Jets(inputTagJets_.size());
        for(size_t j = 0; j < inputTagJets_.size(); ++j) {
            evt.getByToken(tokenJets_[j], Jets[j]);
        }
        
        std::unique_ptr<vector<VBFNjetTag> > vbfnjet_tags(new vector<VBFNjetTag>);
        
        for(unsigned int idipho = 0; idipho < diPhotons->size(); idipho++) {
           
            //edm::Ptr<flashgg::VBFDiPhoDiJetMVAResult> vbfdipho_mvares = vbfDiPhoDiJetMvaResults->ptrAt(idipho);
            edm::Ptr<flashgg::DiPhotonMVAResult>      mvares          = mvaResults->ptrAt(idipho); 
            edm::Ptr<flashgg::DiPhotonCandidate>      dipho           = diPhotons->ptrAt(idipho);
           
            //VBFNjetTag vbfnjet_tags_obj(dipho, mvares, vbfdipho_mvares);
            VBFNjetTag vbfnjet_tags_obj(dipho, mvares);
            vbfnjet_tags_obj.includeWeights( *dipho );
           
            //highest pt jets
            std::vector<edm::Ptr<Jet> >            tagJets_bypt;
            
            unsigned int jetCollectionIndex = diPhotons->ptrAt(idipho)->jetCollectionIndex();            
            for( unsigned int ijet = 0; ijet < Jets[jetCollectionIndex]->size(); ijet++) {
                edm::Ptr<flashgg::Jet> jet = Jets[jetCollectionIndex]->ptrAt(ijet);
                if(!jet->passesJetID (flashgg::Tight2017)) continue;
                float dRPhoLeadJet = deltaR(jet->eta(), jet->phi(), 
                                            dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi()) ;
                float dRPhoSubLeadJet = deltaR(jet->eta(), jet->phi(), 
                                               dipho->subLeadingPhoton()->superCluster()->eta(), dipho->subLeadingPhoton()->superCluster()->phi());
                
                if(dRPhoLeadJet < 0.4 || dRPhoSubLeadJet < 0.4) continue;
                tagJets_bypt.push_back(jet);
            }   

            //highest mjj jets
            std::vector<edm::Ptr<Jet> >            tagJets_bymjj;                 // first 2 jets: highest mjj; following: all the other jets by pt
            std::vector< tuple<float, unsigned int, unsigned int> >  pair_bymjj;  // (mjj, idx1, idx2)
            
            for ( unsigned int ijet  = 0; ijet < tagJets_bypt.size() ; ijet ++)
                {
                    auto jet = tagJets_bypt.at(ijet);
                    TLorentzVector tlv_ijet;
                    tlv_ijet.SetPxPyPzE(jet->px(), jet->py(), jet->pz(), jet->energy());
                    
                    for( unsigned int kjet = ijet+1; kjet < tagJets_bypt.size(); kjet++) {
                        auto jet_trail = tagJets_bypt.at(kjet);
                        TLorentzVector tlv_kjet;
                        tlv_kjet.SetPxPyPzE(jet_trail->px(), jet_trail->py(), jet_trail->pz(), jet_trail->energy());
                        TLorentzVector tlv_jetpair = tlv_ijet+tlv_kjet;
                        pair_bymjj.push_back(make_tuple(tlv_jetpair.M(),ijet,kjet));                    
                    }                
                }
            
            tagJets_bymjj = tagJets_bypt; 
            
            if (pair_bymjj.size()>0)
                {
                    std::sort(pair_bymjj.begin(),pair_bymjj.end());
                    if (debug_)
                        {                  
                            std::cout << "@@@ DEBUG"<<endl;
                            std::cout << "Jet pairs by Mjj: "<<endl;
                            for (size_t i = 0; i < pair_bymjj.size(); ++i)
                                {
                                    auto j = pair_bymjj[i];
                                    std::cout << "  mjj: " <<std::get<0>(j)<< "  idx1: " <<  std::get<1>(j) << " idx2: " <<  std::get<2>(j)<< std::endl;   
                                }                    
                        }
                    unsigned int idx1 = std::get<1>(*(pair_bymjj.rbegin()));
                    unsigned int idx2 = std::get<2>(*(pair_bymjj.rbegin()));
                    move(tagJets_bymjj, idx1, 0);
                    move(tagJets_bymjj, idx2, 1);
                }
            
            if (debug_)
                {                                      
                    std::cout << "  Jets by pt: ";
                    for (size_t i = 0; i < tagJets_bypt.size(); ++i)  std::cout << "  " <<  tagJets_bypt[i]->pt() << "  ";   
                    std::cout << std::endl;
                    std::cout << "  Jets by mjj: ";
                    for (size_t i = 0; i < tagJets_bymjj.size(); ++i)  std::cout << "  " <<  tagJets_bymjj[i]->pt() << "  ";   
                    std::cout << '\n';
                }

            vbfnjet_tags_obj.setJets_bymjj(tagJets_bymjj);
            vbfnjet_tags_obj.setJets_bypt(tagJets_bypt);     
            vbfnjet_tags->push_back(vbfnjet_tags_obj);    
            
        }
        evt.put(std::move(vbfnjet_tags));
    }  

}

typedef flashgg::VBFNjetTagProducer FlashggVBFNjetTagProducer;
DEFINE_FWK_MODULE( FlashggVBFNjetTagProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
