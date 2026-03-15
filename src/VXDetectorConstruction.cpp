#include "VXDetectorConstruction.hpp"

G4VPhysicalVolume *DetectorConstruction::Construct() {

	G4bool checkOverlaps = true;
	
	G4double energy[2] = {
		1.239841939 * eV / 0.9, //	900nm
		1.239841939 * eV / 0.2	//	200nm
	};



	//-----------------------------------------------------------------------------
	//	materials
	//-----------------------------------------------------------------------------
	G4NistManager *nist = G4NistManager::Instance();

	//	air
	G4Material *airMat = nist->FindOrBuildMaterial("G4_AIR");
	G4MaterialPropertiesTable *mptAir = new G4MaterialPropertiesTable();
	G4double rindexAir[2] = {1.0, 1.0};
	mptAir->AddProperty("RINDEX", energy, rindexAir, 2);
	airMat->SetMaterialPropertiesTable(mptAir);

	//	Al reflector
	G4Material *alMat = nist->FindOrBuildMaterial("G4_Al");
	G4MaterialPropertiesTable *mptAl = new G4MaterialPropertiesTable();
	G4double rindexAl[2] = {1.2, 1.2};
	mptAl->AddProperty("RINDEX", energy, rindexAl, 2);

		//	TODO: update optical properties

	alMat->SetMaterialPropertiesTable(mptAl);

	//	scintillator (BC-408)
	G4Material *BC408Mat = nist->BuildMaterialWithNewDensity("BC_408", "G4_PLASTIC_SC_VINYLTOLUENE", 1.023 * g / cm3);
	G4MaterialPropertiesTable *mptBC408 = new G4MaterialPropertiesTable();
	G4double rindexBC408[2] = {1.58, 1.58};
	G4double fractionBC408[2] = {1.0, 1.0};
	G4double absLengthBC408[2] = {3.8*m, 3.8*m};
	mptBC408->AddProperty("RINDEX", energy, rindexBC408, 2);
	mptBC408->AddProperty("ABSLENGTH", energy, absLengthBC408, 2);
	mptBC408->AddProperty("FASTCOMPONENT", energy, fractionBC408, 2, true);
	mptBC408->AddConstProperty("SCINTILLATIONYIELD", 10. / keV);
	mptBC408->AddConstProperty("RESOLUTIONSCALE", 1.);
	mptBC408->AddConstProperty("FASTTIMECONSTANT", 2.1 * ns, true);
	mptBC408->AddConstProperty("YIELDRATIO", 1., true);

		//	TODO: update optical properties

	BC408Mat->SetMaterialPropertiesTable(mptBC408);

	//	TiO2 coating (EJ-510)
	G4Element* elTi	= new G4Element("Titanium", "Ti", 22., 47.867 * g / mole);
	G4Element* elC 	= new G4Element("Carbon", "C", 6., 12.011 * g / mole);
	G4Element* elH 	= new G4Element("Hydrogen", "H", 1., 1.0078 * g / mole);
	G4Element* elO 	= new G4Element("Oxygen", "O", 8., 15.999 * g / mole);
	G4Material* TiO2Mat = new G4Material("TiO2", 13 * mg / cm3, 4);
	TiO2Mat->AddElement(elTi, 11.294394883 * perCent);
	TiO2Mat->AddElement(elC, 18.8520451103 * perCent);
	TiO2Mat->AddElement(elH, 37.8724120518 * perCent);
	TiO2Mat->AddElement(elO, 31.9811479549 * perCent);
	G4MaterialPropertiesTable *mptTiO2 = new G4MaterialPropertiesTable();

		//	TODO: set optical properties

	TiO2Mat->SetMaterialPropertiesTable(mptTiO2);



	//-----------------------------------------------------------------------------
	//	detector construction
	//-----------------------------------------------------------------------------

	//	worldbox
	G4Box *solidWorld = new G4Box("solidWorld", 0.1 * (0.5 * m), 0.1 * (0.5 * m), 0.5 * (0.5 * m));

	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, airMat, "logicWorld");
	G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);

	//	scintillator
	G4Box *solidBar = new G4Box("solidBar", 0.04 * (0.5 * m), 0.01 * (0.5 * m), 0.32 * (0.5 * m));
	std::vector<G4TwoVector> channel;
	for (int i = 0; i <= 32; i++) {
		G4double theta = M_PI * i / 32;
		channel.push_back(G4TwoVector(0.002 * (0.5 * m) * cos(theta), 0.002 * (0.5 * m) + 0.002 * (0.5 * m) * sin(theta)));
	}
	for (int i = 0; i <= 32; i++) {
		G4double theta = M_PI + M_PI * i / 32;
		channel.push_back(G4TwoVector(0.002 * (0.5 * m) * cos(theta), -0.002 * (0.5 * m) + 0.002 * (0.5 * m) * sin(theta)));
	}
	G4ExtrudedSolid *solidChannel = new G4ExtrudedSolid("solidChannel", channel, 0.33 * (0.5 * m), G4TwoVector(0, 0), 1.0, G4TwoVector(0, 0), 1.0);
	G4SubtractionSolid *solidScint = new G4SubtractionSolid("solidScint", solidBar, solidChannel);

	G4LogicalVolume *logicScint = new G4LogicalVolume(solidScint, BC408Mat, "logicScint");
	G4VPhysicalVolume *physScint = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicScint, "physScint", logicWorld, false, 0, checkOverlaps);

	//	coating
	G4Box *solidCoat_ = new G4Box("solidCoat_", 0.0402 * (0.5 * m), 0.0102 * (0.5 * m), 0.32 * (0.5 * m));
	G4SubtractionSolid *solidCoat = new G4SubtractionSolid("solidCoat", solidCoat_, solidBar);

	G4LogicalVolume *logicCoat = new G4LogicalVolume(solidCoat, TiO2Mat, "logicCoat");
	G4VPhysicalVolume *physCoat = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicCoat, "physCoat", logicWorld, false, 0, checkOverlaps);



	//-----------------------------------------------------------------------------
	//	detector construction
	//-----------------------------------------------------------------------------

		//	TODO: optical surfaces




	return physWorld;
}



void DetectorConstruction::ConstructSDandField() {

}
