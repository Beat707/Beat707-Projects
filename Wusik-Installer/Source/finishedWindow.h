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

#ifndef __JUCE_HEADER_8F3393DA8E687134__
#define __JUCE_HEADER_8F3393DA8E687134__

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
class finishedWindow  : public Component,
                        public ButtonListener
{
public:
    //==============================================================================
    finishedWindow ();
    ~finishedWindow();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	MainContentComponent* mainComp;
	void setVersion(String text) { productVersionLabel->setText(text, NotificationType::dontSendNotification); };
	TextButton* getExitButton() { return exitButton.get(); };
	void setInstallLabel(String text)
	{
		textEditor->setText(text, NotificationType::dontSendNotification);
		textEditor->repaint();
	};
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
    ScopedPointer<Label> productVersionLabel;
    ScopedPointer<TextEditor> textEditor;
    ScopedPointer<TextButton> exitButton;
    Image cachedImage_wusikLogo_png_1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (finishedWindow)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_8F3393DA8E687134__
