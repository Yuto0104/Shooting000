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
#include "object.h"
#include "object2D.h"
#include "score.h"
#include "gauge2D.h"
#include "motion_player3D.h"
#include "renderer.h"
#include "application.h"
#include "texture.h"
#include "game.h"

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
	m_pObject2D->LoadTex(-1);
	m_pObject2D->SetCol(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));

	// スコアの生成
	m_pScore = CScore::Create(6, true);
	assert(m_pScore != nullptr);
	m_pScore->SetScore(0);

	// ゲージの生成
	m_pGauge2D = CGauge2D::Create();
	assert(m_pGauge2D != nullptr);
	m_pGauge2D->SetCol(D3DXCOLOR(0.2f, 0.9f, 1.0f, 1.0f));
	m_pGauge2D->SetMaxNumber((float)CMotionPlayer3D::MAX_ENERGY);
	m_pGauge2D->SetCoefficient(0.06f);

	// 大きさの設定
	SetSize(D3DXVECTOR3(70.0f, 500.0f, 0.0f));

	// 位置の設定
	SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));

	// 向きの設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CEnergyGage::Uninit()
{
	m_pScore->Uninit();
	m_pGauge2D->Uninit();

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
	CMotionPlayer3D *pPlayer = CGame::GetMotionPlayer3D();

	if (pPlayer != nullptr)
	{
		CObject::COLOR_TYPE colorType = pPlayer->GetColorType();

		if (colorType == CObject::TYPE_WHITE)
		{
			m_pGauge2D->SetCol(D3DXCOLOR(0.2f, 0.9f, 1.0f, 1.0f));
		}
		else if (colorType == CObject::TYPE_BLACK)
		{
			m_pGauge2D->SetCol(D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f));
		}
	}
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
	m_pScore->SetPos(D3DXVECTOR3(m_pos.x, (m_pos.y - m_size.y / 2.0f) + m_pScore->GetSize().y / 2.0f, m_pos.z));
	m_pGauge2D->SetPos(D3DXVECTOR3(m_pos.x, (m_pos.y + m_size.y / 2.0f - 7.0f), m_pos.z));
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
	m_pScore->SetRot(m_rot);
	m_pGauge2D->SetRot(m_rot);
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
	m_pScore->SetWholeSize(D3DXVECTOR3(m_size.x, size.y / 10.0f, size.z));
	m_pScore->SetSize(D3DXVECTOR3(m_size.x / 3.0f, size.y / 10.0f, size.z));
	m_pGauge2D->SetSize(D3DXVECTOR3(m_size.x / 10.0f * 8.0f, size.y / 10.0f * 9.0f, size.z));
}