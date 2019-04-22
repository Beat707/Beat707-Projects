/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_48F717855E9293DA__
#define __JUCE_HEADER_48F717855E9293DA__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
class MainContentComponent;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class w4kWindow  : public Component,
                   public ButtonListener,
                   public LabelListener
{
public:
    //==============================================================================
    w4kWindow ();
    ~w4kWindow();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	MainContentComponent* mainComp;
	void setVersion(String text) { productVersionLabel->setText(text, NotificationType::dontSendNotification); };
	Label* getpluginsFolder() { return pluginsFolder.get(); };
	Label* getdataFolder() { return dataFolder.get(); };
	Label* getuserFolder() { return userFolder.get(); };
	Label* getwsFolder() { return wsFolder.get(); };
	bool isResetUserSettings() { return resetSoftware->getToggleState(); };
	bool isReplaceFiles() { return replaceFiles->getToggleState(); };
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void labelTextChanged (Label* labelThatHasChanged) override;

    // Binary resources:
    static const char* wusikLogo_png;
    static const int wusikLogo_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> label1;
    ScopedPointer<TextButton> pluginsBrowse;
    ScopedPointer<Label> pluginsFolder;
    ScopedPointer<Label> label2;
    ScopedPointer<TextButton> dataBrowse;
    ScopedPointer<Label> dataFolder;
    ScopedPointer<Label> label4;
    ScopedPointer<TextButton> wsBrowse;
    ScopedPointer<Label> wsFolder;
    ScopedPointer<TextButton> installButton;
    ScopedPointer<Label> productVersionLabel;
    ScopedPointer<ToggleButton> resetSoftware;
    ScopedPointer<ToggleButton> replaceFiles;
    ScopedPointer<Label> label3;
    ScopedPointer<TextButton> userBrowse;
    ScopedPointer<Label> userFolder;
    Image cachedImage_wusikLogo_png_1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (w4kWindow)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_48F717855E9293DA__
