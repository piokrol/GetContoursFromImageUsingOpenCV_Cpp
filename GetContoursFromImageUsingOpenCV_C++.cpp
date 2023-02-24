
#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>
#include <CAM/CAMAll.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"


using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;


Ptr<Application> _app;
Ptr<UserInterface> _ui;
Ptr<Product> _product;
Ptr<Design> _design;

bool checkReturn(Ptr<Base> returnObj)
{
	if (returnObj)
		return true;
	else
		if (_app && _ui)
		{
			std::string errDesc;
			_app->getLastError(&errDesc);
			_ui->messageBox(errDesc);
			return false;
		}
		else
			return false;
}

extern "C" XI_EXPORT bool run(const char* context)
{
	_app = Application::get();
	if (!_app)
		return false;

	_ui = _app->userInterface();
	if (!_ui)
		return false;

	_product = _app->activeProduct();
	if (!_product)
		return false;

	Ptr<Design> _design = _product;
	if (!_design)
		return false;


	// Create a command definition and add a button to the CREATE panel.
	Ptr<CommandDefinition> cmdDef = _ui->commandDefinitions()->addButtonDefinition("ResizerModifierCPPAddIn", "Resizer Modifier", "This modifier can be used to resize an object using dimensional values instead of scale factor.", "Resources/Resizer Modifier");
	if (!checkReturn(cmdDef))
		return false;

	Ptr<ToolbarPanel> createPanel = _ui->allToolbarPanels()->itemById("SolidModifyPanel");
	if (!checkReturn(createPanel))
		return false;

	Ptr<CommandControl> resizerModifier = createPanel->controls()->addCommand(cmdDef);
	if (!checkReturn(resizerModifier))
		return false;

	// Connect to the Command Created event.
	Ptr<CommandCreatedEvent> commandCreatedEvent = cmdDef->commandCreated();
	//commandCreatedEvent->add(&_resizerCmdCreated);

	std::string strContext = context;
	if (strContext.find("IsApplicationStartup", 0) != std::string::npos)
	{
		if (strContext.find("false", 0) != std::string::npos)
		{
			_ui->messageBox("The \"Resizer Modifier\" command has been added\nto the MODIFY panel of the MODEL workspace.");
		}
	}
	return true;
}


extern "C" XI_EXPORT bool stop(const char* context)
{
	if (_ui)
	{
		_ui->messageBox("Stop addin");
		_ui = nullptr;
	}

	return true;
}


#ifdef XI_WIN

#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hmodule, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#endif // XI_WIN
