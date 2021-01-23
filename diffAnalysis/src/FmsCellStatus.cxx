// Filename: AnFmsCellStatus.cxx
// Description: Read FMS QA root file and make status table for cells to identify dead, bad, bit shifted and hot channels.
// Author: Latif Kabir < latif@jlab.org >
// Created: Fri Jul 19 01:03:54 2019 (-0400)
// URL: jlab.org/~latif

#include "BrightStInclude.h"
#include "StRootInclude.h"
#include "RootInclude.h"
#include <vector>
using namespace std;

void FmsCellStatus(TString inFile,Int_t fillNum)
{
//    gROOT->SetBatch(kTRUE);
    //gStyle->SetOptLogy(1);
    //--------------------------------------------
    //Need for DB access
    StChain *chain = new StChain;
    StMuDstMaker *mMaker = new StMuDstMaker(0, 0, "", "resources/temp/temp.list", "", 1000); 
    mMaker->SetStatus("*",0);
    mMaker->SetStatus("Fms*",1);
    //connect to STAR FMS database
    St_db_Maker *stDb = new St_db_Maker("StarDb", "MySQL:StarDb");
    stDb->SetDateTime(20160301, 0); // An arbitrary time is set, since all we care about is the geometry.
    StFmsDbMaker *fmsDBMaker = new StFmsDbMaker("FmsDbMk");
    fmsDBMaker->Init();
    chain->Init();
    chain->EventLoop(1);
    //Check if database is working fine
    cout << "------->Max channel for det 8:"<< fmsDBMaker->maxChannel(8) <<endl;
    cout  <<"------->Max channel for det 11:"<< fmsDBMaker->maxChannel(11) <<endl;

    //---------------------------------------------
    const Int_t oMaxCh = 571; 
    const Int_t iMaxCh = 288;
//    TStar::ExitIfInvalid(inFile);
    TFile *file = new TFile(inFile);
    struct DetChPair
    {
	Int_t det;
	Int_t ch;
    } det_ch;
    
    vector < DetChPair > deadChList;
    vector < DetChPair > badChList;
    vector < DetChPair > hotChList;
    vector < DetChPair > bitShChList;

    
    TH2D *hist2d = new TH2D("FMSMap", "FMS Cell Status Map", 100, -101.0, 101.0, 150, -101.0, 101.0);

    TText *text = new TText();
    text->SetTextSize(0.015);
    TCanvas *c0 = new TCanvas("c0", "FMS Cell Status Map");
    
    StThreeVectorF fmsVec;
    Double_t x, y;
    Double_t x_offset = -1.5;
    Double_t y_offset = -1.5;
    
    for(Int_t i = 0; i < 4; ++i)
    {
	Int_t MaxCh;
	if(i == 0 || i == 1)
	    MaxCh = oMaxCh;
	else
	    MaxCh = iMaxCh;
	for (Int_t l = 0; l < MaxCh; l++) 
	{
	    fmsVec = fmsDBMaker->getStarXYZ(i + 8, l + 1);
	    hist2d->Fill(fmsVec.x(), fmsVec.y(), l+1);
	}   
    }

	hist2d->Draw("TEXT0E");
	c0->SaveAs(Form("FmsChMapQa_fill%i.pdf",fillNum));
}
