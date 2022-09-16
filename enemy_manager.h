//=============================================================================
//
// エネミーマネージャー処理(enemy_manager.h)
// Author : 唐﨑結斗
// 概要 : エネミーマネージャーを行う
//
//=============================================================================
#ifndef _ENEMY_MANAGER_H_		// このマクロ定義がされてなかったら
#define _ENEMY_MANAGER_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"
#include "enemy3D.h"

//=============================================================================
// エネミーマネージャークラス
// Author : 唐﨑結斗
// 概要 : エネミーマネージャーを行うクラス
//=============================================================================
class CEnemyManager : public CObject
{
public:
	//--------------------------------------------------------------------
	// 敵の種別の列挙型
	// Author : 唐﨑結斗
	// 概要 : 敵の種別分けをする
	//--------------------------------------------------------------------
	enum EENEMY_MOVE_TYPE
	{
		TYPE_NORMAL = 0,		// 通常
		MAX_TYPE,				// 種別数の最大数
	};

	//--------------------------------------------------------------------
	// 敵のステータス
	// Author : 唐﨑結斗
	// 概要 : 敵のステータス
	//--------------------------------------------------------------------
	struct ENEMY_STATUS
	{
		COLOR_TYPE				colorType;				// カラータイプ
		CEnemy3D::SHOT_MODE		shotMode;				// 弾の種別
		D3DXVECTOR3				collisionSize;			// あたり判定の大きさ
		D3DXVECTOR3				size;					// 大きさ
		float					fBulletRot;				// 弾の発射方向
		float					fDiffusionWidth;		// 弾の拡散範囲
		float					fBulletSpeed;			// 弾速
		float					fCoeffBullet;			// 弾の追従の減衰係数
		int						nMaxBullet;				// 弾の発射数
		int						nModelID;				// モデルID
		int						nLife;					// 体力
		int						nScore;					// スコア
		int						nMaxShot;				// 弾発射するカウント
	};

	//--------------------------------------------------------------------
	// 敵の配置情報
	// Author : 唐﨑結斗
	// 概要 : 敵の配置情報
	//--------------------------------------------------------------------
	struct ENEMY_ARRANGEMENT
	{
		D3DXVECTOR3			pos;			// 位置
		int					nMoveID;		// 移動情報を格納
		int					nFrame;			// エネミーの発生フレーム数
		int					nID;			// 識別番号
		int					nDrawType;		// 描画タイプ
		EENEMY_MOVE_TYPE	type;			// 種別
	};

	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CEnemyManager *Create();			// エネルギーゲージの生成

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CEnemyManager();
	~CEnemyManager() override;

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init() override;														// 初期化
	void Uninit() override;															// 終了
	void Update() override;															// 更新
	void Draw() override;															// 描画
	void SetPos(const D3DXVECTOR3 &pos) override { pos; }							// 位置のセッター
	void SetPosOld(const D3DXVECTOR3 &posOld) override { posOld; }					// 過去位置のセッター
	void SetRot(const D3DXVECTOR3 &rot) override { rot; }							// 向きのセッター
	void SetSize(const D3DXVECTOR3 &size) override { size; }						// 大きさのセッター
	D3DXVECTOR3 GetPos() override { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }			// 位置のゲッター
	D3DXVECTOR3 GetPosOld()  override { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 過去位置のゲッター
	D3DXVECTOR3 GetRot()  override { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 向きのゲッター
	D3DXVECTOR3 GetSize()  override { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 大きさのゲッター
	int GetCntFrame() { return m_nCntFrame; }										// フレーム数のゲッター
	void LoadFile(char *pFileName);

private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void SetEnemy();

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	D3DXVECTOR3				m_pos;					// 位置
	D3DXVECTOR3				m_posOld;				// 過去の位置
	D3DXVECTOR3				m_rot;					// 向き
	D3DXVECTOR3				m_size;					// 大きさ
	ENEMY_STATUS			*m_status;				// 敵のステータス
	ENEMY_ARRANGEMENT		*m_arrangement;			// 敵の配置
	CEnemy3D::ENEMY_MOVE	*m_move;				// 移動情報
	int						m_nMaxType;				// 種類の最大数
	int						m_nMaxInstall;			// 最大設置数
	int						m_nMaxMove;				// 移動情報数
	int						m_nGameEndFrame;		// ゲームの終了時間
	int						m_nCntFrame;			// フレームカウント
	int						m_nCntSetEnemy;			// 敵の設置カウント
};

#endif
