#pragma once

#include "G4VUserDetectorConstruction.hh"





class DetectorConstruction : public G4VUserDetectorConstruction {
public:
	DetectorConstruction(int);
	~DetectorConstruction() override = default;

	virtual G4VPhysicalVolume *Construct() override;

private:
	int cfg;
	G4LogicalVolume *logicSiPM;
	G4LogicalVolume *logicScintCenter;
	G4LogicalVolume *logicScintSide;
	G4LogicalVolume *logicScintCorner;

	virtual void ConstructSDandField() override;
};
