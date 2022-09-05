//=============================================================================
//
// オブジェクトクラス(object.h)
// Author : 唐﨑結斗
// 概要 : オブジェクト生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "object.h"
#include "renderer.h"
#include "application.h"
#include "camera.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CObject *CObject::m_apObject[MAX_LEVEL][MAX_OBJECT] = {};		// 敵インスタンス

//=============================================================================
// インスタンスの解放
// Author : 唐﨑結斗
// 概要 : すべてのインスタンスを解放する処理
//=============================================================================
void CObject::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < MAX_LEVEL; nCntPriority++)
	{
		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
		{
			if (m_apObject[nCntPriority][nCntObject] != nullptr)
			{// インスタンスが使用されてる
				// オブジェクト終了処理
				m_apObject[nCntPriority][nCntObject]->Uninit();
			}
		}
	}
}

//=============================================================================
// 遷移時のインスタンスの解放
// Author : 唐﨑結斗
// 概要 : 遷移時のインスタンスを解放する処理
//=============================================================================
void CObject::ReleaseScene(void)
{
	for (int nCntPriority = 0; nCntPriority < MAX_LEVEL; nCntPriority++)
	{
		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
		{
			if (m_apObject[nCntPriority][nCntObject] != nullptr
				&& m_apObject[nCntPriority][nCntObject]->GetObjType() != OBJTYPE_FADE)
			{// インスタンスが使用されてる
				// オブジェクト終了処理
				m_apObject[nCntPriority][nCntObject]->Uninit();
			}
		}
	}
}

//=============================================================================
// すべてのインスタンスの更新
// Author : 唐﨑結斗
// 概要 : 使用されてるインスタンスの更新処理を呼び出す
//=============================================================================
void CObject::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < MAX_LEVEL; nCntPriority++)
	{
		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
		{
			if (m_apObject[nCntPriority][nCntObject] != nullptr)
			{// インスタンスが使用されてる
				// オブジェクト更新処理
				m_apObject[nCntPriority][nCntObject]->Update();
			}
		}
	}
}

//=============================================================================
// すべてのインスタンスの描画
// Author : 唐﨑結斗
// 概要 : 使用されてるインスタンスの描画処理を呼び出す
//=============================================================================
void CObject::DrawAll(EObjectDrowType objectDrowType)
{
	if (objectDrowType == DROWTYPE_GAME)
	{
		CApplication::GetCamera()->Set();
	}
	else
	{
		CApplication::GetCameraBG()->Set();
	}

	for (int nCntPriority = 0; nCntPriority < MAX_LEVEL; nCntPriority++)
	{
		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
		{
			if (m_apObject[nCntPriority][nCntObject] != nullptr
				&& m_apObject[nCntPriority][nCntObject]->m_objectDrowType == objectDrowType)
			{// インスタンスが使用されてる
				// オブジェクト描画処理
				m_apObject[nCntPriority][nCntObject]->Draw();
			}
		}
	}
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CObject::CObject(int nPriority /*= PRIORITY_LEVEL0*/) :
m_objectType(OBJTYPE_NONE),
m_objectDrowType(DROWTYPE_GAME),
m_colorType(TYPE_NONE)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (m_apObject[nPriority][nCntObject] == nullptr)
		{// インスタンスが使用されてない
			m_apObject[nPriority][nCntObject] = this;
			m_nNumID = nCntObject;
			m_nLevPriority = nPriority;
			break;
		}
	}
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CObject::~CObject()
{
	
}

//=============================================================================
// インスタンスの解放
// Author : 唐﨑結斗
// 概要 : すべてのインスタンスを解放する処理
//=============================================================================
void CObject::Release(void)
{
	if (m_apObject[m_nLevPriority][m_nNumID] != nullptr)
	{// インスタンスが使用されてる
		// メモリの解放
		const int nID = m_nNumID;
		const int nLevPriority = m_nLevPriority;
		delete m_apObject[nLevPriority][nID];
		m_apObject[nLevPriority][nID] = nullptr;
	}
}

