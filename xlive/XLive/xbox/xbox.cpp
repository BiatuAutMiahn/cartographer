#include "Globals.h"
#include "xlivedefs.h"
#include "xliveless.h"
#include "resource.h"
#include "XLive\xbox\xbox.h"
#include "XLive\XAM\xam.h"
#include "H2MOD\Discord\DiscordInterface.h"
#include "H2MOD\Modules\Config\Config.h"
#include <iostream>
#include <sstream>
#include <codecvt>
#include <unordered_map>

extern void Check_Overlapped(PXOVERLAPPED pOverlapped);
XSESSION_LOCAL_DETAILS sessionDetails;



// #5260: XShowSigninUI
int WINAPI XShowSigninUI(DWORD cPanes, DWORD dwFlags) // TODO: If this is called, pop cartographer account system UI.
{
	sys_ui = -1;
	return ERROR_SUCCESS;
}


// #5215: XShowGuideUI
int WINAPI XShowGuideUI(DWORD dwUserIndex)
{
	// signin change
	sys_ui = -1;

	extern void GSCustomMenuCall_Guide(); // Custom menu Guide Popup from H2MOD
	GSCustomMenuCall_Guide();

	return 0;
}


DWORD WINAPI XShowArcadeUI(DWORD dwUserIndex)
{
	return 0;
}

// 5299: XShowGuideKeyRemapUI
DWORD WINAPI XShowGuideKeyRemapUI(DWORD dwUserIndex)
{
	return S_OK;
}

// #5275: XShowFriendsUI
int WINAPI XShowFriendsUI(DWORD dwUserIndex)
{
	TRACE("XShowFriendsUI");
	return 0;
}

// #5271: XShowPlayersUI
int WINAPI XShowPlayersUI(DWORD dwUserIndex)
{
	TRACE("XShowPlayersUI");
	return 1;
}

// #5006: XShowMessagesUI
int WINAPI XShowMessagesUI(DWORD dwUserIndex)
{
	TRACE("XShowMessagesUI");
	return 1;   // ERROR_NOT_LOGGED_ON
}

// #5209: XShowMessageComposeUI
int WINAPI XShowMessageComposeUI(DWORD dwUserIndex, void * pXuidRecepients, DWORD cRecipients, void * wszText)
{
	TRACE("XShowMessageComposeUI");
	return 1;   // ERROR_NOT_LOGGED_ON
}

// #5208: XShowGameInviteUI
int WINAPI XShowGameInviteUI(DWORD dwUserIndex, void * pXuidRecipients, DWORD cRecipients, LPCWSTR pszText)
{
	TRACE("XShowGameInviteUI");
	return 1; // ERROR_NOT_LOGGED_ON
}

// #5210: XShowFriendRequestUI
int WINAPI XShowFriendRequestUI(DWORD dwUserIndex, XUID xuidUser)
{
	TRACE("XShowFriendRequestUI");
	return 1;
}

// #5212: XShowCustomPlayerListUI
DWORD WINAPI XShowCustomPlayerListUI(DWORD dwUserIndex, DWORD dwFlags, LPCWSTR pzsTitle, LPCWSTR pszDescription, CONST BYTE *pbImage, DWORD cbImage, CONST XPLAYERLIST_USER *rgPlayers, DWORD cPlayers, CONST XPLAYERLIST_BUTTON *pXButton, CONST XPLAYERLIST_BUTTON *pYButton, XPLAYERLIST_RESULT *pResult, XOVERLAPPED *pOverlapped)
{
	TRACE("XShowCustomPlayerListUI");
	return 1;
}

HWND hGameWnd = NULL;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {

	DWORD wndPID;
	GetWindowThreadProcessId(hwnd, &wndPID);
	if (wndPID == *(DWORD*)&lParam)
	{
		hGameWnd = hwnd;
		return FALSE;
	}
	return TRUE;
}

struct XShowKeyboardUI_DATA
{
	LPCWSTR wseDefaultText;
	LPCWSTR wszTitleText;
	LPCWSTR wszDescriptionText;
	LPWSTR wszResultText;
	DWORD cchResultText;
	DWORD ret;
};


