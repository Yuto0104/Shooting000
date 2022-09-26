//=============================================================================
//
// 3D追従バレットクラス(follow_bullet3D.cpp)
// Author : 唐﨑結斗
// 概要 : オブジェクト生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "follow_bullet3D.h"
#include "renderer.h"
#include "application.h"

#include "motion_player3D.h"
#include "enemy3D.h"
#include "score.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : 3Dバレットを生成する
//=============================================================================
CFollowBullet3D * CFollowBullet3D::Create(void)
{
	// オブジェクトインスタンス
	CFollowBullet3D *pFollowBullet3D = nullptr;

	// メモリの解放
	pFollowBullet3D = new CFollowBullet3D;

	// メモリの確保ができなかった
	assert(pFollowBullet3D != nullptr);

	// 数値の初期化
	pFollowBullet3D->Init();

	// インスタンスを返す
	return pFollowBullet3D;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CFollowBullet3D::CFollowBullet3D()
{
	m_pTarget = nullptr;				// 追従のターゲット
	m_fCoefficient = 0.0f;				// 減衰係数
	m_fAddCoefficient = 0.0f;			// 減衰係数の加算値

	// オブジェクトの種別設定
	SetObjType(CObject::OBJTYPE_3DBULLET);
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CFollowBullet3D::~CFollowBullet3D()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CFollowBullet3D::Init()
{
	// 初期化
	CBullet3D::Init();

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CFollowBullet3D::Uninit()
{
	// 終了
	CBullet3D::Uninit();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CFollowBullet3D::Update()
{
	// ターゲットが設定されていない場合
	SetTarget();

	// 減衰係数の加算
	AddCoefficient();

	// 追従
	Follow();

	// 更新
	CBullet3D::Update();
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CFollowBullet3D::Draw()
{
	// 描画
	CBullet3D::Draw();
}

//=============================================================================
// ターゲットの設定
// Author : 唐﨑結斗
// 概要 : 一番近いオブジェクトをターゲットに設定
//=============================================================================
void CFollowBullet3D::SetTarget()
{
	// 距離を格納
	m_pTarget = nullptr;
	float fDistance = FLT_MAX;
	float fDistanceCopy = 0;

	for (int nCntPriority = 0; nCntPriority < CObject::MAX_LEVEL; nCntPriority++)
	{
		if (m_pTarget == nullptr)
		{
			for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
			{
				// オブジェクトインスタンスの取得
				CObject *pObject = CObject::MyGetObject(nCntPriority, nCntObj);

				if (pObject != nullptr
					&& (pObject->GetObjType() == CObject::OBJTYPE_3DENEMY
						|| pObject->GetObjType() == CObject::OBJTYPE_3DBOSS
						|| pObject->GetObjType() == CObject::OBJTYPE_3DPLAYER)
					&& pObject->GetObjType() != CObject::OBJTYPE_3DBULLET
					&& pObject->GetObjType() != GetParent())
				{// タイプが一致した場合
					// オブジェクトの距離を算出
					fDistanceCopy = D3DXVec3Length(&(pObject->GetPos() - GetPos()));

					if (fDistanceCopy < fDistance)
					{// 距離が現在のものより短い場合
						// 距離の保存
						fDistance = fDistanceCopy;

						// オブジェクトをターゲットに設定
						m_pTarget = pObject;
					}
				}
			}
		}
	}
}

//=============================================================================
// 追従
// Author : 唐﨑結斗
// 概要 : 目的の方向に徐々に移動する
//=============================================================================
void CFollowBullet3D::Follow()
{
	D3DXVECTOR3 moveVec = GetMoveVec();
	D3DXVECTOR3 moveVecDest = moveVec;

	if (m_pTarget != nullptr)
	{
		D3DXVECTOR3 distance = m_pTarget->GetPos() - GetPos();

		moveVecDest.y = atan2f(distance.x, distance.z);
		moveVecDest.x = atan2f(sqrtf((distance.x * distance.x) + (distance.z * distance.z)), distance.y);
		moveVecDest.z = moveVec.z;

		// 移動方向の設定
		D3DXVECTOR3 add = (moveVecDest - moveVec);

		// 正規化
		add.x = CApplication::RotNormalization(add.x);
		add.y = CApplication::RotNormalization(add.y);
		add.z = CApplication::RotNormalization(add.z);

		// 移動方向の算出
		moveVec += add * m_fCoefficient;

		// 正規化
		moveVec.x = CApplication::RotNormalization(moveVec.x);
		moveVec.y = CApplication::RotNormalization(moveVec.y);
		moveVec.z = CApplication::RotNormalization(moveVec.z);
	}

	// 移動方向の設定
	SetMoveVec(moveVec);
}

//=============================================================================
// 減衰係数の加算
// Author : 唐﨑結斗
// 概要 : 減衰係数の加算値が0以上の場合加算する
//=============================================================================
void CFollowBullet3D::AddCoefficient()
{
	if (m_fAddCoefficient > 0.0f
		&& m_fCoefficient < 1.0f)
	{
		m_fCoefficient += m_fAddCoefficient;

		if (m_fCoefficient > 1.0f)
		{
			m_fCoefficient = 1.0f;
		}
	}
}

