//=============================================================================
//
// カメラマネージャー処理(camera_manager.h)
// Author : 唐�ｱ結斗
// 概要 : カメラのアクション設定を行う
//
//=============================================================================
#ifndef _CAMERA_MANAGER_H_		// このマクロ定義がされてなかったら
#define _CAMERA_MANAGER_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"
#include "camera.h"

//=============================================================================
// カメラマネージャークラス
// Author : 唐�ｱ結斗
// 概要 : カメラのアクション設定を行うクラス
//=============================================================================
class CCameraManager
{
public:
	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CCameraManager();
	~CCameraManager();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	// 初期化
	void Init(void);

	// 終了
	void Uninit(void);

	// カメラのアクション情報のゲッター
	CCamera::CAMERA_ACTION *GetCameraAction() { return m_pCameraAction; }

	// カメラのアクション数のゲッター
	int GetNumAction() { return m_nMaxAction; }

	// ファイルの読み込み
	void LoadFile(const char *pFileName);

	// 視点のゲッター
	D3DXVECTOR3 GetPosV() { return m_posV; }

	// 注視点のゲッター
	D3DXVECTOR3 GetPosR() { return m_posR; }

	// 向きのゲッター
	D3DXVECTOR3 GetRot() { return m_rot; }

private:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CCamera::CAMERA_ACTION		*m_pCameraAction;			// カメラのアクション
	D3DXVECTOR3					m_posV;						// 視点
	D3DXVECTOR3					m_posR;						// 注視点
	D3DXVECTOR3					m_rot;						// 向き
	int							m_nMaxAction;				// アクション数
};

#endif


