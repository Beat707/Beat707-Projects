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

#ifndef __JUCE_HEADER_BA78ADCD1D9EAB08__
#define __JUCE_HEADER_BA78ADCD1D9EAB08__

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
class startupText  : public Component,
                     public ButtonListener
{
public:
    //==============================================================================
    startupText ();
    ~startupText();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	MainContentComponent* mainComp;
	void updateReadme(String filename)
	{
		File readMe(filename);
		if (readMe.existsAsFile())
		{
			StringArray readLines;
			readMe.readLines(readLines);
			if (readLines.size() > 0) textEditor->setText(readLines.joinIntoString("\n"),NotificationType::dontSendNotification);
			//
			installfiles->setVisible(false);
			browse->setVisible(false);
			nextStep->setVisible(true);
		}
	};
	void setVersion(String text) { productVersionLabel->setText(text, NotificationType::dontSendNotification); };
	void setInstallfiles(String path) { installfiles->setText(path, NotificationType::dontSendNotification); };
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;

    // Binary resources:
    static const char* wusikLogo_png;
    static const int wusikLogo_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextEditor> textEditor;
    ScopedPointer<Label> productVersionLabel;
    ScopedPointer<TextButton> nextStep;
    ScopedPointer<Label> installfiles;
    ScopedPointer<TextButton> browse;
    Image cachedImage_wusikLogo_png_1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (startupText)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_BA78ADCD1D9EAB08__
