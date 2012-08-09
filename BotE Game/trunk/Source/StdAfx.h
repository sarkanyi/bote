// stdafx.h: Includedatei f�r Standardsystem-Includedateien
// oder h�ufig verwendete, projektspezifische Includedateien,
// die nur selten ge�ndert werden.

#pragma once
#define _CRT_SECURE_NO_WARNINGS		// h�ufige Sicherheitswarnungen ausschalten
#pragma warning( disable : 4244 )

//#pragma warning( 3 : 4702 )//Unerreichbarer Code
#pragma warning( 3 : 4701 )//Potenziell nicht initialisierte lokale Variable 'Name' verwendet
#pragma warning( 3 : 4189 )//'Bezeichner': Lokale Variable ist initialisiert, aber nicht referenziert
#pragma warning( 3 : 4100 )//Unreferenzierter formaler Parameter
#pragma warning( 3 : 4389 )//'Operator': Konflikt zwischen signed und unsigned

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN				// Selten verwendete Teile der Windows-Header ausschlie�en
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // einige CString-Konstruktoren sind explizit

// Deaktiviert das Ausblenden einiger h�ufiger und oft ignorierter Warnungen durch MFC
#define _AFX_ALL_WARNINGS

#include <afxwin.h>					// MFC-Kern- und -Standardkomponenten
#include <afxext.h>					// MFC-Erweiterungen
#include <afxsock.h>				// MFC Netzwerkunterst�tzung


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>				// MFC-Unterst�tzung f�r allgemeine Steuerelemente von Internet Explorer 4
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>					// MFC-Unterst�tzung f�r allgemeine Windows-Steuerelemente
#endif // _AFX_NO_AFXCMN_SUPPORT

//#include <afxcontrolbars.h>		// MFC-Unterst�tzung f�r Multifunktionsleisten und Steuerleisten

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include <gdiplus.h>				// GDI+ Verwendung
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

