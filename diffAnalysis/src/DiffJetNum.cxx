// Filename: DiffJetAnaTreeQa.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Jun 11 13:08:16 2020 (-0400)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "StRootInclude.h"
#include <vector>
#include <iostream>
#include "TGraph.h"

using namespace std;

//Use wildcat in the file name to combine many files.
void DiffJetNum(TString inFileName, TString outName)
{
    TChain *ch = new TChain("T");
    
    TFile *file = new TFile(inFileName);   
    TFile *outFile = new TFile(outName, "recreate");

    TH1I *nTrigDist;
    TH1D *nJetsRun;
    TH1I *runId;
    TH1D *nTrigJP0;
    TH1D *nTrigJP1;
    TH1D *nTrigJP2;
    TH1D *nTrigsmbs1;
    TH1D *nTrigsmbs2;
    TH1D *nTrigsmbs3;
    TH1D *nTriglgbs1;
    TH1D *nTriglgbs2;
    TH1D *nTriglgbs3;

    TH1D *nJetsTrigJP0;
    TH1D *nJetsTrigJP1;
    TH1D *nJetsTrigJP2;
    TH1D *nJetsTrigsmbs1;
    TH1D *nJetsTrigsmbs2;
    TH1D *nJetsTrigsmbs3;
    TH1D *nJetsTriglgbs1;
    TH1D *nJetsTriglgbs2;
    TH1D *nJetsTriglgbs3;

    nTrigDist = (TH1I*) file->Get("h1trigId");
    nJetsRun = (TH1D*) file->Get("h1NJetsByRun");
    runId = (TH1I*) file->Get("h1runId");
    nTrigJP0 = (TH1D*) file->Get("h1nFMSJP0");
    nTrigJP1 = (TH1D*) file->Get("h1nFMSJP1");
    nTrigJP2 = (TH1D*) file->Get("h1nFMSJP2");
    nTrigsmbs1 = (TH1D*) file->Get("h1nsmbs1");
    nTrigsmbs2 = (TH1D*) file->Get("h1nsmbs2");
    nTrigsmbs3 = (TH1D*) file->Get("h1nsmbs3");
    nTriglgbs1 = (TH1D*) file->Get("h1nlgbs1");
    nTriglgbs2 = (TH1D*) file->Get("h1nlgbs2");
    nTriglgbs3 = (TH1D*) file->Get("h1nlgbs3");

    nJetsTrigJP0 = (TH1D*) file->Get("h1nJets_FMSJP0");
    nJetsTrigJP1 = (TH1D*) file->Get("h1nJets_FMSJP1");
    nJetsTrigJP2 = (TH1D*) file->Get("h1nJets_FMSJP2");
    nJetsTrigsmbs1 = (TH1D*) file->Get("h1nJets_smbs1");
    nJetsTrigsmbs2 = (TH1D*) file->Get("h1nJets_smbs2");
    nJetsTrigsmbs3 = (TH1D*) file->Get("h1nJets_smbs3");
    nJetsTriglgbs1 = (TH1D*) file->Get("h1nJets_lgbs1");
    nJetsTriglgbs2 = (TH1D*) file->Get("h1nJets_lgbs2");
    nJetsTriglgbs3 = (TH1D*) file->Get("h1nJets_lgbs3");

	vector <Double_t> runlist;
	vector <Double_t> JP0list;
	vector <Double_t> JP1list;
	vector <Double_t> JP2list;
	vector <Double_t> smbs1list;
        vector <Double_t> smbs2list;
        vector <Double_t> smbs3list;
	vector <Double_t> lgbs1list;
        vector <Double_t> lgbs2list;
        vector <Double_t> lgbs3list;

	TString triggerNames_fms[9] = {"FMS-JP0", "FMS-JP1", "FMS-JP2", "FMS-sm-bs1", "FMS-sm-bs2", "FMS-sm-bs3", "FMS-lg-bs1", "FMS-lg-bs2", "FMS-lg-bs3"};
	
	Double_t IDtrig[9] = {1,2,3,4,5,6,7,8,9};
	Double_t trigDist[9];


	trigDist[0] = nTrigDist->GetBinContent(11) + nTrigDist->GetBinContent(31);
	trigDist[1] = nTrigDist->GetBinContent(10) + nTrigDist->GetBinContent(30);
	trigDist[2] = nTrigDist->GetBinContent(9) + nTrigDist->GetBinContent(29);
	trigDist[3] = nTrigDist->GetBinContent(2) + nTrigDist->GetBinContent(42);
	trigDist[4] = nTrigDist->GetBinContent(3) + nTrigDist->GetBinContent(23);
	trigDist[5] = nTrigDist->GetBinContent(4) + nTrigDist->GetBinContent(44);
	trigDist[6] = nTrigDist->GetBinContent(5) + nTrigDist->GetBinContent(45);
	trigDist[7] = nTrigDist->GetBinContent(6) + nTrigDist->GetBinContent(26);
	trigDist[8] = nTrigDist->GetBinContent(7) + nTrigDist->GetBinContent(27);

	Double_t nJets, nTrig, nJetsAve;
	Double_t currentRun;

	for (Int_t i=1;i<2800;++i)
	{

		currentRun = i + 6600;
		if (runId->GetBinContent(i) > 0)
		{
			runlist.push_back(currentRun);
			nJets = nJetsTrigJP0->GetBinContent(i);
			nTrig = nTrigJP0->GetBinContent(i);
			if (nTrig > 0) nJetsAve = nJets / nTrig; else nJetsAve = -1;
			JP0list.push_back(nJetsAve);
			nJets = nJetsTrigJP1->GetBinContent(i);
			nTrig = nTrigJP1->GetBinContent(i);
                        if (nTrig > 0) nJetsAve = nJets / nTrig; else nJetsAve = -1;
                        JP1list.push_back(nJetsAve);
			nJets = nJetsTrigJP2->GetBinContent(i);
			nTrig = nTrigJP2->GetBinContent(i);
                        if (nTrig > 0) nJetsAve = nJets / nTrig; else nJetsAve = -1;
                        JP2list.push_back(nJetsAve);

			nJets = nJetsTrigsmbs1->GetBinContent(i);
                        nTrig = nTrigsmbs1->GetBinContent(i);
                        if (nTrig > 0) nJetsAve = nJets / nTrig; else nJetsAve = -1;
                        smbs1list.push_back(nJetsAve);
			nJets = nJetsTrigsmbs2->GetBinContent(i);
                        nTrig = nTrigsmbs2->GetBinContent(i);
                        if (nTrig > 0) nJetsAve = nJets / nTrig; else nJetsAve = -1;
                        smbs2list.push_back(nJetsAve);
			nJets = nJetsTrigsmbs3->GetBinContent(i);
                        nTrig = nTrigsmbs3->GetBinContent(i);
                        if (nTrig > 0) nJetsAve = nJets / nTrig; else nJetsAve = -1;
                        smbs3list.push_back(nJetsAve);

			nJets = nJetsTriglgbs1->GetBinContent(i);
                        nTrig = nTriglgbs1->GetBinContent(i);
                        if (nTrig > 0) nJetsAve = nJets / nTrig; else nJetsAve = -1;
                        lgbs1list.push_back(nJetsAve);
			nJets = nJetsTriglgbs2->GetBinContent(i);
                        nTrig = nTriglgbs2->GetBinContent(i);
                        if (nTrig > 0) nJetsAve = nJets / nTrig; else nJetsAve = -1;
                        lgbs2list.push_back(nJetsAve);
                        nJets = nJetsTriglgbs3->GetBinContent(i);
                        nTrig = nTriglgbs3->GetBinContent(i);
                        if (nTrig > 0) nJetsAve = nJets / nTrig; else nJetsAve = -1;
                        lgbs3list.push_back(nJetsAve);
		}

	}
	
	TGraph* trigger_distribution = new TGraph(9,IDtrig,trigDist);
	TGraph* gr_JP0 = new TGraph(runlist.size(),(&runlist[0]),(&JP0list[0]));
	TGraph* gr_JP1 = new TGraph(runlist.size(),(&runlist[0]),(&JP1list[0]));
	TGraph* gr_JP2 = new TGraph(runlist.size(),(&runlist[0]),(&JP2list[0]));
	TGraph* gr_smbs1 = new TGraph(runlist.size(),(&runlist[0]),(&smbs1list[0]));
        TGraph* gr_smbs2 = new TGraph(runlist.size(),(&runlist[0]),(&smbs2list[0]));
        TGraph* gr_smbs3 = new TGraph(runlist.size(),(&runlist[0]),(&smbs3list[0]));
	TGraph* gr_lgbs1 = new TGraph(runlist.size(),(&runlist[0]),(&lgbs1list[0]));
        TGraph* gr_lgbs2 = new TGraph(runlist.size(),(&runlist[0]),(&lgbs2list[0]));
        TGraph* gr_lgbs3 = new TGraph(runlist.size(),(&runlist[0]),(&lgbs3list[0]));

	trigger_distribution->SetName("trigDist");
	gr_JP0->SetName("JP0");
	gr_JP1->SetName("JP1");
	gr_JP2->SetName("JP2");
	gr_smbs1->SetName("smbs1");
        gr_smbs2->SetName("smbs2");
        gr_smbs3->SetName("smbs3");
	gr_lgbs1->SetName("lgbs1");
        gr_lgbs2->SetName("lgbs2");
        gr_lgbs3->SetName("lgbs3");	

	trigger_distribution->SetTitle("FMS trigger distribution");

	TAxis *ax = trigger_distribution->GetHistogram()->GetXaxis();
   	Double_t x1 = ax->GetBinLowEdge(1); 
   	Double_t x2 = ax->GetBinUpEdge(ax->GetNbins());
	trigger_distribution->GetHistogram()->GetXaxis()->Set(9,x1,x2);
	for (Int_t i=0;i<9;++i) {trigger_distribution->GetHistogram()->GetXaxis()->SetBinLabel(i+1,triggerNames_fms[i]);}
	gr_JP0->SetTitle("Average jets for FMS-JP0 trigger for run");
	gr_JP0->GetXaxis()->SetTitle("run number [DDRR]");
	gr_JP1->SetTitle("Average jets for FMS-JP1 trigger for run");
        gr_JP1->GetXaxis()->SetTitle("run number [DDRR]");
	gr_JP2->SetTitle("Average jets for FMS-JP2 trigger for run");
        gr_JP2->GetXaxis()->SetTitle("run number [DDRR]");
	gr_smbs1->SetTitle("Average jets for FMS-sm-bs1 trigger for run");
        gr_smbs1->GetXaxis()->SetTitle("run number [DDRR]");
        gr_smbs2->SetTitle("Average jets for FMS-sm-bs2 trigger for run");
        gr_smbs2->GetXaxis()->SetTitle("run number [DDRR]");
        gr_smbs3->SetTitle("Average jets for FMS-sm-bs3 trigger for run");
        gr_smbs3->GetXaxis()->SetTitle("run number [DDRR]");
	gr_lgbs1->SetTitle("Average jets for FMS-lg-bs1 trigger for run");
        gr_lgbs1->GetXaxis()->SetTitle("run number [DDRR]");
        gr_lgbs2->SetTitle("Average jets for FMS-lg-bs2 trigger for run");
        gr_lgbs2->GetXaxis()->SetTitle("run number [DDRR]");
        gr_lgbs3->SetTitle("Average jets for FMS-lg-bs3 trigger for run");
        gr_lgbs3->GetXaxis()->SetTitle("run number [DDRR]");

	trigger_distribution->SetFillColor(40);

	trigger_distribution->Write();
	gr_JP0->Write();
	gr_JP1->Write();
	gr_JP2->Write();
	gr_smbs1->Write();
        gr_smbs2->Write();
        gr_smbs3->Write();
	gr_lgbs1->Write();
        gr_lgbs2->Write();
        gr_lgbs3->Write();

    outFile->Write();
    outFile->Close();
    delete ch;
}
