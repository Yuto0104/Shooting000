//=============================================================================
//
// 2Dゲージクラス(gauge2D.h)
// Author : 唐﨑結斗
// 概要 : オブジェクト生成を行う
//
//=============================================================================
#ifndef _GAUGE2D_H_		// このマクロ定義がされてなかったら
#define _GAUGE2D_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"
#include "texture.h"

//=============================================================================
// 2Dオブジェクトクラス
// Author : 唐﨑結斗
// 概要 : 2Dオブジェクト生成を行うクラス
//=============================================================================
class CGauge2D : public CObject
{
public:
	// 頂点フォーマット
	const unsigned int	FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	//*****************************************************************************
	// 構造体定義
	//*****************************************************************************
	// 頂点データ
	struct VERTEX_2D
	{
		D3DXVECTOR3		pos;		// 位置
		float			rhw;		// rhw
		D3DCOLOR		col;		// カラー
		D3DXVECTOR2		tex;		// テクスチャ座標
	};

	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CGauge2D *Create(void);				// 2Dオブジェクトの生成

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CGauge2D(int nPriority = CObject::PRIORITY_LEVEL3);
	~CGauge2D() override;

	//--------------------------------------------------------------------
	// オーバーライド関数
	//--------------------------------------------------------------------
	HRESULT Init() override;																// 初期化
	void Uninit() override;																	// 終了
	void Update() override;																	// 更新
	void Draw() override;																	// 描画
	void SetPos(const D3DXVECTOR3 &pos) override;											// 位置のセッター
	void SetPosOld(const D3DXVECTOR3 &posOld) override { m_posOld = posOld; }				// 過去位置のセッター
	void SetRot(const D3DXVECTOR3 &rot) override;											// 向きのセッター
	void SetSize(const D3DXVECTOR3 &size) override;											// 大きさのセッター
	D3DXVECTOR3 GetPos() override { return m_pos; }											// 位置のゲッター
	D3DXVECTOR3 GetPosOld()  override { return m_posOld; }									// 過去位置のゲッター
	D3DXVECTOR3 GetRot()  override { return m_rot; }										// 向きのゲッター
	D3DXVECTOR3 GetSize()  override { return m_size; }										// 大きさのゲッター
	void SetCol(const D3DCOLOR &col);														// 色のセッター
	void SetMaxNumber(const float fMaxNumber);												// 数値の最大
	void SetNumber(const float fNumber) { m_fDestNumber = fNumber; }						// 数値の設定
	void SetCoefficient(const float fCoefficient) { m_fCoefficient = fCoefficient; }		// 減衰係数の設定

protected:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void SetVtx();															// 頂点座標などの設定
	void SetTex(const D3DXVECTOR2 &minTex, const D3DXVECTOR2 &maxTex);		// テクスチャ座標の設定
	void LoadTex(const int nNumTex) { m_nNumTex = nNumTex; }				// テクスチャの設定

private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void SetGauge();

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	LPDIRECT3DVERTEXBUFFER9			m_pVtxBuff;					// 頂点バッファ
	D3DXVECTOR3						m_pos;						// 位置
	D3DXVECTOR3						m_posOld;					// 過去の位置
	D3DXVECTOR3						m_rot;						// 向き
	D3DXVECTOR3						m_size;						// 大きさ
	D3DXVECTOR3						m_maxSize;					// 大きさ最大値
	D3DCOLOR						m_col;						// カラー
	int								m_nNumTex;					// テクスチャの種別
	float							m_fAngle;					// 対角線の角度
	float							m_fLength;					// 対角線の長さ
	float							m_fAngleOrigin;				// もとの対角線の角度
	float							m_fLengthOrigin;			// もと対角線の長さ
	float							m_fMaxNumber;				// 数値の最大数
	float							m_fNumber;					// 数値
	float							m_fDestNumber;				// 目的の数値
	float							m_fCoefficient;				// 減衰係数
};

#endif



