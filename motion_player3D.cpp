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
#include "motion.h"
#include "move.h"
#include "renderer.h"
#include "application.h"
#include "keyboard.h"
#include "sound.h"
#include "mouse.h"
#include "game.h"
#include "tutorial.h"

#include "calculation.h"
#include "bullet3D.h"
#include "follow_bullet3D.h"
#include "camera.h"
#include "life_manager.h"
#include "gauge2D.h"
#include "score.h"
#include "energy_gage.h"
#include "effect3D.h"
#include "particle.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************
const float CMotionPlayer3D::ROTATE_SPEED = (0.1f);

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : 3Dモーションプレイヤーを生成する
//=============================================================================
CMotionPlayer3D * CMotionPlayer3D::Create()
{
	// オブジェクトインスタンス
	CMotionPlayer3D *pMotionPlayer3D = nullptr;

	// メモリの解放
	pMotionPlayer3D = new CMotionPlayer3D;

	// メモリの確保ができなかった
	assert(pMotionPlayer3D != nullptr);

	// 数値の初期化
	pMotionPlayer3D->Init();

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
	m_pEffect3D = nullptr;							// エフェクトのインスタンス
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的の向き
	m_nNumMotion = TYPE_NEUTRAL;					// モーションタイプ
	m_state = STATE_NEUTRAL;						// 状態
	m_nCntShot = 0;									// 弾発射までのカウント
	m_nLife = 0;									// 寿命
	m_nInvalidLife = 0;								// 追加ライフ
	m_nEnergy = 0;									// エネルギー
	m_nCntState = 0;								// 状態制御のカウント
	m_nCntFrame = 0;								// フレームカウント
	m_bPressShot = false;							// 長押し弾を使用してるかどうか
	m_bLockShot = false;							// 弾発射が可能かどうか
	m_bFollowShot = false;							// 追従弾を発射しているか否
	m_bUse = false;									// 使用状況

	// オブジェクトの種別設定
	SetObjType(CObject::OBJTYPE_3DPLAYER);
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
HRESULT CMotionPlayer3D::Init()
{
	// 初期化
	CModel3D::Init();

	if (m_pMotion[0] == nullptr
		&& m_pMotion[1] == nullptr)
	{// モーション情報
		m_pMotion[0] = new CMotion("data/MOTION/white.txt");
		assert(m_pMotion[0] != nullptr);
		m_pMotion[1] = new CMotion("data/MOTION/black.txt");
		assert(m_pMotion[1] != nullptr);
	}

	if (m_pMove == nullptr)
	{// 移動
		m_pMove = new CMove;
		assert(m_pMove != nullptr);
	}

	// 移動情報の初期化
	m_pMove->SetMoving(1.0f, 15.0f, 0.0f, 0.1f);

	// メンバ変数の初期化
	m_nNumMotion = 0;
	m_nNumMotionOld = m_nNumMotion;
	m_pMotion[0]->SetMotion(m_nNumMotion);
	m_pMotion[1]->SetMotion(m_nNumMotion);
	m_bMotion = true;
	m_bMotionBlend = false;
	SetColorType(CModel3D::TYPE_WHITE);
	SetColisonSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));
	m_nLife = 5;
	m_nInvalidLife = 0;
	SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	SetSize(D3DXVECTOR3(1.2f, 1.2f, 1.2f));
	m_pEffect3D = CEffect3D::Create();
	m_pEffect3D->SetPos(GetPos());
	m_pEffect3D->SetSize(D3DXVECTOR3(60.0f, 60.0f, 60.0f));
	m_pEffect3D->SetLife(-1);
	m_pEffect3D->SetRenderMode(CEffect3D::MODE_ADD);
	m_pEffect3D->LoadTex(27);
	m_bUse = true;

	switch (CApplication::GetMode())
	{
	case CApplication::MODE_GAME:
		// プレイヤーの取得
		CGame::SetUsePlayer(m_bUse);
		break;

	case CApplication::MODE_TUTORIAL:
		// プレイヤーの取得
		CTutorial::SetUsePlayer(m_bUse);
		break;

	default:
		break;
	}

	return E_NOTIMPL;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CMotionPlayer3D::Uninit()
{
	if (m_pMove != nullptr)
	{// 終了処理
		// メモリの解放
		delete m_pMove;
		m_pMove = nullptr;
	}

	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{
		if (m_pMotion[nCntMotion] != nullptr)
		{// 終了処理
			m_pMotion[nCntMotion]->Uninit();

			// メモリの解放
			delete m_pMotion[nCntMotion];
			m_pMotion[nCntMotion] = nullptr;
		}
	}	

	// 終了
	CModel3D::Uninit();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CMotionPlayer3D::Update()
{
	switch (CApplication::GetMode())
	{
	case CApplication::MODE_GAME:
		// プレイヤーの取得
		m_bUse = CGame::GetUsePlayer();
		break;

	case CApplication::MODE_TUTORIAL:
		// プレイヤーの取得
		m_bUse = CTutorial::GetUsePlayer();
		break;

	default:
		break;
	}

	if (m_bUse)
	{// 情報の取得
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 rot = GetRot();

		if (!m_bFollowShot)
		{// ニュートラルモーションの入力
			m_nNumMotion = TYPE_NEUTRAL;
		}

		// 移動
		pos += Move();

		// 回転
		Rotate();

		// 弾の発射
		Shot();

		// エネルギー消費
		Consumption();

		// 色の変更
		ChangeColor();

		// モーションの設定
		MotionSet();

		// y座標の修正
		pos.y = 0.0f;

		// 移動情報の設定
		SetPos(pos);
		m_pEffect3D->SetPos(pos);

		if (GetColorType() == CObject::TYPE_WHITE)
		{
			m_pEffect3D->SetColor(D3DXCOLOR(0.5f, 0.7f, 1.0f, 0.5f));
		}
		else if (GetColorType() == CObject::TYPE_BLACK)
		{
			m_pEffect3D->SetColor(D3DXCOLOR(1.0f, 0.1f, 0.1f, 0.5f));
		}

		// 敵との当たり判定
		CollisionEnemy();

		// スクリーンのあたり判定
		CollisionScreen();

		// 状態の制御
		SetState();

		// 更新
		CModel3D::Update();

		// 死亡処理
		Death();
	}
	else if (!m_bUse)
	{
		switch (CApplication::GetMode())
		{
		case CApplication::MODE_GAME:
			// カウントアップ
			m_nCntFrame++;

			if (m_nCntFrame >= 40)
			{// 終了
				Uninit();
				CGame::SetGame(false);
			}

			break;

		case CApplication::MODE_TUTORIAL:
			Init();

			// ライフの設定
			CTutorial::GetLifeManager()->SetLife();
			break;

		default:
			break;
		}	
	}
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CMotionPlayer3D::Draw()
{
	switch (CApplication::GetMode())
	{
	case CApplication::MODE_GAME:
		// プレイヤーの取得
		m_bUse = CGame::GetUsePlayer();
		break;

	case CApplication::MODE_TUTORIAL:
		// プレイヤーの取得
		m_bUse = CTutorial::GetUsePlayer();
		break;

	default:
		break;
	}

	if (m_bUse)
	{// 描画
		CModel3D::Draw();

		// ワールドマトリックスの取得
		D3DXMATRIX mtxWorld = GetMtxWorld();

		switch (m_state)
		{
		case CMotionPlayer3D::STATE_NEUTRAL:
			// パーツの描画設定
			m_pMotion[GetColorType() - 1]->SetParts(mtxWorld);
			break;

		case CMotionPlayer3D::STATE_DAMAGE:
			// パーツの描画設定
			m_pMotion[GetColorType() - 1]->SetParts(mtxWorld, D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f));
			break;

		default:
			break;
		}
	}
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
	//SetRot(rot);
}

//=============================================================================
// 移動
// Author : 唐﨑結斗
// 概要 : 移動を行う
//=============================================================================
D3DXVECTOR3 CMotionPlayer3D::Move()
{
	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// メンバ変数の取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (pKeyboard->GetPress(DIK_W)
		|| pKeyboard->GetPress(DIK_A)
		|| pKeyboard->GetPress(DIK_D)
		|| pKeyboard->GetPress(DIK_S))
	{// 移動キーが押された
		if (!m_bFollowShot)
		{// 移動モーション
			m_nNumMotion = TYPE_MOVE;
		}

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
		move = D3DXVECTOR3(sinf(m_rotDest.y), 0.0f, cosf(m_rotDest.y));

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

	// 摩擦係数を考慮した移動
	m_pMove->Moving(move);

	return m_pMove->GetMove();
}

//=============================================================================
// 弾の発射
// Author : 唐﨑結斗
// 概要 : キー入力が行われた場合、弾を発射する
//=============================================================================
void CMotionPlayer3D::Shot()
{
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();
	
	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// Object2Dのメンバ変数の取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 size = GetSize();

	// 変数宣言
	D3DXVECTOR3 bulletPos;		// 弾の発射位置
	CBullet3D *pBullet3D;		// 3D弾の生成
	D3DXCOLOR bulletColor;		// 弾の色

	if (GetColorType() == CObject::TYPE_WHITE)
	{// 弾の色の設定
		bulletColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (GetColorType() == CObject::TYPE_BLACK)
	{// 弾の色の設定
		bulletColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (pKeyboard->GetPress(DIK_SPACE)
		&& !m_bLockShot)
	{// 弾の発射
		pSound->PlaySound(CSound::SOUND_LABEL_SE_SHOT);

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
		pBullet3D->SetColor(bulletColor);
		pBullet3D->SetColorType(GetColorType());
		pBullet3D->SetParent(CObject::OBJTYPE_3DPLAYER);

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

			pSound->PlaySound(CSound::SOUND_LABEL_SE_SHOT);

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
			pBullet3D->SetColor(bulletColor);
			pBullet3D->SetColorType(GetColorType());
			pBullet3D->SetParent(CObject::OBJTYPE_3DPLAYER);

			// 弾の座標の算出
			bulletPos = D3DXVECTOR3(-20.0f, 18.0f, -45.0f);

			// ワールド座標にキャスト
			bulletPos = WorldCastVtx(bulletPos, pos, rot);

			// 弾の生成
			pBullet3D = CBullet3D::Create();
			pBullet3D->SetPos(bulletPos);
			pBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
			pBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, rot.y, 0.0f));
			pBullet3D->SetSpeed(10.0f);
			pBullet3D->SetColor(bulletColor);
			pBullet3D->SetColorType(GetColorType());
			pBullet3D->SetParent(CObject::OBJTYPE_3DPLAYER);

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

//=============================================================================
// スクリーンのあたり判定
// Author : 唐﨑結斗
// 概要 : スクリーンとプレイヤーのあたり判定を行う
//=============================================================================
void CMotionPlayer3D::CollisionScreen()
{
	// 位置の取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 worldPos;

	// 当たり判定の取得
	D3DXVECTOR3 collision = CObject::GetColisonSize();

	// スクリーン座標にキャスト
	D3DXVECTOR3 screenPos = CApplication::ScreenCastWorld(pos);
	D3DXVECTOR3 screenPosUp = CApplication::ScreenCastWorld(pos + collision);
	D3DXVECTOR3 screenPosUnder = CApplication::ScreenCastWorld(pos - collision);

	if (screenPosUnder.x < 0.0f)
	{
		worldPos = CApplication::WorldCastScreen(D3DXVECTOR3(collision.x, screenPos.y, screenPos.z));
		SetPos(D3DXVECTOR3(worldPos.x, pos.y, worldPos.z));
		screenPos = CApplication::ScreenCastWorld(GetPos());
	}
	else if (screenPosUp.x > (float)CRenderer::SCREEN_WIDTH)
	{
		worldPos = CApplication::WorldCastScreen(D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH - collision.x, screenPos.y, screenPos.z));
		SetPos(D3DXVECTOR3(worldPos.x, pos.y, worldPos.z));
		screenPos = CApplication::ScreenCastWorld(GetPos());
	}

	if (screenPosUp.y < 0.0f)
	{
		worldPos = CApplication::WorldCastScreen(D3DXVECTOR3(screenPos.x, collision.y, screenPos.z));
		SetPos(D3DXVECTOR3(worldPos.x, pos.y, worldPos.z));
		screenPos = CApplication::ScreenCastWorld(GetPos());
	}
	else if (screenPosUnder.y > (float)CRenderer::SCREEN_HEIGHT)
	{
		worldPos = CApplication::WorldCastScreen(D3DXVECTOR3(screenPos.x, (float)CRenderer::SCREEN_HEIGHT - collision.y, screenPos.z));
		SetPos(D3DXVECTOR3(worldPos.x, pos.y, worldPos.z));
		screenPos = CApplication::ScreenCastWorld(GetPos());
	}
}

//=============================================================================
// 色の変更
// Author : 唐﨑結斗
// 概要 : キーが押されると色を変更する
//=============================================================================
void CMotionPlayer3D::ChangeColor()
{
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	if (pKeyboard->GetTrigger(DIK_Q))
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_TRANCEFARM);

		if (GetColorType() == CObject::TYPE_WHITE)
		{
			SetColorType(CObject::TYPE_BLACK);
		}
		else if (GetColorType() == CObject::TYPE_BLACK)
		{
			SetColorType(CObject::TYPE_WHITE);
		}
	}
}

