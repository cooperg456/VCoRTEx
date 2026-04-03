#include "VXActionInitialization.hpp"
#include "VXPrimaryGenerator.hpp"
#include "VXRunAction.hpp"





void ActionInitialization::BuildForMaster() const {
	SetUserAction(new RunAction());
}





void ActionInitialization::Build() const {
	PrimaryGenerator *generator = new PrimaryGenerator();
	SetUserAction(generator);

	SetUserAction(new PrimaryGenerator());
    SetUserAction(new RunAction());
}
