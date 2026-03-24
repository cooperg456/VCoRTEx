#include "VXActionInitialization.hpp"
#include "VXPrimaryGenerator.hpp"






ActionInitialization::ActionInitialization(G4ThreeVector gunPosition, G4ThreeVector gunMomentum, G4double gunEnergy, G4String particleName) : G4VUserActionInitialization() {
	gunPos = gunPosition;
	gunMom = gunMomentum;
	gunMeV = gunEnergy;
	pName = particleName;
}





void ActionInitialization::BuildForMaster() const {

}





void ActionInitialization::Build() const {
	PrimaryGenerator *generator = new PrimaryGenerator(gunPos, gunMom, gunMeV, pName);
	SetUserAction(generator);
}
