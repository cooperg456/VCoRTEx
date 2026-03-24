#include "VXPhysicsList.hpp"

#include "G4OpticalPhysics.hh"





PhysicsList::PhysicsList() : FTFP_BERT() {
	RegisterPhysics(new G4OpticalPhysics());
}
