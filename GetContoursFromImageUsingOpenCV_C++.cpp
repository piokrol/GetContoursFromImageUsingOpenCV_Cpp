
#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>
#include <CAM/CAMAll.h>
#include <opencv2/opencv.hpp>
#include <math.h>


using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;
using namespace std;


cv::Mat selectedImage;



Ptr<Application> _app;
Ptr<UserInterface> _ui;
Ptr<Product> _product;
Ptr<Design> _design;


//TODO variable for description and name
string _cmdid = "getContoursFromImageCPP";
string _panelid = "SolidModifyPanel";

string _iconFolderBrowse = "Resources/Browse";

//print to console
bool _debug = true;

string _image;



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

//print message
void log(string message) {
	if (_debug) {
		_app->log(message);
	}
};

void getImage() {
   

	
	//cv::imread("_image", cv::IMREAD_COLOR);
	//cv::Size imgSize = selectedImage.size();
	//int width = imgSize.width;
	//int height = imgSize.height;
	//log(width);S
};


// Command InputChanged event handler.
class GetContourFromImageChangedHandler : public adsk::core::InputChangedEventHandler
{
public:
	void notify(const Ptr<InputChangedEventArgs>& eventArgs) override
	{
		Ptr<CommandInput> changedInput = eventArgs->input();


		if (changedInput->id() == "browseOutput") {
			log("input");
		}
	}
}_getcontourInputChanged;


// Command Created event handler.
class GetContourFromImageCommandCreatedEventHandler : public adsk::core::CommandCreatedEventHandler
{
public:
	void notify(const Ptr<CommandCreatedEventArgs>& eventArgs) override
	{
		Ptr<Command> cmd = eventArgs->command();
		// Define the inputs.
		Ptr<CommandInputs> inputs = cmd->commandInputs();

		_image = inputs->addBoolValueInput("browseOutput", "Browse image for ", false, _iconFolderBrowse);
		
		// Connect to the command related events.
		Ptr<InputChangedEvent> inputChangedEvent = cmd->inputChanged();
		if (!inputChangedEvent)
			return;
		bool isOk = inputChangedEvent->add(&_getcontourInputChanged);
		if (!isOk)
			return;
		
	}
} _getcontourCmdCreated;

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
	Ptr<CommandDefinition> cmdDef = _ui->commandDefinitions()->addButtonDefinition(_cmdid, "Get contours from image", "Get contours from image-des", "Resources/Get Contours");
	if (!checkReturn(cmdDef))
		return false;

	Ptr<ToolbarPanel> createPanel = _ui->allToolbarPanels()->itemById(_panelid);
	if (!checkReturn(createPanel))
		return false;

	Ptr<CommandControl> resizerModifier = createPanel->controls()->addCommand(cmdDef);
	if (!checkReturn(resizerModifier))
		return false;

	// Connect to the Command Created event.
	Ptr<CommandCreatedEvent> commandCreatedEvent = cmdDef->commandCreated();
	commandCreatedEvent->add(&_getcontourCmdCreated);

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

	Ptr<ToolbarPanel> createPanel = _ui->allToolbarPanels()->itemById(_panelid);
	if (!checkReturn(createPanel))
		return false;

	Ptr<CommandControl> resizerModifier = createPanel->controls()->itemById(_cmdid);
	if (checkReturn(resizerModifier))
		resizerModifier->deleteMe();

	Ptr<CommandDefinition> cmdDef = _ui->commandDefinitions()->itemById(_cmdid);
	if (checkReturn(cmdDef))
		cmdDef->deleteMe();
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
