#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"





class PrimaryGenerator : public G4VUserPrimaryGeneratorAction {
public:
	PrimaryGenerator();
	~PrimaryGenerator();

	virtual void GeneratePrimaries(G4Event *);

private:
	G4ParticleGun *fParticleGun;
};