//=============================================================================
// モーションの設定
// Author : 唐﨑結斗
// 概要 : モーションの変更やモーションブレンドの設定を行う
//=============================================================================
void CMotionPlayer3D::MotionSet()
{
	// 現在のモーション番号の保管
	m_nNumMotionOld = m_nNumMotion;

	if (!m_bMotion)
	{// 使用してるモーションがない場合
		m_nNumMotion = TYPE_NEUTRAL;
	}

	if (m_nNumMotionOld != m_nNumMotion)
	{// モーションタイプが変更された時
		m_pMotion[GetColorType() - 1]->CntReset((int)(m_nNumMotionOld));
		m_bMotionBlend = true;
	}

	if (m_bMotionBlend)
	{// モーションブレンドを使用してる
		m_bMotionBlend = m_pMotion[GetColorType() - 1]->MotionBlend((int)(m_nNumMotion));
	}
	else if (!m_bMotionBlend)
	{// モーションブレンドを使用してない場合
		m_bMotion = m_pMotion[GetColorType() - 1]->PlayMotion((int)(m_nNumMotion));
		m_bFollowShot = false;
	}
}

//=============================================================================
// 回復
// Author : 唐﨑結斗
// 概要 : ライフの回復を行う
//=============================================================================
void CMotionPlayer3D::Recovery()
{
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	if (m_nLife < MAX_LIFE
		&& m_nEnergy >= ENERGY_RECOVERY)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_HEEL);

		int nRecoveryEnergy = (int)ENERGY_RECOVERY;

		// エネルギーの消費
		Charge(-nRecoveryEnergy);

		m_nLife++;

		if (m_nLife >= MAX_LIFE)
		{
			m_nLife = MAX_LIFE;
		}

		switch (CApplication::GetMode())
		{
		case CApplication::MODE_GAME:
			// ライフの設定
			CGame::GetLifeManager()->SetLife();
			break;

		case CApplication::MODE_TUTORIAL:
			// ライフの設定
			CTutorial::GetLifeManager()->SetLife();
			break;

		default:
			break;
		}

		// 変数宣言
		D3DXVECTOR3 pos = GetPos();

		// パーティクルの生成
		CParticle *pParticle = CParticle::Create();
		pParticle->SetPos(pos);
		pParticle->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
		pParticle->SetPopRange(D3DXVECTOR3(100.0f, 0.0f, 100.0f));
		pParticle->SetSpeed(10.0f);
		pParticle->SetEffectLife(60);
		pParticle->SetMoveVec(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pParticle->SetLife(10);
		pParticle->SetColor(D3DXCOLOR(0.1f, 1.0f, 0.5f, 1.0f));
		pParticle->SetMaxEffect(5);

		m_bRecovery = true;
	}
}

