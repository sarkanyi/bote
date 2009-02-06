// MainBaseView.cpp : implementation file
//

#include "stdafx.h"
#include "botf2.h"
#include "MainBaseView.h"


// CMainBaseView

IMPLEMENT_DYNCREATE(CMainBaseView, CView)

CMainBaseView::CMainBaseView()
{

}

CMainBaseView::~CMainBaseView()
{
}

BEGIN_MESSAGE_MAP(CMainBaseView, CView)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CMainBaseView drawing

void CMainBaseView::OnDraw(CDC* pDC)
{
	// TODO: add draw code here	
}


// CMainBaseView diagnostics

#ifdef _DEBUG
void CMainBaseView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMainBaseView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMainBaseView message handlers

void CMainBaseView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	m_TotalSize = CSize(1075, 750);	
}

void CMainBaseView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class
	CBotf2Doc* pDoc = (CBotf2Doc*)GetDocument();

	CRect client;
	GetClientRect(&client);
		
	// MapMode setzen
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(m_TotalSize);		
	pDC->SetViewportExt(client.right, client.bottom);
		
	pDC->SetViewportOrg(0, 0);
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetBkMode(TRANSPARENT);
		
	CView::OnPrepareDC(pDC, pInfo);
}

BOOL CMainBaseView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;
	//return CView::OnEraseBkgnd(pDC);
}

void CMainBaseView::DrawButtons(CDC* pDC, CArray<CMyButton*>* buttonArray, int counter)
{
	CBotf2Doc* pDoc = (CBotf2Doc*)GetDocument();
	ASSERT(pDoc);

	// Wenn wir im jeweiligen Men� sind, pr�fen, ob der dazugeh�rige Button auch inaktiv ist.
	for (int j = 0; j < buttonArray->GetSize(); j++)
		if (counter == j && buttonArray->GetAt(j)->GetState() != 2)
		{
			for (int i = 0; i < buttonArray->GetSize(); i++)
				if (buttonArray->GetAt(i)->GetState() == 2)
					buttonArray->GetAt(i)->SetState(0);
			buttonArray->GetAt(j)->SetState(2);
			break;
		}
	// Buttons zeichnen
	for (int i = 0; i < buttonArray->GetSize(); i++)
		buttonArray->GetAt(i)->DrawButton(pDC, pDoc->GetGraphicPool());
}

void CMainBaseView::ButtonReactOnMouseOver(const CPoint &point, CArray<CMyButton*>* buttonArray)
{
	CBotf2Doc* pDoc = (CBotf2Doc*)GetDocument();
	ASSERT(pDoc);

	for (int i = 0; i < buttonArray->GetSize(); i++)
	{
		if (buttonArray->GetAt(i)->ClickedOnButton(point))
		{			
			if (buttonArray->GetAt(i)->Activate())
			{
				pDoc->m_pSoundManager->PlaySound(SNDMGR_SOUND_MAINMENU);
				CRect r = buttonArray->GetAt(i)->GetRect();
				CalcDeviceRect(r);
				InvalidateRect(r, FALSE);
			}
		}
		else if (buttonArray->GetAt(i)->Deactivate())
		{
			CRect r = buttonArray->GetAt(i)->GetRect();
			CalcDeviceRect(r);
			InvalidateRect(r, FALSE);
		}
	}
}

BOOLEAN CMainBaseView::ButtonReactOnLeftClick(const CPoint &point, CArray<CMyButton*>* buttonArray, int &counter, BOOLEAN invalidate, BOOLEAN onlyActivate)
{
	short button = -1;
	for (int i = 0; i < buttonArray->GetSize(); i++)
		if (buttonArray->GetAt(i)->ClickedOnButton(point))
		{
			// Wenn der Button schon deaktiviert ist, dann kann er nicht angeklickt werden
			if (buttonArray->GetAt(i)->GetState() == 2)
				return FALSE;			
			button = i;
			if (!onlyActivate)
			{
				// Button auf inaktiv schalten, anderen inaktiven Button auf normal schalten
				for (int j = 0; j < buttonArray->GetSize(); j++)
					if (buttonArray->GetAt(j)->GetState() == 2)
					{
						buttonArray->GetAt(j)->SetState(0);
						break;
					}
				buttonArray->GetAt(i)->SetState(2);
			}
			else
			{
				// Button auf aktiv schalten, anderen aktiven Button auf normal schalten
				for (int j = 0; j < buttonArray->GetSize(); j++)
					if (buttonArray->GetAt(j)->GetState() == 1)
					{
						buttonArray->GetAt(j)->SetState(0);
						break;
					}
					buttonArray->GetAt(i)->SetState(1);
			}
			counter = button;
			if (invalidate)
				Invalidate(FALSE);
			return TRUE;
		}
	return FALSE;
}

void CMainBaseView::LoadRaceFont(CDC* pDC)
{
	CBotf2Doc* pDoc = (CBotf2Doc*)GetDocument();
	m_Font.Detach();
	m_Font.DeleteObject();
	pDC->SetTextColor(CFontLoader::CreateFont(pDoc->GetPlayersRace(), 2, 3, &m_Font));
	pDC->SelectObject(&m_Font);
}

