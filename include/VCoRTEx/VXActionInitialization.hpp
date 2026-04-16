#pragma once

#include "G4VUserActionInitialization.hh"
#include "G4ThreeVector.hh"





class ActionInitialization : public G4VUserActionInitialization {
public:
	ActionInitialization() = default;
	~ActionInitialization() override = default;

	virtual void BuildForMaster() const override;
	virtual void Build() const override;
};