//=============================================================================
// 範囲内の判定
// Author : 唐﨑結斗
// 概要 : ターゲットが範囲内に入ったかの判定
//=============================================================================
bool CObject::ColisonRange2D(CObject * target)
{// 自分の情報を取得する
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 size = GetSize();

	// 目標の情報取得
	D3DXVECTOR3 posTarget = target->GetPos();
	D3DXVECTOR3 sizeTarget = target->GetSize();

	if (pos.x - size.x >= posTarget.x - sizeTarget.x
		&& pos.x + size.x <= posTarget.x + sizeTarget.x
		&& pos.y - size.y >= posTarget.y - sizeTarget.y
		&& pos.y + size.y <= posTarget.y + sizeTarget.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// 矩形の判定
// Author : 唐﨑結斗
// 概要 : ターゲットとの矩形判定
//=============================================================================
bool CObject::ColisonRectangle2D(CObject *target, bool bExtrude)
{// 返り値用の変数
	bool bColison = false;

	// 自分の情報を取得する
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posOld = GetPosOld();
	D3DXVECTOR3 size = GetSize() / 2.0f;

	// 目標の情報取得
	D3DXVECTOR3 posTarget = target->GetPos();
	D3DXVECTOR3 sizeTarget = target->GetSize() / 2.0f;

	if (pos.y - size.y  < posTarget.y + sizeTarget.y 
		&& pos.y + size.y  > posTarget.y - sizeTarget.y )
	{// モデル内にいる(Y軸)
		if (posOld.x + size.y  <= posTarget.x - sizeTarget.x 
			&& pos.x + size.y  > posTarget.x - sizeTarget.x )
		{
			if (bExtrude)
			{// 押し出しを使用する
				pos.x = posTarget.x - sizeTarget.x  - size.y ;
			}

			bColison = true;
		}
		if (posOld.x - size.y  >= posTarget.x + sizeTarget.x 
			&& pos.x - size.y  < posTarget.x + sizeTarget.x )
		{
			if (bExtrude)
			{// 押し出しを使用する
				pos.x = posTarget.x + sizeTarget.x  + size.y ;
			}

			bColison = true;
		}
	}
	if (pos.x - size.x  < posTarget.x + sizeTarget.x 
		&& pos.x + size.x  > posTarget.x - sizeTarget.x )
	{// モデル内にいる(X軸)
		if (posOld.y + size.y  <= posTarget.y - sizeTarget.y 
			&& pos.y + size.y  > posTarget.y - sizeTarget.y )
		{
			if (bExtrude)
			{// 押し出しを使用する
				pos.y = posTarget.y - sizeTarget.y  - size.y ;
			}

			bColison = true;
		}
		if (posOld.y - size.y  >= posTarget.y + sizeTarget.y 
			&& pos.y - size.y  < posTarget.y + sizeTarget.y )
		{
			if (bExtrude)
			{// 押し出しを使用する
				pos.y = posTarget.y + sizeTarget.y  + size.y ;
			}

			bColison = true;
		}
	}

	// 位置の設定
	SetPos(pos);
	return bColison;
}

//=============================================================================
// 円の判定
// Author : 唐﨑結斗
// 概要 : ターゲットとの円判定
//=============================================================================
bool CObject::ColisonCircle2D(CObject * target, bool bExtrude)
{
	// 変数宣言
	bool bCollision = false;

	// 自分の情報を取得する
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 size = GetSize() / 2.0f;

	// 目標の情報取得
	D3DXVECTOR3 posTarget = target->GetPos();
	D3DXVECTOR3 sizeTarget = target->GetSize() / 2.0f;

	// 判定を行う距離を算出
	float fJudgeDistance = sqrtf((size.x * size.x) + (size.y * size.y));
	fJudgeDistance += sqrtf((sizeTarget.x * sizeTarget.x) + (sizeTarget.y * sizeTarget.y));

	// お互いの位置の差を算出
	D3DXVECTOR3 distance = posTarget - pos;
	float fDistance = sqrtf((distance.x * distance.x) + (distance.y * distance.y));

	if (fDistance <= fJudgeDistance)
	{// 位置の差が判定を行う距離より短い場合
		bCollision = true;

		if (bExtrude)
		{
			float fRotDif = atan2f(distance.x, distance.y);
			pos.x = posTarget.x - (sinf(fRotDif) * fJudgeDistance);
			pos.y = posTarget.y - (cosf(fRotDif) * fJudgeDistance);
		}
	}

	// 位置の設定
	SetPos(pos);
	return bCollision;
}

//=============================================================================
// 球の判定
// Author : 唐﨑結斗
// 概要 : ターゲットとの球判定
//=============================================================================
bool CObject::ColisonSphere3D(CObject *target, D3DXVECTOR3 size, D3DXVECTOR3 targetSize, bool bExtrude)
{
	// 変数宣言
	bool bCollision = false;

	// 自分の情報を取得する
	D3DXVECTOR3 pos = GetPos();
	size /= 2.0f;

	// 目標の情報取得
	D3DXVECTOR3 posTarget = target->GetPos();
	targetSize /= 2.0f;

	// 判定を行う距離を算出
	float fJudgeDistance = sqrtf((size.x * size.x) + (size.y * size.y) + (size.z * size.z));
	fJudgeDistance += sqrtf((targetSize.x * targetSize.x) + (targetSize.y * targetSize.y) + (targetSize.z * targetSize.z));

	// お互いの位置の差を算出
	D3DXVECTOR3 distance = posTarget - pos;
	float fDistance = sqrtf((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z));

	if (fDistance <= fJudgeDistance)
	{// 位置の差が判定を行う距離より短い場合
		bCollision = true;

		if (bExtrude)
		{
			// 角度の算出
			D3DXVECTOR3 rotDiff;
			rotDiff.y = atan2f(distance.x, distance.z);
			rotDiff.x = atan2f(sqrtf((distance.x * distance.x) + (distance.z * distance.z)), distance.y);
			rotDiff.z = 0.0f;

			// 位置の算出
			pos.z = posTarget.z - sinf(rotDiff.x) * cosf(rotDiff.y) * fJudgeDistance;
			pos.x = posTarget.x - sinf(rotDiff.x) * sinf(rotDiff.y) * fJudgeDistance;
			pos.y = posTarget.y - cosf(rotDiff.x) * fJudgeDistance;

			// 位置の設定
			SetPos(pos);
		}
	}

	return bCollision;
}
