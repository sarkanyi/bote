#include "stdafx.h"
#include "NetworkHandler.h"
#include "Botf2.h"

CNetworkHandler::CNetworkHandler(CBotf2Doc *pDoc) : m_pDoc(pDoc)
{
	ASSERT(pDoc);
}

CNetworkHandler::~CNetworkHandler()
{
}

void CNetworkHandler::OnPrepareGame()
{
	// Server: Spieldaten vorbereiten, an Clients senden
	server.NextRound(m_pDoc);
}

void CNetworkHandler::OnCalculateGame()
{
	using namespace network;
	// Server: alle EndOfRound-Nachrichten eingetroffen

	// Daten aller Clients ins Server-Dokument deserialisieren
	BOOL bSomethingDone = FALSE;
	for (int i = RACE_FIRST; i < RACE_LAST; i++)
		if (server.DeserializeEndOfRoundToDoc((RACE)i, m_pDoc))
			bSomethingDone = TRUE;

	ASSERT(bSomethingDone);

	// wenn Daten vorhanden waren, n�chste Runde berechnen, Daten an Clients senden
	if (!bSomethingDone) return;
	m_pDoc->NextRound();
	server.NextRound(m_pDoc);
}

void CNetworkHandler::OnNextRound(network::CNextRound *pMsg)
{
	ASSERT(pMsg);

	// Client: Rundendaten vom Server sind eingetroffen, in Dokument �bernehmen,
	// Oberfl�che zum Spielen wieder aktivieren
	m_pDoc->m_bDataReceived = false;
	
	pMsg->DeserializeToDoc(m_pDoc);
	// ...
	if (m_pDoc->m_bGameOver)
	{
		client.Disconnect();
		Sleep(1000);
		// Hartes Exit!
		exit(0);
	}

	// wurde schon ein Fenster erstellt, so wird die Nachricht der Aktualisierung an das Hauptfenster
	// geschickt
	CWnd* pWnd = ((CBotf2App*)AfxGetApp())->GetMainWnd();
	if (pWnd)
		pWnd->PostMessage(WM_UPDATEVIEWS, 0, 0);
	// gibt es noch kein Fenster (direkt beim Spielstart bzw. Laden), so geht die Message
	// an die Application und wird automatisch geschickt, sobald das Fenster da ist
	else
	{
		AfxGetApp()->PostThreadMessage(WM_UPDATEVIEWS, 0, 0);
		// hier schon setzen, da die Funktion erst sp�ter aufgerufen wird und man sonst nicht
		// beginnen kann
		m_pDoc->m_bDataReceived = true;
	}
			
	CSoundManager* pSoundManager = CSoundManager::GetInstance();
	ASSERT(pSoundManager);

	if (m_pDoc->m_iRound > 1)
		pSoundManager->PlaySound(SNDMGR_SOUND_ENDOFROUND, SNDMGR_PRIO_HIGH);
	pSoundManager->PlayMessages(600, 200);	
}

void CNetworkHandler::OnChatMsg(network::CChatMsg *pMsg)
{
	if (m_pDoc->m_bDataReceived)
	{
		((CBotf2App*)AfxGetApp())->GetChatDlg()->AddChatMsg(pMsg->GetSenderName(), pMsg->GetMessage());
		AfxGetApp()->PostThreadMessage(WM_SHOWCHATDLG, 0, 0);
	}
}

void CNetworkHandler::OnClientLost(const CString &strUserName)
{
	CString msg;
	msg.Format("Connection to client \"%s\" lost.", strUserName);
	AfxMessageBox(msg, MB_ICONINFORMATION | MB_OK);
}