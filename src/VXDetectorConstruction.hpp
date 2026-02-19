#pragma once

#include "G4VUserDetectorConstruction.hh"


class DetectorConstruction : public G4VUserDetectorConstruction {
public:
	DetectorConstruction() = default;
	~DetectorConstruction() override = default;

	virtual G4VPhysicalVolume *Construct();
};