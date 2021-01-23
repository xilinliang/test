// Filename: DiffJetAnaTreeQa.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Jun 11 13:08:16 2020 (-0400)
// URL: jlab.org/~latif
// created by Xilin

#include "RootInclude.h"
#include "StRootInclude.h"
#include "BrJetMaker/TStJetCandidate.h"
#include "BrJetMaker/TStJetEvent.h"
#include "BrJetMaker/TStJetSkimEvent.h"
#include "BrContainers/TStRpsTrackData.h"
#include "BrContainers/TStEventData.h"
#include <iostream>

using namespace std;

//Use wildcat in the file name to combine many files.
void DiffJetTrigCount(TString inFileName, TString outName, TString det)
{
    TChain *ch = new TChain("T");
    ch->Add(inFileName);
    
    TStJetEvent *jetEvent = 0;
    TStJetSkimEvent *skimEvent;
    TStJetCandidate *jet;
    TStJetTower *tower;
    TStJetParticle *particle;
    TStRpsTrackData *rpsTrack =0;    
    TStEventData *event;

    TClonesArray *rpsArr = new TClonesArray("TStRpsTrackData");

    ch->SetBranchAddress("jetEvents", &jetEvent);
    ch->SetBranchAddress("rpTrack",&rpsArr);

    
    TFile *outFile = new TFile(outName, "recreate");
    TH1D *h1nJets_all = new TH1D("h1nJets_all", "Number of Jets from All [TPC/BEMC + EEMC + FMS]", 10, 0, 10);
    TH1D *h1nJets  = new TH1D("h1nJets ", "Number of Jets [FMS or EEMC]", 10, 0, 10);
    TH1I *h1trigId = new TH1I("h1trigId","Trigger ID [480XXX]",100,790,889);
    TH1I *h1runId  = new TH1I("h1runId","Run Id [DDRR]",2800,6601,9400);
    TH1D *h1nFMSJP0 = new TH1D("h1nFMSJP0","Number of FMS JP0 trigger",2800,6601,9400);
    TH1D *h1nFMSJP1 = new TH1D("h1nFMSJP1","Number of FMS JP1 trigger",2800,6601,9400);
    TH1D *h1nFMSJP2 = new TH1D("h1nFMSJP2","Number of FMS JP2 trigger",2800,6601,9400);


    Double_t etaMin;
    Double_t etaMax;
    Double_t detZ; //For FMS
    if(det == "fms")
    {
	etaMin = 2.0;
	etaMax = 4.5;
	detZ = 735.; //For FMS
    }
    else if(det == "eemc")
    {
	etaMin = 1.0;
	etaMax = 2.0;
	detZ = kEEmcZSMD; //For EEMC
    }
    else
    {
	cout << "Invalid detector" <<endl;
	return;
    }
    
    
    Int_t trigId,RunId,InputRunId;
    Double_t nJets = 0;
    
    cout << "Total Entries to be processed: "<< ch->GetEntries() <<endl;

    for(Int_t evt = 0; evt < ch->GetEntries(); ++evt)
    {
	ch->GetEntry(evt);

	if(evt % 1000 == 0)
	    cout << "Events Processed: "<< evt <<endl;
	
	skimEvent = jetEvent->GetEvent();
	
	RunId = skimEvent->GetRunNumber();
	InputRunId = RunId % 1000 + 100 * ((RunId / 1000) % 100);
	h1runId->Fill(InputRunId);

	trigId = skimEvent->GetTrigId();
	h1trigId->Fill(trigId % 1000);

	nJets = jetEvent->GetNumberOfJets();
	h1nJets_all->Fill(nJets);
	
	if ((trigId == 480830)||(trigId == 480810)) h1nFMSJP0->Fill(InputRunId);

	//Exclude FMS small-bs3 trigger that gives ring of fire issue. But this removes most of high energetic jets.
	if(det == "fms")
	{
	    if(skimEvent->GetTrigFlag(5))
		continue;
	}
	//Alternative way to reduce ring of fire, require: BBCMult > 2 and TofMult > 2
	
    }



    outFile->Write();
    outFile->Close();
    delete ch;
}
