/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent() :
	currentStage(kStartupText),
	myProgressWindowThread(this)
{
	startupTextWindow.mainComp = this;
	FinishedWindow.mainComp = this;
	setPathsWindow.mainComp = this;

    setSize (600, 520);

	installFolder = File::getSpecialLocation(
		File::currentApplicationFile).getParentDirectory().getFullPathName() +
		File::separator + "InstallFiles";

	addAndMakeVisible(startupTextWindow);
	addChildComponent(FinishedWindow);
	FinishedWindow.getExitButton()->setVisible(false);
	addChildComponent(setPathsWindow);
	addAndMakeVisible(toolTipWindow);
	toolTipWindow.setAlwaysOnTop(true);
	startupTextWindow.updateReadme(installFolder + File::separator + "readme.txt");
	startupTextWindow.setInstallfiles(installFolder);

	// ----------- //

	checkVersionFile();
}

//-----------------------------------------------------------------------------------------------------------
MainContentComponent::~MainContentComponent()
{
}

//-----------------------------------------------------------------------------------------------------------
void MainContentComponent::checkVersionFile()
{
	File checkVersion(installFolder + File::separator + "version.txt");
	if (checkVersion.existsAsFile())
	{
		StringArray readLines;
		checkVersion.readLines(readLines);
		if (readLines.size() > 0)
		{
			startupTextWindow.setVersion(readLines[0]);
			FinishedWindow.setVersion(readLines[0]);
			setPathsWindow.setVersion(readLines[0]);
		}
	}
}

//-----------------------------------------------------------------------------------------------------------
void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xffeeddff));
}

//-----------------------------------------------------------------------------------------------------------
void MainContentComponent::resized()
{
	startupTextWindow.setBounds(getLocalBounds());
	FinishedWindow.setBounds(getLocalBounds());
	setPathsWindow.setBounds(getLocalBounds());
}

#if JUCE_WINDOWS
	#define GETWINKEY(a, b) if (WindowsRegistry::keyExists(a) && WindowsRegistry::getValue(a).isNotEmpty()) b = WindowsRegistry::getValue(a)
#endif

