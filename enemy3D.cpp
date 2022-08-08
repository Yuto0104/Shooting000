//=============================================================================
//
// 3Dエネミークラス(enemy3D.cpp)
// Author : 唐�ｱ結斗
// 概要 : 3Dエネミー生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "enemy3D.h"
#include "renderer.h"
#include "application.h"
#include "score.h"
#include "bullet3D.h"

//=============================================================================
// インスタンス生成
// Author : 唐�ｱ結斗
// 概要 : 3Dエネミーを生成する
//=============================================================================
CEnemy3D * CEnemy3D::Create(const char *pName)
{
	// オブジェクトインスタンス
	CEnemy3D *pEnemy3D = nullptr;

	// メモリの解放
	pEnemy3D = new CEnemy3D;

	// メモリの確保ができなかった
	assert(pEnemy3D != nullptr);

	// 数値の初期化
	pEnemy3D->Init(pName);

	// インスタンスを返す
	return pEnemy3D;
}

//=============================================================================
// コンストラクタ
// Author : 唐�ｱ結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CEnemy3D::CEnemy3D()
{// オブジェクトの種別設定
	SetObjType(CObject::OBJTYPE_3DENEMY);

	m_nLife = 0;		// 体力
	m_nScore = 0;		// スコア
	m_nCntShot = 0;		// 弾発射までのカウント
}

//=============================================================================
// デストラクタ
// Author : 唐�ｱ結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CEnemy3D::~CEnemy3D()
{

}

//=============================================================================
// 初期化
// Author : 唐�ｱ結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CEnemy3D::Init(const char * pName)
{
	// モデルの初期化
	CModel3D::Init(pName);

	// あたり判定の設定
	SetColisonSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));

	// 体力
	m_nLife = 100;

	// スコア
	m_nScore = 100;

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐�ｱ結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CEnemy3D::Uninit()
{
	// モデルの終了
	CModel3D::Uninit();
}

//=============================================================================
// 更新
// Author : 唐�ｱ結斗
// 概要 : 2D更新を行う
//=============================================================================
void CEnemy3D::Update()
{
	D3DXVECTOR3 rot = GetRot();
	D3DXCOLOR bulletColor;					// 弾の色

	if (GetColorType() == CObject::TYPE_WHITE)
	{// 弾の色の設定
		bulletColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (GetColorType() == CObject::TYPE_BLACK)
	{// 弾の色の設定
		bulletColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}

	m_nCntShot++;

	if (m_nCntShot % 180 == 0)
	{
		// 弾の生成
		CBullet3D * pBullet3D = CBullet3D::Create();
		pBullet3D->SetPos(GetPos());
		pBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
		pBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, rot.y, 0.0f));
		pBullet3D->SetSpeed(10.0f);
		pBullet3D->SetColor(bulletColor);
		pBullet3D->SetColorType(GetColorType());
		pBullet3D->SetParent(CObject::OBJTYPE_3DENEMY);
	}

	// モデルの更新
	CModel3D::Update();
}

//=============================================================================
// 描画
// Author : 唐�ｱ結斗
// 概要 : 2D描画を行う
//=============================================================================
void CEnemy3D::Draw()
{
	// モデルの描画
	CModel3D::Draw();
}

//=============================================================================
// ヒット
// Author : 唐�ｱ結斗
// 概要 : 引数分のダメージを与える
//=============================================================================
void CEnemy3D::Hit(int nAttack)
{
	m_nLife -= nAttack;

	if (m_nLife <= 0)
	{
		m_nLife = 0;
		CScore *pScore = CApplication::GetScore();
		pScore->AddScore(m_nScore);
		Uninit();
	}
}
