#pragma once

//	geant4
#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"


class PhysicsList : public G4VModularPhysicsList {
public:
	PhysicsList() { 
		RegisterPhysics(new G4EmStandardPhysics()); 
	};
	
	~PhysicsList() override = default;
};