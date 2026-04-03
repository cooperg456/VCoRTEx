#include "VXDetectorConstruction.hpp"
#include "VXSensitiveDetector.hpp"

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
#include "G4VisAttributes.hh"





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
	const G4double assemblyLength	= barLength + 2 * (fiberStickout + sipmThickness);
	const G4double assemblyWidth	= barWidth;
	const G4double assemblyHeight	= barHeight;





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
	G4MaterialPropertiesTable *mptSi = new G4MaterialPropertiesTable();

	G4double rindexSi[2] = {3.88, 3.88};

	mptSi->AddProperty("RINDEX", smallArrayEnergy, rindexSi, 2);

	Si->SetMaterialPropertiesTable(mptSi);

	//	polystyrene scintillator	
	//	TODO: find datasheet reference
	G4Material *scintMat = new G4Material("VX_POLYSTYRENE_SCINTILLATOR", 1.06 * g / cm3, 1);
	scintMat->AddMaterial(nist->FindOrBuildMaterial("G4_POLYSTYRENE"), 1.);
	G4MaterialPropertiesTable *mptScint = new G4MaterialPropertiesTable();

	G4double rindexScint[2] = {1.6, 1.6};
	G4double absLengthScint[2] = {4 * m, 4 * m};
	G4double emissionScint[50] = {
		0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,  0., 
		0., 0., 0., 0., 0., 0., 0., 0.003, 0.015, 0.060, 0.185, 0.432, 0.756, 0.955, 1.000, 
		0.891, 0.648, 0.381, 0.175, 0.060, 0.015, 0.003, 0., 0., 0., 0., 0., 0., 0., 0.
	};

	mptScint->AddProperty("RINDEX", smallArrayEnergy, rindexScint, 2);
	mptScint->AddProperty("ABSLENGTH", smallArrayEnergy, absLengthScint, 2);
	mptScint->AddProperty("SCINTILLATIONCOMPONENT1", bigArrayEnergy, emissionScint, 50);
	mptScint->AddConstProperty("SCINTILLATIONYIELD", 8. / MeV);	//	REVERT BEFORE PUSH
	mptScint->AddConstProperty("RESOLUTIONSCALE", 1.0);
	mptScint->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 10. * ns);

	scintMat->SetMaterialPropertiesTable(mptScint);

	scintMat->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

	//	TiO2 extrusion (scintillator coating)
  	G4Material *coatingMat = new G4Material("Coating", 1.52 * g / cm3, 2);
	coatingMat->AddMaterial(nist->FindOrBuildMaterial("G4_TITANIUM_DIOXIDE"), 15 * perCent);
	coatingMat->AddMaterial(nist->FindOrBuildMaterial("G4_POLYSTYRENE"), 85 * perCent);

	//	PMMA (wls fiber cladding)
	//	REF: https://www.phenix.bnl.gov/WWW/publish/donlynch/RXNP/Safety%20Review%206_22_06/Kuraray-PSF-Y11.pdf
	G4Material* PMMA = new G4Material("PMMA", 1.19 * g / cm3, 3);
	PMMA->AddElement(nist->FindOrBuildElement("C"), 5);
	PMMA->AddElement(nist->FindOrBuildElement("H"), 8);
	PMMA->AddElement(nist->FindOrBuildElement("O"), 2);
	G4MaterialPropertiesTable *mptPMMA = new G4MaterialPropertiesTable();

	G4double rindexPMMA[2] = {1.49, 1.49};

	mptPMMA->AddProperty("RINDEX", smallArrayEnergy, rindexPMMA, 2);

	PMMA->SetMaterialPropertiesTable(mptPMMA);

	// wls polystyrene (wls fiber core)
	// REF: (Y-11) https://www.phenix.bnl.gov/WWW/publish/donlynch/RXNP/Safety%20Review%206_22_06/Kuraray-PSF-Y11.pdf
	G4Material *fiberCoreMat = new G4Material("VX_WLS_POLYSTYRENE", 1.05 * g / cm3, 1);
	fiberCoreMat->AddMaterial(nist->FindOrBuildMaterial("G4_POLYSTYRENE"), 1.);
	G4MaterialPropertiesTable *mptFiberCore = new G4MaterialPropertiesTable();

	G4double rindexFiberCore[2] = {1.59, 1.59};
	G4double bulkAbsLengthFiber[2] = {3.5 * m, 3.5 * m};
	G4double peakAbsLengthFiber = 1. * mm;
	G4double absSpectrumFiberCore[50] = {
		0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 
		0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.008573, 0.065726, 0.404907, 0.828498, 
		0.782608, 0.961917, 0.831293, 0.691549, 0.617250, 0.551525, 0.402927, 0.285764, 
		0.211465, 0.149531, 0.089851, 0.057539, 0.037746, 0., 0., 0., 0., 0.
	};
	G4double absLengthFiberCore[50];
	for (G4int i = 0; i < 50; i++) {
		absLengthFiberCore[i] = (absSpectrumFiberCore[i] > 0.) ? peakAbsLengthFiber / absSpectrumFiberCore[i] : bulkAbsLengthFiber[0];
	}
	G4double emissionFiberCore[50] = {
		0., 0., 0., 0., 0., 0., 0., 0., 0., 0.017589, 0.019470, 0.028345, 0.036557, 0.043630, 
		0.052160, 0.072584, 0.086942, 0.135685, 0.175130, 0.245463, 0.297067, 0.372115, 0.527775, 
		0.740046, 0.783649, 0.705925, 0.789530, 0.983787, 0.785159, 0.310975, 0.058571, 0.016159, 
		0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.
	};

	mptFiberCore->AddProperty("RINDEX", smallArrayEnergy, rindexFiberCore, 2);
	mptFiberCore->AddProperty("ABSLENGTH", smallArrayEnergy, bulkAbsLengthFiber, 2);
	mptFiberCore->AddProperty("WLSABSLENGTH", bigArrayEnergy, absLengthFiberCore, 50);
	mptFiberCore->AddProperty("WLSCOMPONENT", bigArrayEnergy, emissionFiberCore, 50);
	mptFiberCore->AddConstProperty("WLSTIMECONSTANT", 0.5 * ns);

	fiberCoreMat->SetMaterialPropertiesTable(mptFiberCore);





	//-----------------------------------------------------------------------------
	//	detector construction
	//-----------------------------------------------------------------------------

	//	worldbox
	G4Box *solidWorld 					= new G4Box("solidWorld", assemblyLength / 2 + 0.05 * m, assemblyLength / 2 + 0.05 * m, assemblyLength / 2 + 0.05 * m);

	G4LogicalVolume *logicWorld 		= new G4LogicalVolume(solidWorld, airMat, "logicWorld");
	G4VPhysicalVolume *physWorld 		= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);

	//	detector assembly
	G4Box *solidAssembly 				= new G4Box("solidAssembly", assemblyWidth / 2, assemblyHeight / 2, assemblyLength / 2);
	
	G4LogicalVolume *logicAssembly 		= new G4LogicalVolume(solidAssembly, airMat, "logicAssembly");

	//	scintillator coating
	G4Box *solidCoat 					= new G4Box("solidCoat", barWidth / 2, barHeight / 2, barLength / 2);

	G4LogicalVolume *logicCoat 			= new G4LogicalVolume(solidCoat, coatingMat, "logicCoat");
	G4VPhysicalVolume *physCoat 		= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicCoat, "physCoat", logicAssembly, false, 0, checkOverlaps);

	//	scintillator bar
	G4Box *solidScintCenter				= new G4Box("solidScintCenter", scindWidth / 2 - cornerRadius, scintHeight / 2, barLength / 2);
	G4Box *solidScintSide				= new G4Box("solidScintSide", cornerRadius / 2, scintHeight / 2 - cornerRadius, barLength / 2);
	G4Tubs *solidScintCorner			= new G4Tubs("solidScintCorner", 0., cornerRadius, barLength / 2, 0. * deg, 90. * deg);

	G4LogicalVolume *logicScintCenter 	= new G4LogicalVolume(solidScintCenter, scintMat, "logicScintCenter");
	G4VPhysicalVolume *physScintCenter	= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicScintCenter, "physScintCenter", logicCoat, false, 0, checkOverlaps);

	G4LogicalVolume *logicScintSide 	= new G4LogicalVolume(solidScintSide, scintMat, "logicScintSide");
	G4VPhysicalVolume *physScintSideR	= new G4PVPlacement(0, G4ThreeVector( (scindWidth - cornerRadius) / 2, 0., 0.), logicScintSide, "physScintSide", logicCoat, false, 0, checkOverlaps);
	G4VPhysicalVolume *physScintSideL	= new G4PVPlacement(0, G4ThreeVector(-(scindWidth - cornerRadius) / 2, 0., 0.), logicScintSide, "physScintSide", logicCoat, false, 1, checkOverlaps);

	auto rot1 = new G4RotationMatrix();
	rot1->rotateZ(90. * deg);
	auto rot2 = new G4RotationMatrix();
	rot2->rotateZ(180. * deg);
	auto rot3 = new G4RotationMatrix();
	rot3->rotateZ(-90. * deg);

	G4LogicalVolume *logicScintCorner	= new G4LogicalVolume(solidScintCorner, scintMat, "logicScintCorner");
	G4VPhysicalVolume *physScintCorner1	= new G4PVPlacement(0,    G4ThreeVector( (scindWidth / 2 - cornerRadius),  (scintHeight / 2 - cornerRadius), 0.), logicScintCorner, "physScintCorner", logicCoat, false, 0, checkOverlaps);
	G4VPhysicalVolume *physScintCorner2	= new G4PVPlacement(rot1, G4ThreeVector( (scindWidth / 2 - cornerRadius), -(scintHeight / 2 - cornerRadius), 0.), logicScintCorner, "physScintCorner", logicCoat, false, 1, checkOverlaps);
	G4VPhysicalVolume *physScintCorner3	= new G4PVPlacement(rot2, G4ThreeVector(-(scindWidth / 2 - cornerRadius), -(scintHeight / 2 - cornerRadius), 0.), logicScintCorner, "physScintCorner", logicCoat, false, 2, checkOverlaps);
	G4VPhysicalVolume *physScintCorner4	= new G4PVPlacement(rot3, G4ThreeVector(-(scindWidth / 2 - cornerRadius),  (scintHeight / 2 - cornerRadius), 0.), logicScintCorner, "physScintCorner", logicCoat, false, 3, checkOverlaps);

	//	scintillator channel
	G4Box *solidChannelCore				= new G4Box("solidChannelCore", channelWidth / 2, (channelHeight - channelWidth) / 2, barLength / 2);
	G4Tubs *solidChannelSide			= new G4Tubs("solidChannelSide", 0., channelWidth / 2, barLength / 2, 0., 180. * deg);

	G4LogicalVolume *logicChannelCore 	= new G4LogicalVolume(solidChannelCore, airMat, "logicChannelCore");
	G4VPhysicalVolume *physChannelCore	= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicChannelCore, "physChannelCore", logicScintCenter, false, 0, checkOverlaps);

	G4LogicalVolume *logicChannelSide 	= new G4LogicalVolume(solidChannelSide, airMat, "logicChannelSide");
	G4VPhysicalVolume *physChannelSideT	= new G4PVPlacement(0,    G4ThreeVector(0.,  (channelHeight - channelWidth) / 2, 0.), logicChannelSide, "physChannelSide", logicScintCenter, false, 0, checkOverlaps);
	G4VPhysicalVolume *physChannelSideB	= new G4PVPlacement(rot2, G4ThreeVector(0., -(channelHeight - channelWidth) / 2, 0.), logicChannelSide, "physChannelSide", logicScintCenter, false, 1, checkOverlaps);

	//	optical fiber
	G4Tubs *solidFiberClad 				= new G4Tubs("solidFiberClad", 0., fiberDiameter / 2, barLength / 2, 0., 360. * deg);
	G4Tubs *solidFiberCore 				= new G4Tubs("solidFiberCore", 0., (fiberDiameter - cladThickness) / 2, barLength / 2, 0., 360. * deg);
	G4Tubs *solidFiberCladShort			= new G4Tubs("solidFiberCladShort", 0., fiberDiameter / 2, fiberStickout / 2, 0., 360. * deg);
	G4Tubs *solidFiberCoreShort 		= new G4Tubs("solidFiberCoreShort", 0., (fiberDiameter - cladThickness) / 2, fiberStickout / 2, 0., 360. * deg);

	G4LogicalVolume *logicFiberClad 	= new G4LogicalVolume(solidFiberClad, PMMA, "logicFiberClad");
	G4VPhysicalVolume *physFiberClad	= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicFiberClad, "physFiberClad", logicChannelCore, false, 0, checkOverlaps);

	G4LogicalVolume *logicFiberCore 	= new G4LogicalVolume(solidFiberCore, fiberCoreMat, "logicFiberCore");
	G4VPhysicalVolume *physFiberCore	= new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicFiberCore, "physFiberCore", logicFiberClad, false, 0, checkOverlaps);

	G4LogicalVolume *logicFiberClad2 	= new G4LogicalVolume(solidFiberCladShort, PMMA, "logicFiberClad2");
	G4VPhysicalVolume *physFiberClad2	= new G4PVPlacement(0, G4ThreeVector(0., 0., (barLength + fiberStickout) / 2), logicFiberClad2, "physFiberClad2", logicAssembly, false, 0, checkOverlaps);

	G4LogicalVolume *logicFiberCore2 	= new G4LogicalVolume(solidFiberCoreShort, fiberCoreMat, "logicFiberCore2");
	G4VPhysicalVolume *physFiberCore2	= new G4PVPlacement(0, G4ThreeVector(0., 0., (barLength + fiberStickout) / 2), logicFiberCore2, "physFiberCore2", logicAssembly, false, 0, checkOverlaps);

	// reflectors
	G4Box *solidMirror1 				= new G4Box("solidMirror1", barWidth / 2, barHeight / 2, mirrorThickness / 2);
	G4SubtractionSolid *solidMirror2 	= new G4SubtractionSolid("solidMirror2", solidMirror1, solidFiberClad);

	G4LogicalVolume *logicMirror1 		= new G4LogicalVolume(solidMirror1, Al, "logicMirror1");
	G4VPhysicalVolume *physMirror1		= new G4PVPlacement(0, G4ThreeVector(0., 0., -(barLength + mirrorThickness) / 2), logicMirror1, "physMirror1", logicAssembly, false, 0, checkOverlaps);

	G4LogicalVolume *logicMirror2		= new G4LogicalVolume(solidMirror2, Al, "logicMirror2");
	G4VPhysicalVolume *physMirror2		= new G4PVPlacement(0, G4ThreeVector(0., 0., (barLength + mirrorThickness) / 2), logicMirror2, "physMirror2", logicAssembly, false, 0, checkOverlaps);

	//	photon detector
	G4Box *solidSiPM					= new G4Box("solidSiPM", sipmSidelength / 2, sipmSidelength / 2, sipmThickness / 2);

	logicSiPM 							= new G4LogicalVolume(solidSiPM, Si, "logicSiPM");
	G4VPhysicalVolume *physSiPM			= new G4PVPlacement(0, G4ThreeVector(0., 0., (barLength + sipmThickness) / 2 + fiberStickout), logicSiPM, "physSiPM", logicAssembly, false, 0, checkOverlaps);




	
	//-----------------------------------------------------------------------------
	//	optical surfaces
	//-----------------------------------------------------------------------------

	//	coating
	G4OpticalSurface *scintCoatSurface = new G4OpticalSurface("scintCoatSurface");
	scintCoatSurface->SetType(dielectric_metal);
	scintCoatSurface->SetFinish(groundfrontpainted);
	scintCoatSurface->SetModel(unified);

	G4MaterialPropertiesTable *mptCoat = new G4MaterialPropertiesTable();
	G4double reflectivityCoat[2] = {0.95, 0.95};
	mptCoat->AddProperty("REFLECTIVITY", smallArrayEnergy, reflectivityCoat, 2);
	scintCoatSurface->SetMaterialPropertiesTable(mptCoat);

	new G4LogicalBorderSurface("scintCoatSurf", physScintCenter, physCoat, scintCoatSurface);
	new G4LogicalBorderSurface("scintCoatSurf", physScintSideR, physCoat, scintCoatSurface);
	new G4LogicalBorderSurface("scintCoatSurf", physScintSideL, physCoat, scintCoatSurface);
	new G4LogicalBorderSurface("scintCoatSurf", physScintCorner1, physCoat, scintCoatSurface);
	new G4LogicalBorderSurface("scintCoatSurf", physScintCorner2, physCoat, scintCoatSurface);
	new G4LogicalBorderSurface("scintCoatSurf", physScintCorner3, physCoat, scintCoatSurface);
	new G4LogicalBorderSurface("scintCoatSurf", physScintCorner4, physCoat, scintCoatSurface);

	//	reflector
	G4OpticalSurface *scintRefSurface = new G4OpticalSurface("scintRefSurface");
	scintRefSurface->SetType(dielectric_metal);
	scintRefSurface->SetFinish(polished);
	scintRefSurface->SetModel(unified);

	G4MaterialPropertiesTable *mptRef = new G4MaterialPropertiesTable();
	G4double reflectivityRef[2] = {0.95, 0.95};
	mptRef->AddProperty("REFLECTIVITY", smallArrayEnergy, reflectivityRef, 2);
	scintRefSurface->SetMaterialPropertiesTable(mptRef);

	new G4LogicalBorderSurface("scintRefSurface", physScintCenter, physMirror1, scintRefSurface);
	new G4LogicalBorderSurface("scintRefSurface", physScintSideR, physMirror1, scintRefSurface);
	new G4LogicalBorderSurface("scintRefSurface", physScintSideL, physMirror1, scintRefSurface);
	new G4LogicalBorderSurface("scintRefSurface", physScintCorner1, physMirror1, scintRefSurface);
	new G4LogicalBorderSurface("scintRefSurface", physScintCorner2, physMirror1, scintRefSurface);
	new G4LogicalBorderSurface("scintRefSurface", physScintCorner3, physMirror1, scintRefSurface);
	new G4LogicalBorderSurface("scintRefSurface", physScintCorner4, physMirror1, scintRefSurface);

	new G4LogicalBorderSurface("scintRefSurface", physScintCenter, physMirror2, scintRefSurface);
	new G4LogicalBorderSurface("scintRefSurface", physScintSideR, physMirror2, scintRefSurface);
	new G4LogicalBorderSurface("scintRefSurface", physScintSideL, physMirror2, scintRefSurface);
	new G4LogicalBorderSurface("scintRefSurface", physScintCorner1, physMirror2, scintRefSurface);
	new G4LogicalBorderSurface("scintRefSurface", physScintCorner2, physMirror2, scintRefSurface);
	new G4LogicalBorderSurface("scintRefSurface", physScintCorner3, physMirror2, scintRefSurface);
	new G4LogicalBorderSurface("scintRefSurface", physScintCorner4, physMirror2, scintRefSurface);

	new G4LogicalBorderSurface("scintRefSurface", physFiberClad, physMirror1, scintRefSurface);
	new G4LogicalBorderSurface("scintRefSurface", physFiberCore, physMirror1, scintRefSurface);





	//-----------------------------------------------------------------------------
	//	detector module placements
	//-----------------------------------------------------------------------------
	
	auto rot4 = new G4RotationMatrix();
	rot4->rotateY(-90. * deg);

	std::vector<std::pair<G4RotationMatrix*, G4ThreeVector>> placements = {
		//	layer 1 x
		{0, {-140, -145, 0}},     {0, {-100, -145, 0}},      {0, {-60, -145, 0}},     {0, {-20, -145, 0}},
		{0, {20, -145, 0}},       {0, {60, -145, 0}},        {0, {100, -145, 0}},     {0, {140, -145, 0}},
		//	layer 1 y
		{rot4, {0, -135, -140}},  {rot4, {0, -135, -100}},   {rot4, {0, -135, -60}},  {rot4, {0, -135, -20}},
		{rot4, {0, -135, 20}},    {rot4, {0, -135, 60}},     {rot4, {0, -135, 100}},  {rot4, {0, -135, 140}},
		//	layer 2 x
		{0, {-140, -105, 0}},     {0, {-100, -105, 0}},      {0, {-60, -105, 0}},     {0, {-20, -105, 0}},
		{0, {20, -105, 0}},       {0, {60, -105, 0}},        {0, {100, -105, 0}},     {0, {140, -105, 0}},
		//	layer 2 y
		{rot4, {0, -95, -140}},   {rot4, {0, -95, -100}},    {rot4, {0, -95, -60}},   {rot4, {0, -95, -20}},
		{rot4, {0, -95, 20}},     {rot4, {0, -95, 60}},      {rot4, {0, -95, 100}},   {rot4, {0, -95, 140}},
		//	layer 3 x
		{0, {-140, -65, 0}},      {0, {-100, -65, 0}},       {0, {-60, -65, 0}},      {0, {-20, -65, 0}},
		{0, {20, -65, 0}},        {0, {60, -65, 0}},         {0, {100, -65, 0}},      {0, {140, -65, 0}},
		//	layer 3 y
		{rot4, {0, -55, -140}},   {rot4, {0, -55, -100}},    {rot4, {0, -55, -60}},   {rot4, {0, -55, -20}},
		{rot4, {0, -55, 20}},     {rot4, {0, -55, 60}},      {rot4, {0, -55, 100}},   {rot4, {0, -55, 140}},
		//	layer 4 x
		{0, {-140, -25, 0}},      {0, {-100, -25, 0}},       {0, {-60, -25, 0}},      {0, {-20, -25, 0}},
		{0, {20, -25, 0}},        {0, {60, -25, 0}},         {0, {100, -25, 0}},      {0, {140, -25, 0}},
		//	layer 4 y
		{rot4, {0, -15, -140}},   {rot4, {0, -15, -100}},    {rot4, {0, -15, -60}},   {rot4, {0, -15, -20}},
		{rot4, {0, -15, 20}},     {rot4, {0, -15, 60}},      {rot4, {0, -15, 100}},   {rot4, {0, -15, 140}},
		//	layer 5 x
		{0, {-140, 15, 0}},       {0, {-100, 15, 0}},        {0, {-60, 15, 0}},       {0, {-20, 15, 0}},
		{0, {20, 15, 0}},         {0, {60, 15, 0}},          {0, {100, 15, 0}},       {0, {140, 15, 0}},
		//	layer 5 y
		{rot4, {0, 25, -140}},    {rot4, {0, 25, -100}},     {rot4, {0, 25, -60}},    {rot4, {0, 25, -20}},
		{rot4, {0, 25, 20}},      {rot4, {0, 25, 60}},       {rot4, {0, 25, 100}},    {rot4, {0, 25, 140}},
		//	layer 6 x
		{0, {-140, 55, 0}},       {0, {-100, 55, 0}},        {0, {-60, 55, 0}},       {0, {-20, 55, 0}},
		{0, {20, 55, 0}},         {0, {60, 55, 0}},          {0, {100, 55, 0}},       {0, {140, 55, 0}},
		//	layer 6 y
		{rot4, {0, 65, -140}},    {rot4, {0, 65, -100}},     {rot4, {0, 65, -60}},    {rot4, {0, 65, -20}},
		{rot4, {0, 65, 20}},      {rot4, {0, 65, 60}},       {rot4, {0, 65, 100}},    {rot4, {0, 65, 140}},
		//	layer 7 x
		{0, {-140, 95, 0}},       {0, {-100, 95, 0}},        {0, {-60, 95, 0}},       {0, {-20, 95, 0}},
		{0, {20, 95, 0}},         {0, {60, 95, 0}},          {0, {100, 95, 0}},       {0, {140, 95, 0}},
		//	layer 7 y
		{rot4, {0, 105, -140}},   {rot4, {0, 105, -100}},    {rot4, {0, 105, -60}},   {rot4, {0, 105, -20}},
		{rot4, {0, 105, 20}},     {rot4, {0, 105, 60}},      {rot4, {0, 105, 100}},   {rot4, {0, 105, 140}},
		//	layer 8 x
		{0, {-140, 135, 0}},      {0, {-100, 135, 0}},       {0, {-60, 135, 0}},   	  {0, {-20, 135, 0}},
		{0, {20, 135, 0}},        {0, {60, 135, 0}},         {0, {100, 135, 0}},      {0, {140, 135, 0}},
		//	layer 8 y
		{rot4, {0, 145, -140}},   {rot4, {0, 145, -100}},    {rot4, {0, 145, -60}},   {rot4, {0, 145, -20}},
		{rot4, {0, 145, 20}},     {rot4, {0, 145, 60}},      {rot4, {0, 145, 100}},   {rot4, {0, 145, 140}}
	};

	for (size_t i = 0; i < placements.size(); i++) {
		new G4PVPlacement(placements[i].first, placements[i].second, logicAssembly, "physAssembly", logicWorld, false, i, checkOverlaps);
	}

	



	return physWorld;
}





void DetectorConstruction::ConstructSDandField() {

	auto *SD = new SensitiveDetector("SiPMSD");

	G4SDManager::GetSDMpointer()->AddNewDetector(SD);

	logicSiPM->SetSensitiveDetector(SD);
}
