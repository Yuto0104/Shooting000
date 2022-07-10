//=============================================================================
//
// 3Dモデルクラス(model3D.h)
// Author : 唐﨑結斗
// 概要 : 3Dモデル生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "model3D.h"
#include "renderer.h"
#include "application.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : 3Dモデルを生成する
//=============================================================================
CModel3D * CModel3D::Create(char * pName)
{
	// オブジェクトインスタンス
	CModel3D *pModel3D = nullptr;

	// メモリの解放
	pModel3D = new CModel3D;

	// メモリの確保ができなかった
	assert(pModel3D != nullptr);

	// 数値の初期化
	pModel3D->Init(pName);

	// インスタンスを返す
	return pModel3D;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CModel3D::CModel3D()
{
	m_pMesh = nullptr;									// メッシュ情報へのポインタ
	m_pBuffer = nullptr;								// マテリアル情報へのポインタ
	m_nNumMat = NULL;									// マテリアル情報の数
	m_mtxWorld = {};									// ワールドマトリックス
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 過去位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 大きさ
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CModel3D::~CModel3D()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CModel3D::Init()
{
	// メンバ変数の初期化
	m_pMesh = nullptr;									// メッシュ情報へのポインタ
	m_pBuffer = nullptr;								// マテリアル情報へのポインタ
	m_nNumMat = NULL;									// マテリアル情報の数
	m_mtxWorld = {};									// ワールドマトリックス
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 過去位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);				// 大きさ

	return S_OK;
}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CModel3D::Init(char * pName)
{
	// メンバ変数の初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 過去位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);				// 大きさ

	// レンダラーのゲット
	CRenderer *pRenderer = CApplication::GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(pName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffer,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CModel3D::Uninit()
{
	// メッシュの破棄
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// マテリアルの破棄
	if (m_pBuffer != NULL)
	{
		m_pBuffer->Release();
		m_pBuffer = NULL;
	}

	// オブジェクト3Dの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 2D更新を行う
//=============================================================================
void CModel3D::Update()
{

}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 2D描画を行う
//=============================================================================
void CModel3D::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScaling;

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	// マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);											// 行列初期化関数

	// サイズの反映
	D3DXMatrixScaling(&mtxScaling, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScaling);					// 行列掛け算関数

	// 向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);			// 行列回転関数
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);						// 行列掛け算関数 

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);				// 行列移動関数
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);					// 行列掛け算関数

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	if (m_pBuffer != nullptr)
	{// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_pBuffer->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
		{// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// モデルパーツの描画
			m_pMesh->DrawSubset(nCntMat);
		}
	}

	// 保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// 位置のセッター
// Author : 唐﨑結斗
// 概要 : 位置のメンバ変数に引数を代入
//=============================================================================
void CModel3D::SetPos(const D3DXVECTOR3 &pos)
{
	// 位置の設定
	m_pos = pos;
}

//=============================================================================
// 向きのセッター
// Author : 唐﨑結斗
// 概要 : 向きのメンバ変数に引数を代入
//=============================================================================
void CModel3D::SetRot(const D3DXVECTOR3 &rot)
{
	// 位置の設定
	m_rot = rot;
}

//=============================================================================
// 大きさのセッター
// Author : 唐﨑結斗
// 概要 : 大きさのメンバ変数に引数を代入
//=============================================================================
void CModel3D::SetSize(const D3DXVECTOR3 & size)
{
	// 大きさの設定
	m_size = size;
}

//=============================================================================
// ワールド座標へのキャスト処理
// Author : 唐﨑結斗
// 概要 : pos,rotの行列計算を行い、ローカル座標をワールド座標に変換する
//=============================================================================
D3DXVECTOR3 CModel3D::WorldCastVtx(D3DXVECTOR3 vtx, D3DXVECTOR3 FormerPos, D3DXVECTOR3 FormerRot)
{
	// 変数宣言
	D3DXMATRIX		mtxWorldVtx;				// ワールドマトリックス
	D3DXMATRIX		mtxRot, mtxTrans;			// 計算用マトリックス

	// ワールドマトリックスの初期化
	// 行列初期化関数(第一引数の[行列]を[単位行列]に初期化)
	D3DXMatrixIdentity(&mtxWorldVtx);

	// 位置を反映
	// 行列移動関数 (第一引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixTranslation(&mtxTrans, vtx.x, vtx.y, vtx.z);
	D3DXMatrixMultiply(&mtxWorldVtx, &mtxWorldVtx, &mtxTrans);		// 行列掛け算関数

	// 向きの反映
	// 行列回転関数 (第一引数に[ヨー(y)ピッチ(x)ロール(z)]方向の回転行列を作成)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, FormerRot.y, FormerRot.x, FormerRot.z);

	// 行列掛け算関数 (第二引数 * 第三引数を第一引数に格納)
	D3DXMatrixMultiply(&mtxWorldVtx, &mtxWorldVtx, &mtxRot);

	// 位置を反映
	// 行列移動関数 (第一引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixTranslation(&mtxTrans, FormerPos.x, FormerPos.y, FormerPos.z);
	D3DXMatrixMultiply(&mtxWorldVtx, &mtxWorldVtx, &mtxTrans);		// 行列掛け算関数

	return D3DXVECTOR3(mtxWorldVtx._41, mtxWorldVtx._42, mtxWorldVtx._43);
}
