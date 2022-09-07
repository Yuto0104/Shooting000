//=============================================================================
//
// 2Dバレット(bullet2D.h)
// Author : 唐﨑結斗
// 概要 : 2Dバレット生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "bullet2D.h"
#include "renderer.h"
#include "application.h"
#include "explosion2D.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : 2Dバレットを生成する
//=============================================================================
CBullet2D * CBullet2D::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const float fSpeed)
{
	// オブジェクトインスタンス
	CBullet2D *pBullet2D = nullptr;

	pBullet2D = new CBullet2D;

	if (pBullet2D != nullptr)
	{// 数値の初期化
		pBullet2D->Init();
		pBullet2D->SetPos(pos);
		pBullet2D->SetPosOld(pos);
		pBullet2D->SetRot(rot);
		pBullet2D->SetSpeed(fSpeed);
		pBullet2D->Move();
	}
	else
	{// メモリの確保ができなかった
		assert(false);
	}

	// インスタンスを返す
	return pBullet2D;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CBullet2D::CBullet2D()
{
	m_move = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
	m_fSpeed = 0.0f;
	SetObjType(CObject::OBJTYPE_2DBULLET);				// オブジェクトの種別設定
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CBullet2D::~CBullet2D()
{

}

//=============================================================================
// 2Dバレット初期化
// Author : 唐﨑結斗
// 概要 : テクスチャの設定し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CBullet2D::Init()
{
	// オブジェクト2Dの初期化
	CObject2D::Init();

	// テクスチャの設定
	LoadTex(0);

	// サイズ
	D3DXVECTOR3 size = GetSize();
	size = D3DXVECTOR3(10.0f, 10.0f, 0.0f);
	SetSize(size);

	// 寿命
	m_nLife = 60;

	// テクスチャ座標の設定
	SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	return S_OK;
}

//=============================================================================
// 2Dバレット終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CBullet2D::Uninit()
{
	// オブジェクト2Dの終了
	CObject2D::Uninit();
}

//=============================================================================
// 2Dバレット更新
// Author : 唐﨑結斗
// 概要 : 2Dバレットの更新を行う
//=============================================================================
void CBullet2D::Update()
{
	// Object2Dのメンバ変数の取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 size = GetSize();
	SetPosOld(pos);

	// オブジェクト2Dの更新
	CObject2D::Update();

	// 移動
	pos -= m_move;
	SetPos(pos);

	for (int nCntPriority = 0; nCntPriority < CObject::MAX_LEVEL; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			// オブジェクトインスタンスの取得
			CObject *pObject = CObject::MyGetObject(nCntPriority, nCntObj);

			if (pObject != nullptr)
			{
				if ((pObject->GetObjType() == CObject::OBJTYPE_2DENEMY
					|| pObject->GetObjType() == CObject::OBJTYPE_2DPLAYER)
					&& ColisonCircle2D(pObject, false))
				{// タイプが一致した場合
					//m_nLife = 1;

					// 対象との距離の算出
					D3DXVECTOR3 posDiff = pObject->GetPos() - GetPos();

					// 対象までの角度の算出
					float fRotDest = atan2f(posDiff.x, posDiff.y);

					// 角度の反転
					fRotDest *= 1.0f;

					if (fRotDest >= D3DX_PI)
					{// 移動方向の正規化
						fRotDest -= D3DX_PI * 2;
					}
					else if (fRotDest <= -D3DX_PI)
					{// 移動方向の正規化
						fRotDest += D3DX_PI * 2;
					}

					// 移動量(方向と速力)の算出
					m_move.x = sinf(fRotDest) * 5.0f;
					m_move.y = cosf(fRotDest) * 5.0f;

					break;
				}
			}
		}
	}

	if (m_nLife > 0)
	{// ライフがある場合
		m_nLife--;

		if (m_nLife <= 0)
		{// 0以下の場合

			m_nLife = 0;
			CExplosion2D::Create(pos, rot);
			Uninit();
		}
	}
}

//=============================================================================
// 2Dバレット描画
// Author : 唐﨑結斗
// 概要 : 2Dバレットの描画を行う
//=============================================================================
void CBullet2D::Draw()
{// プレイヤー2Dの描画
	CObject2D::Draw();
}

//=============================================================================
// 移動
// Author : 唐﨑結斗
// 概要 : 速度と方向をもった移動量を算出し返す
//=============================================================================
D3DXVECTOR3 CBullet2D::Move(void)
{
	// 移動量の算出
	D3DXVECTOR3 rot = GetRot();
	m_move.x = sinf(rot.z) * m_fSpeed;
	m_move.y = cosf(rot.z) * m_fSpeed;

	return m_move;
}
