//=============================================================================
//
// 3Dモーションプレイヤークラス(motion_player3D.h)
// Author : 唐﨑結斗
// 概要 : 3Dモーションプレイヤー生成を行う
//
//=============================================================================
#ifndef _MOTION_PLAYER3D_H_			// このマクロ定義がされてなかったら
#define _MOTION_PLAYER3D_H_			// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "model3D.h"
#include "main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CMotion;
class CMove;

//=============================================================================
// 3Dモーションプレイヤークラス
// Author : 唐﨑結斗
// 概要 : 3Dモーションプレイヤー生成を行うクラス
//=============================================================================
class CMotionPlayer3D : public CModel3D
{
public:
	//--------------------------------------------------------------------
	// 定数定義
	//--------------------------------------------------------------------
	static const float				ROTATE_SPEED;
	static const unsigned int		MAX_CNT_SHOT = 15;			// 弾発射までのカウント
	static const unsigned int		MAX_MOTION = 2;				// モーションのカウント
	static const unsigned int		MAX_LIFE = 5;				// ライフの最大値
	static const unsigned int		MAX_ENERGY = 100;			// エネルギーの最大
	static const unsigned int		MAX_CNT_RECOVERY = 60;		// 回復までのカウント
	static const unsigned int		ENERGY_RECOVERY = 20;		// 回復に消費するエネルギー
	static const unsigned int		ENERGY_FOLLOW_SHOT = 10;	// 追従弾発射に消費するエネルギー

	//=============================================================================
	// モーションのタイプ列挙型
	// Author : 唐﨑結斗
	// 概要 : モーションの種別
	//=============================================================================
	enum MOTION_TYPE
	{
		TYPE_NEUTRAL = 0,			// ニュートラルモーション
		TYPE_MOVE,					// 移動モーション
		TYPE_ATTACK,				// 攻撃モーション
		MAX_MOTION_TYPE,			// タイプの最大数
	};

	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CMotionPlayer3D *Create();			// 3Dモーションプレイヤーの生成

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CMotionPlayer3D();
	~CMotionPlayer3D();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init() override;								// 初期化
	void Uninit() override;									// 終了
	void Update() override;									// 更新
	void Draw() override;									// 描画
	void Hit();												// ダメージ
	void Charge(const int nEnergy);							// エネルギー吸収
	int GetLife() { return m_nLife; }						// 体力のゲッター
	int GetInvalidLife() { return m_nInvalidLife; }			// 追加体力のゲッター
	
private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void Rotate();								// 回転
	D3DXVECTOR3 Move();							// 移動
	void Shot();								// 弾の発射
	void CollisionScreen();						// スクリーンのあたり判定
	void ChangeColor();							// 色の変更
	void MotionSet();							// モーションの設定
	void Recovery();							// 回復
	void FollowShot();							// 追従弾の発射
	void Consumption();							// エネルギー消費

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CMotion			*m_pMotion[MAX_MOTION];			// モーションインスタンス
	CMove			*m_pMove;						// 移動クラスのインスタンス
	D3DXVECTOR3		m_rotDest;						// 目的の向き
	int				m_nCntShot;						// 弾発射までのカウント
	int				m_nNumMotion;					// 現在のモーション番号
	int				m_nNumMotionOld;				// 過去のモーション番号
	int				m_nLife;						// 寿命
	int				m_nInvalidLife;					// 追加ライフ
	int				m_nEnergy;						// エネルギー
	int				m_nCntConsumption;				// エネルギー消費カウント
	int				m_nCombo;						// コンボ
	bool			m_bMotion;						// モーションの使用状況
	bool			m_bMotionBlend;					// モーションブレンドの使用状況
	bool			m_bPressShot;					// 長押し弾を使用してるかどうか
	bool			m_bLockShot;					// 弾発射が可能かどうか
	bool			m_bPressConsumption;			// エネルギー消費してるかどうか
	bool			m_bLockConsumption;				// エネルギー消費が可能かどうか
	bool			m_bRecovery;					// 回復を行ったかどうか
	bool			m_bFollowShot;					// 追従弾を発射しているか否
};

#endif