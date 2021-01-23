// Filename: AnFmsCellStatus.cxx
// Description: Read FMS QA root file and make status table for cells to identify dead, bad, bit shifted and hot channels.
// Author: Latif Kabir < latif@jlab.org >
// Created: Fri Jul 19 01:03:54 2019 (-0400)
// URL: jlab.org/~latif

#include "BrightStInclude.h"
#include "StRootInclude.h"
#include "RootInclude.h"
#include <vector>
#include "TH2.h"
#include <iostream>
using namespace std;

void AnFmsCellStatus(Int_t fillNum)
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
    stDb->SetDateTime(20141210, 0); // An arbitrary time is set, since all we care about is the geometry.
    StFmsDbMaker *fmsDBMaker = new StFmsDbMaker("FmsDbMk");
    fmsDBMaker->Init();
    chain->Init();
    chain->EventLoop(1);
    //Check if database is working fine
//    cout << "------->Max channel for det 8:"<< fmsDBMaker->maxChannel(8) <<endl;
//    cout  <<"------->Max channel for det 11:"<< fmsDBMaker->maxChannel(11) <<endl;

    //---------------------------------------------
    const Int_t oMaxCh = 571; 
    const Int_t iMaxCh = 288;
//    TStar::ExitIfInvalid(inFile);
    struct DetChPair
    {
	Int_t det;
	Int_t ch;
    } det_ch;

    ifstream FmsGainFile;
    FmsGainFile.open("fms_gain_out.txt");

    TFile *file = new TFile(Form("root_result/FmsQaByFill/FmsQaFill%i_0111.root",fillNum));
    
    TFile *outFile = new TFile("FmsCellIdCorr.root","recreate"); 
    TH2D *hist2d_large = new TH2D("FMSMap_l", "FMS Cell Status Map", 80, -101.0, 101.0, 100, -101.0, 101.0);
    TH2D *hist2d_small = new TH2D("FMSMap_s", "FMS Cell Status Map", 80, -101.0, 101.0, 100, -101.0, 101.0);

    TH2D *JetXY;
    JetXY = (TH2D*)file->Get("h2xy_fms");

    TText *text = new TText();
    text->SetTextSize(0.015);
    TCanvas *c0 = new TCanvas("c0", "FMS Cell Status Map",5000,5000);
    TCanvas *c_bkg = new TCanvas("FmsCellId", "FMS Cell Status Map");  
  
    StThreeVectorF fmsVec;
    Double_t x, y;
    Double_t x_offset = -1.5;
    Double_t y_offset = -1.5;
    Int_t detNum,ChNum;
    Double_t ChGain;
    Int_t ChAct[4][600] ={0} ;

    while (FmsGainFile >> detNum >> ChNum >> ChGain)
	{

		cout<<ChNum<<"Ch gain:"<<ChGain<<endl;
		if (ChGain > 0) {ChAct[detNum-1][ChNum-1] = 1;}
		cout<<ChAct[detNum-1][ChNum-1]<<endl;

	}

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
	    cout<<i<<" "<<l<<" "<<ChAct[i][l]<<endl;
	    if (ChAct[i][l] == 0) continue;
	    if (i==0 || i ==1) hist2d_large->Fill(fmsVec.x(), fmsVec.y(), l+1); else hist2d_small->Fill(fmsVec.x(), fmsVec.y(), l+1);
//	    cout<<"det:"<<i+8<<"  ch:"<<l+1<<"  x="<<fmsVec.x()<<"  y="<<fmsVec.y()<<endl;
	}   
    }

	c0->cd();

	gStyle->SetOptStat(0);
	gStyle->SetPaintTextFormat("g");
	hist2d_large->SetMinimum(1);
	hist2d_large->SetMarkerSize(0.45);
	hist2d_large->SetMarkerColor(1);
	hist2d_small->SetMinimum(1);
        hist2d_small->SetMarkerSize(0.40);
        hist2d_small->SetMarkerColor(41);
	JetXY->Draw("colz same");
	hist2d_large->Draw("TEXT same");
	hist2d_small->Draw("TEXT same");
//	c0->SaveAs(Form("pdf_result/FmsHotChQa_0113/FmsChMapQa_fill%i.pdf",fillNum));


//	c_bkg->cd();
//	gStyle->SetOptStat(0);
//	hist2d_large->Draw("TEXT same");
//        hist2d_small->Draw("TEXT same");
//	c_bkg->SaveAs("FmsCellId.root");

	outFile->Write();
	outFile->Close();


}
