/*************************************************************************/
/** Copyright.															**/
/** FileName: DemoProc.c												**/
/** Author: Polarix														**/
/** Description: User operation interface.								**/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "DemoProc.h"

#ifdef _SIMPLE_GUI_VIRTUAL_ENVIRONMENT_SIMULATOR_
#include "SDKInterface.h"
#include "SGUI_FlashData.h"
#else
#include "OLED.h"
#include "DemoActions.h"
#endif

//=======================================================================//
//= Static variable declaration.									    =//
//=======================================================================//
SGUI_SCR_DEV				g_stDeviceInterface;
HMI_SCREEN_OBJECT*			g_arrpstScreenObjs[] =
							{
								&g_stHMIDemo_ScrollingText,
								&g_stHMIDemo_List,
								&g_stHMIDemo_TextNotice,
								&g_stHMIDemo_RTCNotice,
								&g_stHMIDemo_VariableBox,
								&g_stHMI_DemoRealtimeGraph,
							};
HMI_ENGINE_OBJECT			g_stDemoEngine;

//=======================================================================//
//= Static function declare.								            =//
//=======================================================================//
static void					KeyPressEventProc(void);
static void					RTCEventProc(void);
static void					SysTickTimerEventProc(void);

//=======================================================================//
//= Function define.										            =//
//=======================================================================//

/*****************************************************************************/
/** Function Name:	SimpleGUI_DemoProcess   								**/
/** Purpose:		Simple GUI HMI engine and interface demo process.       **/
/** Parameters:		None.													**/
/** Return:			HMI_ENGINE_RESULT.										**/
/** Notice:			This function demonstrates how to use the interface and **/
/**                 HMI engine of Simple GUI.                               **/
/*****************************************************************************/
HMI_ENGINE_RESULT InitializeEngine(void)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	HMI_ENGINE_RESULT           eProcessResult;
	int							iIndex;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	eProcessResult =			HMI_RET_NORMAL;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	/* Clear structure. */
	SGUI_SystemIF_MemorySet(&g_stDeviceInterface, 0x00, sizeof(SGUI_SCR_DEV));
	SGUI_SystemIF_MemorySet(&g_stDemoEngine, 0x00, sizeof(HMI_ENGINE_OBJECT));
#ifdef _SIMPLE_GUI_VIRTUAL_ENVIRONMENT_SIMULATOR_
	/* Initialize display size. */
	g_stDeviceInterface.stSize.Width = 128;
	g_stDeviceInterface.stSize.Height = 64;
	/* Initialize interface object. */
	g_stDeviceInterface.fnSetPixel = SGUI_SDK_SetPixel;
	g_stDeviceInterface.fnGetPixel = SGUI_SDK_GetPixel;
	g_stDeviceInterface.fnClear = SGUI_SDK_ClearDisplay;
	g_stDeviceInterface.fnSyncBuffer = SGUI_SDK_RefreshDisplay;
#else
	/* Initialize display size. */
	g_stDeviceInterface.stSize.Width = 128;
	g_stDeviceInterface.stSize.Height = 64;
	/* Initialize interface object. */
	g_stDeviceInterface.fnSetPixel = OLED_SetPixel;
	g_stDeviceInterface.fnGetPixel = OLED_GetPixel;
	g_stDeviceInterface.fnClear = OLED_ClearDisplay;
	g_stDeviceInterface.fnSyncBuffer = OLED_RefreshScreen;