//=============================================================================
// 追従弾の発射
// Author : 唐﨑結斗
// 概要 : 追従弾を発射する
//=============================================================================
void CMotionPlayer3D::FollowShot()
{
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	if (m_nEnergy >= ENERGY_FOLLOW_SHOT)
	{
		// 攻撃モーション
		m_nNumMotion = TYPE_ATTACK;
		m_bFollowShot = true;

		int nFollowShotEnergy = (int)ENERGY_FOLLOW_SHOT;

		// エネルギーの消費
		Charge(-nFollowShotEnergy);

		// Object2Dのメンバ変数の取得
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 rot = GetRot();

		// 変数宣言
		D3DXVECTOR3 bulletPos;					// 弾の発射位置
		CFollowBullet3D *pFollowBullet3D;		// 追従弾の生成
		D3DXCOLOR bulletColor;					// 弾の色

		if (GetColorType() == CObject::TYPE_WHITE)
		{// 弾の色の設定
			bulletColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else if (GetColorType() == CObject::TYPE_BLACK)
		{// 弾の色の設定
			bulletColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		}

		// 弾の発射位置
		bulletPos = D3DXVECTOR3(0.0f, 18.0f, 45.0f);

		// ワールド座標にキャスト
		bulletPos = WorldCastVtx(bulletPos, pos, rot);

		// 弾の生成
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_SHOT);

			float fAttRot = 0.5f / 4 * nCnt;
			pFollowBullet3D = CFollowBullet3D::Create();
			pFollowBullet3D->SetPos(bulletPos);
			pFollowBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
			pFollowBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, rot.y - D3DX_PI * (0.5f - fAttRot), 0.0f));
			pFollowBullet3D->SetSpeed(30.0f);
			pFollowBullet3D->SetColor(bulletColor);
			pFollowBullet3D->SetColorType(GetColorType());
			pFollowBullet3D->SetCoefficient(0.01f);
			pFollowBullet3D->SetAddCoefficient(0.01f);
			pFollowBullet3D->SetParent(CObject::OBJTYPE_3DPLAYER);

			pFollowBullet3D = CFollowBullet3D::Create();
			pFollowBullet3D->SetPos(bulletPos);
			pFollowBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
			pFollowBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, rot.y - D3DX_PI * (-0.5f + fAttRot), 0.0f));
			pFollowBullet3D->SetSpeed(30.0f);
			pFollowBullet3D->SetColor(bulletColor);
			pFollowBullet3D->SetColorType(GetColorType());
			pFollowBullet3D->SetCoefficient(0.01f);
			pFollowBullet3D->SetAddCoefficient(0.01f);
			pFollowBullet3D->SetParent(CObject::OBJTYPE_3DPLAYER);
		}
	}
}

