#include "VXPhysicsList.hpp"

#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"





PhysicsList::PhysicsList() {
	RegisterPhysics(new G4EmStandardPhysics()); 
	RegisterPhysics(new G4OpticalPhysics());
}
