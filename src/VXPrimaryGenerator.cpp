#include "VXPrimaryGenerator.hpp"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"





PrimaryGenerator::PrimaryGenerator(G4ThreeVector gunPosition, G4ThreeVector gunMomentum, G4double gunEnergy, G4String particleName) {
	fParticleGun = new G4ParticleGun(1);

	fParticleGun->SetParticlePosition(gunPosition);
	fParticleGun->SetParticleMomentumDirection(gunMomentum);
	fParticleGun->SetParticleEnergy(gunEnergy);
	fParticleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle(particleName));
}





PrimaryGenerator::~PrimaryGenerator() {
	delete fParticleGun;
}





void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
	fParticleGun->GeneratePrimaryVertex(anEvent);
}
