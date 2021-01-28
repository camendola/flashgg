#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/VBFTag.h"
#include "flashgg/DataFormats/interface/VBFNjetTag.h"
#include "flashgg/DataFormats/interface/VBFTagTruth.h"

#include "flashgg/DataFormats/interface/PDFWeightObject.h"

#include <vector>
#include <algorithm>


using namespace std;
using namespace edm;

namespace flashgg {

  class VBFNjetTagProducer : public EDProducer
  {
  public:
    VBFNjetTagProducer( const ParameterSet & );

  private:
    void produce( Event &, const EventSetup & ) override;
    EDGetTokenT<View<DiPhotonCandidate> >                       diPhotonToken_;
    EDGetTokenT<View<VBFDiPhoDiJetMVAResult> >                  vbfDiPhoDiJetMvaResultToken_;
    EDGetTokenT<View<DiPhotonMVAResult> >                       mvaResultToken_;
    std::vector<edm::EDGetTokenT<View<flashgg::Jet> > >         tokenJets_;
    std::vector<edm::InputTag>                                  inputTagJets_;
    typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;
    
  };
  
  
  
  VBFNjetTagProducer::VBFNjetTagProducer( const ParameterSet &iConfig ) :
    diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
    vbfDiPhoDiJetMvaResultToken_( consumes<View<flashgg::VBFDiPhoDiJetMVAResult> >( iConfig.getParameter<InputTag> ( "VBFDiPhoDiJetMVAResultTag" ) ) ),
    mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),   
    inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ){
    
    for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
      auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
      tokenJets_.push_back(token);
    }
    
    
    produces<vector<VBFNjetTag> >(); 
  }
  
  
  void VBFNjetTagProducer::produce( Event &evt, const EventSetup &)
  {
    Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
    evt.getByToken( diPhotonToken_, diPhotons );

    Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
    evt.getByToken( mvaResultToken_, mvaResults );
    
    Handle<View<flashgg::VBFDiPhoDiJetMVAResult> > vbfDiPhoDiJetMvaResults;
    evt.getByToken( vbfDiPhoDiJetMvaResultToken_, vbfDiPhoDiJetMvaResults );
    
    JetCollectionVector Jets(inputTagJets_.size());
    for(size_t j = 0; j < inputTagJets_.size(); ++j) {
      evt.getByToken(tokenJets_[j], Jets[j]);
    }
    
    std::unique_ptr<vector<VBFNjetTag> > vbfnjet_tags(new vector<VBFNjetTag>);
    
    for(unsigned int idipho = 0; idipho < diPhotons->size(); idipho++) {
      edm::Ptr<flashgg::VBFDiPhoDiJetMVAResult> vbfdipho_mvares = vbfDiPhoDiJetMvaResults->ptrAt(idipho);
      edm::Ptr<flashgg::DiPhotonMVAResult>      mvares         = mvaResults->ptrAt(idipho); 
      edm::Ptr<flashgg::DiPhotonCandidate>      dipho          = diPhotons->ptrAt(idipho);
      
      VBFNjetTag vbfnjet_tags_obj(dipho,mvares,vbfdipho_mvares);
      vbfnjet_tags_obj.includeWeights( *dipho );
      //Jets
      std::vector<edm::Ptr<Jet> > tagJets;
      unsigned int jetCollectionIndex = diPhotons->ptrAt(idipho)->jetCollectionIndex();
      
      for( unsigned int ijet = 0; ijet < Jets[jetCollectionIndex]->size(); ijet++) {
	
	edm::Ptr<flashgg::Jet> jet = Jets[jetCollectionIndex]->ptrAt(ijet);
	if(!jet->passesJetID (flashgg::Tight2017)) continue;
	float dRPhoLeadJet = deltaR(jet->eta(), jet->phi(), 
				    dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi()) ;
	float dRPhoSubLeadJet = deltaR(jet->eta(), jet->phi(), 
				       dipho->subLeadingPhoton()->superCluster()->eta(), dipho->subLeadingPhoton()->superCluster()->phi());
	
	if(dRPhoLeadJet < 0.4 || dRPhoSubLeadJet < 0.4) continue;
	tagJets.push_back(jet);
      }
      
      vbfnjet_tags_obj.setJets(tagJets);     
      vbfnjet_tags->push_back(vbfnjet_tags_obj);    
        
    }
    evt.put(std::move(vbfnjet_tags));
  }
}


typedef flashgg::VBFNjetTagProducer FlashggVBFNjetTagProducer;
DEFINE_FWK_MODULE( FlashggVBFNjetTagProducer );