//-----------------------------------------------------------------------------------------------------------
void MainContentComponent::nextStageWindow()
{
	if (currentStage == kStartupText)
	{
		File checkVersion(installFolder + File::separator + "Version.txt");
		if (checkVersion.existsAsFile())
		{
			currentStage = kSetPaths;
			checkVersionFile();
			startupTextWindow.setVisible(false);
			setPathsWindow.setVisible(true);
			//
			File checkVersion(installFolder + File::separator + "paths.txt");
			StringArray readLines;
			checkVersion.readLines(readLines);
			//
			// Try to find registry info
			#if JUCE_WINDOWS
				String gPlugFolder = readLines[0]; 
				String gDataFolder = readLines[1]; 
				String gWusikStationFolder = "none (optional)";
				String gUserFolder = "none (optional)";
				//
				GETWINKEY(readLines[2], gWusikStationFolder);
				else GETWINKEY("HKEY_CURRENT_USER\\Software\\Wusik.com\\Wusikstation\\DataDirectory", gWusikStationFolder);
				else GETWINKEY("HKEY_CURRENT_USER\\Software\\WusikEngine\\WusikEngine\\DataDirectory", gWusikStationFolder);
				//
				GETWINKEY(readLines[3], gDataFolder);
				GETWINKEY(readLines[4], gUserFolder);
				//
				GETWINKEY(readLines[5], gPlugFolder);
				else GETWINKEY("HKEY_LOCAL_MACHINE\\SOFTWARE\\VST\\VSTPluginsPath", gPlugFolder);
			#else
				String gPlugFolder = readLines[0];
				String gDataFolder = readLines[1];
				String gWusikStationFolder = "none (optional)";
				String gUserFolder = "none (optional)";
				if (File(readLines[2]).existsAsFile())
				{
					plistManager plists;
					plists.readFile(readLines[2]);
					gPlugFolder = plists.getKey("Install Path Plugins");
					gWusikStationFolder = plists.getKey("Install Path Wusik Station");
					gUserFolder = plists.getKey("User Path");
				}
				if (File(readLines[3]).existsAsFile())
				{
					plistManager plists;
					plists.readFile(readLines[3]);
					gDataFolder = plists.getKey("Wusik Data Path");
				}
			#endif
			//
			setPathsWindow.getpluginsFolder()->setText(gPlugFolder, NotificationType::dontSendNotification);
			setPathsWindow.getdataFolder()->setText(gDataFolder, NotificationType::dontSendNotification);
			setPathsWindow.getwsFolder()->setText(gWusikStationFolder, NotificationType::dontSendNotification);
			setPathsWindow.getuserFolder()->setText(gUserFolder, NotificationType::dontSendNotification);
		}
		else
		{
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "Error!", "The selected source install folder does not have the required files!\nBrowse to the correct installation folder and try again.");
		}
		repaint();
	}
	else
	{
		// Store Paths //
		File checkVersion(installFolder + File::separator + "paths.txt");
		StringArray readLines;
		checkVersion.readLines(readLines);
		//
		#if JUCE_WINDOWS
			WindowsRegistry::setValue(readLines[2], setPathsWindow.getwsFolder()->getText());
			WindowsRegistry::setValue(readLines[3], setPathsWindow.getdataFolder()->getText());
			WindowsRegistry::setValue(readLines[4], setPathsWindow.getuserFolder()->getText());
			WindowsRegistry::setValue(readLines[5], setPathsWindow.getpluginsFolder()->getText());
		#else
			plistManager plists;
			if (File(readLines[2]).existsAsFile()) plists.readFile(readLines[2]);
			plists.addKey("Install Path Wusik Station", setPathsWindow.getwsFolder()->getText());
			plists.addKey("Install Path Plugins", setPathsWindow.getpluginsFolder()->getText());
			plists.addKey("User Path", setPathsWindow.getuserFolder()->getText());
			plists.writeFile(readLines[2]);
			//
			plistManager plists2;
			if (File(readLines[3]).existsAsFile()) plists2.readFile(readLines[3]);
			plists2.addKey("Wusik Data Path", setPathsWindow.getdataFolder()->getText());
			plists2.writeFile(readLines[3]);
		#endif
		currentStage = kInstallProgression;
		setPathsWindow.setVisible(false);
		FinishedWindow.setVisible(true);
		repaint();
		//
		myProgressWindowThread.resetUserSettings = setPathsWindow.isResetUserSettings();
		myProgressWindowThread.replaceFiles = setPathsWindow.isReplaceFiles();
		myProgressWindowThread.dataPath = setPathsWindow.getdataFolder()->getText();
		myProgressWindowThread.pluginsPath = setPathsWindow.getpluginsFolder()->getText();
		myProgressWindowThread.sourcePath = installFolder;
		FinishedWindow.setInstallLabel("Source Path: " + myProgressWindowThread.sourcePath +
			"\nDestination Plugins Path: " + myProgressWindowThread.pluginsPath +
			"\nDestination Data Path: " + myProgressWindowThread.dataPath +
			(setPathsWindow.isReplaceFiles() ? "\nReplace files" : "\nUpdate files"));
		//
		bool completedOk = myProgressWindowThread.runThread();
		if (myProgressWindowThread.logFileWrite != nullptr)
		{
			myProgressWindowThread.logFileWrite->flush();
			myProgressWindowThread.logFileWrite = nullptr;
		}
		//
		#if JUCE_WINDOWS
			File logFile(installFolder + File::separator + "install log.txt");
		#else
			File logFile = File("/tmp/install log.txt");
		#endif
		StringArray logLines;
		logFile.readLines(logLines);
		//
		if (completedOk)
		{
			FinishedWindow.setInstallLabel("> Installation completed\n\n" + 
				logLines.joinIntoString("\n"));
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Finished", "Installation Completed");
		}
		else
		{
			FinishedWindow.setInstallLabel("> Install canceled by user!\n\n" + 
				logLines.joinIntoString("\n") + "\n\n> Install canceled by user!");
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "Canceled!", "Installation Canceled by User!");
		}
		

		FinishedWindow.getExitButton()->setVisible(true);
		repaint();
		//
		logFile.deleteFile();
	}
}

//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------

