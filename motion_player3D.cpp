//=============================================================================
//
// 3Dモーションプレイヤークラス(model3D.h)
// Author : 唐﨑結斗
// 概要 : 3Dモーションプレイヤー生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "motion_player3D.h"
#include "renderer.h"
#include "application.h"
#include "keyboard.h"

#include "bullet3D.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************
const float CMotionPlayer3D::ROTATE_SPEED = (0.1f);

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : 3Dモーションプレイヤーを生成する
//=============================================================================
CMotionPlayer3D * CMotionPlayer3D::Create(char * pName)
{
	// オブジェクトインスタンス
	CMotionPlayer3D *pMotionPlayer3D = nullptr;

	// メモリの解放
	pMotionPlayer3D = new CMotionPlayer3D;

	// メモリの確保ができなかった
	assert(pMotionPlayer3D != nullptr);

	// 数値の初期化
	pMotionPlayer3D->Init(pName);

	// インスタンスを返す
	return pMotionPlayer3D;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CMotionPlayer3D::CMotionPlayer3D()
{
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的の向き
	m_motionType = TYPE_NEUTRAL;					// モーションタイプ
	m_nCntShot = 0;									// 弾発射までのカウント
	m_bPressShot = false;							// 長押し弾を使用してるかどうか
	m_bLockShot = false;							// 弾発射が可能かどうか
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CMotionPlayer3D::~CMotionPlayer3D()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CMotionPlayer3D::Init(const char *pMotionName)
{
	// 初期化
	CMotionChar3D::Init(pMotionName);

	return E_NOTIMPL;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CMotionPlayer3D::Uninit()
{
	// 終了
	CMotionChar3D::Uninit();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CMotionPlayer3D::Update()
{
	// ニュートラルモーションの入力
	m_motionType = TYPE_NEUTRAL;

	// 移動
	Move();

	// 回転
	Rotate();

	// 弾の発射
	Shot();

	// モーション番号の設定
	SetNumMotion(m_motionType);

	// 更新
	CMotionChar3D::Update();
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CMotionPlayer3D::Draw()
{
	// 描画
	CMotionChar3D::Draw();
}

//=============================================================================
// 回転
// Author : 唐﨑結斗
// 概要 : 回転を行う
//=============================================================================
void CMotionPlayer3D::Rotate()
{
	// 向きの取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// 向きの更新
	rot.y += (m_rotDest.y - rot.y) * ROTATE_SPEED;

	if (rot.y >= D3DX_PI)
	{// 移動方向の正規化
		rot.y -= D3DX_PI * 2;
	}
	else if (rot.y <= -D3DX_PI)
	{// 移動方向の正規化
		rot.y += D3DX_PI * 2;
	}

	// 向きの設定
	SetRot(rot);
}

//=============================================================================
// 移動
// Author : 唐﨑結斗
// 概要 : 移動を行う
//=============================================================================
void CMotionPlayer3D::Move()
{
	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// メンバ変数の取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	if (pKeyboard->GetPress(DIK_W)
		|| pKeyboard->GetPress(DIK_A)
		|| pKeyboard->GetPress(DIK_D)
		|| pKeyboard->GetPress(DIK_S))
	{// 移動キーが押された
		// 移動モーション
		m_motionType = TYPE_MOVE;

		if (pKeyboard->GetPress(DIK_W))
		{// [↑]キーが押された時
			if (pKeyboard->GetPress(DIK_A))
			{// [→]キーが押された時
			 // 移動方向の更新
				m_rotDest.y = D3DX_PI * -0.25f;
			}
			else if (pKeyboard->GetPress(DIK_D))
			{// [←]キーが押された時
			 // 移動方向の更新
				m_rotDest.y = D3DX_PI * 0.25f;
			}
			else
			{// 移動方向の更新
				m_rotDest.y = D3DX_PI * 0.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_S))
		{// [↓]キーが押された時
			if (pKeyboard->GetPress(DIK_A))
			{// [→]キーが押された時
			 // 移動方向の更新
				m_rotDest.y = D3DX_PI * -0.75f;
			}
			else if (pKeyboard->GetPress(DIK_D))
			{// [←]キーが押された時
			 // 移動方向の更新
				m_rotDest.y = D3DX_PI * 0.75f;
			}
			else
			{// 移動方向の更新q
				m_rotDest.y = D3DX_PI;
			}
		}
		else if (pKeyboard->GetPress(DIK_A))
		{// [→]キーが押された時
		 // 移動方向の更新
			m_rotDest.y = D3DX_PI * -0.5f;
		}
		else if (pKeyboard->GetPress(DIK_D))
		{// [←]キーが押された時
		 // 移動方向の更新
			m_rotDest.y = D3DX_PI * 0.5f;
		}

		if (m_rotDest.y > D3DX_PI)
		{// 移動方向の正規化
			m_rotDest.y -= D3DX_PI * 2;
		}
		else if (m_rotDest.y < -D3DX_PI)
		{// 移動方向の正規化
			m_rotDest.y += D3DX_PI * 2;
		}

		// 移動量の計算
		D3DXVECTOR3 move = D3DXVECTOR3(sinf(m_rotDest.y), 0.0f, cosf(m_rotDest.y));

		// 移動
		pos += move * 1.0f;

		// 目的の向きの反転
		m_rotDest.y -= D3DX_PI;
	}

	// 目的の向きの補正
	if (m_rotDest.y - rot.y >= D3DX_PI)
	{// 移動方向の正規化
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - rot.y <= -D3DX_PI)
	{// 移動方向の正規化
		m_rotDest.y += D3DX_PI * 2;
	}

	// 移動情報の設定
	SetPos(pos);
}

//=============================================================================
// 弾の発射
// Author : 唐﨑結斗
// 概要 : キー入力が行われた場合、弾を発射する
//=============================================================================
void CMotionPlayer3D::Shot(void)
{
	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// Object2Dのメンバ変数の取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 size = GetSize();

	// 変数宣言
	D3DXVECTOR3 bulletPos;		// 弾の発射位置
	CBullet3D *pBullet3D;		// 3D弾の生成

	if (pKeyboard->GetPress(DIK_SPACE)
		&& !m_bLockShot)
	{// 弾の発射
		// 弾の発射位置
		bulletPos = D3DXVECTOR3(0.0f, 18.0f, -45.0f);

		// ワールド座標にキャスト
		bulletPos = WorldCastVtx(bulletPos, pos, rot);

		// 弾の生成
		pBullet3D = CBullet3D::Create();
		pBullet3D->SetPos(bulletPos);
		pBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
		pBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, rot.y, 0.0f));
		pBullet3D->SetSpeed(10.0f);

		// カウントの初期化
		m_nCntShot = 0;

		// プレスオン
		m_bPressShot = true;

		// 弾が発射ができるかどうか
		m_bLockShot = true;
	}

	if (pKeyboard->GetRelease(DIK_SPACE))
	{// プレスオフ
		m_bPressShot = false;
	}

	if (m_bPressShot)
	{// プレスオン
		m_nCntShot++;

		if (m_nCntShot >= MAX_CNT_SHOT)
		{// カウントが弾発射までのカウントに達した
			// 弾の発射位置
			bulletPos = D3DXVECTOR3(20.0f, 18.0f, -45.0f);

			// ワールド座標にキャスト
			bulletPos = WorldCastVtx(bulletPos, pos, rot);

			// 弾の生成
			pBullet3D = CBullet3D::Create();
			pBullet3D->SetPos(bulletPos);
			pBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
			pBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, rot.y, 0.0f));
			pBullet3D->SetSpeed(10.0f);

			bulletPos = D3DXVECTOR3(-20.0f, 18.0f, -45.0f);

			// ワールド座標にキャスト
			bulletPos = WorldCastVtx(bulletPos, pos, rot);

			// 弾の生成
			pBullet3D = CBullet3D::Create();
			pBullet3D->SetPos(bulletPos);
			pBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
			pBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, rot.y, 0.0f));
			pBullet3D->SetSpeed(10.0f);

			// カウントの初期化
			m_nCntShot = 0;
		}
	}
	else if (m_bLockShot)
	{// ロックがされてる場合
		m_nCntShot++;

		if (m_nCntShot >= MAX_CNT_SHOT)
		{// カウントの初期化
			m_nCntShot = 0;
			m_bLockShot = false;
		}
	}
}


