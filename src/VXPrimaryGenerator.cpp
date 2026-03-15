#include "VXPrimaryGenerator.hpp"

PrimaryGenerator::PrimaryGenerator() {
	fParticleGun = new G4ParticleGun(1);

	G4double x = 0.		* m;
	G4double y = 0.05	* m;
	G4double z = 0.		* m;
	G4ThreeVector position(x, y, z);

	G4double px = 0.2	* m;
	G4double py = -1.	* m;
	G4double pz = 0.5	* m;
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