void ThreadWithProgressWindowEx::run()
{
	#if JUCE_WINDOWS
		logFile = File(sourcePath + File::separator + "install log.txt");
	#else
		logFile = File("/tmp/install log.txt");
	#endif
	logFile.deleteFile();
	logFileWrite = logFile.createOutputStream();
	//
	totalEvents = totalFilesUpdated = 0;
	ScopedPointer<DirectoryIterator> fileList = new DirectoryIterator(File(sourcePath), true, "*", File::TypesOfFileToFind::findFiles);
	while (fileList->next())
	{ 
		if (fileList->getFile().getFileName().compareIgnoreCase("version.txt") != 0 &&
			fileList->getFile().getFileName().compareIgnoreCase("readme.txt") != 0 &&
            fileList->getFile().getFileName().compareIgnoreCase(".DS_Store") != 0 &&
			fileList->getFile().getFileName().compareIgnoreCase("install log.txt") != 0) totalEvents++;
	}
	//
	if (updateLog("> total number of files to be processed: " + String(totalEvents), false)) return;
	if (updateLog("> source path: " + sourcePath, false)) return;
	if (updateLog("> plugins path: " + pluginsPath, false)) return;
	if (updateLog("> data path: " + dataPath, false)) return;
	//
	#if JUCE_WINDOWS
		if (updateLog(">\n> processing plugin files", false)) return;
		fileList = new DirectoryIterator(File(sourcePath + File::separator + "plugins"), false, "*", File::TypesOfFileToFind::findFiles);
		while (fileList->next())
		{
			if (fileList->getFile().getFileExtension().compareIgnoreCase(".vst") == 0 || 
				fileList->getFile().getFileExtension().compareIgnoreCase(".vst3") == 0 ||
				fileList->getFile().getFileExtension().compareIgnoreCase(".dll") == 0 ||
				fileList->getFile().getFileExtension().compareIgnoreCase(".exe") == 0)
			{
				if (updateLog(processFile(pluginsPath, fileList->getFile()), true)) return;
			}
		}
	#else
		if (updateLog(">\n> processing plugin files", false)) return;
		fileList = new DirectoryIterator(File(sourcePath + File::separator + "plugins" + File::separator + "Components"), false, "*", File::TypesOfFileToFind::findFilesAndDirectories);
		while (fileList->next())
		{
			if (fileList->getFile().getFileName().compareIgnoreCase(".DS_Store") != 0)
			{
				if (updateLog(processFile(pluginsPath + File::separator + "Components", fileList->getFile(), true), true)) return; 
			}
		}
		//
		fileList = new DirectoryIterator(File(sourcePath + File::separator + "plugins" + File::separator + "VST"), false, "*", File::TypesOfFileToFind::findFilesAndDirectories);
		while (fileList->next())
		{
			if (fileList->getFile().getFileName().compareIgnoreCase(".DS_Store") != 0)
			{
				if (updateLog(processFile(pluginsPath + File::separator + "VST", fileList->getFile(), true), true)) return;
			}
		}
        //
		fileList = new DirectoryIterator(File(sourcePath + File::separator + "plugins" + File::separator + "VST3"), false, "*", File::TypesOfFileToFind::findFilesAndDirectories);
		while (fileList->next())
		{
			if (fileList->getFile().getFileName().compareIgnoreCase(".DS_Store") != 0)
			{
				if (updateLog(processFile(pluginsPath + File::separator + "VST3", fileList->getFile(), true), true)) return;
			}
		}
	#endif
	//
	if (updateLog(">\n> processing skin files", false)) return;
	fileList = new DirectoryIterator(File(sourcePath + File::separator + "data" + File::separator + "Skins"), true, "*", File::TypesOfFileToFind::findFiles);
	while (fileList->next())
	{
		if (fileList->getFile().getFileExtension().compareIgnoreCase(".png") == 0 ||
			fileList->getFile().getFileExtension().compareIgnoreCase(".xml") == 0 ||
			fileList->getFile().getFileExtension().compareIgnoreCase(".psd") == 0 ||
			fileList->getFile().getFileExtension().compareIgnoreCase(".txt") == 0)
		{
			String originalPath = fileList->getFile().getRelativePathFrom(sourcePath + File::separator + "data");
			if (updateLog(processFile(File(dataPath).getChildFile(originalPath).getParentDirectory().getFullPathName()
				, fileList->getFile()), true)) return;
		}
	}
	//
	if (updateLog(">\n> processing extra files", false)) return;
	//
	fileList = new DirectoryIterator(File(sourcePath + File::separator + "data" + File::separator + "Extras"), true, "*", File::TypesOfFileToFind::findFiles);
	while (fileList->next())
	{
		if (fileList->getFile().getFileExtension().compareIgnoreCase(".txt") == 0 ||
			fileList->getFile().getFileExtension().compareIgnoreCase(".MLETemplate") == 0 ||
			fileList->getFile().getFileExtension().compareIgnoreCase(".w4ktplt") == 0)
		{
			String originalPath = fileList->getFile().getRelativePathFrom(sourcePath + File::separator + "data");
			if (updateLog(processFile(File(dataPath).getChildFile(originalPath).getParentDirectory().getFullPathName()
				, fileList->getFile(), false, true), true)) return;
		}
	}
	//
	if (updateLog(">\n> processing tun (scala) files", false)) return;
	//
	fileList = new DirectoryIterator(File(sourcePath + File::separator + "data" + File::separator + "TUN Files"), true, "*", File::TypesOfFileToFind::findFiles);
	while (fileList->next())
	{
		if (fileList->getFile().getFileExtension().compareIgnoreCase(".tun") == 0)
		{
			String originalPath = fileList->getFile().getRelativePathFrom(sourcePath + File::separator + "data");
			if (updateLog(processFile(File(dataPath).getChildFile(originalPath).getParentDirectory().getFullPathName()
				, fileList->getFile(), false, true), true)) return;
		}
	}
	//
	if (updateLog(">\n> processing extra vst effects files", false)) return;
	//
	#if JUCE_WINDOWS
		fileList = new DirectoryIterator(File(sourcePath + File::separator + "data" + File::separator + "VST Effects 32"), true, "*", File::TypesOfFileToFind::findFiles);
		while (fileList->next())
		{
			if (fileList->getFile().getFileExtension().compareIgnoreCase(".dll") == 0)
			{
				String originalPath = fileList->getFile().getRelativePathFrom(sourcePath + File::separator + "data");
				if (updateLog(processFile(File(dataPath).getChildFile(originalPath).getParentDirectory().getFullPathName()
					, fileList->getFile(), false, true), true)) return;
			}
		}
		//
		fileList = new DirectoryIterator(File(sourcePath + File::separator + "data" + File::separator + "VST Effects 64"), true, "*", File::TypesOfFileToFind::findFiles);
		while (fileList->next())
		{
			if (fileList->getFile().getFileExtension().compareIgnoreCase(".dll") == 0)
			{
				String originalPath = fileList->getFile().getRelativePathFrom(sourcePath + File::separator + "data");
				if (updateLog(processFile(File(dataPath).getChildFile(originalPath).getParentDirectory().getFullPathName()
					, fileList->getFile(), false, true), true)) return;
			}
		}
	#else
        fileList = new DirectoryIterator(File(sourcePath + File::separator + "data" + File::separator + "VST Effects"), false, "*", File::TypesOfFileToFind::findFilesAndDirectories);
		while (fileList->next())
		{
			if (fileList->getFile().getFileName().compareIgnoreCase(".DS_Store") != 0)
			{
				String originalPath = fileList->getFile().getRelativePathFrom(sourcePath + File::separator + "data");
				if (updateLog(processFile(File(dataPath).getChildFile(originalPath).getParentDirectory().getFullPathName(),
					fileList->getFile(), true), true)) return;
			}
		}
	#endif
	//
	if (updateLog(">\n> processing soundset files", false)) return;
	fileList = new DirectoryIterator(File(sourcePath + File::separator + "data" + File::separator + "SoundSets"), true, "*", File::TypesOfFileToFind::findFiles);
	while (fileList->next())
	{
		if (fileList->getFile().getFileExtension().compareIgnoreCase(".wav") == 0 ||
			fileList->getFile().getFileExtension().compareIgnoreCase(".wusiksnd") == 0 ||
			fileList->getFile().getFileExtension().compareIgnoreCase(".dashsnd") == 0 ||
			fileList->getFile().getFileExtension().compareIgnoreCase(".txt") == 0 ||
			fileList->getFile().getFileExtension().compareIgnoreCase(".aiff") == 0 ||
			fileList->getFile().getFileExtension().compareIgnoreCase(".sfz") == 0)
		{
			String originalPath = fileList->getFile().getRelativePathFrom(sourcePath + File::separator + "data");
			if (updateLog(processFile(File(dataPath).getChildFile(originalPath).getParentDirectory().getFullPathName()
				, fileList->getFile()), true)) return;
		}
	}
	//
	if (updateLog(">\n> processing preset files", false)) return;
	fileList = new DirectoryIterator(File(sourcePath + File::separator + "data" + File::separator + "Presets"), true, "*", File::TypesOfFileToFind::findFiles);
	while (fileList->next())
	{
		if (fileList->getFile().getFileExtension().compareIgnoreCase(".w4kpreset") == 0 ||
			fileList->getFile().getFileExtension().compareIgnoreCase(".w8kpreset") == 0 ||
			fileList->getFile().getFileExtension().compareIgnoreCase(".wspreset") == 0 ||
			fileList->getFile().getFileExtension().compareIgnoreCase(".WusikPRST") == 0 ||
			fileList->getFile().getFileExtension().compareIgnoreCase(".txt") == 0)
		{
			String originalPath = fileList->getFile().getRelativePathFrom(sourcePath + File::separator + "data");
			if (updateLog(processFile(File(dataPath).getChildFile(originalPath).getParentDirectory().getFullPathName()
				, fileList->getFile()), true)) return;
		}
	}
	//
	if (resetUserSettings)
	{
		if (updateLog("> reset user settings: deleting Main Settings.xml file", false)) return;
		File(dataPath + File::separator + "Main Settings.xml").deleteFile();
	}
	//
	if (updateLog(">\n> installation Finished !", false)) return;
	if (updateLog("      total number of files processed: " + String(eventsCounter), false)) return;
	if (updateLog("      total number of files updated: " + String(totalFilesUpdated), false)) return;
	logFileWrite->flush();
	logFileWrite = nullptr;
};

