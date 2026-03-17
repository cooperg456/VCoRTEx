#include "VXDetectorConstruction.hpp"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4SDManager.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SubtractionSolid.hh"





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


	const G4double barLength 		= 32.0 	* cm;
	const G4double barWidth 		= 4.00 	* cm;
	const G4double barHeight 		= 1.00 	* cm;
	const G4double coatThickness 	= 0.25 	* mm;
	const G4double scindWidth 		= barWidth - coatThickness;
	const G4double scintHeight 		= barHeight - coatThickness;
	const G4double cornerRadius 	= 2.50 	* mm;
	const G4double mirrorThickness	= 0.03	* mm;
	const G4double channelWidth		= 2.50	* mm;
	const G4double channelHeight	= 4.00	* mm;
	const G4double fiberDiameter	= 1.00	* mm;
	const G4double cladThickness	= 0.03	* mm;
	const G4double fiberStickout	= 10.0	* mm;
	const G4double sipmSidelength	= 2.00	* mm;
	const G4double sipmThickness	= 1.35	* mm;





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
	G4MaterialPropertiesTable *mptPMMA = new G4MaterialPropertiesTable();

	G4double rindexPMMA[2] = {1.59, 1.59};

	mptPMMA->AddProperty("RINDEX", smallArrayEnergy, rindexPMMA, 2);

	PMMA->SetMaterialPropertiesTable(mptPMMA);

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
	G4Box *solidWorld 					= new G4Box("solidWorld", 0.1 * (0.5 * m), 0.1 * (0.5 * m), 0.5 * (0.5 * m));

	G4LogicalVolume *logicWorld 		= new G4LogicalVolume(solidWorld, airMat, "logicWorld");
	G4VPhysicalVolume *physWorld 		= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);

	//	scintillator coating
	G4Box *solidCoat 					= new G4Box("solidCoat", barWidth/ 2, barHeight / 2, barLength / 2);

	G4LogicalVolume *logicCoat 			= new G4LogicalVolume(solidCoat, coatingMat, "logicCoat");
	G4VPhysicalVolume *physCoat 		= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicCoat, "physCoat", logicWorld, false, 0, checkOverlaps);

	//	scintillator bar
	G4Box *solidScintCenter				= new G4Box("solidScintCenter", scindWidth / 2 - cornerRadius, scintHeight / 2, barLength / 2);
	G4Box *solidScintSide				= new G4Box("solidScintSide", cornerRadius / 2, scintHeight / 2 - cornerRadius, barLength / 2);
	G4Tubs *solidScintCorner			= new G4Tubs("solidScintCorner", 0., cornerRadius, barLength / 2, 0. * deg, 90. * deg);

	G4LogicalVolume *logicScintCenter 	= new G4LogicalVolume(solidScintCenter, scintMat, "logicScintCenter");
	G4VPhysicalVolume *physScintCenter	= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicScintCenter, "physScintCenter", logicCoat, false, 0, checkOverlaps);

	G4LogicalVolume *logicScintSideR 	= new G4LogicalVolume(solidScintSide, scintMat, "logicScintSideR");
	G4VPhysicalVolume *physScintSideR	= new G4PVPlacement(0, G4ThreeVector((scindWidth - cornerRadius) / 2, 0., 0.), logicScintSideR, "physScintSideR", logicCoat, false, 0, checkOverlaps);

	G4LogicalVolume *logicScintSideL 	= new G4LogicalVolume(solidScintSide, scintMat, "logicScintSideL");
	G4VPhysicalVolume *physScintSideL	= new G4PVPlacement(0, G4ThreeVector(- (scindWidth - cornerRadius) / 2, 0., 0.), logicScintSideL, "physScintSideL", logicCoat, false, 0, checkOverlaps);

	G4LogicalVolume *logicScintCorner1	= new G4LogicalVolume(solidScintCorner, scintMat, "logicScintCorner1");
	G4VPhysicalVolume *physScintCorner1	= new G4PVPlacement(0, G4ThreeVector(scindWidth / 2 - cornerRadius, scintHeight / 2 - cornerRadius, 0.), logicScintCorner1, "physScintCorner1", logicCoat, false, 0, checkOverlaps);

	auto rot1 = new G4RotationMatrix();
	rot1->rotateZ(90. * deg);
	G4LogicalVolume *logicScintCorner2	= new G4LogicalVolume(solidScintCorner, scintMat, "logicScintCorner2");
	G4VPhysicalVolume *physScintCorner2	= new G4PVPlacement(rot1, G4ThreeVector(scindWidth / 2 - cornerRadius, -(scintHeight / 2 - cornerRadius), 0.), logicScintCorner2, "physScintCorner2", logicCoat, false, 0, checkOverlaps);

	auto rot2 = new G4RotationMatrix();
	rot2->rotateZ(180. * deg);
	G4LogicalVolume *logicScintCorner3	= new G4LogicalVolume(solidScintCorner, scintMat, "logicScintCorner3");
	G4VPhysicalVolume *physScintCorner3	= new G4PVPlacement(rot2, G4ThreeVector(-(scindWidth / 2 - cornerRadius), -(scintHeight / 2 - cornerRadius), 0.), logicScintCorner3, "physScintCorner3", logicCoat, false, 0, checkOverlaps);

	auto rot3 = new G4RotationMatrix();
	rot3->rotateZ(-90. * deg);
	G4LogicalVolume *logicScintCorner4	= new G4LogicalVolume(solidScintCorner, scintMat, "logicScintCorner4");
	G4VPhysicalVolume *physScintCorner4	= new G4PVPlacement(rot3, G4ThreeVector(-(scindWidth / 2 - cornerRadius), scintHeight / 2 - cornerRadius, 0.), logicScintCorner4, "physScintCorner4", logicCoat, false, 0, checkOverlaps);

	//	scintillator channel
	G4Box *solidChannelCore				= new G4Box("solidChannelCore", channelWidth / 2, (channelHeight - channelWidth) / 2, barLength / 2);
	G4Tubs *solidChannelSide			= new G4Tubs("solidChannelSide", 0., channelWidth / 2, barLength / 2, 0., 180. * deg);

	G4LogicalVolume *logicChannelCore 	= new G4LogicalVolume(solidChannelCore, airMat, "logicChannelCore");
	G4VPhysicalVolume *physChannelCore	= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicChannelCore, "physChannelCore", logicScintCenter, false, 0, checkOverlaps);

	G4LogicalVolume *logicChannelSideT 	= new G4LogicalVolume(solidChannelSide, airMat, "logicChannelSideT");
	G4VPhysicalVolume *physChannelSideT	= new G4PVPlacement(0, G4ThreeVector(0., (channelHeight - channelWidth) / 2, 0.), logicChannelSideT, "physChannelSideT", logicScintCenter, false, 0, checkOverlaps);

	G4LogicalVolume *logicChannelSideB 	= new G4LogicalVolume(solidChannelSide, airMat, "logicChannelSideB");
	G4VPhysicalVolume *physChannelSideB	= new G4PVPlacement(rot2, G4ThreeVector(0., -(channelHeight - channelWidth) / 2, 0.), logicChannelSideB, "physChannelSideB", logicScintCenter, false, 0, checkOverlaps);

	//	optical fiber
	G4Tubs *solidFiberClad 				= new G4Tubs("solidFiberClad", 0., fiberDiameter / 2, barLength / 2, 0., 360. * deg);
	G4Tubs *solidFiberCore 				= new G4Tubs("solidFiberCore", 0., (fiberDiameter - cladThickness) / 2, barLength / 2, 0., 360. * deg);
	G4Tubs *solidFiberCladShort			= new G4Tubs("solidFiberCladShort", 0., fiberDiameter / 2, fiberStickout / 2, 0., 360. * deg);
	G4Tubs *solidFiberCoreShort 		= new G4Tubs("solidFiberCoreShort", 0., (fiberDiameter - cladThickness) / 2, fiberStickout / 2, 0., 360. * deg);

	G4LogicalVolume *logicFiberClad 	= new G4LogicalVolume(solidFiberClad, PMMA, "logicFiberClad");
	G4VPhysicalVolume *physFiberClad 	= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicFiberClad, "physFiberClad", logicChannelCore, false, 0, checkOverlaps);

	G4LogicalVolume *logicFiberCore 	= new G4LogicalVolume(solidFiberCore, fiberCoreMat, "logicFiberCore");
	G4VPhysicalVolume *physFiberCore 	= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicFiberCore, "physFiberCore", logicFiberClad, false, 0, checkOverlaps);

	G4LogicalVolume *logicFiberClad2 	= new G4LogicalVolume(solidFiberCladShort, PMMA, "logicFiberClad2");
	G4VPhysicalVolume *physFiberClad2 	= new G4PVPlacement(0, G4ThreeVector(0., 0., (barLength + fiberStickout) / 2), logicFiberClad2, "physFiberClad2", logicWorld, false, 0, checkOverlaps);

	G4LogicalVolume *logicFiberCore2 	= new G4LogicalVolume(solidFiberCoreShort, fiberCoreMat, "logicFiberCore2");
	G4VPhysicalVolume *physFiberCore2 	= new G4PVPlacement(0, G4ThreeVector(0., 0., (barLength + fiberStickout) / 2), logicFiberCore2, "physFiberCore2", logicWorld, false, 0, checkOverlaps);
	
	//	back reflector
	G4Box *solidMirror1 				= new G4Box("solidMirror1", barWidth / 2, barHeight / 2, mirrorThickness / 2);
	G4SubtractionSolid *solidMirror2__ 	= new G4SubtractionSolid("solidMirror2__", solidMirror1, solidChannelCore);
	G4SubtractionSolid *solidMirror2_ 	= new G4SubtractionSolid("solidMirror2_", solidMirror2__, solidChannelSide, 0, G4ThreeVector(0., (channelHeight - channelWidth) / 2, 0.));
	G4SubtractionSolid *solidMirror2 	= new G4SubtractionSolid("solidMirror2", solidMirror2_, solidChannelSide, rot2, G4ThreeVector(0., -(channelHeight - channelWidth) / 2, 0.));

	G4LogicalVolume *logicMirror1 		= new G4LogicalVolume(solidMirror1, Al, "logicMirror1");
	G4VPhysicalVolume *physMirror1 		= new G4PVPlacement(0, G4ThreeVector(0., 0., -(barLength + mirrorThickness) / 2), logicMirror1, "physMirror1", logicWorld, false, 0, checkOverlaps);

	G4LogicalVolume *logicMirror2		= new G4LogicalVolume(solidMirror2, Al, "logicMirror2");
	G4VPhysicalVolume *physMirror2 		= new G4PVPlacement(0, G4ThreeVector(0., 0., (barLength + mirrorThickness) / 2), logicMirror2, "physMirror2", logicWorld, false, 0, checkOverlaps);

	//	photon detector
	G4Box *solidSiPM					= new G4Box("solidChannelCore", sipmSidelength / 2, sipmSidelength / 2, sipmThickness / 2);

	G4LogicalVolume *logicSiPM 			= new G4LogicalVolume(solidSiPM, Si, "logicSiPM");
	G4VPhysicalVolume *physSiPM 		= new G4PVPlacement(0, G4ThreeVector(0., 0., (barLength + sipmThickness) / 2 + fiberStickout), logicSiPM, "physSiPM", logicWorld, false, 0, checkOverlaps);




	/*
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
	*/




	return physWorld;
}





void DetectorConstruction::ConstructSDandField() {

}