//=============================================================================
// エネルギー消費
// Author : 唐﨑結斗
// 概要 : エネルギーを消費し、処理を行う
//=============================================================================
void CMotionPlayer3D::Consumption()
{
	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	if (pKeyboard->GetPress(DIK_RETURN)
		&& !m_bPressConsumption)
	{// 弾の発射
		// カウントの初期化
		m_nCntConsumption = 0;

		// プレスオン
		m_bPressConsumption = true;
	}

	if (pKeyboard->GetRelease(DIK_RETURN))
	{// プレスオフ
		m_bPressConsumption = false;

		if (m_bRecovery)
		{
			m_bRecovery = false;
		}
		else
		{
			FollowShot();
		}

		m_nCntConsumption = 0;
	}

	if (m_bPressConsumption)
	{// プレスオン
		m_nCntConsumption++;

		if (m_nCntConsumption >= MAX_CNT_RECOVERY)
		{// カウントが弾発射までのカウントに達した
			// カウントの初期化
			m_nCntConsumption = 0;

			// 回復
			Recovery();
		}
	}
}

//=============================================================================
// 状態の制御
// Author : 唐﨑結斗
// 概要 : 状態の制御を行う
//=============================================================================
void CMotionPlayer3D::SetState()
{
	switch (m_state)
	{
	case CMotionPlayer3D::STATE_NEUTRAL:
		break;

	case CMotionPlayer3D::STATE_DAMAGE:
		m_nCntState++;

		if (m_nCntState >= INVINCIBLE_TIME)
		{
			m_nCntState = 0;
			m_state = STATE_NEUTRAL;
		}

		break;

	case CMotionPlayer3D::MAX_STATE:
		break;

	default:
		break;
	}
}

