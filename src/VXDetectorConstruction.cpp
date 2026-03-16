#include "VXDetectorConstruction.hpp"

G4VPhysicalVolume *DetectorConstruction::Construct() {

	G4bool checkOverlaps = true;

	G4double hc = 1.239841939 * eV;	//	hc/μm

	G4double smallArrayEnergy[2] = {
		hc / 0.7,	//	700nm
		hc / 0.3	//	300nm
	};

	G4double bigArrayEnergy[50] = {
		hc / 0.700, hc / 0.692, hc / 0.684, hc / 0.676, hc / 0.667, 
		hc / 0.659, hc / 0.651, hc / 0.643, hc / 0.635, hc / 0.627, 
		hc / 0.618, hc / 0.610, hc / 0.602, hc / 0.594, hc / 0.586, 
		hc / 0.578, hc / 0.569, hc / 0.561, hc / 0.553, hc / 0.545, 
		hc / 0.537, hc / 0.529, hc / 0.520, hc / 0.512, hc / 0.504, 
		hc / 0.496, hc / 0.488, hc / 0.480, hc / 0.471, hc / 0.463, 
		hc / 0.455, hc / 0.447, hc / 0.439, hc / 0.431, hc / 0.422, 
		hc / 0.414, hc / 0.406, hc / 0.398, hc / 0.390, hc / 0.382, 
		hc / 0.373, hc / 0.365, hc / 0.357, hc / 0.349, hc / 0.341, 
		hc / 0.333, hc / 0.324, hc / 0.316, hc / 0.308, hc / 0.300
	};





	//-----------------------------------------------------------------------------
	//	dimensions
	//-----------------------------------------------------------------------------

	const G4double scintLength 		= 32.0 	* cm;
	const G4double scindWidth 		= 4.00 	* cm;
	const G4double scintHeight 		= 1.00 	* cm;
	const G4double coatThickness 	= 0.10 	* mm;
	const G4double cornerRadius 	= 2.00 	* mm;
	const G4double mirrorThickness	= 0.30	* mm;
	const G4double channelWidth		= 2.00	* mm;
	const G4double channelHeight	= 3.00	* mm;
	const G4double fiberdiameter	= 1.00	* mm;
	const G4double cladThickness	= 0.30	* mm;
	const G4double fiberStickout	= 10.0	* mm;
	const G4double sipmSidelength	= 2.00	* mm;





	//-----------------------------------------------------------------------------
	//	materials
	//-----------------------------------------------------------------------------

	G4NistManager *nist = G4NistManager::Instance();

	//	air (world)
	G4Material *airMat = nist->FindOrBuildMaterial("G4_AIR");
	G4MaterialPropertiesTable *mptAir = new G4MaterialPropertiesTable();

	mptAir->AddProperty("RINDEX", "Air");

	airMat->SetMaterialPropertiesTable(mptAir);

	//	Al (reflectors)
	G4Material *Al = nist->FindOrBuildMaterial("G4_Al");

	//	Si (silicon photomultiplier)
	G4Material *Si = nist->FindOrBuildMaterial("G4_Si");

	//	polystyrene scintillator
	G4Material *scintMat = new G4Material("VX_POLYSTYRENE_SCINTILLATOR", 1.06 * g / cm3, 1);
	scintMat->AddMaterial(nist->FindOrBuildMaterial("G4_POLYSTYRENE"), 1.);
	G4MaterialPropertiesTable *mptScint = new G4MaterialPropertiesTable();

	G4double rindexScint[2] = {1.6, 1.6};
	G4double absLengthScint[2] = {4 * m, 4 * m};
	G4double emissionScint[50] = {
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0
	};

	mptScint->AddProperty("RINDEX", smallArrayEnergy, rindexScint, 2);
	mptScint->AddProperty("ABSLENGTH", smallArrayEnergy, absLengthScint, 2);
	mptScint->AddProperty("SCINTILLATIONCOMPONENT1", bigArrayEnergy, emissionScint, 50);
	mptScint->AddConstProperty("SCINTILLATIONYIELD", 10. / keV);
	mptScint->AddConstProperty("RESOLUTIONSCALE", 1.0);
	mptScint->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 10. * ns);

	scintMat->SetMaterialPropertiesTable(mptScint);

	scintMat->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

	//	TiO2 extrusion (scintillator coating)
  	G4Material *coatingMat = new G4Material("Coating", 1.52 * g / cm3, 2);
	coatingMat->AddMaterial(nist->FindOrBuildMaterial("G4_TITANIUM_DIOXIDE"), 15 * perCent);
	coatingMat->AddMaterial(nist->FindOrBuildMaterial("G4_POLYSTYRENE"), 85 * perCent);

	//	PMMA (wls fiber cladding)
	G4Material* PMMA = new G4Material("PMMA", 1.19 * g / cm3, 3);
	PMMA->AddElement(nist->FindOrBuildElement("C"), 5);
	PMMA->AddElement(nist->FindOrBuildElement("H"), 8);
	PMMA->AddElement(nist->FindOrBuildElement("O"), 2);

	// wls polystyrene (wls fiber core)
	G4Material *fiberCoreMat = new G4Material("VX_WLS_POLYSTYRENE", 1.05 * g / cm3, 1);
	fiberCoreMat->AddMaterial(nist->FindOrBuildMaterial("G4_POLYSTYRENE"), 1.);
	G4MaterialPropertiesTable *mptFiberCore = new G4MaterialPropertiesTable();

	G4double rindexFiber[2] = {1.59, 1.59};
	G4double absLengthFiber[2] = {3.5 * m, 3.5 * m};
	G4double absFiberCore[50] = {
		5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m,
		5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m,
		5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m,
		5.40 * m, 5.40 * m, 1.10 * m, 1.10 * m, 1.10 * m, 1.10 * m, 1.10 * m, 1.10 * m, 1.10 * m,
		1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,
		1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm
	};
	G4double emissionFiberCore[50] = {
		0.05, 0.10, 0.30, 0.50, 0.75, 1.00, 1.50, 1.85, 2.30, 2.75,
        3.25, 3.80, 4.50, 5.20, 6.00, 7.00, 8.50, 9.50, 11.1, 12.4,
		12.9, 13.0, 12.8, 12.3, 11.1, 11.0, 12.0, 11.0, 17.0, 16.9,
		15.0, 9.00, 2.50, 1.00, 0.05, 0.00, 0.00, 0.00, 0.00, 0.00,
		0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00
	};

	mptFiberCore->AddProperty("RINDEX", smallArrayEnergy, rindexFiber, 2);
	mptFiberCore->AddProperty("WLSABSLENGTH", bigArrayEnergy, absFiberCore, 50);
	mptFiberCore->AddProperty("WLSCOMPONENT", bigArrayEnergy, emissionFiberCore, 50);
	mptFiberCore->AddConstProperty("WLSTIMECONSTANT", 0.5 * ns);

	fiberCoreMat->SetMaterialPropertiesTable(mptFiberCore);





	//-----------------------------------------------------------------------------
	//	detector construction
	//-----------------------------------------------------------------------------

	//	worldbox
	G4Box *solidWorld = new G4Box("solidWorld", 0.1 * (0.5 * m), 0.1 * (0.5 * m), 0.5 * (0.5 * m));

	G4LogicalVolume *logicWorld 	= new G4LogicalVolume(solidWorld, airMat, "logicWorld");
	G4VPhysicalVolume *physWorld 	= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);

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
	G4ExtrudedSolid *solidChannel 	= new G4ExtrudedSolid("solidChannel", channel, 0.33 * (0.5 * m), G4TwoVector(0, 0), 1.0, G4TwoVector(0, 0), 1.0);
	G4SubtractionSolid *solidScint 	= new G4SubtractionSolid("solidScint", solidBar, solidChannel);

	G4LogicalVolume *logicScint 	= new G4LogicalVolume(solidScint, scintMat, "logicScint");
	G4VPhysicalVolume *physScint 	= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicScint, "physScint", logicWorld, false, 0, checkOverlaps);

	//	coating
	G4Box *solidCoat_ = new G4Box("solidCoat_", 0.0402 * (0.5 * m), 0.0102 * (0.5 * m), 0.32 * (0.5 * m));
	G4SubtractionSolid *solidCoat = new G4SubtractionSolid("solidCoat", solidCoat_, solidBar);

	G4LogicalVolume *logicCoat 		= new G4LogicalVolume(solidCoat, coatingMat, "logicCoat");
	G4VPhysicalVolume *physCoat 	= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicCoat, "physCoat", logicWorld, false, 0, checkOverlaps);

	//	reflectors
	G4Box *solidRef1 = new G4Box("solidRef1", 0.0402 * (0.5 * m), 0.0102 * (0.5 * m), 0.00015 * (0.5 * m));

	G4LogicalVolume *logicRef1 		= new G4LogicalVolume(solidRef1, Al, "logicRef1");
	G4VPhysicalVolume *physRef1 	= new G4PVPlacement(0, G4ThreeVector(0., 0., -0.16015 * m), logicRef1, "physRef1", logicWorld, false, 0, checkOverlaps);

	G4Box *solidRef2_ = new G4Box("solidRef2_", 0.0402 * (0.5 * m), 0.0102 * (0.5 * m), 0.00015 * (0.5 * m));
	G4SubtractionSolid *solidRef2 	= new G4SubtractionSolid("solidRef2", solidRef2_, solidBar);
	
	G4LogicalVolume *logicRef2 		= new G4LogicalVolume(solidRef2, Al, "logicRef2");
	G4VPhysicalVolume *physRef2 	= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.16015 * m), logicRef1, "physRef2", logicWorld, false, 0, checkOverlaps);

	//	optical fiber
	G4Tubs *solidFiber = new G4Tubs("solidFiber", 0., 0.001 * (0.5 * m), 0.33 * (0.5 * m), 0., 360. * deg);

	G4LogicalVolume *logicFiber 	= new G4LogicalVolume(solidFiber, fiberCoreMat, "logicFiber");
	G4VPhysicalVolume *physFiber 	= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.005 * m), logicFiber, "physFiber", logicWorld, false, 0, checkOverlaps);





	//-----------------------------------------------------------------------------
	//	optical surfaces
	//-----------------------------------------------------------------------------

	//	scintillator - coating
	G4OpticalSurface *scintCoatSurface = new G4OpticalSurface("scintCoatSurface");
	scintCoatSurface->SetType(dielectric_metal);
	scintCoatSurface->SetFinish(groundfrontpainted);
	scintCoatSurface->SetModel(unified);

	G4MaterialPropertiesTable *mptCoat = new G4MaterialPropertiesTable();
	G4double reflectivityCoat[2] = {0.95, 0.95};
	mptCoat->AddProperty("REFLECTIVITY", smallArrayEnergy, reflectivityCoat, 2);
	scintCoatSurface->SetMaterialPropertiesTable(mptCoat);

	new G4LogicalBorderSurface("scintCoatSurf", physScint, physCoat, scintCoatSurface);

	//	scintillator - reflector
	G4OpticalSurface *scintRefSurface = new G4OpticalSurface("scintRefSurface");
	scintRefSurface->SetType(dielectric_metal);
	scintRefSurface->SetFinish(polished);
	scintRefSurface->SetModel(unified);

	G4MaterialPropertiesTable *mptRef = new G4MaterialPropertiesTable();
	G4double reflectivityRef[2] = {0.95, 0.95};
	mptRef->AddProperty("REFLECTIVITY", smallArrayEnergy, reflectivityRef, 2);
	scintRefSurface->SetMaterialPropertiesTable(mptRef);





	return physWorld;
}





void DetectorConstruction::ConstructSDandField() {

}
