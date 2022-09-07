//=============================================================================
//
// 2D爆発(explosion2D.cpp)
// Author : 唐﨑結斗
// 概要 : 2D爆発生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "explosion2D.h"
#include "renderer.h"
#include "application.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : 2Dバレットを生成する
//=============================================================================
CExplosion2D * CExplosion2D::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot)
{
	// オブジェクトインスタンス
	CExplosion2D *pExplosion2D = nullptr;

	pExplosion2D = new CExplosion2D;

	if (pExplosion2D != nullptr)
	{// 数値の初期化
		pExplosion2D->Init();
		pExplosion2D->SetPos(pos);
		pExplosion2D->SetRot(rot);
	}
	else
	{// メモリの確保ができなかった
		assert(false);
	}

	// インスタンスを返す
	return pExplosion2D;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CExplosion2D::CExplosion2D()
{
	// メモリの確保
	m_pTexAnim = new CTexAnim;
	assert(m_pTexAnim != nullptr);
	m_nLife = 0;										// 寿命
	SetObjType(CObject::OBJTYPE_2DEXPLOSION);			// オブジェクトの種別設定
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CExplosion2D::~CExplosion2D()
{

}

//=============================================================================
// ポリゴンの初期化
// Author : 唐﨑結斗
// 概要 : テクスチャの設定し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CExplosion2D::Init()
{
	// オブジェクト2Dの初期化
	CObject2D::Init();

	// テクスチャの設定
	LoadTex(3);

	// サイズ
	D3DXVECTOR3 size = GetSize();
	size = D3DXVECTOR3(50.0f, 50.0f, 0.0f);
	SetSize(size);

	// 寿命
	m_nLife = 50;

	// アニメーションの初期化
	m_pTexAnim->SetAnim(8, 1, m_nLife, false);

	// テクスチャ座標の設定
	SetTex(D3DXVECTOR2(0.0f, 0.0f), m_pTexAnim->GetAddTex());

	return S_OK;
}

//=============================================================================
// ポリゴンの終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CExplosion2D::Uninit()
{
	if (m_pTexAnim != nullptr)
	{// 終了処理
	 // メモリの解放
		delete m_pTexAnim;
		m_pTexAnim = nullptr;
	}

	// オブジェクト2Dの終了
	CObject2D::Uninit();
}

//=============================================================================
// ポリゴンの更新
// Author : 唐﨑結斗
// 概要 : 2Dポリゴンの更新を行う
//=============================================================================
void CExplosion2D::Update()
{
	// オブジェクト2Dの更新
	CObject2D::Update();

	// アニメーションの再生
	m_pTexAnim->PlayAnim();
	D3DXVECTOR2 texAddAnim = m_pTexAnim->GetAddTex();
	CTexAnim::TEX_PATTERN texNumAnim = m_pTexAnim->GetNumAnim();

	// テクスチャ座標の設定
	SetTex(D3DXVECTOR2(0.0f + (texAddAnim.x * texNumAnim.x), 0.0f + (texAddAnim.y * texNumAnim.y)),
		D3DXVECTOR2(texAddAnim.x + (texAddAnim.x * texNumAnim.x), texAddAnim.y + (texAddAnim.y * texNumAnim.y)));

	if (m_nLife > 0)
	{// ライフがある場合
		m_nLife--;

		if (m_nLife <= 0)
		{// 0以下の場合
			m_nLife = 0;
			Uninit();
		}
	}
}

//=============================================================================
// ポリゴンの描画
// Author : 唐﨑結斗
// 概要 : 2Dポリゴンの描画を行う
//=============================================================================
void CExplosion2D::Draw()
{// プレイヤー2Dの描画
	CObject2D::Draw();
}

