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

#include "life_manager.h"
#include "life.h"
#include "motion_player3D.h"
#include "renderer.h"
#include "application.h"
#include "game.h"
#include "tutorial.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : スコアを生成する
//=============================================================================
CLifeManager *CLifeManager::Create()
{
	// オブジェクトインスタンス
	CLifeManager *pLifeManager = nullptr;

	pLifeManager = new CLifeManager;

	// メモリの確保ができなかった
	assert(pLifeManager != nullptr);

	// 数値の初期化
	pLifeManager->Init();

	// インスタンスを返す
	return pLifeManager;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CLifeManager::CLifeManager(int nPriority /*= CObject::PRIORITY_LEVEL3*/) : CObject(nPriority)
{
	m_pLife = nullptr;									// ライフ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 過去の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 大きさ
	m_nLife = 0;										// ライフ
	m_nAddLife = 0;										// 追加ライフ
	m_nMaxLife = 0;										// 最大ライフ
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CLifeManager::~CLifeManager()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CLifeManager::Init()
{
	// 位置の設定
	m_pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);

	// 向きの設定
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 大きさの設定
	m_size = D3DXVECTOR3(300.0f, 50.0f, 0.0f);

	// ライフの設定
	SetLife();

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CLifeManager::Uninit()
{
	UninitLifeAll();

	// ライフマネージャーの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CLifeManager::Update()
{
	
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CLifeManager::Draw()
{

}

//=============================================================================
// 位置のセッター
// Author : 唐﨑結斗
// 概要 : 位置のメンバ変数に引数を代入
//=============================================================================
void CLifeManager::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;

	// ライフの設定
	SetLife();
}

//=============================================================================
// 向きのセッター
// Author : 唐﨑結斗
// 概要 : 向きのメンバ変数に引数を代入
//=============================================================================
void CLifeManager::SetRot(const D3DXVECTOR3 &rot)
{
	m_rot = rot;

	// ライフの設定
	SetLife();
}

//=============================================================================
// 大きさのセッター
// Author : 唐﨑結斗
// 概要 : 大きさのメンバ変数に引数を代入
//=============================================================================
void CLifeManager::SetSize(const D3DXVECTOR3 & size)
{
	m_size = size;

	// ライフの設定
	SetLife();
}

//=============================================================================
// ライフの終了
// Author : 唐﨑結斗
// 概要 : ライフの終了
//=============================================================================
void CLifeManager::UninitLifeAll()
{
	if (m_pLife != nullptr)
	{// 終了処理
		for (int nCntLife = 0; nCntLife < m_nMaxLife; nCntLife++)
		{
			m_pLife[nCntLife]->Uninit();
		}

		// メモリの解放
		delete[] m_pLife;
		m_pLife = nullptr;
	}
}

//=============================================================================
// ライフのセッター
// Author : 唐﨑結斗
// 概要 : スコアに数値を代入して、ナンバーの設定
//=============================================================================
void CLifeManager::SetLife()
{
	CMotionPlayer3D * pMotionPlayer3D = nullptr;

	switch (CApplication::GetMode())
	{
	case CApplication::MODE_GAME:
		// プレイヤーの取得
		pMotionPlayer3D = CGame::GetMotionPlayer3D();
		break;

	case CApplication::MODE_TUTORIAL:
		// プレイヤーの取得
		pMotionPlayer3D = CTutorial::GetMotionPlayer3D();
		break;

	default:
		break;
	}

	// ライフの終了
	UninitLifeAll();

	// ライフの設定
	m_nLife = pMotionPlayer3D->GetLife();
	m_nAddLife = pMotionPlayer3D->GetInvalidLife();
	m_nMaxLife = CMotionPlayer3D::MAX_LIFE + m_nAddLife;

	// ライフのメモリ確保
	m_pLife = new CLife*[m_nMaxLife];
	assert(m_pLife != nullptr);

	for (int nCntLife = 0; nCntLife < m_nMaxLife; nCntLife++)
	{// ライフの設定
		m_pLife[nCntLife] = CLife::Create();

		// 大きさの設定
		m_pLife[nCntLife]->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));

		// ライフ大きさの取得
		D3DXVECTOR3 lifeSize = m_pLife[nCntLife]->GetSize();

		// 位置の設定
		m_pLife[nCntLife]->SetPos(D3DXVECTOR3(m_pos.x + lifeSize.x / 2.0f + (lifeSize.x * nCntLife), m_pos.y, 0.0f));

		// 向きの設定
		m_pLife[nCntLife]->SetRot(m_rot);

		m_pLife[nCntLife]->SetLifeState(CLife::STATE_INVALID);
	}

	for (int nCntLife = 0; nCntLife < m_nLife; nCntLife++)
	{
		m_pLife[nCntLife]->SetLifeState(CLife::STATE_NORMAL);
	}

	for (int nCntLife = CMotionPlayer3D::MAX_LIFE; nCntLife < m_nMaxLife; nCntLife++)
	{
		m_pLife[nCntLife]->SetLifeState(CLife::STATE_ADDITION);
	}
}
