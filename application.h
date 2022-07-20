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
class CSound;
class CCamera;
class CPlayer2D;
class CMotionPlayer3D;
class CScore;

//=============================================================================
// アプリケーションクラス
// Author : 唐﨑結斗
// 概要 : 各クラスの呼び出しを行うクラス
//=============================================================================
class CApplication
{
public:
	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CRenderer *GetRenderer() { return m_pRenderer; }								// レンダラーのゲッター
	static CKeyboard *GetKeyboard() { return m_pKeyboard; }								// キーボードのゲッター
	static CMouse *GetMouse() { return m_pMouse; }										// マウスのゲッター
	static CTexture *GetTexture() { return m_pTexture; }								// テクスチャのゲッター
	static CSound *GetSound() { return m_pSound; }										// サウンドのゲッター
	static CCamera *GetCamera() { return m_pCamera; }									// カメラのゲッター
	static CCamera *GetCameraBG() { return m_pCameraBG; }								// 背景カメラのゲッター
	static CPlayer2D *GetPlayer2D() { return m_pPlayer2D; }								// プレイヤーのゲッター
	static CMotionPlayer3D *GetMotionPlayer3D() { return m_MotionPlayer3D; }			// モーションプレイヤーのゲッター
	static CScore *GetScore() { return m_pScore; }										// スコアの取得

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
	static CRenderer *m_pRenderer;						// rendererクラス
	static CKeyboard *m_pKeyboard;						// キーボードクラス
	static CMouse *m_pMouse;							// マウスクラス
	static CTexture *m_pTexture;						// テクスチャクラス
	static CSound *m_pSound;							// サウンドクラス
	static CCamera *m_pCamera;							// ゲームカメラクラス
	static CCamera *m_pCameraBG;						// 背景カメラクラス
	static CPlayer2D *m_pPlayer2D;						// プレイヤークラス
	static CMotionPlayer3D *m_MotionPlayer3D;			// モーションプレイヤークラス
	static CScore *m_pScore;							// スコアクラス
};

#endif