#endif

	do
	{
		/* Prepare HMI engine object. */
		g_stDemoEngine.ScreenCount = sizeof(g_arrpstScreenObjs)/sizeof(*g_arrpstScreenObjs);
		g_stDemoEngine.ScreenObjPtr = g_arrpstScreenObjs;
		g_stDemoEngine.Interface = &g_stDeviceInterface;

		/* Initialize all screen object. */
		if(NULL != g_stDemoEngine.ScreenObjPtr)
		{
			for(iIndex=0; iIndex<g_stDemoEngine.ScreenCount; iIndex++)
			{
				if( (NULL != g_stDemoEngine.ScreenObjPtr[iIndex])
					&& (NULL != g_stDemoEngine.ScreenObjPtr[iIndex]->pstActions)
					&& (g_stDemoEngine.ScreenObjPtr[iIndex]->pstActions->Initialize)
					)
				{
					g_stDemoEngine.ScreenObjPtr[iIndex]->pstActions->Initialize(&g_stDeviceInterface);
					g_stDemoEngine.ScreenObjPtr[iIndex]->pstPrevious = NULL;
				}
			}
		}
		else
		{

		}
		/* Active engine object. */
		eProcessResult = HMI_ActiveEngine(&g_stDemoEngine, HMI_SCREEN_ID_DEMO_SCROLLING_TEXT);
		if(HMI_PROCESS_FAILED(eProcessResult))
		{
			/* Active engine failed. */
			break;
		}
		/* Start engine process. */
		eProcessResult = HMI_StartEngine(NULL);
		if(HMI_PROCESS_FAILED(eProcessResult))
		{
			/* Start engine failed. */
			break;
		}
	}while(0);


	return eProcessResult;
}

int DemoMainProcess(void)
{
	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
    // Initialize Engine.
    InitializeEngine();

    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    while(1)
    {
        // Check and process heart-beat timer event.
        if(true == SGUI_SDK_GetEventSyncFlag(ENV_FLAG_IDX_SDK_TIM_EVENT))
        {
        	SysTickTimerEventProc();
            SGUI_SDK_SetEvnetSyncFlag(ENV_FLAG_IDX_SDK_TIM_EVENT, false);
        }
        // Check and process key press event.
        if(true == SGUI_SDK_GetEventSyncFlag(ENV_FLAG_IDX_SDK_KEY_EVENT))
        {
            KeyPressEventProc();
            SGUI_SDK_SetEvnetSyncFlag(ENV_FLAG_IDX_SDK_KEY_EVENT, false);
        }
        // Check and process RTC event.
        if(true == SGUI_SDK_GetEventSyncFlag(ENV_FLAG_IDX_SDK_RTC_EVENT))
        {
            RTCEventProc();
            SGUI_SDK_SetEvnetSyncFlag(ENV_FLAG_IDX_SDK_RTC_EVENT, false);
        }
    }

	return 0;
}

void KeyPressEventProc(void)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	KEY_PRESS_EVENT		stEvent;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	HMI_EVENT_INIT(stEvent);

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	stEvent.Head.iType = EVENT_TYPE_ACTION;
	stEvent.Head.iID = EVENT_ID_KEY_PRESS;
	stEvent.Data.uiKeyValue = SGUI_SDK_GetKeyEventData();
	// Post key press event.
	HMI_ProcessEvent((HMI_EVENT_BASE*)(&stEvent));
}

void SysTickTimerEventProc(void)
{
    /*----------------------------------*/
    /* Variable Declaration				*/
    /*----------------------------------*/
    DATA_EVENT				stEvent;

    /*----------------------------------*/
    /* Initialize						*/
    /*----------------------------------*/
    HMI_EVENT_INIT(stEvent);

    /*----------------------------------*/
    /* Process							*/
    /*----------------------------------*/
    stEvent.Head.iType =	EVENT_TYPE_DATA;
    stEvent.Head.iID =		EVENT_ID_TIMER;
    stEvent.Data.iValue = (rand() % 200)-100;
    // Post timer event.
    HMI_ProcessEvent((HMI_EVENT_BASE*)&stEvent);
}


void RTCEventProc(void)
{
    /*----------------------------------*/
    /* Variable Declaration				*/
    /*----------------------------------*/
    HMI_GENERAL_EVENT		stEvent;

    /*----------------------------------*/
    /* Initialize						*/
    /*----------------------------------*/
    HMI_EVENT_INIT(stEvent);

    /*----------------------------------*/
    /* Process							*/
    /*----------------------------------*/
    stEvent.Head.iType =	EVENT_TYPE_DATA;
    stEvent.Head.iID =		EVENT_ID_RTC;
    // Post RTC update message to a screen.
    HMI_ProcessEvent((HMI_EVENT_BASE*)&stEvent);
}