BOOL CALLBACK MyDlgProc_KeyboardUI(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static XShowKeyboardUI_DATA* keydata = NULL;

	switch (message)
	{
	case WM_INITDIALOG:
	{
		keydata = (XShowKeyboardUI_DATA*)lParam;
		if (keydata)
		{
			RECT desktop;
			RECT dialog;
			const HWND hDesktop = GetDesktopWindow();
			GetWindowRect(hDesktop, &desktop);
			GetWindowRect(hDlg, &dialog);
			SetWindowPos(hDlg, HWND_TOPMOST, (desktop.right / 2) - (dialog.right / 2), (desktop.bottom / 2) - (dialog.bottom / 2), NULL, NULL, SWP_NOSIZE);

			SetWindowText(hDlg, keydata->wszTitleText);
			SetDlgItemText(hDlg, IDC_EDIT1, keydata->wseDefaultText);
			SetDlgItemText(hDlg, IDC_DSC1, keydata->wszDescriptionText);
		}
		return TRUE;
	}

	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			keydata->ret = LOWORD(wParam);

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}


		if (LOWORD(wParam) == IDOK)
		{
			keydata->ret = LOWORD(wParam);

			if (keydata && keydata->wszResultText && keydata->cchResultText)
				GetDlgItemText(hDlg, IDC_EDIT1, keydata->wszResultText, keydata->cchResultText);

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}

	return FALSE;
}


// #5216: XShowKeyboardUI
DWORD WINAPI XShowKeyboardUI(DWORD dwUserIndex, DWORD dwFlags, LPCWSTR wseDefaultText, LPCWSTR wszTitleText, LPCWSTR wszDescriptionText, LPWSTR wszResultText, DWORD cchResultText, PXOVERLAPPED pOverlapped)
{
	TRACE("XShowKeyboardUI  (dwUserIndex = %d, dwFlags = %X, wseDefaultText = %s, wszTitleText = %s, wszDescriptionText = %s, wszResultText = %X, cchResultText = %X, pOverlapped = %X)",
		dwUserIndex, dwFlags, wseDefaultText, wszTitleText, wszDescriptionText, wszResultText, cchResultText, pOverlapped);


	DWORD dwPid = GetCurrentProcessId();
	EnumWindows(EnumWindowsProc, (LPARAM)&dwPid);
	//if(!IsWindow(hGameWnd))
	hGameWnd = NULL;


	XShowKeyboardUI_DATA keydata;


	if (cchResultText && wszResultText)
	{
		keydata.cchResultText = cchResultText;
		keydata.wseDefaultText = wseDefaultText;
		keydata.wszDescriptionText = wszDescriptionText;
		keydata.wszTitleText = wszTitleText;
		keydata.wszResultText = wszResultText;
	}


	DialogBoxParam(hThis, MAKEINTRESOURCE(IDD_XSHOWKEYBOARDUI), hGameWnd, MyDlgProc_KeyboardUI, (LPARAM)&keydata);


	if (keydata.ret == IDOK)
		keydata.ret = ERROR_SUCCESS;

	else
		keydata.ret = ERROR_CANCELLED;


	TRACE("- code = %X", keydata.ret);


	if (pOverlapped)
	{
		pOverlapped->InternalLow = keydata.ret;
		pOverlapped->dwExtendedError = keydata.ret;


		Check_Overlapped(pOverlapped);


		return ERROR_IO_PENDING;
	}


	return keydata.ret;
}

// #5252: XShowGamerCardUI
int WINAPI XShowGamerCardUI(DWORD dwUserIndex,XUID XuidPlayer)
{
	TRACE("XShowGamerCardUI");


	// signin change
	sys_ui = -1;


	return 0;
}

// #5250
DWORD WINAPI XShowAchievementsUI(DWORD dwUserIndex)
{
	TRACE("XShowAchievementsUI");
	return 0;
}

// #5365: XShowMarketplaceUI
DWORD WINAPI XShowMarketplaceUI(DWORD dwUserIndex, DWORD dwEntryPoint, ULONGLONG dwOfferId, DWORD dwContentCategories)
{
	TRACE("XShowMarketplaceUI");
	return 1;
}

// 5366 XShowMarketplaceDownloadItemsUI
DWORD WINAPI XShowMarketplaceDownloadItemsUI(DWORD dwUserIndex, DWORD dwEntryPoint, CONST ULONGLONG* pOfferIDs, DWORD dwOfferIdCount, HRESULT *phrResult, PXOVERLAPPED pOverlapped)
{
	TRACE("XShowMarketplaceDownloadItemsUI");

	// not done - error now
	return 0x57;
}

