#include <fstream>

#include "VXSensitiveDetector.hpp"
#include "VXRunAction.hpp"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"





SensitiveDetector::SensitiveDetector(G4String name)
    : G4VSensitiveDetector(name) {}





void SensitiveDetector::Initialize(G4HCofThisEvent *) {}





void SensitiveDetector::EndOfEvent(G4HCofThisEvent *) {}





G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *) {
	G4Track *track = aStep->GetTrack();

	if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition())
		return false;

	G4int copyNo = aStep->GetPreStepPoint()->GetTouchable()->GetCopyNumber(1);
	G4StepPoint *pre = aStep->GetPreStepPoint();

	auto *runAction = const_cast<RunAction *>(
		static_cast<const RunAction *>(
			G4RunManager::GetRunManager()->GetUserRunAction()));

	runAction->FillHit(
		pre->GetPosition().x() / mm,
		pre->GetPosition().y() / mm,
		pre->GetPosition().z() / mm,
		pre->GetKineticEnergy() / eV,
		pre->GetGlobalTime() / ns,
		copyNo,
		G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID()
	);

	track->SetTrackStatus(fStopAndKill);

	return true;
}