//=============================================================================
// 敵との当たり判定
// Author : 唐﨑結斗
// 概要 : 敵との当たり判定
//=============================================================================
void CMotionPlayer3D::CollisionEnemy()
{
	for (int nCntPriority = 0; nCntPriority < CObject::MAX_LEVEL; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			// オブジェクトインスタンスの取得
			CObject *pObject = CObject::MyGetObject(nCntPriority, nCntObj);

			if (pObject != nullptr)
			{
				if ((pObject->GetObjType() == CObject::OBJTYPE_3DENEMY
					|| pObject->GetObjType() == CObject::OBJTYPE_3DBOSS))
				{// タイプが一致した場合
					if (ColisonSphere3D(pObject, GetColisonSize(), pObject->GetColisonSize(), true))
					{
						Hit();
						break;
					}
				}
			}
		}
	}
}

//=============================================================================
// 死亡処理
// Author : 唐﨑結斗
// 概要 : 死ぬときの処理を行う
//=============================================================================
void CMotionPlayer3D::Death()
{
	if (m_nLife == 0)
	{
		// サウンド情報の取得
		CSound *pSound = CApplication::GetSound();
		pSound->PlaySound(CSound::SOUND_LABEL_SE_EXPLOSION);

		// 変数宣言
		D3DXVECTOR3 pos = GetPos();
		m_pEffect3D->Uninit();

		// パーティクルの生成
		CParticle *pParticle = CParticle::Create();
		pParticle->SetPos(pos);
		pParticle->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
		pParticle->SetPopRange(D3DXVECTOR3(3.0f, 3.0f, 3.0f));
		pParticle->SetSpeed(5.0f);
		pParticle->SetEffectLife(30);
		pParticle->SetMoveVec(D3DXVECTOR3(D3DX_PI * 2.0f, D3DX_PI * 2.0f, 0.0f));
		pParticle->SetLife(10);
		pParticle->SetColor(D3DXCOLOR(1.0f, 0.4f, 0.1f, 1.0f));
		pParticle->SetMaxEffect(5);

		switch (CApplication::GetMode())
		{
		case CApplication::MODE_GAME:
			// プレイヤーの取得
			CGame::SetUsePlayer(false);
			break;

		case CApplication::MODE_TUTORIAL:
			// プレイヤーの取得
			CTutorial::SetUsePlayer(false);
			break;

		default:
			break;
		}
	}
}

