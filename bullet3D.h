//=============================================================================
//
// 3Dバレットクラス(bullet3D.h)
// Author : 唐�ｱ結斗
// 概要 : オブジェクト生成を行う
//
//=============================================================================
#ifndef _BULLET3D_H_		// このマクロ定義がされてなかったら
#define _BULLET3D_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"
#include "object3D.h"
#include "texture.h"

//=============================================================================
// 3Dバレットクラス
// Author : 唐�ｱ結斗
// 概要 : 3Dバレット生成を行うクラス
//=============================================================================
class CBullet3D : public CObject3D
{
public:
	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CBullet3D *Create(void);				// 2Dオブジェクトの生成

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CBullet3D();
	~CBullet3D();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init() override;														// 初期化
	void Uninit() override;															// 終了
	void Update() override;															// 更新
	void Draw() override;															// 描画
	void SetMoveVec(const D3DXVECTOR3 moveVec) { m_moveVec = moveVec; }				// 移動方向の設定
	D3DXVECTOR3 GetMoveVec() { return m_moveVec; }									// 移動方向の取得
	void SetSpeed(const float fSpeed) { m_fSpeed = fSpeed; }						// 速度の設定
	void SetParent(const CObject::EObjectType parent) { m_parent = parent; }		// 親タイプの設定
	CObject::EObjectType GetParent() { return m_parent; }							// 親タイプの取得

private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void Collision();		// 衝突

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	D3DXVECTOR3					m_move;					// 移動量
	D3DXVECTOR3					m_moveVec;				// 移動方向
	CObject::EObjectType		m_parent;				// 親タイプ
	float						m_fSpeed;				// 速度
	int							m_nAttack;				// 攻撃力
};

#endif




