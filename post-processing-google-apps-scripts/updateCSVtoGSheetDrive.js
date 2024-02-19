function updateCSVtoGSheetDrive() {
  
  var targetFolderID = '1RPBKonMFfwNOzyJcDQV2QXXRI1oKGESB';

  var targetFile =  DriveApp.getFolderById(targetFolderID).getFilesByName("ship_project.csv").next();
  
  var targetFileID = targetFile.getId();

  
  var rawData = DriveApp.getFileById(targetFileID).getBlob().getDataAsString();
  
  var parsedValues = Utilities.parseCsv(rawData, ',');
    
  var selectedSheet = SpreadsheetApp.openById('1S2AjUl8miuK5UkJMTmlJHd2xys-7onOrgIBjOYfBpG0').getSheets()[0]; // select sheet 0 from our current speadsheet
  
  selectedSheet.clearContents().clearFormats();
  selectedSheet.getRange(1, 1, parsedValues.length, parsedValues[0].length).setValues(parsedValues);

  }
  
  //var test = targetFile.getDateCreated();
  //var test1 = targetFile.getSize();
  //Logger.log(targetFileID);
  //Logger.log('date created ' + test);
  //Logger.log('getsize ' + test1);