// 5375: XMarketplaceGetImageUrl
DWORD WINAPI XMarketplaceGetImageUrl(DWORD dwTitleId, ULONGLONG qwOfferID, DWORD cchStringBuffer, WCHAR *pwszStringBuffer)
{
	return 0;
}

// 5374: XMarketplaceGetDownloadStatus
DWORD WINAPI XMarketplaceGetDownloadStatus(DWORD dwUserIndex, ULONGLONG qwOfferID, LPDWORD pdwResult /* out*/)
{
	return 0;
}

struct XShowMessageBoxUI_DATA
{
	LPCWSTR wszTitleText;
	LPCWSTR wszDescriptionText;
	LPCWSTR *wszButtons;
	DWORD dwButtons;
	DWORD ret;
};


BOOL CALLBACK MyDlgProc_MessageBoxUI(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static XShowMessageBoxUI_DATA* keydata = NULL;

	switch (message)
	{
	case WM_INITDIALOG:
	{
		keydata = (XShowMessageBoxUI_DATA*)lParam;
		if (keydata)
		{
			RECT desktop;
			RECT dialog;
			const HWND hDesktop = GetDesktopWindow();
			GetWindowRect(hDesktop, &desktop);
			GetWindowRect(hDlg, &dialog);
			SetWindowPos(hDlg, HWND_TOPMOST, (desktop.right / 2) - (dialog.right / 2), (desktop.bottom / 2) - (dialog.bottom / 2), NULL, NULL, SWP_NOSIZE);


			SetWindowText(hDlg, keydata->wszTitleText);
			SetDlgItemText(hDlg, IDC_DSC1, keydata->wszDescriptionText);


			if (keydata->dwButtons >= 1)
				SetDlgItemText(hDlg, IDOK, keydata->wszButtons[0]);

			if (keydata->dwButtons >= 2)
				SetDlgItemText(hDlg, IDCANCEL, keydata->wszButtons[1]);
		}
		return TRUE;
	}

	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			keydata->ret = 1;

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}


		if (LOWORD(wParam) == IDOK)
		{
			keydata->ret = 0;

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}

	return FALSE;
}


// #5266: XShowMessageBoxUI
DWORD WINAPI XShowMessageBoxUI(DWORD dwUserIndex, LPCWSTR wszTitle, LPCWSTR wszText, DWORD cButtons, LPCWSTR *pwszButtons,
	DWORD dwFocusButton, DWORD dwFlags, MESSAGEBOX_RESULT *pResult, XOVERLAPPED *pOverlapped)
{
	TRACE("XShowMessageBoxUI  (%s = %s)", wszTitle, wszText);


	// Checkme
	//if( dwFlags & XMB_PASSCODEMODE )
	//if( dwFlags & XMB_VERIFYPASSCODEMODE )


	DWORD dwPid = GetCurrentProcessId();
	EnumWindows(EnumWindowsProc, (LPARAM)&dwPid);
	//if(!IsWindow(hGameWnd))
	hGameWnd = NULL;


	XShowMessageBoxUI_DATA keydata;

	keydata.wszDescriptionText = wszText;
	keydata.wszTitleText = wszTitle;
	keydata.wszButtons = pwszButtons;
	keydata.dwButtons = cButtons;


	if (cButtons == 1)
		DialogBoxParam(hThis, MAKEINTRESOURCE(IDD_XSHOWMESSAGEBOXUI_1), hGameWnd, MyDlgProc_MessageBoxUI, (LPARAM)&keydata);

	else if (cButtons == 2)
		DialogBoxParam(hThis, MAKEINTRESOURCE(IDD_XSHOWMESSAGEBOXUI_2), hGameWnd, MyDlgProc_MessageBoxUI, (LPARAM)&keydata);

	if (pResult)
		pResult->dwButtonPressed = keydata.ret;



	if (pOverlapped)
	{
		pOverlapped->InternalLow = ERROR_SUCCESS;
		pOverlapped->dwExtendedError = ERROR_SUCCESS;

		return ERROR_IO_PENDING;
	}


	return ERROR_SUCCESS;
}

// #5214: XShowPlayerReviewUI
int WINAPI XShowPlayerReviewUI(DWORD dwUserIndex,XUID XuidFeedbackTarget)
{
	TRACE("XShowPlayerReviewUI");
	return 0;
}
