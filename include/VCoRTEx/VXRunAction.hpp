#pragma once

#include "G4UserRunAction.hh"
#include "G4Run.hh"

#include "TFile.h"
#include "TTree.h"

#include <mutex>





class RunAction : public G4UserRunAction {
public:
	RunAction();
	~RunAction() override;

	virtual void BeginOfRunAction(const G4Run *) override;
	virtual void EndOfRunAction(const G4Run *) override;

	void FillHit(G4double, G4double, G4double, G4double, G4int, G4int);

private:
	static TFile *fFile;
	static TTree *fTree;
	static std::mutex fMutex;

	static G4double fX;
	static G4double fY;
	static G4double fEnergy;
	static G4double fTime;
	static G4int fEventID;
	static G4int fDetID;
};