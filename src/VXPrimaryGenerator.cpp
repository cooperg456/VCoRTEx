#include "VXPrimaryGenerator.hpp"

PrimaryGenerator::PrimaryGenerator() {
	fParticleGun = new G4ParticleGun(1);

	G4double x = 0. * m;
	G4double y = 0. * m;
	G4double z = 0. * m;

	G4ThreeVector pos(x, y, z);

	G4double px = 0. * m;
	G4double py = 0. * m;
	G4double pz = 0. * m;

	G4ThreeVector mom(px, py, pz);

	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition *particle = particleTable->FindParticle("e+");

	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(mom);
	fParticleGun->SetParticleEnergy(1. * GeV);
	fParticleGun->SetParticleDefinition(particle);

}

PrimaryGenerator::~PrimaryGenerator() {
	delete fParticleGun;
}

void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
	fParticleGun->GeneratePrimaryVertex(anEvent);
}