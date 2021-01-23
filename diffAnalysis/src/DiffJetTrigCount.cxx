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
    TH1D *h1NJetsByRun = new TH1D("h1NJetsByRun","Number of Jets from All by run;Run Id [DDRR]",2800,6601,9400);
    TH1I *h1trigId = new TH1I("h1trigId","Trigger ID [480XXX]",100,800,899);
    TH1I *h1runId  = new TH1I("h1runId","Run Id [DDRR]",2800,6601,9400);
    TH1D *h1nFMSJP0 = new TH1D("h1nFMSJP0","Number of FMS JP0 trigger",2800,6601,9400);
    TH1D *h1nFMSJP1 = new TH1D("h1nFMSJP1","Number of FMS JP1 trigger",2800,6601,9400);
    TH1D *h1nFMSJP2 = new TH1D("h1nFMSJP2","Number of FMS JP2 trigger",2800,6601,9400);
    TH1D *h1nsmbs1 = new TH1D("h1nsmbs1","Number of FMS-sm-bs1 trigger",2800,6601,9400);
    TH1D *h1nsmbs2 = new TH1D("h1nsmbs2","Number of FMS-sm-bs2 trigger",2800,6601,9400);
    TH1D *h1nsmbs3 = new TH1D("h1nsmbs3","Number of FMS-sm-bs3 trigger",2800,6601,9400);
    TH1D *h1nlgbs1 = new TH1D("h1nlgbs1","Number of FMS-lg-bs1 trigger",2800,6601,9400);
    TH1D *h1nlgbs2 = new TH1D("h1nlgbs2","Number of FMS-lg-bs2 trigger",2800,6601,9400);
    TH1D *h1nlgbs3 = new TH1D("h1nlgbs3","Number of FMS-lg-bs3 trigger",2800,6601,9400);

    TH1D *h1nJets_FMSJP0 = new TH1D("h1nJets_FMSJP0","Number of Jets for FMS JP0 trigger",2800,6601,9400);
    TH1D *h1nJets_FMSJP1 = new TH1D("h1nJets_FMSJP1","Number of Jets for FMS JP1 trigger",2800,6601,9400);
    TH1D *h1nJets_FMSJP2 = new TH1D("h1nJets_FMSJP2","Number of Jets for FMS JP2 trigger",2800,6601,9400);
    TH1D *h1nJets_smbs1 = new  TH1D("h1nJets_smbs1","Number of  Jets for FMS-sm-bs1 trigger",2800,6601,9400);
    TH1D *h1nJets_smbs2 = new  TH1D("h1nJets_smbs2","Number of  Jets for FMS-sm-bs2 trigger",2800,6601,9400);
    TH1D *h1nJets_smbs3 = new  TH1D("h1nJets_smbs3","Number of  Jets for FMS-sm-bs3 trigger",2800,6601,9400);
    TH1D *h1nJets_lgbs1 = new  TH1D("h1nJets_lgbs1","Number of  Jets for FMS-lg-bs1 trigger",2800,6601,9400);
    TH1D *h1nJets_lgbs2 = new  TH1D("h1nJets_lgbs2","Number of  Jets for FMS-lg-bs2 trigger",2800,6601,9400);
    TH1D *h1nJets_lgbs3 = new  TH1D("h1nJets_lgbs3","Number of  Jets for FMS-lg-bs3 trigger",2800,6601,9400);

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

	nJets = jetEvent->GetNumberOfJets();
	if (nJets == 0) continue;


	trigId = skimEvent->GetTrigId();
	h1trigId->Fill(trigId % 1000);

	h1nJets_all->Fill(nJets);
	h1NJetsByRun->Fill(InputRunId,nJets);
	
	if ((trigId == 480830)||(trigId == 480810)) {h1nFMSJP0->Fill(InputRunId); h1nJets_FMSJP0->Fill(InputRunId,nJets);}
	if ((trigId == 480829)||(trigId == 480809)) {h1nFMSJP1->Fill(InputRunId); h1nJets_FMSJP1->Fill(InputRunId,nJets);}
        if ((trigId == 480828)||(trigId == 480808)) {h1nFMSJP2->Fill(InputRunId); h1nJets_FMSJP2->Fill(InputRunId,nJets);}
	if ((trigId == 480841)||(trigId == 480801)) {h1nsmbs1->Fill(InputRunId); h1nJets_smbs1->Fill(InputRunId,nJets); }
	if ((trigId == 480822)||(trigId == 480802)) {h1nsmbs2->Fill(InputRunId); h1nJets_smbs2->Fill(InputRunId,nJets);}
	if ((trigId == 480843)||(trigId == 480803)) {h1nsmbs3->Fill(InputRunId); h1nJets_smbs3->Fill(InputRunId,nJets);}	
	if ((trigId == 480844)||(trigId == 480804)) {h1nlgbs1->Fill(InputRunId); h1nJets_lgbs1->Fill(InputRunId,nJets);}
        if ((trigId == 480825)||(trigId == 480805)) {h1nlgbs2->Fill(InputRunId); h1nJets_lgbs2->Fill(InputRunId,nJets);}
        if ((trigId == 480826)||(trigId == 480806)) {h1nlgbs3->Fill(InputRunId); h1nJets_lgbs3->Fill(InputRunId,nJets);}

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
