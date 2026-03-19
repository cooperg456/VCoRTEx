#pragma once

#include "G4VModularPhysicsList.hh"





class PhysicsList : public G4VModularPhysicsList {
public:
	PhysicsList();
	~PhysicsList() override = default;
};
