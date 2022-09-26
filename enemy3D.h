//=============================================================================
//
// 3Dエネミークラス(enemy3D.h)
// Author : 唐﨑結斗
// 概要 : 3Dエネミー生成を行う
//
//=============================================================================
#ifndef _ENEMY3D_H_			// このマクロ定義がされてなかったら
#define _ENEMY3D_H_			// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"
#include "model3D.h"
#include "main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CEffect3D;

//=============================================================================
// 3Dエネミークラス
// Author : 唐﨑結斗
// 概要 : 3Dエネミー生成を行うクラス
//=============================================================================
class CEnemy3D : public CModel3D
{
public:
	//--------------------------------------------------------------------
	// 定数定義
	//--------------------------------------------------------------------
	static const unsigned int MAX_LINTERN_BULLET = 5;			// 返し弾の最大数
	static const float LINTERN_BULLET_SPAWN_RANGE;				// 返し弾の生成範囲
	static const float LINTERN_BULLET_MOVE_VEC;					// 返し弾の移動方向

	//--------------------------------------------------------------------
	// 移動方法の種別
	// Author : 唐﨑結斗
	// 概要 : 移動方法の種別分けを行う
	//--------------------------------------------------------------------
	enum MOVE_MODE
	{
		MODE_NORMAL = 0,		// 通常移動
		MODE_SIN,				// サインカーブ
		MODE_COS,				// コサインカーブ
		MODE_CIRCLE,			// 円移動
		MODE_NONE,				// 移動しない
		MAX_MODE,				// 移動方法の最大数
	};

	//--------------------------------------------------------------------
	// 発射する弾の種別
	// Author : 唐﨑結斗
	// 概要 : 発射する弾の種別分けを行う
	//--------------------------------------------------------------------
	enum SHOT_MODE
	{
		SHOTMODE_NORMAL = 0,		// 通常
		SHOTMODE_DIFFUSION,			// 拡散
		SHOTMODE_SNIPE,				// スナイプ
		SHOTMODE_FOLLOW,			// 追従
		SHOTMODE_NONE,				// 弾無し
		SHOTMAX_MODE,				// 移動方法の最大数
	};

	//--------------------------------------------------------------------
	// 敵の移動キー
	// Author : 唐﨑結斗
	// 概要 : 敵の移動キー設定
	//--------------------------------------------------------------------
	struct ENEMY_MOVE_KEY
	{
		MOVE_MODE		moveMode;			// 移動方法
		float			fSpeed;				// 移動速度
		float			fMoveVec;			// 移動方向
		float			fAddMoveVec;		// 移動方向の加算値
		float			fWaveLength;		// 波長
		float			fWaveSize;			// 波の大きさ
		int				nFrame;				// 移動を行う時間
	};

	//--------------------------------------------------------------------
	// 敵の移動
	// Author : 唐﨑結斗
	// 概要 : 敵の移動設定
	//--------------------------------------------------------------------
	struct ENEMY_MOVE
	{
		ENEMY_MOVE_KEY		*moveKey;		// 移動キー
		int					nMaxKey;		// 最大キー数
		bool				bLoop;			// ループするかどうか
	};

	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CEnemy3D *Create(const int nNumModel);				// 3Dエネミーの生成
	static int GetCntSetEnemy() { return s_nCntSetEnemy; }		// 設置されている敵の数のカウント

	//--------------------------------------------------------------------
	// 静的メンバ変数
	//--------------------------------------------------------------------
	static int s_nCntSetEnemy;			// 敵のカウント

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CEnemy3D();
	~CEnemy3D();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init(const int nNumModel);																// 初期化
	void Uninit() override;																			// 終了
	void Update() override;																			// 更新
	void Draw() override;																			// 描画
	void Hit(COLOR_TYPE colorType, int nAttack);													// ヒット
	void SetLife(int nLife) { m_nLife = nLife; }													// 体力のセッター
	void SetScore(int nScore) { m_nScore = nScore; }												// スコアのセッター
	void SetMoveData(const ENEMY_MOVE moveData);													// 移動情報のセッター
	void SetShotMode(const SHOT_MODE _shotMode) { m_shotMode = _shotMode; }							// 弾の種別のセッター
	void SetMaxShot(const int nMaxShot) { m_nMaxShot = nMaxShot; }									// 弾の発射カウントの設定
	void SetBulletRot(const float fBulletRot) { m_fBulletRot = fBulletRot; }						// 弾の発射方向の設定
	void SetDiffusionWidth(const float fDiffusionWidth) { m_fDiffusionWidth = fDiffusionWidth; }	// 弾の拡散範囲の設定
	void SetBulletSpeed(const float fBulletSpeed) { m_fBulletSpeed = fBulletSpeed; }				// 弾の発射速度の設定
	void SetCoeffBullet(const float fCoeffBullet) { m_fCoeffBullet = fCoeffBullet; }				// 追従弾の減衰係数の設定
	void SetMaxBullet(const int nMaxBullet) { m_nMaxBullet = nMaxBullet; }							// 弾の最大発射数

private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void Shot();			// 弾の発射
	void SetMove();			// 移動量の設定
	void Move();			// 移動情報の管理
	void SetMoveCopy();		// 移動情報の代入
	void ScreenIn();		// スクリーンの判定

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CEffect3D		*m_pEffect3D;		// エフェクトのインスタンス
	D3DXVECTOR3		m_move;				// 移動量
	ENEMY_MOVE		m_moveData;			// 移動情報
	MOVE_MODE		m_moveMode;			// 移動方法
	SHOT_MODE		m_shotMode;			// 弾の種別
	float			m_fSpeed;			// 移動速度
	float			m_fMoveVec;			// 移動方向
	float			m_fAddMoveVec;		// 移動方向の加算値
	float			m_fWave;			// 波
	float			m_fWaveLength;		// 波長
	float			m_fWaveSize;		// 波の大きさ
	float			m_fBulletRot;		// 弾の発射方向
	float			m_fDiffusionWidth;	// 弾の拡散範囲
	float			m_fBulletSpeed;		// 弾速
	float			m_fCoeffBullet;		// 弾の追従の減衰係数
	int				m_nMaxBullet;		// 弾の発射数
	int				m_nCntKey;			// キーカウント
	int				m_nCntFrame;		// フレームカウント
	int				m_nLife;			// 体力
	int				m_nScore;			// スコア
	int				m_nCntShot;			// 弾発射までのカウント
	int				m_nMaxShot;			// 弾発射するカウント
	bool			m_bMove;			// 移動を行っている
	bool			m_bUse;				// 使用されているかどうか
};

#endif
