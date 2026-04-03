#include "VXRunAction.hpp"

#include <chrono>





TFile *RunAction::fFile = nullptr;
TTree *RunAction::fTree = nullptr;
std::mutex RunAction::fMutex;

G4double RunAction::fX = 0.;
G4double RunAction::fY = 0.;
G4double RunAction::fZ = 0.;
G4double RunAction::fEnergy = 0.;
G4double RunAction::fTime = 0.;
G4int RunAction::fDetID = 0;
G4int RunAction::fEventID = 0;





RunAction::RunAction() : G4UserRunAction() {}





RunAction::~RunAction() {
	std::lock_guard<std::mutex> lock(fMutex);
	if (fFile && IsMaster()) {
		fFile->Close();
		delete fFile;
		fFile = nullptr;
		fTree = nullptr;
	}
}





void RunAction::BeginOfRunAction(const G4Run *run) {
	if (!IsMaster()) return;

	std::lock_guard<std::mutex> lock(fMutex);

	if (!fFile) {
		fFile = new TFile("vcortex_output.root", "RECREATE");
	}

	G4String fileName = "run" + std::to_string(run->GetRunID());

	fFile->cd();
	fTree = new TTree(fileName, fileName);
	fTree->Branch("x_mm", &fX);
	fTree->Branch("y_mm", &fY);
	fTree->Branch("z_mm", &fZ);
	fTree->Branch("energy_eV", &fEnergy);
	fTree->Branch("time_ns", &fTime);
	fTree->Branch("detID", &fDetID);
	fTree->Branch("eventID", &fEventID);
}





void RunAction::EndOfRunAction(const G4Run *) {
	if (!IsMaster()) return;

	std::lock_guard<std::mutex> lock(fMutex);

	if (fTree) {
		fTree->Write();
	}
}





void RunAction::FillHit(G4double x, G4double y, G4double z, G4double energy, G4double time, G4int detID, G4int eventID) {
	std::lock_guard<std::mutex> lock(fMutex);

	fX = x;  
	fY = y;  
	fZ = z;
	fEnergy = energy;
	fTime = time;
	fDetID = detID;
	fEventID = eventID;

	if (fTree) fTree->Fill();
}