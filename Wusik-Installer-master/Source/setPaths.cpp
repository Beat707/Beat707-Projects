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

//[Headers] You can add your own extra header files here...
#include "MainComponent.h"
//[/Headers]

#include "setPaths.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
w4kWindow::w4kWindow ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (label1 = new Label ("Label1",
                                           TRANS("Plugins Folder:\n")));
    label1->setTooltip (TRANS("This will open the file browser so you can find the location where plugins goes: VST, AU, ..."));
    label1->setFont (Font (15.00f, Font::plain));
    label1->setJustificationType (Justification::centredLeft);
    label1->setEditable (false, false, false);
    label1->setColour (TextEditor::textColourId, Colours::black);
    label1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (pluginsBrowse = new TextButton ("pluginsBrowse"));
    pluginsBrowse->setTooltip (TRANS("This will open the file browser so you can find the location where plugins goes: VST, AU, ..."));
    pluginsBrowse->setButtonText (TRANS("Browse"));
    pluginsBrowse->addListener (this);
    pluginsBrowse->setColour (TextButton::buttonColourId, Colour (0xffebebeb));

    addAndMakeVisible (pluginsFolder = new Label ("pluginsFolder",
                                                  TRANS("...")));
    pluginsFolder->setTooltip (TRANS("This will open the file browser so you can find the location where plugins goes: VST, AU, ..."));
    pluginsFolder->setFont (Font (15.00f, Font::plain));
    pluginsFolder->setJustificationType (Justification::centred);
    pluginsFolder->setEditable (true, true, false);
    pluginsFolder->setColour (TextEditor::textColourId, Colours::black);
    pluginsFolder->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    pluginsFolder->addListener (this);

    addAndMakeVisible (label2 = new Label ("Label1",
                                           TRANS("Data Folder:")));
    label2->setTooltip (TRANS("The Wusik Data folder will hold the following required files: Skins, Modules, Presets, ..."));
    label2->setFont (Font (15.00f, Font::plain));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (dataBrowse = new TextButton ("dataBrowse"));
    dataBrowse->setTooltip (TRANS("The Wusik Data folder will hold the following required files: Skins, Modules, Presets, ..."));
    dataBrowse->setButtonText (TRANS("Browse"));
    dataBrowse->addListener (this);
    dataBrowse->setColour (TextButton::buttonColourId, Colour (0xffebebeb));

    addAndMakeVisible (dataFolder = new Label ("dataFolder",
                                               TRANS("...")));
    dataFolder->setTooltip (TRANS("The Wusik Data folder will hold the following required files: Skins, Modules, Presets, ..."));
    dataFolder->setFont (Font (15.00f, Font::plain));
    dataFolder->setJustificationType (Justification::centred);
    dataFolder->setEditable (true, true, false);
    dataFolder->setColour (TextEditor::textColourId, Colours::black);
    dataFolder->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    dataFolder->addListener (this);

    addAndMakeVisible (label4 = new Label ("Label1",
                                           TRANS("W.Station Folder:")));
    label4->setTooltip (TRANS("This is only used if you have Wusik Station installed. Otherwise, ignore this option."));
    label4->setFont (Font (15.00f, Font::plain));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (wsBrowse = new TextButton ("wsBrowse"));
    wsBrowse->setTooltip (TRANS("This is only used if you have Wusik Station installed. Otherwise, ignore this option."));
    wsBrowse->setButtonText (TRANS("Browse"));
    wsBrowse->addListener (this);
    wsBrowse->setColour (TextButton::buttonColourId, Colour (0xffebebeb));

    addAndMakeVisible (wsFolder = new Label ("wsFolder",
                                             TRANS("...")));
    wsFolder->setTooltip (TRANS("This is only used if you have Wusik Station installed. Otherwise, ignore this option."));
    wsFolder->setFont (Font (15.00f, Font::plain));
    wsFolder->setJustificationType (Justification::centred);
    wsFolder->setEditable (true, true, false);
    wsFolder->setColour (TextEditor::textColourId, Colours::black);
    wsFolder->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    wsFolder->addListener (this);

    addAndMakeVisible (installButton = new TextButton ("installButton"));
    installButton->setButtonText (TRANS("Install / Update"));
    installButton->addListener (this);
    installButton->setColour (TextButton::buttonColourId, Colour (0xffebebeb));

    addAndMakeVisible (productVersionLabel = new Label ("productVersionLabel",
                                                        TRANS("Wusik Installer")));
    productVersionLabel->setFont (Font (15.00f, Font::plain));
    productVersionLabel->setJustificationType (Justification::centred);
    productVersionLabel->setEditable (false, false, false);
    productVersionLabel->setColour (TextEditor::textColourId, Colours::black);
    productVersionLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (resetSoftware = new ToggleButton ("resetSoftware"));
    resetSoftware->setTooltip (TRANS("This will remove user settings store in the Main Settings.xml file under Wusik Data"));
    resetSoftware->setButtonText (TRANS("reset user settings"));
    resetSoftware->addListener (this);

    addAndMakeVisible (replaceFiles = new ToggleButton ("replaceFiles"));
    replaceFiles->setTooltip (TRANS("When not selected it will update the files by date: newer files will replace older files."));
    replaceFiles->setButtonText (TRANS("replace files"));
    replaceFiles->addListener (this);

    addAndMakeVisible (label3 = new Label ("Label1",
                                           TRANS("User Folder:")));
    label3->setTooltip (TRANS("This is a special User folder where you can have your own samples, SFZ, WAV and even user presets. Otherwise, ignore this option."));
    label3->setFont (Font (15.00f, Font::plain));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (userBrowse = new TextButton ("userBrowse"));
    userBrowse->setTooltip (TRANS("This is a special User folder where you can have your own samples, SFZ, WAV and even user presets. Otherwise, ignore this option."));
    userBrowse->setButtonText (TRANS("Browse"));
    userBrowse->addListener (this);
    userBrowse->setColour (TextButton::buttonColourId, Colour (0xffebebeb));

    addAndMakeVisible (userFolder = new Label ("userFolder",
                                               TRANS("...")));
    userFolder->setTooltip (TRANS("This is a special User folder where you can have your own samples, SFZ, WAV and even user presets. Otherwise, ignore this option."));
    userFolder->setFont (Font (15.00f, Font::plain));
    userFolder->setJustificationType (Justification::centred);
    userFolder->setEditable (true, true, false);
    userFolder->setColour (TextEditor::textColourId, Colours::black);
    userFolder->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    userFolder->addListener (this);

    cachedImage_wusikLogo_png_1 = ImageCache::getFromMemory (wusikLogo_png, wusikLogo_pngSize);

    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 520);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

