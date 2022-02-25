#include "MCUManager.h"
#include "MCULogger.h"

int main(int argc, char* argv[])
{
	mcs::MCULogger logger("mcuServer");

	mcu::MCUManager mgr("./config/MCU.ini");

	mgr.Init();

	return mgr.Exec();
}
