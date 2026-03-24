#pragma once

#include "G4VUserDetectorConstruction.hh"





class DetectorConstruction : public G4VUserDetectorConstruction {
public:
	DetectorConstruction(G4String);
	~DetectorConstruction() override = default;

	virtual G4VPhysicalVolume *Construct() override;

private:
	G4String outFile;

	G4LogicalVolume *logicSiPM;

	virtual void ConstructSDandField() override;
};
