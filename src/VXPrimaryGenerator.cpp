#include "VXPrimaryGenerator.hpp"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"





PrimaryGenerator::PrimaryGenerator() {
	fParticleGun = new G4ParticleGun(1);

	G4double x = -1.	* m;
	G4double y = -1		* m;
	G4double z = 1.		* m;
	G4ThreeVector position(x, y, z);

	G4double px = 1		* m;
	G4double py = 1.	* m;
	G4double pz = -1	* m;
	G4ThreeVector momentum(px, py, pz);

	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition *particle = particleTable->FindParticle("mu-");

	fParticleGun->SetParticlePosition(position);
	fParticleGun->SetParticleMomentumDirection(momentum);
	fParticleGun->SetParticleEnergy(1. * GeV);
	fParticleGun->SetParticleDefinition(particle);
}





PrimaryGenerator::~PrimaryGenerator() {
	delete fParticleGun;
}





void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
	fParticleGun->GeneratePrimaryVertex(anEvent);
}
