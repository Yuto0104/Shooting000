//=============================================================================
//
// エネルギーゲージクラス(energy_gage.h)
// Author : 唐﨑結斗
// 概要 : エネルギーゲージの設定を行う
//
//=============================================================================
#ifndef _ENERGY_GAGE_H_		// このマクロ定義がされてなかったら
#define _ENERGY_GAGE_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObject2D;
class CScore;
class CGauge2D;

//=============================================================================
// エネルギーゲージクラス
// Author : 唐﨑結斗
// 概要 : エネルギーゲージ生成を行うクラス
//=============================================================================
class CEnergyGage : public CObject
{
public:
	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CEnergyGage *Create();			// エネルギーゲージの生成

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CEnergyGage(int nPriority = CObject::PRIORITY_LEVEL3);
	~CEnergyGage() override;

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
	CScore *GetScore() { return m_pScore; }											// スコアのゲッター
	CGauge2D *GetGauge2D() { return m_pGauge2D; }									// ゲージのゲッター

private:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CObject2D			*m_pObject2D;		// 2Dオブジェクト
	CScore				*m_pScore;			// スコア
	CGauge2D			*m_pGauge2D;		// ゲージ
	D3DXVECTOR3			m_pos;				// 位置
	D3DXVECTOR3			m_posOld;			// 過去の位置
	D3DXVECTOR3			m_rot;				// 向き
	D3DXVECTOR3			m_size;				// 大きさ
	int					m_nLife;			// ライフ
	int					m_nAddLife;			// 追加ライフ
	int					m_nMaxLife;			// 最大ライフ
};

#endif



