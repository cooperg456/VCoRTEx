#include "VXDetectorConstruction.hpp"

G4VPhysicalVolume *DetectorConstruction::Construct() {
	G4bool checkOverlaps = true;
	G4double energy[2] = {
		1.239841939 * eV / 0.9, //	900nm
		1.239841939 * eV / 0.2	//	200nm
	};





	//    materials
	G4NistManager *nist = G4NistManager::Instance();

	//	air
	G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
	G4MaterialPropertiesTable *mptAir = new G4MaterialPropertiesTable();
	G4double rindexAir[2] = {1.0, 1.0};
	mptAir->AddProperty("RINDEX", energy, rindexAir, 2);
	worldMat->SetMaterialPropertiesTable(mptAir);

	//	scintillator (BC-408)
	G4Material *scintMat = nist->BuildMaterialWithNewDensity("BC_408", "G4_PLASTIC_SC_VINYLTOLUENE", 1.023 * g / cm3);
	G4MaterialPropertiesTable *mptScint = new G4MaterialPropertiesTable();
	G4double rindexScint[2] = {1.58, 1.58};
	G4double fractionScint[2] = {1.0, 1.0};
	G4double absLengthScint[2] = {3.8*m, 3.8*m};
	mptScint->AddProperty("RINDEX", energy, rindexScint, 2);
	mptScint->AddProperty("ABSLENGTH", energy, absLengthScint, 2);
	mptScint->AddProperty("FASTCOMPONENT", energy, fractionScint, 2, true);
	mptScint->AddConstProperty("SCINTILLATIONYIELD", 100. / MeV);
	mptScint->AddConstProperty("RESOLUTIONSCALE", 1.);
	mptScint->AddConstProperty("FASTTIMECONSTANT", 2.1 * ns, true);
	mptScint->AddConstProperty("YIELDRATIO", 1., true);
	scintMat->SetMaterialPropertiesTable(mptScint);

	//	TiO2 coating (EJ-510)
	G4Element* elTi	= new G4Element("Titanium", "Ti", 22., 47.867 * g / mole);
	G4Element* elC 	= new G4Element("Carbon", "C", 6., 12.011 * g / mole);
	G4Element* elH 	= new G4Element("Hydrogen", "H", 1., 1.0078 * g / mole);
	G4Element* elO 	= new G4Element("Oxygen", "O", 8., 15.999 * g / mole);
	G4Material* TiO2_CoatingMat = new G4Material("TiO2_Coating", 13 * mg / cm3, 4);
	TiO2_CoatingMat->AddElement(elTi, 11.294394883 * perCent);
	TiO2_CoatingMat->AddElement(elC, 18.8520451103 * perCent);
	TiO2_CoatingMat->AddElement(elH, 37.8724120518 * perCent);
	TiO2_CoatingMat->AddElement(elO, 31.9811479549 * perCent);
	G4MaterialPropertiesTable *mptTiO2_Coating = new G4MaterialPropertiesTable();

		//	optical properties ...

	TiO2_CoatingMat->SetMaterialPropertiesTable(mptTiO2_Coating);





	//	worldbox
	G4Box *solidWorld = new G4Box("solidWorld", 0.5 * 0.1 * m, 0.5 * 0.1 * m, 0.5 * 0.5 * m);
	G4LogicalVolume *logicWorld 	= new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
	G4VPhysicalVolume *physWorld 	= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);

	//	detector mesh
	auto meshScint = CADMesh::TessellatedMesh::FromOBJ("../assets/g4_scint_assmb.obj");
	//	wls fiber
	//G4LogicalVolume *logicFiber 	= new G4LogicalVolume(meshScint->GetSolid("optical_fiber"), scintMat, "logicFiber");
	//G4VPhysicalVolume *physFiber 	= new G4PVPlacement(G4RotateX3D(-90. * deg), logicFiber, "physFiber", logicWorld, false, 0, checkOverlaps);
	//	scintillator
	G4LogicalVolume *logicScint 	= new G4LogicalVolume(meshScint->GetSolid("scint_body"), scintMat, "logicScint");
	G4VPhysicalVolume *physScint 	= new G4PVPlacement(G4RotateX3D(-90. * deg), logicScint, "physScint", logicWorld, false, 0, checkOverlaps);
	//	TiO2 coating
	//G4LogicalVolume *logicCoating 	= new G4LogicalVolume(meshScint->GetSolid("scint_coating"), TiO2_CoatingMat, "logicCoating");
	//G4VPhysicalVolume *physCoating 	= new G4PVPlacement(G4RotateX3D(-90. * deg), logicCoating, "physCoating", logicWorld, false, 0, checkOverlaps);
	//	Al reflectors
	//G4LogicalVolume *logicReflect1 	= new G4LogicalVolume(meshScint->GetSolid("reflector_1"), scintMat, "logicReflect1");
	//G4VPhysicalVolume *physReflect1 = new G4PVPlacement(G4RotateX3D(-90. * deg), logicReflect1, "physReflect1", logicWorld, false, 0, checkOverlaps);
	//G4LogicalVolume *logicReflect2 	= new G4LogicalVolume(meshScint->GetSolid("reflector_2"), scintMat, "logicReflect2");
	//G4VPhysicalVolume *physReflect2 = new G4PVPlacement(G4RotateX3D(-90. * deg), logicReflect2, "physReflect2", logicWorld, false, 0, checkOverlaps);
	//	3Dp housings
	//G4LogicalVolume *logicHous1 	= new G4LogicalVolume(meshScint->GetSolid("led_housing"), scintMat, "logicHous1");
	//G4VPhysicalVolume *physHous1 	= new G4PVPlacement(G4RotateX3D(-90. * deg), logicHous1, "physHous1", logicWorld, false, 0, checkOverlaps);
	//G4LogicalVolume *logicHous2 	= new G4LogicalVolume(meshScint->GetSolid("sipm_housing"), scintMat, "logicHous2");
	//G4VPhysicalVolume *physHous2 	= new G4PVPlacement(G4RotateX3D(-90. * deg), logicHous2, "physHous2", logicWorld, false, 0, checkOverlaps);
	//	SiPM detector
	//G4LogicalVolume *logicSipm 		= new G4LogicalVolume(meshScint->GetSolid("sipm"), scintMat, "logicSipm");
	//G4VPhysicalVolume *physSipm 	= new G4PVPlacement(G4RotateX3D(-90. * deg), logicSipm, "physSipm", logicWorld, false, 0, checkOverlaps);
	




	
	return physWorld;
}



void DetectorConstruction::ConstructSDandField() {

}