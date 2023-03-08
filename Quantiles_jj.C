/**
 * Get quantile from 2D dR vs tprimeMass histogram
 * Then plot dR vs Quantile..
 * CDozen
*/
#include <iostream>
//#include <cmath>
#include <TH1D.h>
#include <TH2F.h>
#include <TFile.h>
#include <TRandom3.h>
#include <TMath.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <THStack.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TMultiGraph.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TString.h>
#include <TMath.h>

void Quantiles_jj(){
  TFile *f2 = new TFile ("inputfiles/20230227_3T_2018UL_Mtop140_FullBRew_cut5drbbfit70expo/BkgEst3Tlistinput2018ULData.root","read");

  //dd1=TPMassvsDRjjW_CD
  TH2F* dd1 = (TH2F*)f2->Get("TPMassvsDRjjW_CD");

  //create canvas and test print
  TCanvas * can = new TCanvas("can", "can", 600, 450);
  dd1->Draw("Colz");
  

  //SetProjectionY for DRjjW
  TH1F * zz[50];
  for(int i =0;i<dd1->GetNbinsX();i++){
    TString hname= Form("ProjectionY_%d",i); //Get DR_jjplots depends on each TprimeMass bins
    zz[i] = (TH1F*) dd1->ProjectionY(hname,i);
    //zz[i]->Draw();
  }
  
  TCanvas * can2 = new TCanvas("can2", "can2", 800, 600);
  can2->Divide(5,10);
  can2->cd();
  std::cout<<"NbinsX:"<<dd1->GetNbinsX()<<std::endl;
  std::cout<<"NbinsY:"<<dd1->GetNbinsY()<<std::endl;

  const int nq  = 50;
  Double_t xq[nq];     // position where to compute the quantiles in [0,1]
  Double_t yq1[nq];  // array to contain the quantiles
  Double_t yq2[nq];  // array to contain the quantiles
  double a,b;
  
  //b=0.50;
  //b=0.65;
  b=0.80; 
  // y bins array
  Double_t xbins[nq]; //dR
  Double_t ybins[nq];
  Double_t quantile[nq]; // mass quantile
  for(int i=0;i<dd1->GetNbinsX();i++){
    can2->cd(i+1);
    zz[i]->Draw();
    zz[i]->GetQuantiles(1,&a,&b);
    TAxis* xAxis = dd1->GetXaxis();
    TAxis* yAxis = dd1->GetYaxis();
    ybins[i]=yAxis->GetBinUpEdge(i);
    //xbins[i]=xAxis->GetBinCenter(i); 
    xbins[i]=xAxis->GetBinUpEdge(i); 
    quantile[i]=a;
    std::cout<<"bin"<<i<<"\t LE:"<<xbins[i]<<"\t a:"<<a<<"\t b:"<<b<<std::endl;
    //std::cout<<"bin"<<i<<"\t LE_Y:"<<ybins[i]<<"\t a:"<<a<<"\t b:"<<b<<std::endl;
    //std::cout<<"binY"<<i<<"\t LEY:"<<ybins[i]<<std::endl;
  }


  TCanvas * can3 = new TCanvas("can3", "can3", 800, 600);
  can3->cd();
  TGraph *gr = new TGraph(nq,xbins, quantile); 

  //gStyle->SetPalette(kSolar);
  //TGraph *g1 = new TGraph(5,x,y1); g1->SetTitle("Graph with a red star");
  gPad->SetGrid();
  gStyle->SetOptFit(111111);
  gr->SetLineColor(0); //cut7
  gr->SetMarkerColor(kRed+2);
  gr->SetMarkerStyle(20);
  gr->SetTitle("DR_jjW _2M1L_weighted");
  gr->GetXaxis()->SetTitle("TprimeMass(GeV)");
  gr->GetYaxis()->SetTitle("dR_jjW(quantile)");

  
  gr->Draw("apl");
  //gPad->BuildLegend();
  //TF1 *f4 = new TF1("f4","expo+[c]",330,1290);//28 feb bu fiti kullandin..
  TF1 *g2 = new TF1("g2","pol3",330,1290);//1.sirada
  //TF1 *g2 = new TF1("g2","pol2(2)+expo(0)",340,1290);
  g2->SetLineColor(4); 
  //gr->Fit(g2,"R");

  // Create an empty string called "Correction" using the TString class
  TString Correction = "";

  // Loop over the first four parameters of the fit function
  for (int i = 0; i < 4; i++)
  {
    // Get the i-th fit parameter
    double param = g2->GetParameter(i);

    // Check the sign of the parameter and add the appropriate sign symbol to the correction string
    if (param >= 0 && i > 0)
    {
      Correction += " + ";
    }
    else if (param < 0)
    {
      Correction += " - ";
      param = -param;
    }

    // Create a stringstream object to convert the parameter to scientific notation
    std::stringstream myparameter;
    myparameter << std::scientific << param;

    // Define a string indicating what the correction is based on (mass or eta, for example)
    std::string cutTP = "Reconstructed_Tprime->M()";

    // Add a term to the Correction TString that includes the i-th fit parameter
    Correction += Form("%s*pow(%s,%d)", myparameter.str().c_str(), cutTP.c_str(), i);
  }

  // Print the Correction TString to the console
  std::cout << "Correction: " << Correction << std::endl;
  //In this output, there is a minus sign before the first term and a plus sign between each subsequent term, as appropriate  based on the sign of the corresponding fit parameter.

  TLegend* legend = new TLegend(0.40, 0.80, 0.60, 0.90); // (x1, y1, x2, y2) in NDC coordinates

  // set the legend style
  legend->SetBorderSize(1);
  legend->SetFillColor(0);
  legend->SetTextSize(0.03);

  // add an entry to the legend
  legend->AddEntry(gr, "80% quantile", "lep"); // (object, label, option)
  legend->AddEntry(g2, "Fit: pol3", "l");
  legend->Draw();


}