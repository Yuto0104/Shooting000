//=============================================================================
//
// モーションキャラクター3Dクラス(model3D.h)
// Author : 唐﨑結斗
// 概要 : モーションキャラクター3D生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "motion_char3D.h"
#include "motion.h"
#include "renderer.h"
#include "application.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : モーションキャラクター3Dを生成する
//=============================================================================
CMotionChar3D * CMotionChar3D::Create(const char * pName)
{
	// オブジェクトインスタンス
	CMotionChar3D *pMotionChar3D = nullptr;

	// メモリの解放
	pMotionChar3D = new CMotionChar3D;

	// メモリの確保ができなかった
	assert(pMotionChar3D != nullptr);

	// 数値の初期化
	pMotionChar3D->Init(pName);

	// インスタンスを返す
	return pMotionChar3D;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CMotionChar3D::CMotionChar3D()
{
	
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CMotionChar3D::~CMotionChar3D()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CMotionChar3D::Init(const char *pName)
{
	// 初期化
	CModel3D::Init();

	// モーション情報
	m_pMotion = new CMotion(pName);
	assert(m_pMotion != nullptr);

	// メンバ変数の初期化
	m_nNumMotion = 0;
	m_nNumMotionOld = m_nNumMotion;
	m_bMotion = false;
	m_bMotionBlend = false;

	return E_NOTIMPL;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CMotionChar3D::Uninit()
{
	if (m_pMotion != nullptr)
	{// 終了処理
		m_pMotion->Uninit();

		// メモリの解放
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	// 終了
	CModel3D::Uninit();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 2D更新を行う
//=============================================================================
void CMotionChar3D::Update()
{
	// 更新
	CModel3D::Update();

	// 現在のモーション番号の保管
	m_nNumMotionOld = m_nNumMotion;

	if (!m_bMotion)
	{// 使用してるモーションがない場合
		m_nNumMotion = 0;
	}

	if (m_nNumMotionOld != m_nNumMotion)
	{// モーションタイプが変更された時
		m_pMotion->CntReset((int)(m_nNumMotionOld));
		m_bMotionBlend = true;
	}

	if (m_bMotionBlend)
	{// モーションブレンドを使用してる
		m_bMotionBlend = m_pMotion->MotionBlend((int)(m_nNumMotion));
	}
	else if (!m_bMotionBlend)
	{// モーションブレンドを使用してない場合
		m_bMotion = m_pMotion->PlayMotion((int)(m_nNumMotion));
	}
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 2D描画を行う
//=============================================================================
void CMotionChar3D::Draw()
{
	// 描画
	CModel3D::Draw();

	// ワールドマトリックスの取得
	D3DXMATRIX mtxWorld = GetMtxWorld();

	// パーツの描画設定
	m_pMotion->SetParts(mtxWorld);
}

//=============================================================================
// モーションの再読み込み
// Author : 唐﨑結斗
// 概要 : モーションの再読み込みを行う
//=============================================================================
void CMotionChar3D::ReloadMotion(const char * pName)
{// モーションの再読み込み
	m_pMotion->ReloadMotion(pName);
}
