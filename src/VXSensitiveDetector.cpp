#include "VXSensitiveDetector.hpp"


SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name) {
	fTotalEnergyDeposit = 0.;
}

void SensitiveDetector::Initialize(G4HCofThisEvent *) {
	fTotalEnergyDeposit = 0.;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent *) {
	//G4cout << "Deposited energy: " << fTotalEnergyDeposit << " MeV" << G4endl;
}

G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *) {
	//G4double fEnergyDeposit = aStep->GetTotalEnergyDeposit();

	//if (fEnergyDeposit > 0) {
	//	fTotalEnergyDeposit += fEnergyDeposit;
	//}

	return true;
}