w4kWindow::~w4kWindow()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label1 = nullptr;
    pluginsBrowse = nullptr;
    pluginsFolder = nullptr;
    label2 = nullptr;
    dataBrowse = nullptr;
    dataFolder = nullptr;
    label4 = nullptr;
    wsBrowse = nullptr;
    wsFolder = nullptr;
    installButton = nullptr;
    productVersionLabel = nullptr;
    resetSoftware = nullptr;
    replaceFiles = nullptr;
    label3 = nullptr;
    userBrowse = nullptr;
    userFolder = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void w4kWindow::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    g.setColour (Colour (0x1c959595));
    g.fillRect (0, 190, 600, 250);

    g.setColour (Colour (0x30dbdbdb));
    g.fillRect (136, 22, 352, 482);

    g.setColour (Colour (0xffd1d1d1));
    g.fillRect (20, 206, 568, 8);

    g.setColour (Colour (0xffd1d1d1));
    g.fillRect (20, 262, 568, 8);

    g.setColour (Colour (0xffd1d1d1));
    g.fillRect (20, 326, 568, 8);

    g.setColour (Colours::black);
    g.drawImage (cachedImage_wusikLogo_png_1,
                 196, 30, 220, 116,
                 0, 0, cachedImage_wusikLogo_png_1.getWidth(), cachedImage_wusikLogo_png_1.getHeight());

    g.setColour (Colour (0xffd1d1d1));
    g.fillRect (20, 382, 568, 8);

    g.setColour (Colour (0xffd1d1d1));
    g.fillRect (20, 438, 568, 8);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void w4kWindow::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    label1->setBounds (16, 224, 112, 24);
    pluginsBrowse->setBounds (512, 224, 72, 24);
    pluginsFolder->setBounds (138, 225, 328, 24);
    label2->setBounds (16, 287, 112, 24);
    dataBrowse->setBounds (512, 287, 72, 24);
    dataFolder->setBounds (138, 288, 328, 24);
    label4->setBounds (16, 342, 112, 24);
    wsBrowse->setBounds (512, 342, 72, 24);
    wsFolder->setBounds (138, 343, 328, 24);
    installButton->setBounds (232, 456, 152, 24);
    productVersionLabel->setBounds (232, 152, 160, 24);
    resetSoftware->setBounds (456, 480, 127, 24);
    replaceFiles->setBounds (456, 456, 127, 24);
    label3->setBounds (16, 400, 112, 24);
    userBrowse->setBounds (512, 400, 72, 24);
    userFolder->setBounds (138, 401, 328, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void w4kWindow::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == pluginsBrowse)
    {
        //[UserButtonCode_pluginsBrowse] -- add your button handler code here..
		FileChooser browseFolder("Locate where plugins should be stored", File(pluginsFolder->getText()));
		if (!browseFolder.browseForDirectory()) return;
		pluginsFolder->setText(browseFolder.getResult().getFullPathName(), NotificationType::dontSendNotification);
        //[/UserButtonCode_pluginsBrowse]
    }
    else if (buttonThatWasClicked == dataBrowse)
    {
        //[UserButtonCode_dataBrowse] -- add your button handler code here..
		FileChooser browseFolder("Locate where the data files should be stored", File(dataFolder->getText()));
		if (!browseFolder.browseForDirectory()) return;
		dataFolder->setText(browseFolder.getResult().getFullPathName(), NotificationType::dontSendNotification);
        //[/UserButtonCode_dataBrowse]
    }
    else if (buttonThatWasClicked == wsBrowse)
    {
        //[UserButtonCode_wsBrowse] -- add your button handler code here..
		FileChooser browseFolder("Locate the Wusik Station data folder", File(wsFolder->getText()));
		if (!browseFolder.browseForDirectory()) return;
		wsFolder->setText(browseFolder.getResult().getFullPathName(), NotificationType::dontSendNotification);
        //[/UserButtonCode_wsBrowse]
    }
    else if (buttonThatWasClicked == installButton)
    {
        //[UserButtonCode_installButton] -- add your button handler code here..
		mainComp->nextStageWindow();
        //[/UserButtonCode_installButton]
    }
    else if (buttonThatWasClicked == resetSoftware)
    {
        //[UserButtonCode_resetSoftware] -- add your button handler code here..
        //[/UserButtonCode_resetSoftware]
    }
    else if (buttonThatWasClicked == replaceFiles)
    {
        //[UserButtonCode_replaceFiles] -- add your button handler code here..
        //[/UserButtonCode_replaceFiles]
    }
    else if (buttonThatWasClicked == userBrowse)
    {
        //[UserButtonCode_userBrowse] -- add your button handler code here..
		FileChooser browseFolder("Locate an extra (optional) folder for 3rd party or user files", File(userFolder->getText()));
		if (!browseFolder.browseForDirectory()) return;
		userFolder->setText(browseFolder.getResult().getFullPathName(), NotificationType::dontSendNotification);
        //[/UserButtonCode_userBrowse]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void w4kWindow::labelTextChanged (Label* labelThatHasChanged)
{
    //[UserlabelTextChanged_Pre]
    //[/UserlabelTextChanged_Pre]

    if (labelThatHasChanged == pluginsFolder)
    {
        //[UserLabelCode_pluginsFolder] -- add your label text handling code here..
        //[/UserLabelCode_pluginsFolder]
    }
    else if (labelThatHasChanged == dataFolder)
    {
        //[UserLabelCode_dataFolder] -- add your label text handling code here..
        //[/UserLabelCode_dataFolder]
    }
    else if (labelThatHasChanged == wsFolder)
    {
        //[UserLabelCode_wsFolder] -- add your label text handling code here..
        //[/UserLabelCode_wsFolder]
    }
    else if (labelThatHasChanged == userFolder)
    {
        //[UserLabelCode_userFolder] -- add your label text handling code here..
        //[/UserLabelCode_userFolder]
    }

    //[UserlabelTextChanged_Post]
    //[/UserlabelTextChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="w4kWindow" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="600" initialHeight="520">
  <BACKGROUND backgroundColour="ffffffff">
    <RECT pos="0 190 600 250" fill="solid: 1c959595" hasStroke="0"/>
    <RECT pos="136 22 352 482" fill="solid: 30dbdbdb" hasStroke="0"/>
    <RECT pos="20 206 568 8" fill="solid: ffd1d1d1" hasStroke="0"/>
    <RECT pos="20 262 568 8" fill="solid: ffd1d1d1" hasStroke="0"/>
    <RECT pos="20 326 568 8" fill="solid: ffd1d1d1" hasStroke="0"/>
    <IMAGE pos="196 30 220 116" resource="wusikLogo_png" opacity="1" mode="0"/>
    <RECT pos="20 382 568 8" fill="solid: ffd1d1d1" hasStroke="0"/>
    <RECT pos="20 438 568 8" fill="solid: ffd1d1d1" hasStroke="0"/>
  </BACKGROUND>
  <LABEL name="Label1" id="68ffd23cb3d76910" memberName="label1" virtualName=""
         explicitFocusOrder="0" pos="16 224 112 24" tooltip="This will open the file browser so you can find the location where plugins goes: VST, AU, ..."
         edTextCol="ff000000" edBkgCol="0" labelText="Plugins Folder:&#10;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="pluginsBrowse" id="a2c5ebb11f02338d" memberName="pluginsBrowse"
              virtualName="" explicitFocusOrder="0" pos="512 224 72 24" tooltip="This will open the file browser so you can find the location where plugins goes: VST, AU, ..."
              bgColOff="ffebebeb" buttonText="Browse" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <LABEL name="pluginsFolder" id="c8c429eb6d0dff5f" memberName="pluginsFolder"
         virtualName="" explicitFocusOrder="0" pos="138 225 328 24" tooltip="This will open the file browser so you can find the location where plugins goes: VST, AU, ..."
         edTextCol="ff000000" edBkgCol="0" labelText="..." editableSingleClick="1"
         editableDoubleClick="1" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="Label1" id="3388ca352e3f2799" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="16 287 112 24" tooltip="The Wusik Data folder will hold the following required files: Skins, Modules, Presets, ..."
         edTextCol="ff000000" edBkgCol="0" labelText="Data Folder:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="dataBrowse" id="fd9fda75597cbc39" memberName="dataBrowse"
              virtualName="" explicitFocusOrder="0" pos="512 287 72 24" tooltip="The Wusik Data folder will hold the following required files: Skins, Modules, Presets, ..."
              bgColOff="ffebebeb" buttonText="Browse" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <LABEL name="dataFolder" id="9194ac5393bdb56" memberName="dataFolder"
         virtualName="" explicitFocusOrder="0" pos="138 288 328 24" tooltip="The Wusik Data folder will hold the following required files: Skins, Modules, Presets, ..."
         edTextCol="ff000000" edBkgCol="0" labelText="..." editableSingleClick="1"
         editableDoubleClick="1" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="Label1" id="5b71312d18becbb7" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="16 342 112 24" tooltip="This is only used if you have Wusik Station installed. Otherwise, ignore this option."
         edTextCol="ff000000" edBkgCol="0" labelText="W.Station Folder:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="wsBrowse" id="7404f9a016110964" memberName="wsBrowse" virtualName=""
              explicitFocusOrder="0" pos="512 342 72 24" tooltip="This is only used if you have Wusik Station installed. Otherwise, ignore this option."
              bgColOff="ffebebeb" buttonText="Browse" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <LABEL name="wsFolder" id="4159b5303bd5e9ba" memberName="wsFolder" virtualName=""
         explicitFocusOrder="0" pos="138 343 328 24" tooltip="This is only used if you have Wusik Station installed. Otherwise, ignore this option."
         edTextCol="ff000000" edBkgCol="0" labelText="..." editableSingleClick="1"
         editableDoubleClick="1" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="installButton" id="d629872511ba6334" memberName="installButton"
              virtualName="" explicitFocusOrder="0" pos="232 456 152 24" bgColOff="ffebebeb"
              buttonText="Install / Update" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <LABEL name="productVersionLabel" id="9f30619c7f6469a6" memberName="productVersionLabel"
         virtualName="" explicitFocusOrder="0" pos="232 152 160 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Wusik Installer" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <TOGGLEBUTTON name="resetSoftware" id="64cd330633600a8c" memberName="resetSoftware"
                virtualName="" explicitFocusOrder="0" pos="456 480 127 24" tooltip="This will remove user settings store in the Main Settings.xml file under Wusik Data"
                buttonText="reset user settings" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="replaceFiles" id="c533e54f5e6273db" memberName="replaceFiles"
                virtualName="" explicitFocusOrder="0" pos="456 456 127 24" tooltip="When not selected it will update the files by date: newer files will replace older files."
                buttonText="replace files" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <LABEL name="Label1" id="6bb158080015d9b8" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="16 400 112 24" tooltip="This is a special User folder where you can have your own samples, SFZ, WAV and even user presets. Otherwise, ignore this option."
         edTextCol="ff000000" edBkgCol="0" labelText="User Folder:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="userBrowse" id="480ad3553fb77fbe" memberName="userBrowse"
              virtualName="" explicitFocusOrder="0" pos="512 400 72 24" tooltip="This is a special User folder where you can have your own samples, SFZ, WAV and even user presets. Otherwise, ignore this option."
              bgColOff="ffebebeb" buttonText="Browse" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <LABEL name="userFolder" id="13e662b82beba990" memberName="userFolder"
         virtualName="" explicitFocusOrder="0" pos="138 401 328 24" tooltip="This is a special User folder where you can have your own samples, SFZ, WAV and even user presets. Otherwise, ignore this option."
         edTextCol="ff000000" edBkgCol="0" labelText="..." editableSingleClick="1"
         editableDoubleClick="1" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: wusikLogo_png, 7649, "WusikLogo.png"
static const unsigned char resource_w4kWindow_wusikLogo_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,220,0,0,0,116,8,6,0,0,0,238,21,92,2,0,0,0,76,122,84,88,116,82,97,119,32,112,114,111,
102,105,108,101,32,116,121,112,101,32,101,120,105,102,0,0,120,218,227,74,173,200,76,227,82,0,3,99,35,46,19,83,115,11,51,75,51,51,3,32,48,177,52,177,52,74,4,50,44,12,32,192,16,136,140,128,164,37,132,13,
19,67,6,92,0,139,24,15,146,203,144,108,75,0,0,0,9,112,72,89,115,0,0,117,48,0,0,117,48,1,221,51,114,205,0,0,0,7,116,73,77,69,7,222,6,16,14,17,40,87,75,211,32,0,0,0,7,116,69,88,116,65,117,116,104,111,114,
0,169,174,204,72,0,0,0,10,116,69,88,116,67,111,112,121,114,105,103,104,116,0,172,15,204,58,0,0,0,14,116,69,88,116,67,114,101,97,116,105,111,110,32,116,105,109,101,0,53,247,15,9,0,0,0,9,116,69,88,116,83,
111,102,116,119,97,114,101,0,93,112,255,58,0,0,0,11,116,69,88,116,68,105,115,99,108,97,105,109,101,114,0,183,192,180,143,0,0,0,8,116,69,88,116,87,97,114,110,105,110,103,0,192,27,230,135,0,0,0,7,116,69,
88,116,83,111,117,114,99,101,0,245,255,131,235,0,0,0,8,116,69,88,116,67,111,109,109,101,110,116,0,246,204,150,191,0,0,0,6,116,69,88,116,84,105,116,108,101,0,168,238,210,39,0,0,28,108,73,68,65,84,120,156,
237,157,121,84,84,71,214,192,239,235,102,145,29,20,4,186,251,245,66,216,161,27,17,84,16,144,85,5,68,20,27,217,196,64,140,49,102,61,56,137,57,201,124,9,137,38,209,248,157,28,29,199,227,196,157,77,32,42,
42,42,6,81,130,203,48,209,193,56,198,79,19,149,9,161,23,26,16,21,21,197,5,133,247,253,145,224,48,40,178,85,213,123,141,254,206,185,127,228,132,220,123,171,210,245,234,190,122,183,238,165,24,134,129,23,
188,224,5,100,224,177,237,192,11,94,240,60,97,128,82,89,69,69,133,233,187,239,190,219,210,212,212,100,134,82,239,211,48,53,53,237,122,235,173,183,60,62,249,228,147,90,220,182,216,68,169,84,190,114,228,
200,145,109,195,213,67,81,20,76,159,62,125,193,206,157,59,115,80,248,197,69,106,106,106,120,175,190,250,106,157,90,173,150,226,208,175,80,40,190,173,174,174,78,29,150,18,134,97,144,138,191,191,127,14,
0,48,36,36,34,34,98,21,106,255,185,38,158,158,158,255,68,53,95,30,30,30,255,100,123,60,56,229,253,247,223,15,53,52,52,196,242,91,51,48,48,96,22,45,90,52,109,184,62,34,15,41,167,76,153,178,205,200,200,
8,181,218,167,82,95,95,159,86,93,93,205,39,98,140,5,222,127,255,125,185,74,165,154,136,74,159,74,165,154,144,157,157,237,135,74,31,215,56,122,244,104,234,195,135,15,177,232,166,105,90,147,158,158,94,57,
108,69,56,158,52,174,174,174,151,129,192,14,103,96,96,192,188,246,218,107,195,126,234,112,85,2,3,3,215,163,158,51,127,127,255,111,216,30,23,14,41,47,47,55,18,139,197,55,113,253,214,34,34,34,150,163,240,
19,203,161,137,76,38,27,246,59,199,64,120,244,232,17,156,57,115,102,33,9,91,164,249,246,219,111,205,85,42,213,240,222,23,158,66,83,83,211,252,237,219,183,91,160,214,203,54,197,197,197,179,26,26,26,172,
112,232,182,180,180,236,156,58,117,106,14,18,101,56,158,54,223,124,243,141,189,141,141,205,3,32,176,203,217,218,218,182,231,229,229,141,97,251,9,139,90,148,74,101,38,69,81,88,230,108,198,140,25,139,216,
30,31,106,145,203,229,123,113,253,198,228,114,249,247,168,252,196,178,195,45,94,188,248,10,77,211,229,56,116,247,230,250,245,235,166,187,119,239,78,38,97,139,36,23,46,92,88,200,48,12,22,221,117,117,117,
139,177,40,102,137,130,130,130,49,205,205,205,177,56,116,83,20,5,94,94,94,155,80,233,195,246,29,206,207,207,111,11,159,143,255,60,131,97,24,184,116,233,82,6,118,67,4,249,248,227,143,21,90,173,118,50,46,
253,245,245,245,190,239,189,247,222,36,92,250,73,83,82,82,146,114,237,218,53,44,39,117,66,161,176,117,222,188,121,7,80,233,195,182,224,82,83,83,15,209,52,221,136,75,127,79,26,26,26,38,100,103,103,123,
147,176,69,130,170,170,170,215,239,222,189,75,225,210,255,224,193,3,168,174,174,126,3,151,126,210,212,214,214,166,226,138,6,104,154,46,140,139,139,187,139,74,31,182,5,55,117,234,212,71,82,169,116,59,46,
253,61,185,123,247,46,85,85,85,181,128,132,45,220,148,149,149,141,106,104,104,152,135,219,78,125,125,125,226,150,45,91,70,227,182,131,155,175,191,254,218,69,167,211,97,137,6,76,76,76,96,242,228,201,91,
144,42,197,249,34,251,217,103,159,185,88,88,88,116,1,129,195,19,169,84,122,165,178,178,210,128,237,151,247,225,10,206,195,146,222,18,31,31,159,197,246,120,135,43,17,17,17,203,113,205,143,135,135,199,25,
212,254,98,205,165,252,244,211,79,255,45,145,72,254,129,211,70,55,90,173,118,108,81,81,81,28,9,91,56,249,229,151,95,22,48,12,158,240,168,55,23,47,94,212,251,79,42,26,141,6,249,167,147,110,156,157,157,
209,238,110,64,32,121,217,221,221,125,35,69,97,123,29,121,76,103,103,39,156,62,125,90,175,15,79,150,45,91,38,215,104,52,33,164,236,169,213,106,175,37,75,150,132,146,178,135,154,165,75,151,6,104,181,90,
103,28,186,29,28,28,238,207,156,57,243,91,228,138,113,111,249,5,5,5,102,66,161,16,91,6,64,79,25,61,122,244,253,141,27,55,142,101,59,204,25,170,76,154,52,105,13,137,121,234,41,190,190,190,133,108,143,123,
24,243,245,55,92,243,226,239,239,191,29,135,207,216,119,184,244,244,244,118,161,80,88,140,219,14,0,64,107,107,171,241,190,125,251,244,114,151,43,46,46,54,105,106,106,34,238,187,78,167,75,44,40,40,24,75,
218,238,112,169,168,168,48,108,108,108,156,139,67,183,129,129,1,4,6,6,98,201,150,34,114,31,46,36,36,100,155,177,177,49,9,83,240,235,175,191,206,39,98,8,49,123,247,238,85,106,181,90,27,210,118,91,90,90,
140,242,242,242,244,110,206,118,236,216,17,219,216,216,104,139,67,183,84,42,253,85,169,84,30,195,161,155,200,130,75,73,73,249,209,201,201,233,103,18,182,52,26,141,60,59,59,27,89,134,61,41,46,94,188,184,
136,97,200,28,150,244,70,163,209,188,254,195,15,63,232,213,173,139,159,126,250,233,229,206,206,78,44,186,105,154,206,9,13,13,237,194,161,155,200,130,243,247,247,103,196,98,241,102,18,182,238,223,191,15,
21,21,21,122,21,86,254,249,207,127,118,83,169,84,196,14,75,122,83,95,95,239,146,159,159,31,206,150,253,193,146,155,155,107,221,208,208,16,131,67,247,232,209,163,187,148,74,101,1,14,221,0,4,75,44,204,158,
61,187,104,236,216,177,120,46,43,245,162,185,185,57,181,188,188,156,76,12,139,128,234,234,234,55,239,220,185,195,154,253,71,143,30,65,77,77,141,222,228,87,150,149,149,37,93,187,118,205,4,135,110,129,64,
240,221,91,111,189,165,197,161,27,128,224,130,91,188,120,241,85,71,71,199,61,36,108,53,52,52,216,20,23,23,207,33,97,107,184,148,151,151,155,252,246,219,111,41,108,251,161,86,171,103,174,94,189,218,145,
109,63,6,194,165,75,151,230,227,8,191,121,60,30,140,31,63,62,23,185,226,158,54,112,42,239,205,228,201,147,243,72,36,52,119,117,117,193,217,179,103,245,226,32,96,251,246,237,115,154,154,154,88,63,37,108,
109,109,53,58,116,232,208,171,108,251,209,31,95,125,245,149,84,171,213,6,226,208,45,22,139,91,230,205,155,183,31,135,238,110,136,46,184,148,148,148,35,18,137,68,69,194,150,78,167,155,182,126,253,122,154,
132,173,225,112,246,236,217,87,187,186,176,188,159,15,154,186,186,186,133,213,213,213,248,179,20,134,193,247,223,127,63,175,173,173,13,203,83,219,193,193,33,127,218,180,105,88,95,123,136,46,184,41,83,
166,60,18,137,68,249,36,108,221,184,113,131,191,123,247,110,236,73,192,195,225,171,175,190,242,208,106,181,156,201,244,208,104,52,146,252,252,124,44,135,17,40,56,119,238,28,165,86,171,211,113,232,54,51,
51,99,98,98,98,114,112,232,254,47,72,103,7,172,93,187,214,201,202,202,170,19,8,100,81,184,184,184,92,58,121,242,36,197,118,70,68,95,18,28,28,188,154,196,60,12,70,20,10,197,1,182,231,165,47,249,211,159,
254,52,222,216,216,24,203,184,221,220,220,170,73,140,129,120,33,216,119,223,125,247,55,161,80,120,132,132,45,181,90,237,182,123,247,238,96,18,182,6,75,89,89,153,145,86,171,229,220,123,166,70,163,137,94,
187,118,173,132,109,63,158,198,169,83,167,50,30,60,120,128,92,239,31,183,186,115,145,43,126,10,172,84,94,246,245,245,45,224,241,240,155,238,232,232,128,170,170,170,151,177,27,26,2,133,133,133,74,173,86,
139,37,83,98,56,220,188,121,211,96,207,158,61,156,59,60,57,116,232,144,145,78,167,195,82,74,195,222,222,254,78,106,106,234,14,28,186,159,128,141,208,224,143,146,102,215,129,64,136,36,18,137,110,239,222,
189,219,140,237,112,168,183,120,121,121,125,79,98,252,67,17,169,84,218,112,248,240,97,67,182,231,168,167,44,92,184,48,150,199,227,97,25,239,196,137,19,115,72,141,131,149,29,46,58,58,186,67,32,16,20,146,
176,165,211,233,204,139,139,139,19,73,216,26,40,43,86,172,112,213,104,52,156,205,236,208,104,52,194,188,188,188,25,108,251,209,147,179,103,207,190,140,227,52,215,216,216,24,130,130,130,144,223,123,235,
11,214,154,121,68,69,69,109,53,53,53,197,110,135,97,24,184,124,249,114,38,118,67,131,224,224,193,131,11,110,223,190,205,217,227,247,174,174,46,56,127,254,60,103,50,79,182,109,219,102,165,211,233,226,113,
232,166,105,250,226,234,213,171,137,92,146,6,0,118,66,202,110,113,117,117,173,1,2,33,146,149,149,85,215,170,85,171,156,216,14,139,24,134,129,146,146,146,81,98,177,184,153,196,184,135,59,103,203,150,45,
123,137,237,249,98,24,6,230,204,153,147,137,107,156,209,209,209,75,73,142,133,213,118,85,30,30,30,121,36,110,131,223,186,117,139,42,43,43,227,196,137,96,105,105,105,188,86,171,181,103,219,143,254,184,
117,235,22,117,228,200,17,78,148,96,168,173,173,197,242,237,205,198,198,230,126,92,92,28,182,68,229,167,194,230,147,43,63,63,223,218,206,206,238,14,16,120,98,59,59,59,171,142,30,61,202,103,251,105,173,
80,40,170,72,140,23,133,208,52,221,92,86,86,54,138,205,249,90,179,102,141,216,198,198,6,75,33,170,113,227,198,149,146,30,15,171,59,220,252,249,243,111,74,36,18,172,185,107,221,212,215,215,75,74,74,74,
88,61,168,88,190,124,185,83,125,125,125,40,155,62,12,6,157,78,103,95,84,84,52,139,77,31,246,239,223,159,116,227,198,13,228,97,16,159,207,135,241,227,199,19,185,50,214,19,214,59,160,6,6,6,110,54,52,52,
196,110,167,179,179,19,78,158,60,153,129,221,208,51,168,170,170,122,253,246,237,219,172,207,249,64,233,234,234,130,115,231,206,177,118,120,82,83,83,67,233,116,58,44,223,81,197,98,113,99,74,74,202,97,28,
186,159,5,197,48,12,0,0,68,68,68,172,105,108,108,196,255,203,239,237,0,69,81,99,198,140,81,92,187,118,237,2,110,91,50,153,140,223,210,210,210,209,222,222,142,219,212,19,240,249,124,112,117,117,165,46,
94,188,136,108,193,153,152,152,88,25,26,26,138,219,218,218,206,163,210,217,155,49,99,198,48,166,166,166,148,86,171,197,115,189,250,25,200,100,50,94,75,75,11,175,189,189,157,65,173,123,236,216,177,222,
45,45,45,231,80,235,125,26,158,158,158,15,246,236,217,243,30,0,252,231,29,206,213,213,149,181,119,5,111,111,239,19,164,98,104,103,103,231,243,108,140,81,38,147,53,28,57,114,196,8,229,88,226,227,227,23,
203,229,114,236,185,143,190,190,190,187,217,152,51,185,92,94,130,99,60,127,249,203,95,68,214,214,214,29,164,198,17,18,18,194,112,226,29,174,27,149,74,21,180,108,217,50,44,245,5,123,227,236,236,76,246,
84,234,15,68,34,209,230,168,168,168,14,148,58,47,95,190,188,184,161,161,33,118,245,234,213,98,148,122,123,19,16,16,176,209,192,0,105,59,248,126,225,241,120,224,231,231,135,165,218,91,89,89,89,230,205,
155,55,137,71,115,0,28,120,135,3,0,184,115,231,14,239,216,177,99,175,145,176,21,31,31,159,111,99,99,131,244,135,223,31,214,214,214,204,140,25,51,114,81,234,204,202,202,154,164,82,169,124,110,220,184,193,
219,187,119,239,43,40,117,247,38,61,61,189,82,42,149,214,226,180,209,27,145,72,116,51,37,37,5,89,215,154,158,212,215,215,103,226,208,59,32,184,16,82,194,239,33,87,227,241,227,199,137,244,6,144,203,229,
251,73,142,77,161,80,124,135,122,12,126,126,126,185,221,250,157,156,156,26,14,30,60,136,53,247,49,58,58,250,35,146,115,230,239,239,191,1,199,56,222,121,231,157,112,67,67,67,162,191,109,206,133,148,0,0,
26,141,198,49,63,63,127,58,9,91,190,190,190,57,36,110,43,0,252,30,26,249,250,250,34,107,232,7,0,144,159,159,111,165,213,106,31,231,135,170,84,42,225,206,157,59,177,94,28,77,74,74,218,98,107,107,123,31,
167,141,110,12,13,13,33,42,42,10,125,153,113,0,168,174,174,94,240,240,33,145,90,86,79,135,43,59,28,0,48,62,62,62,68,62,68,86,84,84,24,210,52,125,133,196,152,164,82,169,238,240,225,195,72,119,238,184,184,
184,37,189,59,236,248,248,248,28,196,61,111,190,190,190,133,36,230,204,217,217,249,183,234,234,106,228,23,135,139,139,139,109,236,237,237,239,146,254,93,115,114,135,3,0,208,106,181,209,27,54,108,192,158,
246,52,109,218,180,135,98,177,152,200,253,39,154,166,115,167,78,157,250,8,165,206,203,151,47,63,209,97,71,165,82,77,95,177,98,133,4,165,157,222,4,6,6,110,48,50,194,210,104,244,191,112,116,116,44,8,10,
10,98,250,255,203,193,81,92,92,156,222,210,210,130,165,188,222,64,225,212,130,107,109,109,53,46,45,45,205,32,97,43,44,44,12,251,109,5,75,75,203,206,132,132,4,164,217,12,89,89,89,161,106,181,250,137,110,
175,183,110,221,226,87,86,86,98,109,74,185,112,225,194,106,169,84,122,17,167,13,115,115,115,136,141,141,45,194,161,251,242,229,203,88,202,235,13,10,46,133,148,0,192,184,184,184,252,66,34,172,100,24,6,
220,220,220,254,133,115,44,222,222,222,21,168,125,30,63,126,252,246,190,236,73,165,82,109,101,101,37,15,231,156,197,198,198,190,131,115,206,220,221,221,79,226,240,59,59,59,123,188,137,137,9,43,191,105,
206,134,148,0,0,106,181,218,99,233,210,165,68,202,126,187,186,186,230,226,210,205,231,243,97,220,184,113,27,80,234,92,183,110,157,157,78,167,235,179,99,140,86,171,21,21,20,20,96,189,56,58,107,214,172,
66,7,7,7,108,135,39,50,153,12,203,197,228,242,242,242,140,123,247,238,225,80,61,40,56,183,224,58,58,58,224,216,177,99,68,250,117,207,154,53,171,200,214,214,22,125,85,26,0,16,137,68,205,243,231,207,47,
67,169,243,192,129,3,233,87,174,92,233,243,37,170,179,179,19,206,158,61,251,6,74,155,189,89,180,104,81,171,72,36,194,114,130,56,118,236,216,78,28,77,16,75,75,75,77,154,155,155,185,81,50,145,107,33,37,
0,48,2,129,160,125,215,174,93,22,36,194,74,95,95,223,93,56,198,16,26,26,186,18,165,159,53,53,53,124,23,23,151,218,254,236,90,89,89,61,252,235,95,255,42,197,57,103,75,150,44,9,192,81,174,78,161,80,236,
199,225,111,82,82,82,42,169,190,233,79,19,78,135,148,0,0,77,77,77,166,133,133,133,216,122,55,247,196,207,207,47,7,117,249,117,75,75,203,174,240,240,112,164,135,37,57,57,57,83,234,235,235,93,250,251,187,
91,183,110,25,148,148,148,96,141,16,146,147,147,255,41,147,201,144,38,254,82,20,5,222,222,222,88,210,238,46,94,188,248,42,195,48,56,84,15,30,46,238,112,0,192,184,185,185,253,157,196,14,87,89,89,105,32,
149,74,117,40,125,87,40,20,149,168,253,28,63,126,252,128,119,98,169,84,170,169,172,172,196,154,181,19,23,23,183,24,229,174,65,211,116,91,73,73,9,242,234,106,43,87,174,148,90,91,91,99,185,192,58,80,225,
252,14,7,0,208,208,208,16,188,124,249,242,39,142,191,81,19,25,25,249,72,44,22,35,123,178,82,20,5,114,185,28,233,97,201,250,245,235,237,85,42,213,204,129,254,189,70,163,161,11,11,11,177,20,221,233,38,53,
53,181,208,193,193,225,54,42,125,14,14,14,59,149,74,37,242,123,83,229,229,229,153,55,111,222,228,76,193,38,206,46,184,246,246,118,40,47,47,207,32,97,43,50,50,50,215,220,220,28,73,204,33,22,139,175,36,
39,39,35,77,186,61,112,224,192,194,214,214,214,1,247,187,235,234,234,130,51,103,206,96,45,230,154,150,150,118,155,166,105,36,7,28,6,6,6,48,105,210,164,237,40,116,245,164,188,188,220,160,161,161,1,107,
98,247,96,33,123,231,98,144,52,55,55,103,236,223,191,255,99,115,115,115,172,217,253,193,193,193,151,119,238,220,89,243,243,207,63,79,26,174,46,145,72,180,205,204,204,172,163,170,170,10,201,83,149,207,
231,243,106,107,107,7,253,163,209,104,52,209,43,86,172,144,4,4,4,104,80,248,241,52,34,34,34,54,92,184,112,225,181,187,119,239,14,75,143,68,34,81,207,158,61,251,31,168,230,172,155,61,123,246,132,171,213,
106,172,87,151,6,203,227,27,223,110,110,110,76,109,45,209,27,24,253,66,81,20,140,25,51,6,72,36,26,183,183,183,3,138,155,224,54,54,54,128,178,100,4,195,48,112,253,250,117,24,74,17,84,212,190,60,141,235,
215,175,195,112,123,109,155,152,152,128,133,133,5,34,143,254,195,253,251,247,161,173,173,13,185,222,193,18,18,18,2,39,78,156,160,0,56,190,195,49,12,3,215,174,93,99,219,141,65,113,227,198,13,182,93,120,
12,151,124,121,22,247,238,221,3,46,124,148,38,1,103,223,225,94,240,130,145,200,139,5,247,130,23,16,132,211,33,101,55,50,153,236,106,104,104,232,140,206,206,78,172,189,121,41,138,226,157,56,113,98,167,
74,165,146,14,246,191,13,12,12,220,234,236,236,252,13,42,95,76,76,76,140,42,42,42,14,170,213,106,155,225,232,49,55,55,103,50,50,50,230,180,181,181,105,81,249,214,27,99,99,99,81,113,113,113,233,96,223,
129,109,108,108,30,38,38,38,70,223,191,127,255,22,74,127,76,76,76,252,115,114,114,54,176,122,209,180,47,184,250,225,187,167,24,26,26,50,111,190,249,102,56,137,15,225,97,97,97,159,13,214,63,115,115,243,
206,149,43,87,186,161,244,35,51,51,51,14,85,123,166,136,136,136,47,112,207,155,183,183,119,245,96,253,242,241,241,65,126,155,130,97,24,8,8,8,216,194,246,111,182,167,232,197,135,239,158,60,124,248,16,106,
106,106,136,212,185,143,140,140,204,179,178,178,26,212,55,57,137,68,114,226,195,15,63,188,140,210,143,159,126,250,233,77,84,237,153,234,234,234,50,42,42,42,176,254,191,246,244,244,220,56,152,62,17,20,
69,129,167,167,103,30,106,63,118,238,220,105,170,86,171,149,168,245,34,67,31,118,56,0,96,236,236,236,218,55,109,218,52,154,196,46,39,151,203,143,15,212,47,138,162,152,228,228,228,20,148,246,87,172,88,
33,181,180,180,68,86,55,145,207,231,51,153,153,153,9,56,231,108,255,254,253,198,18,137,100,192,77,54,29,29,29,111,149,148,148,152,163,246,35,41,41,41,147,205,68,229,167,137,222,237,112,0,0,87,175,94,53,
45,43,43,35,146,208,236,238,238,190,101,160,79,107,161,80,120,53,57,57,185,20,165,253,202,202,202,69,109,109,109,200,62,160,117,118,118,194,233,211,167,177,102,158,204,156,57,243,129,64,32,216,54,208,
191,23,139,197,7,148,74,229,29,212,126,156,63,127,254,137,242,19,92,66,111,22,28,192,239,181,60,72,216,73,72,72,40,21,8,4,3,202,19,148,74,165,219,19,18,18,144,93,200,60,124,248,48,191,174,174,46,19,149,
190,110,116,58,93,204,218,181,107,165,168,245,246,36,62,62,126,179,185,185,121,191,95,193,13,12,12,192,199,199,39,7,181,253,85,171,86,185,107,181,218,32,212,122,145,162,47,33,37,0,48,166,166,166,204,7,
31,124,160,32,17,86,250,251,251,111,238,207,31,115,115,243,174,143,62,250,8,233,97,73,70,70,134,146,207,231,99,153,191,160,160,160,101,184,231,205,219,219,187,223,222,229,82,169,180,225,196,137,19,200,
91,135,133,134,134,254,47,219,191,209,167,137,94,134,148,0,0,119,239,222,133,227,199,143,19,169,208,28,28,28,156,219,95,90,148,84,42,253,251,138,21,43,144,30,150,156,62,125,122,225,112,83,165,250,162,
177,177,241,213,67,135,14,97,45,187,229,235,235,187,161,191,84,60,154,166,191,13,9,9,65,58,200,99,199,142,241,52,26,13,39,154,110,62,11,189,90,112,0,0,58,157,46,173,180,180,116,192,153,243,67,37,46,46,
238,7,153,76,214,103,114,41,69,81,224,225,225,177,17,165,205,85,171,86,189,212,208,208,48,13,165,206,158,168,213,106,97,113,113,113,44,46,253,0,0,73,73,73,165,34,145,168,185,175,127,111,98,98,194,76,153,
50,37,31,181,221,194,194,194,120,141,70,227,128,90,47,114,244,41,164,4,0,134,199,227,49,243,230,205,155,71,34,172,140,136,136,248,176,47,63,196,98,241,245,93,187,118,153,160,180,55,121,242,228,175,112,
207,159,66,161,192,94,48,54,60,60,124,101,95,246,93,92,92,206,97,10,101,247,178,253,219,236,75,244,54,164,4,120,220,36,144,200,225,73,108,108,108,193,232,209,163,159,250,49,76,32,16,228,37,38,38,34,203,
184,221,187,119,239,40,157,78,135,253,238,150,90,173,158,190,114,229,74,9,78,27,145,145,145,155,254,184,101,253,4,78,78,78,200,107,78,110,220,184,209,177,169,169,9,107,181,50,100,232,219,14,7,0,140,165,
165,229,163,53,107,214,188,68,98,151,147,203,229,223,245,182,111,102,102,214,181,108,217,50,111,148,118,146,147,147,147,72,125,63,10,15,15,255,28,247,188,41,20,138,131,189,237,90,91,91,63,90,183,110,157,
16,181,173,136,136,136,15,216,254,77,62,75,244,122,135,3,0,104,107,107,227,239,219,183,143,200,77,94,47,47,175,220,222,223,228,196,98,113,117,118,118,54,210,142,173,63,255,252,243,27,12,195,160,84,217,
39,245,245,245,11,42,43,43,177,230,209,250,248,248,60,113,120,34,20,10,143,189,253,246,219,58,148,118,126,252,241,71,158,86,171,229,212,173,238,103,161,151,11,14,0,64,163,209,204,63,117,234,20,118,255,
147,146,146,14,208,52,221,218,253,207,60,30,15,228,114,249,86,148,54,178,179,179,221,213,106,117,24,74,157,207,66,163,209,8,138,138,138,226,112,218,72,79,79,47,151,74,165,143,19,166,41,138,2,47,47,47,
228,69,94,119,236,216,17,162,82,169,220,81,235,197,133,222,46,56,181,90,45,206,203,203,195,222,222,42,33,33,225,158,131,131,195,227,247,14,71,71,199,27,169,169,169,72,27,129,28,61,122,116,209,237,219,
200,234,241,244,75,87,87,23,156,62,125,122,17,78,27,211,166,77,123,36,18,137,30,151,10,116,112,112,104,87,42,149,187,80,219,169,170,170,202,224,228,173,128,62,208,219,5,215,217,217,9,63,252,240,67,38,
9,91,83,167,78,45,48,54,254,253,75,132,68,34,41,156,61,123,54,178,204,146,125,251,246,141,170,175,175,39,254,253,72,171,213,78,251,250,235,175,101,56,109,36,38,38,230,218,216,216,60,2,0,176,183,183,223,
151,148,148,132,52,149,43,55,55,215,162,185,185,57,5,165,78,236,232,227,161,73,183,216,217,217,117,108,218,180,201,150,196,225,137,155,155,219,57,51,51,179,174,143,63,254,24,245,97,201,124,182,146,109,
67,67,67,191,36,112,120,178,151,207,231,51,175,191,254,122,44,106,221,113,113,113,11,217,254,13,14,68,244,254,208,164,155,171,87,175,26,150,148,148,164,147,176,37,145,72,114,37,18,201,201,207,63,255,28,
233,97,201,185,115,231,22,145,58,44,233,141,90,173,206,60,114,228,8,218,178,211,189,8,12,12,220,228,228,228,116,37,57,57,249,16,106,221,117,117,117,88,19,178,113,240,184,106,87,72,72,72,173,86,171,197,
91,226,9,3,158,158,158,42,91,91,219,77,128,57,60,30,53,106,212,232,123,247,238,141,167,40,170,18,149,78,129,64,48,234,192,129,3,255,211,222,222,206,74,161,82,30,143,7,137,137,137,27,155,155,155,177,221,
6,167,40,138,98,24,102,58,0,32,93,112,246,246,246,198,229,229,229,31,223,185,115,135,51,69,94,251,98,220,184,113,247,75,75,75,61,0,0,144,110,241,108,200,214,173,91,45,237,237,237,219,129,3,161,195,96,
69,161,80,20,177,61,127,147,39,79,254,156,237,121,24,138,248,248,248,236,101,123,238,134,34,122,29,82,2,0,44,88,176,160,77,32,16,148,176,237,199,96,49,52,52,132,208,208,80,164,159,23,134,194,220,185,115,
183,89,89,89,97,173,21,131,26,62,159,15,19,39,78,68,218,44,133,24,108,175,120,20,178,100,201,146,112,35,35,35,214,159,186,131,17,23,23,151,139,56,26,199,15,69,60,61,61,159,200,166,225,178,200,100,50,45,
142,235,61,47,118,184,1,178,122,245,234,163,98,177,248,223,108,251,49,24,156,157,157,183,226,104,28,63,20,38,78,156,184,13,117,203,46,156,200,100,178,2,212,215,123,72,49,34,22,28,0,128,147,147,19,242,
130,52,184,176,179,179,187,59,119,238,220,1,151,35,192,77,122,122,122,169,72,36,106,98,219,143,129,96,97,97,209,25,17,17,177,133,109,63,134,12,219,91,44,42,89,183,110,157,208,218,218,250,1,112,32,228,
233,79,252,252,252,182,179,61,95,189,37,60,60,124,5,219,243,50,16,241,246,246,62,202,246,92,13,71,70,204,14,247,246,219,111,235,36,18,201,17,182,253,232,15,99,99,99,8,10,10,250,27,219,126,244,38,50,50,
114,179,165,165,37,167,195,180,63,74,235,233,231,97,73,55,108,175,120,148,242,242,203,47,199,227,170,7,130,74,220,220,220,46,176,61,79,125,137,66,161,56,196,246,252,60,75,132,66,97,43,142,46,169,36,101,
196,236,112,0,0,105,105,105,229,34,145,232,10,219,126,60,139,151,94,122,105,61,219,62,244,133,92,46,255,27,137,214,96,67,69,44,22,239,192,209,37,149,36,220,157,221,33,48,125,250,244,135,18,137,4,75,99,
118,20,56,56,56,220,75,72,72,64,122,211,0,37,105,105,105,135,36,18,9,39,15,79,70,141,26,5,193,193,193,250,29,78,194,8,91,112,0,0,193,193,193,155,81,181,15,70,141,72,36,42,90,184,112,97,107,255,127,201,
14,177,177,177,29,52,77,115,230,244,180,39,50,153,236,92,106,106,234,89,182,253,24,46,35,110,193,125,249,229,151,181,82,169,244,20,219,126,244,198,216,216,24,194,194,194,56,255,132,142,137,137,217,98,
109,109,205,185,7,150,88,44,222,228,235,235,203,57,191,6,203,136,91,112,0,0,110,110,110,155,7,211,88,130,4,50,153,236,108,90,90,90,13,219,126,244,199,135,31,126,168,18,8,4,7,217,246,163,39,246,246,246,
15,102,207,158,205,217,80,124,48,140,200,5,55,103,206,156,221,142,142,142,228,174,80,247,3,69,81,224,226,226,162,55,79,232,9,19,38,228,112,233,240,68,40,20,238,89,188,120,241,117,182,253,64,1,119,102,
21,33,105,105,105,109,34,145,104,39,219,126,116,227,232,232,120,59,61,61,125,59,219,126,12,148,249,243,231,239,151,72,36,125,22,115,37,9,159,207,135,192,192,192,28,182,253,64,197,136,92,112,0,0,65,65,
65,91,141,140,176,86,245,30,48,34,145,168,24,117,121,1,156,68,70,70,62,226,202,225,137,147,147,147,74,169,84,86,177,237,7,42,70,236,130,75,76,76,60,37,147,201,126,97,219,15,19,19,19,38,60,60,124,3,219,
126,12,150,153,51,103,110,177,180,180,124,196,182,31,2,129,96,91,120,120,56,167,51,96,6,5,219,95,222,113,202,212,169,83,151,0,203,217,17,30,30,30,53,108,207,195,80,69,46,151,179,154,121,98,99,99,211,185,
102,205,26,9,219,243,128,82,70,236,14,7,0,144,144,144,80,104,103,103,199,90,13,53,138,162,64,46,151,35,109,248,65,18,95,95,223,111,216,188,182,227,232,232,88,158,149,149,165,102,205,1,12,140,232,5,247,
198,27,111,180,8,4,130,125,108,217,119,112,112,104,77,76,76,212,219,227,236,180,180,180,239,68,34,81,35,27,182,121,60,30,248,249,249,113,54,107,104,168,140,232,5,7,0,16,16,16,144,203,214,17,183,76,38,
43,154,59,119,174,222,28,150,244,102,250,244,233,15,165,82,41,43,135,39,52,77,95,79,73,73,217,195,134,109,156,140,248,5,151,156,156,92,33,147,201,144,214,179,31,8,102,102,102,16,22,22,166,183,225,100,
55,81,81,81,91,251,234,132,131,19,129,64,176,61,54,54,86,127,74,42,15,20,182,95,34,73,72,80,80,16,241,202,84,94,94,94,127,103,123,220,168,68,46,151,63,209,9,7,167,152,154,154,118,45,95,190,156,72,107,
105,210,50,226,119,56,0,128,89,179,102,229,90,90,90,18,203,242,160,40,10,220,220,220,244,126,119,235,102,220,184,113,155,72,134,229,52,77,215,124,242,201,39,255,71,204,32,65,158,139,5,183,116,233,210,
58,161,80,72,236,227,41,77,211,173,105,105,105,122,87,186,175,47,210,210,210,190,147,72,36,68,194,114,138,162,192,221,221,61,151,132,45,54,120,46,22,28,0,128,143,143,15,177,195,19,161,80,152,167,84,42,
145,53,252,96,155,232,232,232,135,66,161,144,72,122,213,216,177,99,239,164,166,166,34,239,146,202,21,158,155,5,151,154,154,90,34,16,8,110,224,182,99,97,97,209,21,19,19,51,98,194,201,110,226,227,227,183,
144,56,60,161,105,122,79,114,114,114,27,110,59,172,193,246,75,36,73,9,8,8,248,6,48,191,240,123,122,122,30,101,123,156,184,196,219,219,27,107,193,88,35,35,35,38,43,43,107,10,219,227,196,41,207,205,14,7,
0,16,25,25,185,197,196,196,4,155,254,63,62,214,110,194,102,128,101,252,253,253,183,224,204,60,145,72,36,255,94,179,102,205,9,108,6,56,192,115,181,224,190,248,226,139,51,34,145,232,95,184,244,11,133,194,
43,169,169,169,35,238,99,109,55,25,25,25,251,104,154,198,118,120,226,228,228,196,137,27,10,56,121,174,22,28,0,128,187,187,123,46,174,219,224,18,137,36,39,38,38,230,1,22,229,28,32,44,44,172,83,42,149,230,
226,208,109,99,99,211,17,29,29,173,55,213,179,135,202,115,183,224,148,74,101,209,152,49,99,238,162,214,107,105,105,217,21,21,21,165,191,37,184,7,200,244,233,211,55,91,89,89,33,191,182,35,149,74,43,179,
178,178,56,89,49,12,41,108,191,68,178,33,126,126,126,223,2,226,23,126,185,92,94,193,246,184,72,201,184,113,227,42,80,206,29,143,199,99,50,50,50,226,217,30,23,9,121,238,118,56,0,128,9,19,38,108,50,48,48,
64,166,143,199,227,129,66,161,208,187,75,166,67,69,161,80,172,71,249,77,147,166,233,38,165,82,137,188,37,49,23,121,46,23,220,43,175,188,114,76,34,145,168,80,233,147,72,36,141,153,153,153,223,161,210,199,
117,82,82,82,14,161,204,60,145,72,36,5,51,103,206,236,64,165,143,203,60,151,11,110,226,196,137,93,40,175,157,8,133,194,45,81,81,81,35,246,176,164,55,49,49,49,29,52,77,35,233,222,106,97,97,1,17,17,17,68,
178,88,184,0,186,184,74,207,8,15,15,207,177,181,181,93,254,240,225,240,110,128,240,249,124,152,52,105,18,235,173,131,73,19,29,29,189,213,222,222,254,127,58,59,59,135,245,97,206,204,204,236,248,167,159,
126,122,9,149,95,92,135,98,24,98,73,244,47,120,193,115,207,115,25,82,190,224,5,108,241,255,87,92,113,13,176,119,233,197,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* w4kWindow::wusikLogo_png = (const char*) resource_w4kWindow_wusikLogo_png;
const int w4kWindow::wusikLogo_pngSize = 7649;


//[EndFile] You can add extra defines here...
//[/EndFile]
