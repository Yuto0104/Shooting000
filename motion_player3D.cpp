//=============================================================================
//
// 3Dモーションプレイヤークラス(model3D.h)
// Author : 唐�ｱ結斗
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
#include "mouse.h"

#include "calculation.h"
#include "bullet3D.h"
#include "camera.h"
#include "life_manager.h"
#include "gauge2D.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************
const float CMotionPlayer3D::ROTATE_SPEED = (0.1f);

//=============================================================================
// インスタンス生成
// Author : 唐�ｱ結斗
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
// Author : 唐�ｱ結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CMotionPlayer3D::CMotionPlayer3D()
{
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的の向き
	m_nNumMotion = TYPE_NEUTRAL;					// モーションタイプ
	m_nCntShot = 0;									// 弾発射までのカウント
	m_nLife = 0;									// 寿命
	m_nInvalidLife = 0;								// 追加ライフ
	m_nEnergy = 0;									// エネルギー
	m_bPressShot = false;							// 長押し弾を使用してるかどうか
	m_bLockShot = false;							// 弾発射が可能かどうか

	// オブジェクトの種別設定
	SetObjType(CObject::OBJTYPE_3DPLAYER);
}

//=============================================================================
// デストラクタ
// Author : 唐�ｱ結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CMotionPlayer3D::~CMotionPlayer3D()
{

}

//=============================================================================
// 初期化
// Author : 唐�ｱ結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CMotionPlayer3D::Init()
{
	// 初期化
	CModel3D::Init();

	// モーション情報
	m_pMotion[0] = new CMotion("data/MOTION/motion.txt");
	assert(m_pMotion[0] != nullptr);
	m_pMotion[1] = new CMotion("data/MOTION/motionShark.txt");
	assert(m_pMotion[1] != nullptr);

	// 移動
	m_pMove = new CMove;
	assert(m_pMove != nullptr);

	// 移動情報の初期化
	m_pMove->SetMoving(1.0f, 15.0f, 0.0f, 0.1f);

	// メンバ変数の初期化
	m_nNumMotion = 0;
	m_nNumMotionOld = m_nNumMotion;
	m_bMotion = false;
	m_bMotionBlend = false;
	SetColorType(CModel3D::TYPE_WHITE);
	SetColisonSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));
	m_nLife = 5;
	m_nInvalidLife = 0;
	
	CGauge2D *pGauge2D = CApplication::GetGauge2D();
	pGauge2D->SetMaxNumber((float)MAX_ENERGY);

	return E_NOTIMPL;
}

//=============================================================================
// 終了
// Author : 唐�ｱ結斗
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
// Author : 唐�ｱ結斗
// 概要 : 更新を行う
//=============================================================================
void CMotionPlayer3D::Update()
{
	// 情報の取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// ニュートラルモーションの入力
	m_nNumMotion = TYPE_NEUTRAL;

	// 移動
	pos += Move();

	// 回転
	Rotate();

	// 弾の発射
	Shot();

	// 色の変更
	ChangeColor();

	// モーションの設定
	MotionSet();

	// 移動情報の設定
	SetPos(pos);

	// スクリーンのあたり判定
	CollisionScreen();

	// 更新
	CModel3D::Update();
}

//=============================================================================
// 描画
// Author : 唐�ｱ結斗
// 概要 : 描画を行う
//=============================================================================
void CMotionPlayer3D::Draw()
{
	// 描画
	CModel3D::Draw();

	// ワールドマトリックスの取得
	D3DXMATRIX mtxWorld = GetMtxWorld();

	// パーツの描画設定
	m_pMotion[GetColorType() - 1]->SetParts(mtxWorld);
}

//=============================================================================
// 回転
// Author : 唐�ｱ結斗
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
// Author : 唐�ｱ結斗
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
		// 移動モーション
		m_nNumMotion = TYPE_MOVE;

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
// Author : 唐�ｱ結斗
// 概要 : キー入力が行われた場合、弾を発射する
//=============================================================================
void CMotionPlayer3D::Shot()
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
			pBullet3D->SetColor(bulletColor);
			pBullet3D->SetColorType(GetColorType());

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
// Author : 唐�ｱ結斗
// 概要 : スクリーンとプレイヤーのあたり判定を行う
//=============================================================================
void CMotionPlayer3D::CollisionScreen()
{
	// 位置の取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 worldPos;

	// スクリーン座標にキャスト
	D3DXVECTOR3 screenPos = CApplication::ScreenCastWorld(pos);

	if (screenPos.x < 0.0f)
	{
		worldPos = CApplication::WorldCastScreen(D3DXVECTOR3(0.0f, screenPos.y, screenPos.z));
		SetPos(D3DXVECTOR3(worldPos.x, pos.y, worldPos.z));
		screenPos = CApplication::ScreenCastWorld(GetPos());
	}
	else if (screenPos.x > (float)CRenderer::SCREEN_WIDTH)
	{
		worldPos = CApplication::WorldCastScreen(D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH, screenPos.y, screenPos.z));
		SetPos(D3DXVECTOR3(worldPos.x, pos.y, worldPos.z));
		screenPos = CApplication::ScreenCastWorld(GetPos());
	}

	if (screenPos.y < 0.0f)
	{
		worldPos = CApplication::WorldCastScreen(D3DXVECTOR3(screenPos.x, 0.0f, screenPos.z));
		SetPos(D3DXVECTOR3(worldPos.x, pos.y, worldPos.z));
		screenPos = CApplication::ScreenCastWorld(GetPos());
	}
	else if (screenPos.y > (float)CRenderer::SCREEN_HEIGHT)
	{
		worldPos = CApplication::WorldCastScreen(D3DXVECTOR3(screenPos.x, (float)CRenderer::SCREEN_HEIGHT, screenPos.z));
		SetPos(D3DXVECTOR3(worldPos.x, pos.y, worldPos.z));
		screenPos = CApplication::ScreenCastWorld(GetPos());
	}
}

//=============================================================================
// 色の変更
// Author : 唐�ｱ結斗
// 概要 : キーが押されると色を変更する
//=============================================================================
void CMotionPlayer3D::ChangeColor()
{
	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();
	CGauge2D *pGauge2D = CApplication::GetGauge2D();

	if (pKeyboard->GetTrigger(DIK_Q))
	{
		if (GetColorType() == CObject::TYPE_WHITE)
		{
			SetColorType(CObject::TYPE_BLACK);
			pGauge2D->SetCol(D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f));
		}
		else if (GetColorType() == CObject::TYPE_BLACK)
		{
			SetColorType(CObject::TYPE_WHITE);
			pGauge2D->SetCol(D3DXCOLOR(0.2f, 0.9f, 1.0f, 1.0f));
		}
	}
	
}

//=============================================================================
// モーションの設定
// Author : 唐�ｱ結斗
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
	}
}

//=============================================================================
// ヒット
// Author : 唐�ｱ結斗
// 概要 : ダメージを与える
//=============================================================================
void CMotionPlayer3D::Hit()
{
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

	// ライフの設定
	CApplication::GetLifeManager()->SetLife();

	if (m_nLife == 0)
	{// 終了
		Uninit();
	}
}

//=============================================================================
// チャージ
// Author : 唐�ｱ結斗
// 概要 : エネルギーを吸収する
//=============================================================================
void CMotionPlayer3D::Charge()
{// エネルギーのインクリメント
	m_nEnergy++;

	if (m_nEnergy > MAX_ENERGY)
	{
		m_nEnergy = MAX_ENERGY;
	}

	CGauge2D *pGauge2D = CApplication::GetGauge2D();
	pGauge2D->SetNumber((float)m_nEnergy);
}


