//=============================================================================
//
// アプリケーションクラス(application.h)
// Author : 唐﨑結斗
// 概要 : 各クラスの呼び出しを行うクラス
//
//=============================================================================
#ifndef _APPLICATION_H_		// このマクロ定義がされてなかったら
#define _APPLICATION_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CRenderer;
class CKeyboard;
class CMouse;
class CTexture;
class CModelManager;
class CCameraManager;
class CSound;
class CCamera;
class CSceneMode;
class CFade;
class CPause;

//=============================================================================
// アプリケーションクラス
// Author : 唐﨑結斗
// 概要 : 各クラスの呼び出しを行うクラス
//=============================================================================
class CApplication
{
public:
	//--------------------------------------------------------------------
	// シーンモードの列挙型
	// Author : 唐﨑結斗
	// 概要 : シーンモードを識別する
	//--------------------------------------------------------------------
	enum SCENE_MODE
	{
		MODE_TITLE = 0,			// タイトル
		MODE_GAME,				// ゲーム
		MODE_RESULT,			// リザルト
		MODE_TUTORIAL,			// チュートリアル
		MAX_MODE,				// シーンの最大数
		MODE_NONE,				// シーン無し
	};

	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CRenderer *GetRenderer() { return m_pRenderer; }								// レンダラーのゲッター
	static CKeyboard *GetKeyboard() { return m_pKeyboard; }								// キーボードのゲッター
	static CMouse *GetMouse() { return m_pMouse; }										// マウスのゲッター
	static CTexture *GetTexture() { return m_pTexture; }								// テクスチャのゲッター
	static CModelManager *GetModelManager() { return m_pModelManager; }					// モデルマネージャーのゲッター
	static CCameraManager *GetCameraManager() { return m_pCameraManager; }				// カメラマネージャーのゲッター
	static CSound *GetSound() { return m_pSound; }										// サウンドのゲッター
	static CCamera *GetCamera() { return m_pCamera; }									// カメラのゲッター
	static CCamera *GetCameraBG() { return m_pCameraBG; }								// 背景カメラのゲッター
	static CFade *GetFade() { return m_pFade; }											// 背景カメラのゲッター
	static D3DXVECTOR3 ScreenCastWorld(const D3DXVECTOR3 &pos);							// ワールド座標をスクリーン座標にキャストする
	static D3DXVECTOR3 WorldCastScreen(const D3DXVECTOR3 &pos);							// ワールド座標をスクリーン座標にキャストする
	static float RotNormalization(float fRot);											// 角度の設定
	static float RotNormalization(float fRot,float fMin,float fMax);					// 角度の設定
	static void SetNextMode(SCENE_MODE mode) { m_nextMode = mode; }						// 次のモードの設定
	static void SetMode(SCENE_MODE mode);												// モードの設定
	static SCENE_MODE GetMode() { return m_mode; }										// モードの取得
	static void SetScore(const int nScore) { m_nScore = nScore; }						// スコアの設定
	static int GetScore() { return m_nScore; }											// スコアの取得
	static HWND GetWnd() { return m_hWnd; }												// ウィンドウの取得
	static CPause *GetPause() { return m_pPause; }										// ポーズの取得

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CApplication();
	~CApplication();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);					// 初期化
	void Uninit();													// 終了
	void Update();													// 更新
	void Draw();													// 描画

private:
	//--------------------------------------------------------------------
	// 静的メンバ変数
	//--------------------------------------------------------------------
	static HWND	m_hWnd;							// ウィンドウ
	static CRenderer *m_pRenderer;				// rendererクラス
	static CKeyboard *m_pKeyboard;				// キーボードクラス
	static CMouse *m_pMouse;					// マウスクラス
	static CTexture *m_pTexture;				// テクスチャクラス
	static CModelManager *m_pModelManager;		// モデルマネージャークラス
	static CCameraManager *m_pCameraManager;	// カメラマネージャークラス
	static CSound *m_pSound;					// サウンドクラス
	static CCamera *m_pCamera;					// ゲームカメラクラス
	static CCamera *m_pCameraBG;				// 背景カメラクラス
	static SCENE_MODE m_mode;					// 現在のモードの格納
	static SCENE_MODE m_nextMode;				// 次に設定するモード
	static CSceneMode *pSceneMode;				// シーンモードを格納
	static CFade *m_pFade;						// フェードクラス
	static CPause *m_pPause;					// ポーズクラス
	static int m_nScore;						// 現在のスコア
};

#endif

