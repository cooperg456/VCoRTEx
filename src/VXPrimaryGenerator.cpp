#include "VXPrimaryGenerator.hpp"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"





PrimaryGenerator::PrimaryGenerator() {
	fParticleGun = new G4ParticleGun(1);

	fParticleGun->SetParticlePosition(G4ThreeVector(200, 200, 200));
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(-1, -1, -1));
	fParticleGun->SetParticleEnergy(1000 * MeV);
	fParticleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("mu-"));
}





PrimaryGenerator::~PrimaryGenerator() {
	delete fParticleGun;
}





void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
	fParticleGun->GeneratePrimaryVertex(anEvent);
}