//=============================================================================
// ヒット
// Author : 唐﨑結斗
// 概要 : ダメージを与える
//=============================================================================
void CMotionPlayer3D::Hit()
{
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	if (m_state == STATE_NEUTRAL)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT);

		m_state = STATE_DAMAGE;

		if (m_nInvalidLife > 0)
		{// 追加ライフの消費
			m_nInvalidLife--;

			if (m_nInvalidLife <= 0)
			{
				m_nInvalidLife = 0;
			}
		}
		else
		{// ライフの消費
			m_nLife--;

			if (m_nLife <= 0)
			{
				m_nLife = 0;
			}
		}

		switch (CApplication::GetMode())
		{
		case CApplication::MODE_GAME:
			// ライフの設定
			CGame::GetLifeManager()->SetLife();
			break;

		case CApplication::MODE_TUTORIAL:
			// ライフの設定
			CTutorial::GetLifeManager()->SetLife();
			break;

		default:
			break;
		}
	}
}

//=============================================================================
// チャージ
// Author : 唐﨑結斗
// 概要 : エネルギーを吸収する
//=============================================================================
void CMotionPlayer3D::Charge(const int nEnergy)
{
	CEnergyGage *pEnergyGage = nullptr;

	switch (CApplication::GetMode())
	{
	case CApplication::MODE_GAME:
		pEnergyGage = CGame::GetEnergyGage();
		break;

	case CApplication::MODE_TUTORIAL:
		pEnergyGage = CTutorial::GetEnergyGage();
		break;

	default:
		break;
	}

	CGauge2D *pEnergyGauge2D = pEnergyGage->GetGauge2D();
	CScore *pEnergy = pEnergyGage->GetScore();

	// エネルギーのインクリメント
	int nEnergyPoint = pEnergy->GetScore();
	int nEnergyPointOld = nEnergyPoint;
	int nDiffEP = 0;

	// エネルギーの加算
	nEnergyPoint += nEnergy;

	// 加算値の差分の算出
	nDiffEP = nEnergyPoint - nEnergyPointOld;

	// 数値の設定
	pEnergy->AddScore(nDiffEP);

	if (nEnergyPoint >= MAX_ENERGY)
	{
		nDiffEP = MAX_ENERGY - nEnergyPoint;
		nEnergyPoint = MAX_ENERGY;
		pEnergy->AddScore(nDiffEP);
	}
	else if (nEnergyPoint <= 0)
	{
		nDiffEP = 0 - nEnergyPoint;
		nEnergyPoint = 0;
		pEnergy->AddScore(nDiffEP);
	}

	// ゲージの設定
	pEnergyGauge2D->SetNumber((float)nEnergyPoint);

	// エネルギーゲージの設定
	m_nEnergy = nEnergyPoint;
}


