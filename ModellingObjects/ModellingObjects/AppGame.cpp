// precompiled headers
#include "pch.h"
#include "Game.h"

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
ref class AppGame sealed : public IFrameworkView
{
	bool didCloseWindow; //change this to true when it's time to shutdown the window
	CGame Game;
public:
	//functions called by Windows
	virtual void Initialize(CoreApplicationView^ appView)
	{
		//subscriube the OnActivated function to handle the Activated 'event'
		appView->Activated += ref new TypedEventHandler<CoreApplicationView ^, IActivatedEventArgs ^>(this, &AppGame::OnActivated);

		didCloseWindow = false;

		CoreApplication::Suspending += ref new EventHandler<SuspendingEventArgs ^>(this, &AppGame::OnSuspending);
		CoreApplication::Resuming	+= ref new EventHandler<Platform::Object ^>(this, &AppGame::OnResuming);
	}

	virtual void SetWindow(CoreWindow^ Window)
	{
		//tell windows to precess this kind os events
		Window->PointerPressed		+= ref new TypedEventHandler<CoreWindow ^, PointerEventArgs ^>(this, &AppGame::PointerPressed);
		Window->PointerWheelChanged += ref new TypedEventHandler<CoreWindow ^, PointerEventArgs ^>(this, &AppGame::OnPointerWheelChanged);
		Window->KeyDown				+= ref new TypedEventHandler<CoreWindow ^,KeyEventArgs ^>(this, &AppGame::OnKeyDown);
		Window->Closed				+= ref new TypedEventHandler<CoreWindow ^, CoreWindowEventArgs ^>(this, &AppGame::OnClosed);
	}

	virtual void Load(String^ EntryPoint){}
	
	virtual void Run() 
	{
		Game.Initialize();
		//Obtained a pointer to the window
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();

		//repeat until widow shuts down
		while (!didCloseWindow)
		{
			//run ProcessEvents() to dispatch events
			Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			//calling update update and render repeatedly
			Game.Update();
			Game.Render();
		}
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
	
	//deal with wheel events
	void OnPointerWheelChanged(CoreWindow ^Sender, PointerEventArgs ^Args)
	{
		int wheel = Args->CurrentPoint->Properties->MouseWheelDelta;
		MessageDialog Dialog("", "");
		Dialog.Content = wheel.ToString();
		Dialog.Title = "Notice!";

		Dialog.ShowAsync();
	}

	void OnSuspending(Object ^sender, SuspendingEventArgs ^args)
	{
		//throw ref new Platform::NotImplementedException();
	}

	void OnResuming(Object ^sender, Object ^args)
	{
		//throw ref new Platform::NotImplementedException();
	}

	void OnClosed(CoreWindow ^sender, CoreWindowEventArgs ^args)
	{
		didCloseWindow = true;
	}

};

//The class definition that creates and instance of our core framework class
ref class AppSource sealed : IFrameworkViewSource
{
public:
	virtual IFrameworkView^ CreateView()
	{
		return ref new AppGame();
	}
};

[MTAThreadAttribute] //define a main() as a multi-threaded-apartment function

// start point of app

int main(Array<String^>^ args)
{
	CoreApplication::Run(ref new AppSource());
	return 0;
}

