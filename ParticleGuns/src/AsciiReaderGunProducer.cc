// self H
#include "MCProduction/ParticleGuns/interface/AsciiReaderGunProducer.h"

// STL
#include <ostream>

// SimDataFormatns
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

// FWCore
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

//SimGeneral
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

// ROOT
#include "TLorentzVector.h"

// Name spaces
using namespace edm;
using namespace std;
using namespace gen;

// -- Constructor
AsciiReaderGunProducer::AsciiReaderGunProducer(const edm::ParameterSet& pset) :
	Py8GunBase(pset)
{

  cout <<"******************************"<<endl;
  cout <<"*   AsciiReaderGunProducer   *"<<endl;
  cout <<"******************************"<<endl;

  edm::ParameterSet pgun_params =  pset.getParameter<edm::ParameterSet>("PGunParameters") ;

  fileName = pgun_params.getParameter<string>("fileName");
  
  // -- keep track of the "original informations"

  fVerbosity = pset.getUntrackedParameter<int>( "Verbosity",0 ) ;
  // after configuration
  beginJob();

}

AsciiReaderGunProducer::~AsciiReaderGunProducer() {
  endJob();
}


bool AsciiReaderGunProducer::generatePartonsAndHadronize()
{
  fMasterGen->event.reset();

  // event loop (well, another step in it...)
  // no need to clean up GenEvent memory - done in HepMCProduct
   
  // here re-create fEvt (memory)
  //
  //fEvt = new HepMC::GenEvent() ;
   
  // now actualy, cook up the event from PDGTable and parameters

  // 1st, primary vertex
  //HepMC::GenVertex* Vtx = new HepMC::GenVertex(HepMC::FourVector(0.,0.,0.));

  // loop over particles

  // read a valid line from the input file
  // make sure to read a line
  string token = "";
  if ( fVerbosity >1) cout <<"[AsciiReader]::[DEBUG] token line: '"<<token<<"'"<<endl;

  while( token == "" or token[0] == '#' ){
 	 if (ascii.eof() ) {
 	   	throw cms::Exception("End Of File")
 	     	<< "Encountered End of File "<< fileName << " while processing entries"<<endl;;
 	 	}
 	 getline ( ascii, token);
  	if ( fVerbosity >1) cout <<"[AsciiReader]::[DEBUG] token line: '"<<token<<"'"<<endl;
  }
  stringstream ss(token); // convert into a stream
  
  /* line
   * run lumi event np pdgid pt eta phi 
   * 0 0 0 1  11  50. 2.3 3.1
   * 0 0 0 2  11  50. 2.2 3.1 13 52. 2.1 2.2
   */

  int runNum  ; ss >> runNum  ;
  int lumiNum ; ss >> lumiNum ;
  int eventNum; ss >> eventNum;

  int barcode = 1 ;
  unsigned int np =1 ; ss >> np;
  if( fVerbosity>1) cout <<"[AsciiReader]::[DEBUG] Producing event with np="<< np<<endl;

  for (unsigned int ip=0; ip < np; ++ip) {
    int particleID ; ss >> particleID  ;
    double pt  ; ss >> pt;
    double eta ; ss >> eta;
    double phi ; ss >> phi;
    double mass = (fMasterGen->particleData).m0( particleID );

    if (fVerbosity>1){
   	cout << "[AsciiReader]::[DEBUG] Producing event with pdg="<< particleID<<" pt="<<pt<<" eta"<<eta<<" phi="<<phi<<" mass="<<mass<<endl;
    }

    // Translate to a TLorentzVector for the Math
    TLorentzVector lv;
    lv.SetPtEtaPhiM(pt,eta,phi,mass);
    double px     = lv.Px() ;
    double py     = lv.Py() ;
    double pz     = lv.Pz() ;
    double ee     = lv.Energy() ;

    if( 1<= fabs(particleID) && fabs(particleID) <= 6) // quarks
	(fMasterGen->event).append( particleID, 23, 101, 0, px, py, pz, ee, mass ); 
    else if (fabs(particleID) == 21)                   // gluons
	(fMasterGen->event).append( 21, 23, 101, 102, px, py, pz, ee, mass );
    else                                               // other
	(fMasterGen->event).append( particleID, 1, 0, 0, px, py, pz, ee, mass );  

    ++barcode ;

  }

   if ( !fMasterGen->next() ){
	   cout <<"[WARING] not filling event!"<<endl;
	   return false;
   }

   if ( fVerbosity >1 ){
	   cout <<"-> filling  event "<<endl;
   }

   event().reset(new HepMC::GenEvent);
   return toHepMC.fill_next_event( fMasterGen->event, event().get() );

}

void AsciiReaderGunProducer::beginJob()
{
	if (fVerbosity > 0 ){
		cout <<"[AsciiReaderGunProducer]::[beginJob] Opening file: "<<fileName<<endl;
	}
	ascii.open(fileName.c_str() );
}

void AsciiReaderGunProducer::endJob()
{
	if (fVerbosity >0 ){
		cout <<"[AsciiReaderGunProducer]::[beginJob] Closing file: "<<fileName<<endl;
	}
	ascii.close();
}


