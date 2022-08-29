//**************************************************************************************************
//
// カメラ処理(camera.h)
// Auther：唐﨑結斗
// 概要 : カメラの設定処理
//
//**************************************************************************************************

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "camera.h"
#include "application.h"
#include "renderer.h"
#include "input.h"
#include "mouse.h"
#include "keyboard.h"
#include "move.h"
#include "calculation.h"
#include "motion_player3D.h"
#include "game.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************
const float CCamera::CAMERA_NEAR = (10.0f);				// ニア
const float CCamera::CAMERA_FUR = (10000.0f);			// ファー

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 注視点
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 上方向ベクトル
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_rotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動方向
	m_mtxProj = {};										// プロジェクションマトリックス
	m_mtxView = {};										// ビューマトリックス
	m_viewType = TYPE_CLAIRVOYANCE;						// 投影方法の種別
	m_fDistance = 0.0f;									// 視点から注視点までの距離
	m_fRotMove = 0.0f;									// 移動方向
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 視点と注視点の間の距離を算出する
//=============================================================================
HRESULT CCamera::Init()
{
	m_posV = D3DXVECTOR3(0.0f, 500.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// 固定
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 視点と注視点の距離
	D3DXVECTOR3 posDiss = m_posR - m_posV;
	m_fDistance = sqrtf((posDiss.y * posDiss.y) + (posDiss.x * posDiss.x) + (posDiss.z * posDiss.z));

	// 角度の算出
	m_rot.y = atan2f(posDiss.x, posDiss.z);
	m_rot.x = atan2f(sqrtf((posDiss.x * posDiss.x) + (posDiss.z * posDiss.z)), posDiss.y);
	m_rot.z = 0.0f;

	// メモリの確保
	m_pRoll = new CMove;
	assert(m_pRoll != nullptr);
	m_pRoll->SetMoving(2.0f, 10.0f, 0.0f, 0.2f);

	if (m_rot.y < -D3DX_PI)
	{// 向きが-D3DX_PI未満の時
		m_rot.y += D3DX_PI * 2;
	}
	if (m_rot.y > D3DX_PI)
	{// 向きがD3DX_PI以上の時
		m_rot.y -= D3DX_PI * 2;
	}
	if (m_rot.x < 0.0f + 0.1f)
	{// 向きが0以下の時
		m_rot.x = 0.0f + 0.1f;
	}
	if (m_rot.x > D3DX_PI - 0.1f)
	{// 向きがD3DX_PI以上の時
		m_rot.x = D3DX_PI - 0.1f;
	}

	// 注視点の算出
	m_posR.z = m_posV.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
	m_posR.x = m_posV.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posR.y = m_posV.y + cosf(m_rot.x) * m_fDistance;

	// 視点の算出
	m_posV.z = m_posR.z - sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
	m_posV.x = m_posR.x - sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posV.y = m_posR.y - cosf(m_rot.x) * m_fDistance;

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : 終了
//=============================================================================
void CCamera::Uninit(void)
{
	if (m_pRoll != nullptr)
	{// 終了処理
	 // メモリの解放
		delete m_pRoll;
		m_pRoll = nullptr;
	}
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新
//=============================================================================
void CCamera::Update(void)
{
	if(m_viewType == TYPE_CLAIRVOYANCE)
	{
		Rotate();
	}
	//Move();
	//FollowCamera();
}

//=============================================================================
// カメラの設定
// Author : 唐﨑結斗
// 概要 : ビューマトリックスの設定
//=============================================================================
void CCamera::Set()
{// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);			// 行列初期化関数

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProj);			// 行列初期化関数

	switch (m_viewType)
	{
	case TYPE_CLAIRVOYANCE:
		// プロジェクションマトリックスの作成(透視投影)
		D3DXMatrixPerspectiveFovLH(&m_mtxProj,											// プロジェクションマトリックス
			D3DXToRadian(45.0f),														// 視野角
			(float)CRenderer::SCREEN_WIDTH / (float)CRenderer::SCREEN_HEIGHT,			// アスペクト比
			CAMERA_NEAR,																// ニア
			CAMERA_FUR);																// ファー
		break;

	case TYPE_PARALLEL:
		// プロジェクションマトリックスの作成(平行投影)
		D3DXMatrixOrthoLH(&m_mtxProj,							// プロジェクションマトリックス
			(float)CRenderer::SCREEN_WIDTH,						// 幅
			(float)CRenderer::SCREEN_HEIGHT,					// 高さ
			CAMERA_NEAR,										// ニア
			CAMERA_FUR);										// ファー
		break;

	default:
		assert(false);
		break;
	}

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProj);
}

//=============================================================================
// カメラの回転
// Author : 唐﨑結斗
// 概要 : 
//=============================================================================
void CCamera::Rotate(void)
{
	// 入力情報の取得
	const float MIN_MOUSE_MOVED = 1.0f;
	const float ROTATE_MOUSE_MOVED = 0.45f;

	CMouse *pMouse = CApplication::GetMouse();
	D3DXVECTOR3 rollDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nRotateType = -1;

	if (pMouse->GetPress(CMouse::MOUSE_KEY_LEFT)
		|| pMouse->GetPress(CMouse::MOUSE_KEY_RIGHT))
	{// マウスの移動量の取得
		D3DXVECTOR3 mouseMove = D3DXVECTOR3(pMouse->GetMouseMove().y, pMouse->GetMouseMove().x, pMouse->GetMouseMove().z);

		if (D3DXVec3Length(&mouseMove) > MIN_MOUSE_MOVED || D3DXVec3Length(&mouseMove) < -MIN_MOUSE_MOVED)
		{// マウスの移動方向のノーマライズ
			D3DXVec3Normalize(&mouseMove, &mouseMove);

			// 移動方向の算出
			rollDir = mouseMove * (D3DX_PI / 180.0f) * ROTATE_MOUSE_MOVED;
		}

		if (pMouse->GetPress(CMouse::MOUSE_KEY_LEFT))
		{// 回転タイプの更新
			nRotateType = 1;
		}
		else
		{// 回転タイプの更新
			nRotateType = 0;
		}
	}

	// 摩擦係数の計算
	m_pRoll->Moving(rollDir);

	// カメラの向きの正規化
	RotNormalization(rollDir.x);
	RotNormalization(rollDir.y);
	RotNormalization(rollDir.z);

	// 回転
	m_rot += m_pRoll->GetMove();

	if (m_rot.y < -D3DX_PI)
	{// 向きが-D3DX_PI未満の時
		m_rot.y += D3DX_PI * 2;
	}
	if (m_rot.y > D3DX_PI)
	{// 向きがD3DX_PI以上の時
		m_rot.y -= D3DX_PI * 2;
	}
	if (m_rot.x < 0.0f + 0.1f)
	{// 向きが0以下の時
		m_rot.x = 0.0f + 0.1f;
	}
	if (m_rot.x > D3DX_PI - 0.1f)
	{// 向きがD3DX_PI以上の時
		m_rot.x = D3DX_PI - 0.1f;
	}

	if (nRotateType == 0)
	{// 注視点の算出
		m_posR.z = m_posV.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
		m_posR.x = m_posV.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
		m_posR.y = m_posV.y + cosf(m_rot.x) * m_fDistance;
	}
	else
	{// 視点の算出
		m_posV.z = m_posR.z - sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
		m_posV.x = m_posR.x - sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
		m_posV.y = m_posR.y - cosf(m_rot.x) * m_fDistance;
	}
}

//=============================================================================
// カメラの移動
// Author : 唐﨑結斗
// 概要 : カメラの移動
//=============================================================================
void CCamera::Move(void)
{
	const float CAMERA_MOVE_SPEED = 0.5f;
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	if (pKeyboard->GetPress(DIK_W) == true
		|| pKeyboard->GetPress(DIK_A) == true
		|| pKeyboard->GetPress(DIK_D) == true
		|| pKeyboard->GetPress(DIK_S) == true)
	{// 移動キーが押された
	 // 前後左右移動
		if (pKeyboard->GetPress(DIK_W) == true)
		{// [W]キーが押された時
			if (pKeyboard->GetPress(DIK_A) == true)
			{// [A]キーが押された時
			 // 移動方向の更新
				m_rotMove.y = D3DX_PI * -0.25f;
				m_rotMove.x = D3DX_PI * -0.25f;
			}
			else if (pKeyboard->GetPress(DIK_D) == true)
			{// [D]キーが押された時
			 // 移動方向の更新
				m_rotMove.y = D3DX_PI * 0.25f;
				m_rotMove.x = D3DX_PI * 0.25f;
			}
			else
			{// 移動方向の更新
				m_rotMove.y = D3DX_PI * 0.0f;
				m_rotMove.x = D3DX_PI * 0.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// [S]キーが押された時
			if (pKeyboard->GetPress(DIK_A) == true)
			{// [A]キーが押された時
			 // 移動方向の更新
				m_rotMove.y = D3DX_PI * -0.75f;
				m_rotMove.x = D3DX_PI * -0.75f;
			}
			else if (pKeyboard->GetPress(DIK_D) == true)
			{// [D]キーが押された時
			 // 移動方向の更新
				m_rotMove.y = D3DX_PI * 0.75f;
				m_rotMove.x = D3DX_PI * 0.75f;
			}
			else
			{// 移動方向の更新
				m_rotMove.y = 0.0f;
				m_rotMove.x = D3DX_PI;
			}
		}
		else if (pKeyboard->GetPress(DIK_A) == true)
		{// [A]キーが押された時
		 // 移動方向の更新
			m_rotMove.y = D3DX_PI * -0.5f;
			m_rotMove.x = D3DX_PI * 0.0f;
		}
		else if (pKeyboard->GetPress(DIK_D) == true)
		{// [D]キーが押された時
		 // 移動方向の更新
			m_rotMove.y = D3DX_PI * 0.5f;
			m_rotMove.x = D3DX_PI * 0.0f;
		}

		// 移動方向の算出
		m_rotMove.x += m_rot.x;

		if (m_rotMove.x > D3DX_PI)
		{// 移動方向の正規化
			m_rotMove.x -= D3DX_PI * 2;
		}
		else if (m_rotMove.x < -D3DX_PI)
		{// 移動方向の正規化
			m_rotMove.x += D3DX_PI * 2;
		}

		m_rotMove.y += m_rot.y;

		if (m_rotMove.y > D3DX_PI)
		{// 移動方向の正規化
			m_rotMove.y -= D3DX_PI * 2;
		}
		else if (m_rotMove.y < -D3DX_PI)
		{// 移動方向の正規化
			m_rotMove.y += D3DX_PI * 2;
		}

		// 視点位置の更新
		m_posV.z += sinf(m_rotMove.x) * cosf(m_rotMove.y) * CAMERA_MOVE_SPEED;
		m_posV.x += sinf(m_rotMove.x) * sinf(m_rotMove.y) * CAMERA_MOVE_SPEED;
		m_posV.y += cosf(m_rotMove.x) * CAMERA_MOVE_SPEED;

		// 注視点の算出
		m_posR.z = m_posV.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
		m_posR.x = m_posV.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
		m_posR.y = m_posV.y + cosf(m_rot.x) * m_fDistance;
	}
}

//=============================================================================
// カメラの追従
// Author : 唐﨑結斗
// 概要 : カメラの追従
//=============================================================================
void CCamera::FollowCamera(void)
{// プレイヤー情報の取得
	CMotionPlayer3D *pMotionPlayer3D = CGame::GetMotionPlayer3D();
	D3DXVECTOR3 posPlayer = pMotionPlayer3D->GetPos();
	D3DXVECTOR3 rotPlayer = pMotionPlayer3D->GetRot();

	//// 目的の注視点の算出
	//m_posRDest.z = posPlayer.z + sinf(rotPlayer.x) * cosf(rotPlayer.y) * m_fDistance;
	//m_posRDest.x = posPlayer.x + sinf(rotPlayer.x) * sinf(rotPlayer.y) * m_fDistance;
	//m_posRDest.y = posPlayer.y + cosf(rotPlayer.x) * m_fDistance;

	m_posRDest.z = posPlayer.z + cosf(rotPlayer.y) * -10.0f;
	m_posRDest.x = posPlayer.x + sinf(rotPlayer.y) * -10.0f;
	m_posRDest.y = posPlayer.y;

	//// 目的の視点の算出
	//m_posVDest.z = posPlayer.z - sinf(rotPlayer.x) * cosf(rotPlayer.y) * m_fDistance;
	//m_posVDest.x = posPlayer.x - sinf(rotPlayer.x) * sinf(rotPlayer.y) * m_fDistance;
	//m_posVDest.y = posPlayer.y - cosf(rotPlayer.x) * m_fDistance;

	m_posVDest.z = posPlayer.z - cosf(m_rot.y) * m_fDistance;
	m_posVDest.x = posPlayer.x - sinf(m_rot.y) * m_fDistance;
	m_posVDest.y = 80.0f;

	// 注視点の移動
	m_posR += (m_posRDest - m_posR) * 0.1f;

	// 視点の移動
	m_posV += (m_posVDest - m_posV) * 0.1f;
}


