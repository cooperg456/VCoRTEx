#include "VXPhysicsList.hpp"

PhysicsList::PhysicsList() {
	RegisterPhysics(new G4EmStandardPhysics()); 
	RegisterPhysics(new G4OpticalPhysics());
}