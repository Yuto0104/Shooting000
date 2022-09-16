//=============================================================================
//
// パーティクルクラス(particle.h)
// Author : 唐﨑結斗
// 概要 : パーティクル生成を行う
//
//=============================================================================
#ifndef _PARTICLE_H_		// このマクロ定義がされてなかったら
#define _PARTICLE_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//=============================================================================
// パーティクルクラス
// Author : 唐﨑結斗
// 概要 : パーティクル生成を行うクラス
//=============================================================================
class CParticle : public CObject
{
private:
	//--------------------------------------------------------------------
	// 定数定義
	//--------------------------------------------------------------------

public:
	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CParticle *Create();		// パーティクルの生成

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CParticle(int nPriority = CObject::PRIORITY_LEVEL3);
	~CParticle() override;

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init() override;														// 初期化
	void Uninit() override;															// 終了
	void Update() override;															// 更新
	void Draw() override;															// 描画
	void SetPos(const D3DXVECTOR3 &pos) override;									// 位置のセッター
	void SetPosOld(const D3DXVECTOR3 &posOld) override { m_posOld = posOld; }		// 過去位置のセッター
	void SetRot(const D3DXVECTOR3 &rot) override;									// 向きのセッター
	void SetSize(const D3DXVECTOR3 &size) override;									// 大きさのセッター
	D3DXVECTOR3 GetPos() override { return m_pos; }									// 位置のゲッター
	D3DXVECTOR3 GetPosOld()  override { return m_posOld; }							// 過去位置のゲッター
	D3DXVECTOR3 GetRot()  override { return m_rot; }								// 向きのゲッター
	D3DXVECTOR3 GetSize()  override { return m_size; }								// 大きさのゲッター
	void SetColor(const D3DXCOLOR &col) { m_color = col; }							// 色の設定			
	void SetPopRange(const D3DXVECTOR3 popRange) { m_popRange = popRange; }			// 生成範囲の設定
	void SetMoveVec(const D3DXVECTOR3 moveVec) { m_moveVec = moveVec; }				// 移動方向の設定
	void SetSpeed(const float fSpeed) { m_fSpeed = fSpeed; }						// 速度の設定
	void SetEffectLife(const int nEffectLife) { m_nEffectLife = nEffectLife; }		// エフェクトの寿命の設定
	void SetLife(const int nLife) { m_nLife = nLife; }								// 寿命の設定
	void SetMaxEffect(const int nMaxEffect) { m_nMaxEffect = nMaxEffect; }			// 位置フレーム出すエフェクト数のセッター

private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void SetEffect();

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	D3DXVECTOR3			m_pos;				// 位置
	D3DXVECTOR3			m_posOld;			// 過去の位置
	D3DXVECTOR3			m_rot;				// 向き
	D3DXVECTOR3			m_popRange;			// 生成範囲
	D3DXVECTOR3			m_size;				// 大きさ
	D3DXVECTOR3			m_moveVec;			// 移動方向
	D3DXCOLOR			m_color;			// 色
	float				m_fSpeed;			// 速度
	int					m_nEffectLife;		// エフェクトの寿命
	int					m_nLife;			// 寿命
	int					m_nMaxEffect;		// 位置フレーム出すエフェクト数
};

#endif


