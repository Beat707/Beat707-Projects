/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com

*/

import java.awt.FileDialog;

public String selectOutputPro(String prompt, String filestring) {
  return selectFileImplPro(prompt, filestring, FileDialog.SAVE);
}

public String selectInputPro(String prompt, String filestring) {
  return selectFileImplPro(prompt, filestring, FileDialog.LOAD);
}


protected String selectFileImplPro(final String prompt, final String filestring, final int mode) {
  this.checkParentFrame();

  try {
    javax.swing.SwingUtilities.invokeAndWait(new Runnable() {
	public void run() {
	  FileDialog fileDialog =
	    new FileDialog(parentFrame, prompt, mode);
	  fileDialog.setFile(filestring);
	  fileDialog.setVisible(true);
	  String directory = fileDialog.getDirectory();
	  String filename = fileDialog.getFile();
	  selectedFile =
	    (filename == null) ? null : new File(directory, filename);
	}
    }
    );
    return (selectedFile == null) ? null : selectedFile.getAbsolutePath();

  } 
  catch (Exception e) {
    e.printStackTrace();
    return null;
  }
}
