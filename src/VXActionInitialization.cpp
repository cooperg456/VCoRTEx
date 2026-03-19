#include "VXActionInitialization.hpp"
#include "VXPrimaryGenerator.hpp"





void ActionInitialization::BuildForMaster() const {

}





void ActionInitialization::Build() const {
	PrimaryGenerator *generator = new PrimaryGenerator();
	SetUserAction(generator);
}
