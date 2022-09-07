//=============================================================================
//
// 2Dプレイヤー(player2D.h)
// Author : 唐﨑結斗
// 概要 : 2Dプレイヤー生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "player2D.h"
#include "bullet2D.h"
#include "renderer.h"
#include "application.h"
#include "keyboard.h"
#include "mouse.h"
#include "sound.h"
#include "move.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************
static const float				MAX_SIZE = (50.0f);				// 大きさ
static const float				SPEED = (0.5f);					// 移動速度
static const float				MAX_SPEED = (15.0f);			// 最高速度
static const float				MIN_SPEED = (0.0f);				// 最低速度
static const float				FRICTION = (0.1f);				// 摩擦係数
static const float				MAX_BULLET_SPEED = (20.0f);		// 弾の速度
static const unsigned int		MAX_CNT_SHOT = (15);			// 弾発射までのカウント

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : 2Dプレイヤーを生成する
//=============================================================================
CPlayer2D * CPlayer2D::Create(void)
{
	// プレイヤーインスタンス
	CPlayer2D *pPlayer2D = nullptr;

	pPlayer2D = new CPlayer2D;

	if (pPlayer2D != nullptr)
	{// 数値の初期化
		pPlayer2D->Init();
	}
	else
	{// メモリの確保ができなかった
		assert(false);
	}

	// インスタンスを返す
	return pPlayer2D;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CPlayer2D::CPlayer2D()
{
	m_pMove = new CMove;
	assert(m_pMove != nullptr);
	m_nCntShot = 0;
	SetObjType(CObject::OBJTYPE_2DPLAYER);
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CPlayer2D::~CPlayer2D()
{

}

//=============================================================================
// プレイヤーの初期化
// Author : 唐﨑結斗
// 概要 : テクスチャの設定し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CPlayer2D::Init()
{
	// プレイヤー2Dの初期化
	CObject2D::Init();

	// テクスチャの設定
	LoadTex(2);

	// サイズ
	D3DXVECTOR3 size = GetSize();
	size = D3DXVECTOR3(MAX_SIZE, MAX_SIZE, 0.0f);
	SetSize(size);

	// 移動情報の初期化
	m_pMove->SetMoving(SPEED, MAX_SPEED, MIN_SPEED, FRICTION);

	// テクスチャ座標の設定
	SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	return S_OK;
}

//=============================================================================
// プレイヤーの終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CPlayer2D::Uninit()
{
	if (m_pMove != nullptr)
	{// 終了処理
		// メモリの解放
		delete m_pMove;
		m_pMove = nullptr;
	}

	// プレイヤー2Dの終了
	CObject2D::Uninit();
}

//=============================================================================
// プレイヤーの更新
// Author : 唐﨑結斗
// 概要 : 2Dプレイヤーの更新を行う
//=============================================================================
void CPlayer2D::Update()
{
	// プレイヤー2Dの更新
	CObject2D::Update();

	// 移動
	D3DXVECTOR3 pos = GetPos();
	SetPosOld(pos);
	pos += Move();
	SetPos(pos);

	// 弾の発射
	Shot();
	
	for (int nCntPriority = 0; nCntPriority < CObject::MAX_LEVEL; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			CObject *pObject = CObject::MyGetObject(nCntPriority, nCntObj);

			if (pObject != nullptr)
			{
				if (pObject->GetObjType() == CObject::OBJTYPE_2DENEMY
					&& ColisonCircle2D(pObject, true))
				{// タイプが一致した場合
					//m_nLife = 1;
					break;
				}
			}
		}
	}
	

	// テクスチャ座標の設定
	SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
}

//=============================================================================
// プレイヤーの描画
// Author : 唐﨑結斗
// 概要 : 2Dプレイヤーの描画を行う
//=============================================================================
void CPlayer2D::Draw()
{// プレイヤー2Dの描画
	CObject2D::Draw();
}

//=============================================================================
// 移動
// Author : 唐﨑結斗
// 概要 : キー入力が行われた場合移動する
//=============================================================================
D3DXVECTOR3 CPlayer2D::Move(void)
{
	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// 目的の移動方向の算出
	D3DXVECTOR3 moveDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (pKeyboard->GetPress(DIK_W))
	{// 上方向
		moveDir.y += -1.0f;
	}
	if (pKeyboard->GetPress(DIK_A))
	{// 左方向
		moveDir.x += -1.0f;
	}
	if (pKeyboard->GetPress(DIK_S))
	{// 下方向
		moveDir.y += 1.0f;
	}
	if (pKeyboard->GetPress(DIK_D))
	{// 右方向
		moveDir.x += 1.0f;
	}

	// 摩擦係数を考慮した移動
	m_pMove->Moving(moveDir);

	return m_pMove->GetMove();
}

//=============================================================================
// 弾の発射
// Author : 唐﨑結斗
// 概要 : キー入力が行われた場合、弾を発射する
//=============================================================================
void CPlayer2D::Shot(void)
{
	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();
	CSound *pSound = CApplication::GetSound();

	// Object2Dのメンバ変数の取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 size = GetSize();

	if (pKeyboard->GetPress(DIK_SPACE)
		&& !m_bLockShot)
	{// 弾の発射
		CBullet2D::Create(D3DXVECTOR3(pos.x, pos.y - size.y, 0.0f), rot, MAX_BULLET_SPEED);

		pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE);

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
			// 弾の発射
			CBullet2D::Create(D3DXVECTOR3(pos.x - size.x / 2, pos.y - size.y, 0.0f), rot, MAX_BULLET_SPEED);
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE);

			CBullet2D::Create(D3DXVECTOR3(pos.x + size.x / 2, pos.y - size.y, 0.0f), rot, MAX_BULLET_SPEED);
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE);

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
