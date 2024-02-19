function cleanupCells() {
  
  
    //open if you wanto to run //var mainSheet = SpreadsheetApp.openById('1S2AjUl8miuK5UkJMTmlJHd2xys-7onOrgIBjOYfBpG0').getSheets()[0]; // select sheet 0 from our current speadsheet
      
    var rangeData = mainSheet.getDataRange();
  
    //var lastColumn = rangeData.getLastColumn();
    var lastRow = rangeData.getLastRow();
      
    var column4 = 4;
    var column5 = 5;
    var column8 = 8;
      
      for (var row = 1; row < lastRow; row++) { 
              
        var currentCellRange = mainSheet.getRange(row,column4);
        //calculate for column 4
        var C4Value = currentCellRange.getValues();
     
        if(C4Value.toString().includes("`C")) {
          
          var tempC3 = C4Value.toString().replace("`C", "°C");
          currentCellRange.setValue(tempC3);        
        }
        
        if(C4Value.toString().includes("*C")) {
          
          var tempC3 = C4Value.toString().replace("*C", "°C");
          currentCellRange.setValue(tempC3);
          
        }
        
        var currentCellRange = mainSheet.getRange(row,column5);
        //calculate for column 5
        var C5Value = currentCellRange.getValues();
        
        if(C5Value.toString().includes("`F")) {
          
          var tempC5 = C5Value.toString().replace("`F", "°F");
          currentCellRange.setValue(tempC5);        
        }
        
        if(C5Value.toString().includes("*F")) {
          
          var tempC5 = C5Value.toString().replace("*F", "°F");
          currentCellRange.setValue(tempC5);
          
        }
        
        
        var currentCellRange = mainSheet.getRange(row,column8);
        //calculate for column 8
        var C8Value = currentCellRange.getValues();
        
        if(C8Value.toString().includes("`C")) {
          
          var tempC8 = C8Value.toString().replace("`C", "°C");
          currentCellRange.setValue(tempC8);        
        }
        
        if(C8Value.toString().includes("*F")) {
          
          var tempC8 = C8Value.toString().replace("*C", "°C");
          currentCellRange.setValue(tempC8);
          
        }
        
      }
    
  }