' Create objects to handle Excel

Set objExcel = CreateObject("Excel.Application")
Set objWorkbook = objExcel.Workbooks.Open(WScript.Arguments.Item(0)) ' Use the first command line argument as the file name


' Process TestFlow
Set objWorksheet = objWorkbook.Sheets("TestFlow")
Set objFSO = CreateObject("Scripting.FileSystemObject")
Set objOutputFile = objFSO.CreateTextFile("TestFlow.csv", True)
For Each row In objWorksheet.UsedRange.Rows
    For Each cell In row.Cells
        objOutputFile.Write(cell.Value & ",") ' Write the cell value and a space
    Next
    objOutputFile.WriteLine ' Write a new line
Next
objOutputFile.Close

' Process TestLimits
Set objWorksheet = objWorkbook.Sheets("Test Limits")
Set objOutputFile = objFSO.CreateTextFile("TestLimits.csv", True)
For Each row In objWorksheet.UsedRange.Rows
    For Each cell In row.Cells
        objOutputFile.Write(cell.Value & ",") ' Write the cell value and a space
    Next
    objOutputFile.WriteLine ' Write a new line
Next
objOutputFile.Close

' Clean up
