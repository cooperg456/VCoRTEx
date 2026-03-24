#include <fstream>

#include "VXSensitiveDetector.hpp"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4SystemOfUnits.hh"





SensitiveDetector::SensitiveDetector(G4String name, G4String outputFilename) : G4VSensitiveDetector(name) {
	hits.resize(0);
	outFile = outputFilename;
}





void SensitiveDetector::Initialize(G4HCofThisEvent *) {
	hits.resize(0);
}





G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *) {
	G4Track *track = aStep->GetTrack();

	if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition())
		return false;

	hits.push_back({
		aStep->GetPreStepPoint()->GetPosition(),
		aStep->GetPreStepPoint()->GetKineticEnergy(),
    	aStep->GetPreStepPoint()->GetGlobalTime()
	});

	track->SetTrackStatus(fStopAndKill);

	return true;
}





void SensitiveDetector::EndOfEvent(G4HCofThisEvent *) {
	G4cout << "SiPM detected " << hits.size() << " optical photons" << G4endl;
	
	std::ofstream file(outFile, std::ios::app);
	file << "x(mm),y(mm),z(mm),energy(eV),time(ns)\n";
	for (PhotonHit hit : hits) {
		file << hit.pos.x() / mm << "," 
			 << hit.pos.y() / mm << "," 
			 << hit.pos.z() / mm << "," 
			 << hit.energy / eV << "," 
			 << hit.time / ns << "\n" ;
	}
	file.close();
}
