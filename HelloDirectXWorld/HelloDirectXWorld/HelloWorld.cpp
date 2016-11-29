// precompiled headers
#include "pch.h"

//include common namespaces namesapces for code simplification
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Popups;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Platform;

//class definition for the core framework of the app
ref class HelloWorld sealed : public IFrameworkView
{
public:
	//functions called by Windows
	virtual void Initialize(CoreApplicationView^ appView)
	{
		//subscriube the OnActivated function to handle the Activated 'event'
		appView->Activated += ref new TypedEventHandler<Windows::ApplicationModel::Core::CoreApplicationView ^, Windows::ApplicationModel::Activation::IActivatedEventArgs ^>(this, &HelloWorld::OnActivated);
	}

	virtual void SetWindow(CoreWindow^ Window){}
	virtual void Load(String^EntryPoint){}
	virtual void Run() {}
	virtual void Uninitialize() {}

	//An event that is called when the application windows is ready to be activated
	void OnActivated(CoreApplicationView ^sender, IActivatedEventArgs ^args){
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();
		Window->Activate();
	}
};

//The class definition that creates and instance of our core framework class
ref class AppSource sealed : IFrameworkViewSource
{
public:
	virtual IFrameworkView^ CreateView()
	{
		return ref new HelloWorld();
	}
};

[MTAThreadAttribute] //define a main() as a multi-threaded-apartment function

// start point of app

int main(Array<String^>^ args)
{
	CoreApplication::Run(ref new AppSource());
	return 0;
}