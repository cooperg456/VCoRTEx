#pragma once

#include "G4VSensitiveDetector.hh"





class SensitiveDetector : public G4VSensitiveDetector {
public:
	SensitiveDetector(G4String);
	~SensitiveDetector() override = default;

private:
	G4double fTotalEnergyDeposit;

	virtual void Initialize(G4HCofThisEvent *) override;
	virtual void EndOfEvent(G4HCofThisEvent *) override;

	virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;
};
