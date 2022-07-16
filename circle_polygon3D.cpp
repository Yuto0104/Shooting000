////=============================================================================
////
//// 3D円ポリゴンクラス(object.h)
//// Author : 唐﨑結斗
//// 概要 : 3D円ポリゴン生成を行う
////
////=============================================================================
//
////*****************************************************************************
//// インクルード
////*****************************************************************************
//#include <assert.h>
//
//#include "circle_polygon3D.h"
//#include "renderer.h"
//#include "application.h"
//
////=============================================================================
//// インスタンス生成
//// Author : 唐﨑結斗
//// 概要 : 2D3D円ポリゴンを生成する
////=============================================================================
//CCirclePolygon3D * CCirclePolygon3D::Create(void)
//{
//	// 3D円ポリゴンインスタンス
//	CCirclePolygon3D *pCirclePolygon3D = nullptr;
//
//	// メモリの解放
//	pCirclePolygon3D = new CCirclePolygon3D;
//
//	// メモリの確保ができなかった
//	assert(pCirclePolygon3D != nullptr);
//
//	// 数値の初期化
//	pCirclePolygon3D->Init();
//
//	// インスタンスを返す
//	return pCirclePolygon3D;
//}
//
////=============================================================================
//// コンストラクタ
//// Author : 唐﨑結斗
//// 概要 : インスタンス生成時に行う処理
////=============================================================================
//CCirclePolygon3D::CCirclePolygon3D()
//{
//	m_pVtxBuff = nullptr;								// 頂点バッファ
//	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
//	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
//	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 大きさ
//	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);			// カラー
//	m_fRot = 0.0f;										// 角度
//	m_fRadius = 0.0f;									// 半径
//	m_nVtx = 0;											// 頂点数
//}
//
////=============================================================================
//// デストラクタ
//// Author : 唐﨑結斗
//// 概要 : インスタンス終了時に行う処理
////=============================================================================
//CCirclePolygon3D::~CCirclePolygon3D()
//{
//
//}
//
////=============================================================================
//// 初期化
//// Author : 唐﨑結斗
//// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
////=============================================================================
//HRESULT CCirclePolygon3D::Init()
//{// レンダラーのゲット
//	CRenderer *pRenderer = CApplication::GetRenderer();
//
//	//頂点バッファの生成
//	pRenderer->GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,		// 確保するバッファサイズ
//		D3DUSAGE_WRITEONLY,
//		FVF_VERTEX_3D,														// 頂点ファーマット
//		D3DPOOL_MANAGED,
//		&m_pVtxBuff,
//		NULL);
//
//	// ポリゴン情報の設定
//	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
//	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
//	m_size = D3DXVECTOR3(100.0f, 0.0f, 100.0f);		// 大きさ
//	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// カラー
//	m_fRot = D3DX_PI * 2.0f;						// 角度
//	m_fRadius = 0.0f;								// 半径
//	m_nVtx = 10;									// 頂点数
//
//	// 頂点座標などの設定
//	SetVtx();
//
//	return S_OK;
//}
//
////=============================================================================
//// 終了
//// Author : 唐﨑結斗
//// 概要 : テクスチャのポインタと頂点バッファの解放
////=============================================================================
//void CCirclePolygon3D::Uninit()
//{//頂点バッファを破棄
//	if (m_pVtxBuff != nullptr)
//	{
//		m_pVtxBuff->Release();
//
//		m_pVtxBuff = nullptr;
//	}
//
//	// オブジェクト3Dの解放
//	Release();
//}
//
////=============================================================================
//// 更新
//// Author : 唐﨑結斗
//// 概要 : 2D更新を行う
////=============================================================================
//void CCirclePolygon3D::Update()
//{
//
//}
//
////=============================================================================
//// 描画
//// Author : 唐﨑結斗
//// 概要 : 2D描画を行う
////=============================================================================
//void CCirclePolygon3D::Draw()
//{// デバイスの取得
//	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
//
//	// 情報の取得
//	D3DXMATRIX mtxWorld = GetMtxWorld();
//	D3DXVECTOR3 pos = GetPos();
//	D3DXVECTOR3 rot = GetRot();
//
//	// 計算用マトリックス
//	D3DXMATRIX mtxRot, mtxTrans, mtxView;
//
//	// ワールドマトリックスの初期化
//	// 行列初期化関数(第一引数の[行列]を[単位行列]に初期化)
//	D3DXMatrixIdentity(&mtxWorld);
//
//	// 向きの反映
//	// 行列回転関数 (第一引数に[ヨー(y)ピッチ(x)ロール(z)]方向の回転行列を作成)
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
//
//	// 行列掛け算関数 (第二引数 * 第三引数を第一引数に格納)
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
//
//	// 位置を反映
//	// 行列移動関数 (第一引数にX,Y,Z方向の移動行列を作成)
//	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);						// 行列掛け算関数
//
//	// ワールドマトリックスの設定
//	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
//
//	// 頂点バッファをデバイスのデータストリームに設定
//	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
//
//	// 頂点フォーマット
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	// Zテストを使用する
//	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
//	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
//
//	// Zテストの設定
//	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
//
//	// αテストを使用する
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
//
//	// αテストの設定
//	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);
//	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
//
//	// ポリゴン描画
//	pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, m_nVtx - 2);
//
//	// ライトを有効	
//	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//
//	// Zテストの終了
//	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
//
//	// αテストの終了
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
//
//	// テクスチャの解除
//	pDevice->SetTexture(0, nullptr);
//}
//
////=============================================================================
//// 位置のセッター
//// Author : 唐﨑結斗
//// 概要 : 位置のメンバ変数に引数を代入
////=============================================================================
//void CCirclePolygon3D::SetPos(const D3DXVECTOR3 &pos)
//{
//	// 位置の設定
//	m_pos = pos;
//
//	// 頂点座標などの設定
//	SetVtx();
//}
//
////=============================================================================
//// 向きのセッター
//// Author : 唐﨑結斗
//// 概要 : 向きのメンバ変数に引数を代入
////=============================================================================
//void CCirclePolygon3D::SetRot(const D3DXVECTOR3 &rot)
//{
//	// 位置の設定
//	m_rot = rot;
//
//	// 頂点座標などの設定
//	SetVtx();
//}
//
////=============================================================================
//// 大きさのセッター
//// Author : 唐﨑結斗
//// 概要 : 大きさのメンバ変数に引数を代入
////=============================================================================
//void CCirclePolygon3D::SetSize(const D3DXVECTOR3 & size)
//{
//	// 大きさの設定
//	m_size = size;
//
//	m_fRadius = sqrtf((size.x * size.x) + (size.z * size.z));
//
//	// 頂点座標などの設定
//	SetVtx();
//}
//
////=============================================================================
//// 頂点座標などの設定
//// Author : 唐﨑結斗
//// 概要 : 3D頂点座標、rhw、頂点カラーを設定する
////=============================================================================
//void CCirclePolygon3D::SetVtx()
//{// 頂点情報の取得
//	VERTEX_3D *pVtx = NULL;
//
//	// 頂点バッファをロック
//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	float addRot = m_fRot / (m_nVtx - 1);
//
//	pVtx[0].pos = m_pos;
//
//	for (int nCntVtx = 1; nCntVtx < m_nVtx; nCntVtx++)
//	{// 頂点座標の設定
//		pVtx[nCntVtx].pos.x = pVtx[0].pos.x + sinf(addRot * nCntVtx) * m_fRadius;
//		pVtx[nCntVtx].pos.y = 0.0f;
//		pVtx[nCntVtx].pos.z = pVtx[0].pos.z + cosf(addRot * nCntVtx) * m_fRadius;
//
//		// 法線の設定
//		pVtx[nCntVtx].nor = D3DXVECTOR3(pVtx[nCntVtx].pos.x, 0.0f, pVtx[nCntVtx].pos.z);
//
//		// 頂点カラーの設定
//		pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//	}
//
//	// 頂点バッファのアンロック
//	m_pVtxBuff->Unlock();
//}
