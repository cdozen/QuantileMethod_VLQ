#include <TH1.h>
#include <TH2.h>
#include <TFile.h>

void create2D()
{
    // Open the ROOT file containing the 1D histograms
    //TFile* file = new TFile("file.root", "READ");
    TFile *file = new TFile ("inputfiles/TT_SL_Twoleptonsthreejetsonebjet_Massthreejetstwosigma_DR18_Sumptleptons160_Bjetonept50_Dimuon.root","read");

    // Get the 1D histograms from the file
    TH1F* h1 = (TH1F*) file->Get("St_cut000");
    TH1F* h2 = (TH1F*) file->Get("Sum_Pt_Two_Leptons_cut000");
    TCanvas * can2 = new TCanvas("can2", "can2", 800, 600);
    can2->Divide(1,2);
    can2->cd(1);
    h1->Draw();
    can2->cd(2);
    h2->Draw();

    // Create a new 2D histogram with the same binning as the 1D histograms
    TH2F* h2d = new TH2F("histogram2d", "2D Histogram", h1->GetNbinsX(), h1->GetXaxis()->GetXmin(), h1->GetXaxis()->GetXmax(), h2->GetNbinsX(), h2->GetXaxis()->GetXmin(), h2->GetXaxis()->GetXmax());

    // Loop over the bins in the 2D histogram and fill them with the values from the 1D histograms
    /*for (int i=1; i<=h1->GetNbinsX(); i++) {
        for (int j=1; j<=h2->GetNbinsX(); j++) {
            h2d->SetBinContent(i, j, 0.0);
        }
    }*/

    // Fill the 2D histogram with data from the 1D histograms
    for (int i=1; i<=h1->GetNbinsX(); i++) {
        for (int j=1; j<=h2->GetNbinsX(); j++) {
            h2d->Fill(h1->GetBinCenter(i), h2->GetBinCenter(j), h1->GetBinContent(i) * h2->GetBinContent(j));
        }
    }

    
TCanvas * can3 = new TCanvas("can3", "can3", 800, 600);
    can3->cd();
    h2d->Draw("COLZ");
    // Save the 2D histogram to a new file
    TFile* outfile = new TFile("output.root", "RECREATE");
    h2d->Write();
    outfile->Close();
}
