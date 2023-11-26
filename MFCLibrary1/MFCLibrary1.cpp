#include "pch.h"
#include "framework.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "rxregsvc.h"
#include "aced.h"
#include "dbents.h"
#include<tchar.h>
#include "rxregsvc.h"
#include "acutads.h"
#include "accmd.h"
#include "arxHeaders.h"

#pragma comment(linker,"/EXPORT:acrxEntryPoint")
#pragma comment(linker,"/EXPORT:acrxGetApiVersion")

#include "acdb.h"
#include "aced.h"
#include "dbmain.h"
#include "dbapserv.h"
#include "gepnt3d.h"
#include "dbents.h"

void Drawline()
{
    // Get the current database
    AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();

    // Open the Block Table for read-only
    AcDbBlockTable* pBlockTable;
    pDb->getSymbolTable(pBlockTable, AcDb::kForRead);

    // Get the Model Space block
    AcDbBlockTableRecord* pBlockTableRecord;
    pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
    pBlockTable->close();

    // Prompt user for the start point
    AcGePoint3d startPt;
    if (acedGetPoint(NULL, _T("\nEnter the start point: "), asDblArray(startPt)) != RTNORM)
    {
        pBlockTableRecord->close();
        return;
    }

    // Prompt user for the end point
    AcGePoint3d endPt;
    if (acedGetPoint(NULL, _T("\nEnter the end point: "), asDblArray(endPt)) != RTNORM)
    {
        pBlockTableRecord->close();
        return;
    }

    // Create the new Line object in memory
    AcDbLine* pLine = new AcDbLine(startPt, endPt);

    // Add the new Line object to Model space
    pBlockTableRecord->appendAcDbEntity(pLine);

    // Close the Model space block
    pBlockTableRecord->close();

    // Close the new line object
    pLine->close();
}

// Registering a command
void InitApp()
{
    acedRegCmds->addCommand(_T("DrawLine_ARX"), _T("DL_ObjectARX"), _T("DL_ObjectARX"), ACRX_CMD_MODAL, Drawline);
}
// Entry point function
extern "C" AcRx::AppRetCode acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt)
{
    switch (msg)
    {
    case AcRx::kInitAppMsg:
        //acrxUnlockApplication(pkt);
        //acrxRegisterAppMDIAware(pkt);
        InitApp();
        break;

    case AcRx::kUnloadAppMsg:
        //UnloadApp();
        // Command Groups to remove
        break;
    }

    return AcRx::kRetOK;
}