#pragma once

//	user defined
#include "VXSensitiveDetector.hpp"
//	geant4
#include "G4VUserDetectorConstruction.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
//	external
#include "CADMesh.hh"


class DetectorConstruction : public G4VUserDetectorConstruction {
public:
	DetectorConstruction() = default;
	~DetectorConstruction() override = default;

	virtual G4VPhysicalVolume *Construct() override;

private:
	G4LogicalVolume *logicDetector;

	virtual void ConstructSDandField() override;
};