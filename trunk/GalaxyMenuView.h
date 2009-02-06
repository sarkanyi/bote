/*
 *   Copyright (C)2004-2008 Sir Pustekuchen
 *
 *   Author   :  Sir Pustekuchen
 *   Home     :  http://birth-of-the-empires.de.vu
 *
 */
#pragma once
#include "Botf2Doc.h"
#include "DBScrollView.h"

/*
Wird GDI Plus benutzt, so m�ssen noch weitere Anpassungen am Code vorgenommen werden,
- diese View sollte nur von CScrollView abgeleitet werden, nicht von CDBScrollView
- die MessageHandler m�ssen auf CScrollView angepasst werden, nicht wie bei GDI auf CDBScrollView
- OnEraseBackground muss in dieser View FALSE zur�ckgeben
- CMemDC darf beim Zeichnen der Galaxieansicht nicht verwendet werden
*/
//#define GDIPLUS

#define GDI

class CGalaxyMenuView : public CDBScrollView
{
protected: // Nur aus Serialisierung erzeugen
	CGalaxyMenuView();
	DECLARE_DYNCREATE(CGalaxyMenuView)

	CSize m_TotalSize;								///< Gr��e der View in logischen Koordinaten
	BOOLEAN m_bScrollToHome;						///< soll zum Heimatsektor zu Beginn der Runde gescrollt werden
		
#ifdef GDIPLUS
	Bitmap* m_pGalaxyBackground;	///< Zeiger auf Hintergrundbild-Objekt
	Bitmap* m_pThumbnail;
#endif
#ifdef GDI
	CBitmap* m_pGalaxyBackground;	///< Zeiger auf Hintergrundbild-Objekt
#endif
		
	double m_fZoom;					///< aktueller Zoom-Faktor
	int m_nRange;
	Sector oldtarget;
	Sector m_oldSelection;	
	
	// Hier Variablen, wenn wir eine Handelroute ziehen wollen
	static BOOLEAN m_bDrawTradeRoute;		///< sollen wir bei MouseMove die Handelroute zeigen
	static CTradeRoute m_TradeRoute;		///< die Handelsroute, die wir auf der Galaxiemap ziehen k�nnen
	// Hier Variablen, wenn wir eine Ressourcenroute ziehen wollen
	static BOOLEAN m_bDrawResourceRoute;	///< sollen wir bei MouseMove die Ressourcenroute zeigen
	static CResourceRoute m_ResourceRoute;	///< die Ressourcenroute, die wir auf der Galaxiemap ziehen k�nnen
	// Hier Variablen f�r die Schiffsbewegung
	static BOOLEAN m_bShipMove;				///< soll ein Schiff auf der Map bewegt werden (Kurs geben)
	CArray<Sector> m_oldPath;				///< alter Pfad eines Schiffes, welcher auf der Map angezeigt wird
	
	/**
	 * Multipliziert die cx und cy-Komponente von pSize mit dem Zoom-Faktor, rundet anschlie�end auf ganze Werte.
	 */
	void Zoom(CSize *pSize) const;
	void Zoom(CPoint *pPoint) const;
	/**
	 * Dividiert die cx und cy-Komponente von pSize durch den Zoom-Faktor, rundet anschlie�end auf ganze Werte.
	 */
	void UnZoom(CSize *pSize) const;
	void UnZoom(CPoint *pPoint) const;

	int GetRangeBorder(const unsigned char range1, const unsigned char range2, int m_nRange) const;
	
	/// Funktion generiert die visuelle Galaxiekarte. Muss bei jeder neuen Runde neu aufgerugen werden.
	void GenerateGalaxyMap(void);
		
public:	
	/// Funktion f�hrt Aufgaben aus, welche zu jeder neuen Runde von der View ausgef�hrt werden m�ssen.
	void OnNewRound();

	/// Funktion legt den visuellen Pfad f�r Schiffe fest.
	/// legt den Pfad des neu angew�hlten Schiffes auf der Map fest und l�scht den alten Pfad.
	void SetNewShipPath();

	/// Funktion legt fest, ob eine anzulegende Ressourcenroute gerade gezeichnet werden soll. Dies wird zum manuellen
	/// Anlegen der Ressourcenrouten ben�tigt.
	/// @param is Wahrheitswert
	static void IsDrawResourceRoute(bool is) {m_bDrawResourceRoute = is;}

	/// Funktion legt fest, ob eine anzulegende Handelsroute gerade gezeichnet werden soll. Dies wird zum manuellen
	/// Anlegen der Handelsrouten ben�tigt.
	/// @param is Wahrheitswert
	static void IsDrawTradeRoute(bool is) {m_bDrawTradeRoute = is;}

	/// Funktion wei�t der zu zeichnenden Ressourcenroute eine bestehende Route zu.
	/// @return zu zeichnende neue Route
	static CResourceRoute* GetDrawnResourceRoute() {return &m_ResourceRoute;}

	/// Funktion wei�t der zu zeichnenden Ressourcenroute eine bestehende Route zu.
	/// @return zu zeichnende neue Route
	static CTradeRoute* GetDrawnTradeRoute() {return &m_TradeRoute;}

	/// Funktion legt fest, ob ein Schiff auf der Map bewegt werden soll
	/// @param is Wahrheitswert
	static void SetMoveShip(BOOLEAN is) {m_bShipMove = is;}

	/// Funktion gibt zur�ck, ob ein Schiff auf der Map bewegt werden soll.
	/// @return Wahrheitswert �ber die Schiffsbewegung auf der Map
	static BOOLEAN IsMoveShip() {return m_bShipMove;}

	// Operationen
public:
	// �berladungen
	// Vom Klassenassistenten generierte �berladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CGalaxyMenuView)
	virtual void OnDraw(CDC* pDC);  // �berladen zum Zeichnen dieser Ansicht
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnInitialUpdate(); // das erste mal nach der Konstruktion aufgerufen
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

	// Implementierung
public:
	virtual ~CGalaxyMenuView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generierte Message-Map-Funktionen
protected:
	//{{AFX_MSG(CGalaxyMenuView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
public:
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
protected:
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
};