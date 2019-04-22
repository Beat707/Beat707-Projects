/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "setPaths.h"
#include "startupText.h"
#include "finishedWindow.h"

enum
{
	kStartupText,
	kSetPaths,
	kInstallProgression
};

class MainContentComponent;

//==============================================================================
class ThreadWithProgressWindowEx : public ThreadWithProgressWindow
{
public:
	ThreadWithProgressWindowEx(MainContentComponent *_mainComp) :
		ThreadWithProgressWindow("... Installing ...", true, true),
		resetUserSettings(false),
		mainComp(_mainComp),
		totalEvents(1),
		eventsCounter(0),
		replaceFiles(false) {};
	//
	void run();
	bool updateLog(String text, bool newEvent);
	String processFile(String destinationPath, File filename, bool isBundleFolder = false, bool forceReplace = false);
	//
	MainContentComponent *mainComp;
	File logFile;
	ScopedPointer<OutputStream> logFileWrite;
	bool resetUserSettings;
	bool replaceFiles;
	String sourcePath;
	String dataPath;
	String pluginsPath;
	String installLog;
	int totalEvents;
	int eventsCounter;
	int totalFilesUpdated;
};

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();
	void nextStageWindow();

    void paint (Graphics&);
    void resized();
	void checkVersionFile();

    //==============================================================================
	int currentStage;
	w4kWindow setPathsWindow;
	startupText startupTextWindow;
	finishedWindow FinishedWindow;
	ThreadWithProgressWindowEx myProgressWindowThread;
	TooltipWindow toolTipWindow;
	String installFolder;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

//-----------------------------------------------------------------------------------------------------------
// Created by TriTone Digital on 21/09/09
#ifndef plistManager_H
#define plistManager_H
class plistManager
{
public:
	ScopedPointer<XmlElement> plist;
	int whichEntry;
	String plistFilePath;

	//------------------------------------------------------------	
	plistManager()
	{
		plist = new XmlElement("dict");
	}

	//------------------------------------------------------------	
	void writeFile(String path_)
	{
		File myxml(path_);
		plist->writeToFile(myxml, "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">");
	}

	//------------------------------------------------------------	
	bool readFile(String path_)
	{
		plistFilePath = path_;
		File myxml(path_);
		if (myxml.exists())
		{
			XmlDocument document(myxml);
			ScopedPointer<XmlElement> temp;
			temp = document.getDocumentElement();
			if (temp->hasTagName("dict"))
			{
				plist = new XmlElement(*temp);
				return true;
			}
			else
			{
				plist = new XmlElement(*temp->getChildByName("dict"));
				return true;
			}
			return false;
		}
	}
	//------------------------------------------------------------		
	bool isValid()
	{
		if (plist != nullptr) return true; else return false;
	}

	//------------------------------------------------------------	
	void addKey(String name_, String value_)
	{
		XmlElement*  name;
		name = new XmlElement("key");
		name->addTextElement(name_);
		plist->addChildElement(name);
		XmlElement* value;
		value = new XmlElement("string");
		value->addTextElement(value_);
		plist->addChildElement(value);
	}

	//------------------------------------------------------------		
	String getKey(String name_)
	{
		for (int i = 0; i<plist->getNumChildElements(); i++)
		{
			if (name_ == plist->getChildElement(i)->getAllSubText())
				return plist->getChildElement(i + 1)->getAllSubText();
		}
		return String::empty;
	}
};
#endif

#define MessageBox(dd,ds) AlertWindow::showMessageBox(AlertWindow::NoIcon, dd,ds)

#endif  // MAINCOMPONENT_H_INCLUDED
