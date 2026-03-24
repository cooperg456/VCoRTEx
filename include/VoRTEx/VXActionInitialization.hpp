#pragma once

#include "G4VUserActionInitialization.hh"
#include "G4ThreeVector.hh"





class ActionInitialization : public G4VUserActionInitialization {
public:
	ActionInitialization(G4ThreeVector, G4ThreeVector, G4double, G4String);
	~ActionInitialization() override = default;

	virtual void BuildForMaster() const override;
	virtual void Build() const override;

	G4ThreeVector gunPos;
	G4ThreeVector gunMom;
	G4double gunMeV;
	G4String pName;
};
