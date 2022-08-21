//=============================================================================
//
// スコアクラス(score.cpp)
// Author : 唐﨑結斗
// 概要 : スコア生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>
#include <math.h>

#include "energy_gage.h"
#include "object2D.h"
#include "score.h"
#include "gauge2D.h"
#include "motion_player3D.h"
#include "renderer.h"
#include "application.h"
#include "texture.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : スコアを生成する
//=============================================================================
CEnergyGage *CEnergyGage::Create()
{
	// オブジェクトインスタンス
	CEnergyGage *pEnergyGage = nullptr;

	pEnergyGage = new CEnergyGage;

	// メモリの確保ができなかった
	assert(pEnergyGage != nullptr);

	// 数値の初期化
	pEnergyGage->Init();

	// インスタンスを返す
	return pEnergyGage;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CEnergyGage::CEnergyGage(int nPriority /*= CObject::PRIORITY_LEVEL3*/) : CObject(nPriority)
{
	m_pObject2D = nullptr;								// 2Dオブジェクト
	m_pScore = nullptr;									// スコア
	m_pGauge2D = nullptr;								// ゲージ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 過去の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 大きさ
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CEnergyGage::~CEnergyGage()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CEnergyGage::Init()
{
	// 2Dオブジェクトの生成
	m_pObject2D = CObject2D::Create();
	assert(m_pObject2D != nullptr);
	m_pObject2D->LoadTex(CTexture::TYPE_NULL);
	m_pObject2D->SetCol(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));

	// スコアの生成
	m_pScore = CScore::Create(3, true);
	assert(m_pScore != nullptr);

	//// ゲージの生成
	//m_pGauge2D = CGauge2D::Create();
	//assert(m_pGauge2D != nullptr);

	// 位置の設定
	SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));

	// 向きの設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	
	// 大きさの設定
	SetSize(D3DXVECTOR3(70.0f, 500.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CEnergyGage::Uninit()
{
	if (m_pObject2D != nullptr)
	{// 終了処理
		m_pObject2D->Uninit();

		// メモリの解放
		delete m_pObject2D;
		m_pObject2D = nullptr;
	}

	if (m_pScore != nullptr)
	{// 終了処理
		m_pScore->Uninit();

		// メモリの解放
		delete m_pScore;
		m_pScore = nullptr;
	}

	if (m_pGauge2D != nullptr)
	{// 終了処理
		m_pGauge2D->Uninit();

		// メモリの解放
		delete m_pGauge2D;
		m_pGauge2D = nullptr;
	}

	// インスタンスの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CEnergyGage::Update()
{

}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CEnergyGage::Draw()
{

}

//=============================================================================
// 位置のセッター
// Author : 唐﨑結斗
// 概要 : 位置のメンバ変数に引数を代入
//=============================================================================
void CEnergyGage::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;

	m_pObject2D->SetPos(m_pos);
}

//=============================================================================
// 向きのセッター
// Author : 唐﨑結斗
// 概要 : 向きのメンバ変数に引数を代入
//=============================================================================
void CEnergyGage::SetRot(const D3DXVECTOR3 &rot)
{
	m_rot = rot;

	m_pObject2D->SetRot(m_rot);
}

//=============================================================================
// 大きさのセッター
// Author : 唐﨑結斗
// 概要 : 大きさのメンバ変数に引数を代入
//=============================================================================
void CEnergyGage::SetSize(const D3DXVECTOR3 & size)
{
	m_size = size;

	m_pObject2D->SetSize(m_size);
}