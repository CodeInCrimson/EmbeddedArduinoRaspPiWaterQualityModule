function moveFilesToSpecialFolder(){
 
    var files = DriveApp.getFiles(); //main drive folder
    var targetFolderID = '1RPBKonMFfwNOzyJcDQV2QXXRI1oKGESB'; //target folder to move to
    
    while (files.hasNext()) {
      
      var selectedfile = files.next();
          
      var sourceFolder = selectedfile.getParents().next();
      
      if (sourceFolder.getId() != '1RPBKonMFfwNOzyJcDQV2QXXRI1oKGESB') {
      
      if(selectedfile.getName() == "ship_project.csv") {
        
        var sourceFileID = selectedfile.getId();
        moveFiles(sourceFileID, targetFolderID);
      }
      
      }
     
    }
    
    deleteFiles();
  }
  
  
  
  function moveFiles(sourceFileId, targetFolderId) {
    
    var file = DriveApp.getFileById(sourceFileId);
    
    DriveApp.getFolderById(targetFolderId).addFile(file);
    
    DriveApp.getRootFolder().removeFile(file);
    
  }
  
  
  
  
  function deleteFiles() {
    
    var allFiles, myFolder, thisFile, thisFileID, i, j;
  
    myFolder = DriveApp.getFolderById('1RPBKonMFfwNOzyJcDQV2QXXRI1oKGESB');
  
    allFiles = myFolder.getFilesByName('ship_project.csv');
  
    var myArray = ["first"];
    
    while (allFiles.hasNext()) {    
      
      thisFile = allFiles.next();
      thisFileID = thisFile.getId();
      
      myArray.push(thisFileID); 
      
      Logger.log('pushed the following: '+ thisFile.getName());
      
    }
    
     myArray.shift();  // remove first element "first"
    
     for (i=0; i<myArray.length-1; i++){
       j = myArray[i];
       DriveApp.getFileById(j).setTrashed(true);
       
       Logger.log('Has entered for loop for removal');
       //Drive.Files.emptyTrash();
     }
      
  }