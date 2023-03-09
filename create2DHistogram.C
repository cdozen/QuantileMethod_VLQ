#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

void create2DHistogram() {
  // Open the ROOT file
  TFile *file = new TFile ("inputfiles/TT_SL_Twoleptonsthreejetsonebjet_Massthreejetstwosigma_DR18_Sumptleptons160_Bjetonept50_Dimuon.root","read");
  //TFile* file = TFile::Open("");

  // Get the two 1D histograms for ST and pt
  TH1F* st_hist = (TH1F*)file->Get("St_cut000");
  TH1F* pt_hist = (TH1F*)file->Get("Sum_Pt_Two_Leptons_cut000");

  // Create a 2D histogram
  int nBinsST = st_hist->GetNbinsX();
  float minST = st_hist->GetXaxis()->GetXmin();
  float maxST = st_hist->GetXaxis()->GetXmax();
  int nBinsPt = pt_hist->GetNbinsX();
  float minPt = pt_hist->GetXaxis()->GetXmin();
  float maxPt = pt_hist->GetXaxis()->GetXmax();
  TH2F* h2D = new TH2F("h2D", "2D Histogram", nBinsST, minST, maxST, nBinsPt, minPt, maxPt);

  // Fill the 2D histogram using the ST_hist and pt_hist histograms
  for (int i = 1; i <= nBinsST; i++) {
    for (int j = 1; j <= nBinsPt; j++) {
      float binContent = st_hist->GetBinContent(i) * pt_hist->GetBinContent(j);
      h2D->SetBinContent(i, j, binContent);
    }
  }

  // Draw the 2D histogram
  h2D->Draw("COLZ");
  TFile* outfile = new TFile("outputv2.root", "RECREATE");
    h2D->Write();
    outfile->Close();

  // Close the ROOT file
  //file->Close();
}
