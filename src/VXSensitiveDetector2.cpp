#include "VXSensitiveDetector2.hpp"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

#include <iostream>





SensitiveDetector2::SensitiveDetector2(G4String name)
    : G4VSensitiveDetector(name), fEdep(0.), fNScint(0) {}

void SensitiveDetector2::Initialize(G4HCofThisEvent *) {
    fEdep = 0.;
	fEScint = 0.;
    fNScint = 0;
}





void SensitiveDetector2::EndOfEvent(G4HCofThisEvent *) {
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    std::cout << "  EScint = " << fEScint <<  "MeV\n"
			  << "  Edep = " << fEdep << " MeV\n"
			  << "  e_light = " << fEScint/fEdep << "\n"
              << "  NScint = " << fNScint << std::endl;
}





G4bool SensitiveDetector2::ProcessHits(G4Step *aStep, G4TouchableHistory *) {
    G4Track *track = aStep->GetTrack();

    // energy deposit from charged particles
    G4double edep = aStep->GetTotalEnergyDeposit();
    if (edep > 0. && track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) {
        fEdep += edep;
    }

    // count scintillation photons on creation
    if (track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()
        && track->GetCurrentStepNumber() == 1
        && track->GetCreatorProcess()
        && track->GetCreatorProcess()->GetProcessName() == "Scintillation") {
        fNScint++;
		fEScint += track->GetKineticEnergy();
    }

    return true;
}