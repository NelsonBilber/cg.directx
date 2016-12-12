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
		appView->Activated += ref new TypedEventHandler<CoreApplicationView ^, IActivatedEventArgs ^>(this, &HelloWorld::OnActivated);
	}

	virtual void SetWindow(CoreWindow^ Window)
	{
		//tell windows to precess this kind os events
		Window->PointerPressed += ref new TypedEventHandler<CoreWindow ^, PointerEventArgs ^>(this, &HelloWorld::PointerPressed);
		Window->KeyDown        += ref new TypedEventHandler<CoreWindow ^,KeyEventArgs ^>(this, &HelloWorld::OnKeyDown);
	}

	virtual void Load(String^ EntryPoint){}
	
	virtual void Run() 
	{
		//Obtained a pointer to the window
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();

		//run ProcessEvents() to dispatch events
		Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);

	}
	
	virtual void Uninitialize() {}

	//An event that is called when the application windows is ready to be activated
	void OnActivated(CoreApplicationView^ sender, IActivatedEventArgs^ args)
	{
		//obtain a pointer to the window
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();

		//activate the window
		Window->Activate();
	}

	//input event handlers
	void PointerPressed(CoreWindow^ Window, PointerEventArgs^ Args) 
	{
		auto point = Args->CurrentPoint;
		MessageDialog Dialog("","");
		Dialog.Content = "X :" + Args->CurrentPoint->Position.X.ToString() + " " + " Y : " + Args->CurrentPoint->Position.Y.ToString();
		Dialog.Title = "Notice!";
		
		Dialog.ShowAsync();
	}

	//deal with keyboards events
	void OnKeyDown(CoreWindow^ Window, KeyEventArgs^ Args)
	{
		MessageDialog Dialog("", "");
		if (Args->VirtualKey == VirtualKey::W)
		{
			Dialog.Content = "Move Forward";
			Dialog.Title = "W pressed";
		}
		else if (Args->VirtualKey == VirtualKey::A)
		{
			Dialog.Content = "Strafe left";
			Dialog.Title = "A pressed";
		}
		else if (Args->VirtualKey == VirtualKey::D)
		{
			Dialog.Content = "Strafe Right";
			Dialog.Title = "D pressed";
		}
		else if (Args->VirtualKey == VirtualKey::S)
		{
			Dialog.Content = "Strafe Back";
			Dialog.Title = "S pressed";
		}

		Dialog.ShowAsync();
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