void CMainBaseView::LoadFontForBigButton(CDC* pDC)
{
	CBotf2Doc* pDoc = (CBotf2Doc*)GetDocument();
	HGDIOBJ h =	bm.Detach();
	DeleteObject(h);	
	h = bm2.Detach();
	DeleteObject(h);
	h = bm_dark.Detach();
	DeleteObject(h);	
	
	bm.DeleteObject();
	bm2.DeleteObject();
	bm_dark.DeleteObject();
	FCObjImage img;
	if (pDoc->GetPlayersRace() == HUMAN)
	{
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE1_PREFIX") + "button_big2.jpg");
		bm.Attach(FCWin32::CreateDDBHandle(img));
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE1_PREFIX") + "button_big2i.jpg");
		bm_dark.Attach(FCWin32::CreateDDBHandle(img));
	}
	else if (pDoc->GetPlayersRace() == FERENGI)
	{
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE2_PREFIX") + "button.jpg");
		bm.Attach(FCWin32::CreateDDBHandle(img));
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE2_PREFIX") + "buttoni.jpg");
		bm_dark.Attach(FCWin32::CreateDDBHandle(img));
	}
	else if (pDoc->GetPlayersRace() == KLINGON)
	{
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE3_PREFIX") + "button.jpg");
		bm.Attach(FCWin32::CreateDDBHandle(img));
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE3_PREFIX") + "buttoni.jpg");
		bm_dark.Attach(FCWin32::CreateDDBHandle(img));
	}
	else if (pDoc->GetPlayersRace() == ROMULAN)
	{
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE4_PREFIX") + "button.jpg");
		bm.Attach(FCWin32::CreateDDBHandle(img));
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE4_PREFIX") + "buttoni.jpg");
		bm_dark.Attach(FCWin32::CreateDDBHandle(img));
	}
	else if (pDoc->GetPlayersRace() == CARDASSIAN)
	{
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE5_PREFIX") + "button.jpg");
		bm.Attach(FCWin32::CreateDDBHandle(img));
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE5_PREFIX") + "button_dark.jpg");
		bm_dark.Attach(FCWin32::CreateDDBHandle(img));
	}
	else if (pDoc->GetPlayersRace() == DOMINION)
	{
	
	}
	img.Destroy();
	CFont font;
	pDC->SetTextColor(CFontLoader::CreateFont(pDoc->GetPlayersRace(),3,2,&font));
	pDC->SelectObject(&font);
}

void CMainBaseView::LoadFontForLittleButton(CDC* pDC)
{
	CBotf2Doc* pDoc = (CBotf2Doc*)GetDocument();
	HGDIOBJ h =	bm.Detach();
	DeleteObject(h);	
	h = bm2.Detach();
	DeleteObject(h);
	h = bm3.Detach();
	DeleteObject(h);
	h = bm4.Detach();
	DeleteObject(h);
	h = bm_dark.Detach();
	DeleteObject(h);

	bm.DeleteObject();
	bm2.DeleteObject();
	bm3.DeleteObject();
	bm4.DeleteObject();
	bm_dark.DeleteObject();
	FCObjImage img;
	if (pDoc->GetPlayersRace() == HUMAN)
	{
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE1_PREFIX") + "button_small3.jpg");
		bm.Attach(FCWin32::CreateDDBHandle(img));
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE1_PREFIX") + "button_small2.jpg");
		bm2.Attach(FCWin32::CreateDDBHandle(img));
	}
	else if (pDoc->GetPlayersRace() == FERENGI)
	{
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE2_PREFIX") + "button_small_mid.jpg");
		bm.Attach(FCWin32::CreateDDBHandle(img));
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE2_PREFIX") + "button_small3.jpg");
		bm2.Attach(FCWin32::CreateDDBHandle(img));
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE2_PREFIX") + "button_small1.jpg");
		bm3.Attach(FCWin32::CreateDDBHandle(img));
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE2_PREFIX") + "button_small2.jpg");
		bm4.Attach(FCWin32::CreateDDBHandle(img));
	}
	else if (pDoc->GetPlayersRace() == KLINGON)
	{
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE3_PREFIX") + "button_small.jpg");
		bm.Attach(FCWin32::CreateDDBHandle(img));
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE3_PREFIX") + "button_small.jpg");
		bm2.Attach(FCWin32::CreateDDBHandle(img));
	}
	else if (pDoc->GetPlayersRace() == ROMULAN)
	{
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE4_PREFIX") + "button_small.jpg");
		bm.Attach(FCWin32::CreateDDBHandle(img));
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE4_PREFIX") + "button_small.jpg");
		bm2.Attach(FCWin32::CreateDDBHandle(img));
	}
	else if (pDoc->GetPlayersRace() == CARDASSIAN)
	{
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE5_PREFIX") + "button2.jpg");
		bm2.Attach(FCWin32::CreateDDBHandle(img));
	}
	else if (pDoc->GetPlayersRace() == DOMINION)
	{
		img.Load(*((CBotf2App*)AfxGetApp())->GetPath() + "Graphics\\Other\\" + CResourceManager::GetString("RACE5_PREFIX") + "button2.jpg");
		bm2.Attach(FCWin32::CreateDDBHandle(img));
	}
	img.Destroy();
	pDC->SetTextColor(CFontLoader::GetFontColor(pDoc->GetPlayersRace(),1));
}

void CMainBaseView::CalcLogicalPoint(CPoint &point)
{
	CRect client;
	GetClientRect(&client);
	
	point.x *= (float)m_TotalSize.cx / (float)client.Width();
	point.y *= (float)m_TotalSize.cy / (float)client.Height();	
}

void CMainBaseView::CalcDeviceRect(CRect &rect)
{
	CRect client;
	GetClientRect(&client);

	CPoint p1 = rect.TopLeft();
	p1.x *= (float)client.Width() / (float)m_TotalSize.cx;
	p1.y *= (float)client.Height() / (float)m_TotalSize.cy;
	
	CPoint p2 = rect.BottomRight();
	p2.x *= (float)client.Width() / (float)m_TotalSize.cx;
	p2.y *= (float)client.Height() / (float)m_TotalSize.cy;
	rect.SetRect(p1, p2);
}