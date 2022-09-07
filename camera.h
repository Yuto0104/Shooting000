//**************************************************************************************************
//
// カメラ処理(camera.h)
// Auther：唐�ｱ結斗
// 概要 : カメラの設定処理
//
//**************************************************************************************************
#ifndef _CAMERA_H_			// このマクロ定義がされてなかったら
#define _CAMERA_H_			// 二重インクルード防止のマクロ定義

//***************************************************************************
// インクルード
//***************************************************************************
#include"main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CMove;

//=============================================================================
// カメラクラス
// Author : 唐�ｱ結斗
// 概要 : カメラ設定を行うクラス
//=============================================================================
class CCamera
{
private:
	//--------------------------------------------------------------------
	// 定数定義
	//--------------------------------------------------------------------
	static const float CAMERA_NEAR;			// ニアの最大値
	static const float CAMERA_FUR;			// ファーの最大値

public:
	//=============================================================================
	// 投影方法のタイプ列挙型
	// Author : 唐�ｱ結斗
	// 概要 : 投影方法の種別
	//=============================================================================
	enum VIEW_TYPE
	{
		TYPE_CLAIRVOYANCE = 0,		// 透視投影
		TYPE_PARALLEL,				// 平行投影
		MAX_VIEW_TYPE,				// タイプの最大数
	};

	//=============================================================================
	// カメラのアクションの管理
	// Author : 唐�ｱ結斗
	// 概要 : カメラのアクションを管理する変数
	//=============================================================================
	struct CAMERA_ACTION
	{
		D3DXVECTOR3			posVDest;				// 目的の視点位置
		D3DXVECTOR3			posRDest;				// 目的の注視点位置
		int					nFrame;					// 行動に要するフレーム数
	};

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CCamera();
	~CCamera();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init(void);							// 初期化
	void Uninit(void);							// 終了
	void Update(void);							// 更新
	void Set(void);								// 設定
	void SetViewType(VIEW_TYPE type);			// タイプの設定

	//D3DXMATRIX transformQuaternionToRotMat(D3DXQUATERNION inQuaternion);
	D3DXVECTOR3 GetPosV() { return m_posV; }
	D3DXVECTOR3 GetPosR() { return m_posR; }
	D3DXVECTOR3 GetRot() { return m_rot; }
	D3DXMATRIX GetMtxView() { return m_mtxView; }
	D3DXMATRIX GetMtxProj() { return m_mtxProj; }
	float GetDistance() { return m_fDistance; }
	void SetPosV(const D3DXVECTOR3 posV);
	void SetPosR(const D3DXVECTOR3 posR);
	void SetRot(const D3DXVECTOR3 rot, const int nShaft);
	void Set(const D3DXVECTOR3 posV, const D3DXVECTOR3 posR, const D3DXVECTOR3 rot);

private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void Rotate(void);				// 回転
	void Move(void);				// 移動
	void FollowCamera(void);		// 追従
	void Action();					// カメラのアクション
	void Zoom();					// カメラの拡縮
	void SetPosV();					// 視点の設定
	void SetPosR();					// 注視点の設定
	void SetRot();					// 向きの設定
	void SetDistance();				// 視点から注視点までの距離を設定する

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CAMERA_ACTION		*m_pCameraAction;	// カメラのアクション
	CMove				*m_pRoll;			// 移動クラスのインスタンス(角度)
	D3DXMATRIX			m_mtxWorld;			// ワールドマトリックス
	D3DXMATRIX			m_mtxProj;			// プロジェクションマトリックス
	D3DXMATRIX			m_mtxView;			// ビューマトリックス
	D3DXVECTOR3			m_posV;				// 視点
	D3DXVECTOR3			m_posR;				// 注視点
	D3DXVECTOR3			m_vecU;				// 上方向ベクトル
	D3DXVECTOR3			m_rot;				// 向き
	D3DXVECTOR3			m_rotMove;			// 移動方向
	D3DXVECTOR3			m_posVDest;			// 視点の目的の位置
	D3DXVECTOR3			m_posRDest;			// 注視点の目的の位置
	VIEW_TYPE			m_viewType;			// 投影の種別
	float				m_fDistance;		// 視点から注視点までの距離
	float				m_fRotMove;			// 移動方向
	int					m_nCntFrame;		// フレームカウント
	int					m_nMaxAction;		// アクション数
	int					m_nNumAction;		// アクション番号
	bool				m_bAutoMove;		// 自動移動
};

#endif
