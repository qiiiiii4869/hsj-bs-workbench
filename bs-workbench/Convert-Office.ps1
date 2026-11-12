param([Parameter(Mandatory=$true)][string]$Source,[Parameter(Mandatory=$true)][string]$Target,[ValidateSet('doc','xls')][string]$Kind)
$ErrorActionPreference='Stop'
$officeApp=$null
$officeDocument=$null
try {
    if($Kind -eq 'doc') {
        $officeApp=New-Object -ComObject Word.Application
        $officeApp.Visible=$false
        $officeApp.DisplayAlerts=0
        $officeApp.AutomationSecurity=3
        $officeDocument=$officeApp.Documents.Open($Source,$false,$true,$false)
        $wordFormat=0
        $officeDocument.SaveAs([ref]$Target,[ref]$wordFormat)
    } else {
        $officeApp=New-Object -ComObject Excel.Application
        $officeApp.Visible=$false
        $officeApp.DisplayAlerts=$false
        $officeApp.AskToUpdateLinks=$false
        $officeApp.AutomationSecurity=3
        $officeDocument=$officeApp.Workbooks.Open($Source,0,$true)
        foreach($officeSheet in $officeDocument.Worksheets) {
            if($officeSheet.UsedRange.Rows.Count -gt 65536 -or $officeSheet.UsedRange.Columns.Count -gt 256) {
                throw 'XLS worksheet capacity exceeded; use XLSX to preserve all data.'
            }
        }
        $officeDocument.CheckCompatibility=$false
        $officeDocument.SaveAs($Target,56)
    }
} finally {
    if($officeDocument) {
        try{$officeDocument.Close($false)}catch{}
        [void][System.Runtime.InteropServices.Marshal]::FinalReleaseComObject($officeDocument)
    }
    if($officeApp) {
        try{$officeApp.Quit()}catch{}
        [void][System.Runtime.InteropServices.Marshal]::FinalReleaseComObject($officeApp)
    }
    [GC]::Collect()
    [GC]::WaitForPendingFinalizers()
}
