#include <iostream>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>

void test2() {

  // Load the ROOT file containing the two 1D histograms
  TFile *file = new TFile ("inputfiles/TT_SL_Twoleptonsthreejetsonebjet_Massthreejetstwosigma_DR18_Sumptleptons160_Bjetonept50_Dimuon.root","read");

    
  if (!file->IsOpen()) {
    std::cout << "Error: could not open ROOT file." << std::endl;
    return;
  }

  // Get the two 1D histograms from the ROOT file
  TH1F *histo_ST = (TH1F*)file->Get("St_cut000");
  TH1F *histo_Pt = (TH1F*)file->Get("Sum_Pt_Two_Leptons_cut000");
  if (!histo_ST || !histo_Pt) {
    std::cout << "Error: could not retrieve histograms from ROOT file." << std::endl;
    file->Close();
    return;
  }

  // Create the 2D histogram
  TH2F *histo_2D = new TH2F("histo_2D", "2D Histogram", histo_ST->GetNbinsX(), histo_ST->GetXaxis()->GetXmin(), histo_ST->GetXaxis()->GetXmax(), histo_Pt->GetNbinsX(), histo_Pt->GetXaxis()->GetXmin(), histo_Pt->GetXaxis()->GetXmax());

  // Fill the 2D histogram with the contents of the two 1D histograms
  for (int i = 1; i <= histo_ST->GetNbinsX(); i++) {
    float bin_center_ST = histo_ST->GetBinCenter(i);
    for (int j = 1; j <= histo_Pt->GetNbinsX(); j++) {
      float bin_content_Pt = histo_Pt->GetBinContent(j);
      histo_2D->Fill(bin_center_ST, bin_content_Pt);
    }
  }
  histo_2D->Draw("COLZ");

  // Save the 2D histogram to a new ROOT file
  TFile *new_file = new TFile("new_file.root", "RECREATE");
  if (!new_file->IsOpen()) {
    std::cout << "Error: could not create new ROOT file." << std::endl;
    file->Close();
    return;
  }
  
  //histo_2D->Write();
  //new_file->Close();

  // Clean up
  //delete histo_ST;
  //delete histo_Pt;
  //delete histo_2D;
  //file->Close();
}