//-----------------------------------------------------------------------------------------------------------
bool ThreadWithProgressWindowEx::updateLog(String text, bool newEvent)
{
	logFileWrite->writeText(text + String("\n"), false, false);

	if (newEvent)
	{
		eventsCounter++;
		setProgress((double) eventsCounter / (double) totalEvents);
	}

	return threadShouldExit();
}

//-----------------------------------------------------------------------------------------------------------
String ThreadWithProgressWindowEx::processFile(String destinationPath, File filename, bool isBundleFolder, bool forceReplace)
{
    if (isBundleFolder) eventsCounter += 4;
	File originalFile = destinationPath + File::separator + filename.getFileName();
	bool doFile = false;
	static String fileLog;
	fileLog = "      " + filename.getFileName() + " - ";
	if (!originalFile.exists())
	{
		fileLog.append(" copied", 999);
		doFile = true;
	}
	else if (replaceFiles || forceReplace)
	{
		fileLog.append(" replaced",999);
		doFile = true;
	}
	else
	{
		if (originalFile.getLastModificationTime() < filename.getLastModificationTime())
		{
			doFile = true;
			fileLog.append("updated",999);
		}
		else
		{
			fileLog.append("ignored", 999);
		}
	}
	//
	if (doFile)
	{
		if (!File(destinationPath).exists())
		{
			Result createDirRes = File(destinationPath).createDirectory();
			if (createDirRes.wasOk())
			{
				fileLog.append(" - also created folder - ", 999);
			} 
			else
			{
				fileLog.append(" - " + createDirRes.getErrorMessage(), 999);
				return fileLog;
			}
		}
		//
		if (isBundleFolder)
		{
			File(destinationPath + File::separator + filename.getFileName()).deleteRecursively();
		}
		//
		if (filename.copyFileTo(File(destinationPath + File::separator + filename.getFileName())))
		{
			fileLog.append(" OK", 999);
			totalFilesUpdated++;
            if (isBundleFolder) totalFilesUpdated += 4;
		}
		else
		{
			fileLog.append(" ERROR", 999);
		}
	}
	//
	return fileLog;
}
