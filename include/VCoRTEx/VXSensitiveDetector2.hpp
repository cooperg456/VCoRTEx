#pragma once

#include "G4VSensitiveDetector.hh"





class SensitiveDetector2 : public G4VSensitiveDetector {
public:
	SensitiveDetector2(G4String);
	~SensitiveDetector2() override = default;

private:
	virtual void Initialize(G4HCofThisEvent *) override;
	virtual void EndOfEvent(G4HCofThisEvent *) override;

	virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;

private:
	G4double fEdep;
	G4double fEScint;
	G4int fNScint;
};