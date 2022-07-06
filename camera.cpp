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

//*****************************************************************************
// 定数定義
//*****************************************************************************
const float CCamera::CAMERA_NEAR = (1.0f);				// ニア
const float CCamera::CAMERA_FUR = (1000.0f);			// ファー

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CCamera::CCamera()
{
	m_quaternion = {};									// クオータニオン
	m_mtxWorld = {};									// ワールドマトリックス
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 注視点
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 上方向ベクトル
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_rotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動方向
	m_quaternion = D3DXQUATERNION(0.0f,0.0f,0.0f,1.0f);
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
	m_posV = D3DXVECTOR3(0.0f, 0.0f, -150.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// 固定
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 視点と注視点の距離
	D3DXVECTOR3 posDiss = m_posR - m_posV;
	m_fDistance = sqrtf((posDiss.y * posDiss.y) + (posDiss.x * posDiss.x) + (posDiss.z * posDiss.z));

	// 角度の算出
	m_rot.y = atan2f(posDiss.x, posDiss.z);
	m_rot.x = atan2f(sqrtf((posDiss.x * posDiss.x) + (posDiss.z * posDiss.z)), posDiss.y);
	m_rot.z = 0.5f;

	// メモリの確保
	m_pRoll = new CMove;
	assert(m_pRoll != nullptr);
	m_pRoll->SetMoving(2.0f, 10.0f, 0.0f, 0.2f);

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
	Rotate();
	Move();
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

	D3DXMATRIX mtxRot , mtxTrans;

	// クォータニオンの使用した姿勢の設定
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);	// クオータニオンによる行列回転
	D3DXMatrixMultiply(&m_mtxView, &m_mtxView, &mtxRot);// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_posV.x, m_posV.y, m_posV.z);			// 行列移動関数
	D3DXMatrixMultiply(&m_mtxView, &m_mtxView, &mtxTrans);					// 行列掛け算関数

	// 逆行列に計算
	D3DXMatrixInverse(&m_mtxView, NULL, &m_mtxView);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProj);			// 行列初期化関数

	switch (m_viewType)
	{
	case TYPE_CLAIRVOYANCE:
		// プロジェクションマトリックスの作成(透視投影)
		D3DXMatrixPerspectiveFovLH(&m_mtxProj,					// プロジェクションマトリックス
			D3DXToRadian(45.0f),								// 視野角
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,			// アスペクト比
			CAMERA_NEAR,										// ニア
			CAMERA_FUR);										// ファー
		break;

	case TYPE_PARALLEL:
		// プロジェクションマトリックスの作成(平行投影)
		D3DXMatrixOrthoLH(&m_mtxProj,							// プロジェクションマトリックス
			(float)SCREEN_WIDTH,								// 幅
			(float)SCREEN_HEIGHT,								// 高さ
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

//D3DXMATRIX CCamera::transformQuaternionToRotMat(D3DXQUATERNION inQuaternion)
//{
//	return D3DXMATRIX();
//}

//=============================================================================
// カメラの回転
// Author : 唐﨑結斗
// Author : YudaKaito
// 概要 : 
//=============================================================================
void CCamera::Rotate(void)
{
	// 入力情報の取得
	const float MIN_MOUSE_MOVED = 5.0f;
	const float ROTATE_MOUSE_MOVED = 0.45f;
	static D3DXVECTOR3 rotVec = {};

	CMouse *pMouse = CApplication::GetMouse();
	D3DXVECTOR3 rollDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nRotateType = -1;

	if (pMouse->GetPress(CMouse::MOUSE_KEY_LEFT))
	{// マウスの移動量の取得
		D3DXVECTOR3 mouseMove = pMouse->GetMouseMove();
		D3DXVECTOR3 vecY = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		if (mouseMove.x > MIN_MOUSE_MOVED || mouseMove.x < -MIN_MOUSE_MOVED)
		{// マウスが一定距離(X)移動したとき
			if (mouseMove.x > MIN_MOUSE_MOVED)
			{
				rollDir.y += (D3DX_PI / 180.0f) * ROTATE_MOUSE_MOVED;
			}
			if (mouseMove.x < -MIN_MOUSE_MOVED)
			{
				rollDir.y += (D3DX_PI / 180.0f) * -ROTATE_MOUSE_MOVED;
			}
			vecY = D3DXVECTOR3(0.0f, 0.0f, 1.0f);		// y軸
		}

		if (mouseMove.y > MIN_MOUSE_MOVED || mouseMove.y < -MIN_MOUSE_MOVED)
		{// マウスが一定距離(Y)移動したとき
			if (mouseMove.y > MIN_MOUSE_MOVED)
			{
				rollDir.x += (D3DX_PI / 180.0f) * ROTATE_MOUSE_MOVED;
			}
			if (mouseMove.y < -MIN_MOUSE_MOVED)
			{
				rollDir.x += (D3DX_PI / 180.0f) * -ROTATE_MOUSE_MOVED;
			}
			vecY = D3DXVECTOR3(0.0f, 0.0f, 1.0f);		// y軸
		}
		// 回転軸の取得
		m_axisVec.y = -rollDir.x;
		m_axisVec.x = rollDir.y;

		// 回転タイプの更新
		nRotateType = 1;

		if (D3DXVec3Length(&m_axisVec) != 0.0f)
		{
			D3DXVECTOR3 axis;									// 回転軸
			D3DXVECTOR3 inverseVec = -m_axisVec;				// rot値を反対にする
			D3DXVec3Cross(&axis, &inverseVec, &vecY);			// 外積で回転軸を算出。

			// クオータニオンの計算
			D3DXQUATERNION quaternion;
			D3DXQuaternionRotationAxis(&quaternion, &axis, 0.1f);    // 回転軸と回転角度を指定

			// クオータニオンを適用
			m_quaternion *= quaternion;

			// クオータニオンのノーマライズ
			D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);
		}

		D3DXMATRIX mtxRot;
		// クォータニオンの使用した姿勢の設定
		D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);	// クオータニオンによる行列回転

		m_posR.z = m_posV.z + sinf(mtxRot._42) * cosf(mtxRot._41) * m_fDistance;
		m_posR.x = m_posV.x + sinf(mtxRot._42) * sinf(mtxRot._41) * m_fDistance;
		m_posR.y = m_posV.y + cosf(mtxRot._42) * m_fDistance;
	}
	else if (pMouse->GetPress(CMouse::MOUSE_KEY_RIGHT))
	{// マウスの移動量の取得
		D3DXVECTOR3 mouseMove = pMouse->GetMouseMove();

		if (mouseMove.x > MIN_MOUSE_MOVED || mouseMove.x < -MIN_MOUSE_MOVED)
		{// マウスが一定距離(X)移動したとき
			if (mouseMove.x > MIN_MOUSE_MOVED)
			{
				rollDir.y += (D3DX_PI / 180.0f) * ROTATE_MOUSE_MOVED;
			}
			if (mouseMove.x < -MIN_MOUSE_MOVED)
			{
				rollDir.y += (D3DX_PI / 180.0f) * -ROTATE_MOUSE_MOVED;
			}
		}

		if (mouseMove.y > MIN_MOUSE_MOVED || mouseMove.y < -MIN_MOUSE_MOVED)
		{// マウスが一定距離(Y)移動したとき
			if (mouseMove.y > MIN_MOUSE_MOVED)
			{
				rollDir.x += (D3DX_PI / 180.0f) * ROTATE_MOUSE_MOVED;
			}
			if (mouseMove.y < -MIN_MOUSE_MOVED)
			{
				rollDir.x += (D3DX_PI / 180.0f) * -ROTATE_MOUSE_MOVED;
			}
		}
		// 回転軸の取得
		m_axisVec.y = -rollDir.x;
		m_axisVec.x = rollDir.y;

		// 回転タイプの更新
		nRotateType = 0;

		if (D3DXVec3Length(&m_axisVec) != 0.0f)
		{
			D3DXVECTOR3 axis;										// 回転軸
			D3DXVECTOR3 inverseVec = -m_axisVec;					// rot値を反対にする
			D3DXVECTOR3 vecY = D3DXVECTOR3(0.0f, 0.0f, 1.0f);		// y軸
			D3DXVec3Cross(&axis, &inverseVec, &vecY);				// 外積で回転軸を算出。

			// クオータニオンの計算
			D3DXQUATERNION quaternion;
			D3DXQuaternionRotationAxis(&quaternion, &axis, -0.01f);    // 回転軸と回転角度を指定

			  // クオータニオンを適用
			m_quaternion *= quaternion;

			// クオータニオンのノーマライズ
			D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);
		}

		D3DXMATRIX mtxRot;
		// クォータニオンの使用した姿勢の設定
		D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);	// クオータニオンによる行列回転
		
		m_posV.z = m_posR.z - sinf(mtxRot._42) * cosf(mtxRot._41) * m_fDistance;
		m_posV.x = m_posR.x - sinf(mtxRot._42) * sinf(mtxRot._41) * m_fDistance;
		m_posV.y = m_posR.y - cosf(mtxRot._42) * m_fDistance;
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

	if (m_rot.x< -D3DX_PI)
	{// 向きが-D3DX_PI未満の時
		m_rot.x += D3DX_PI * 2;
	}
	if (m_rot.x > D3DX_PI)
	{// 向きがD3DX_PI以上の時
		m_rot.x -= D3DX_PI * 2;
	}
}

void CCamera::Move(void)
{
	const float CAMERA_MOVE_SPEED = 0.5f;
	CKeyboard *pKeyboard = CApplication::GetKeyboard();
	m_rotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (pKeyboard->GetPress(DIK_W)
		|| pKeyboard->GetPress(DIK_A)
		|| pKeyboard->GetPress(DIK_D)
		|| pKeyboard->GetPress(DIK_S))
	{// 移動キーが押された
	 // 前後左右移動
		if (pKeyboard->GetPress(DIK_W))
		{// [W]キーが押された時
			if (pKeyboard->GetPress(DIK_A))
			{// [A]キーが押された時
				// 移動方向の更新
				m_rotMove.y = D3DX_PI * -0.25f;
				m_rotMove.x = D3DX_PI * -0.25f;
			}
			else if (pKeyboard->GetPress(DIK_D))
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
		else if (pKeyboard->GetPress(DIK_S))
		{// [S]キーが押された時
			if (pKeyboard->GetPress(DIK_A))
			{// [A]キーが押された時
				// 移動方向の更新
				m_rotMove.y = D3DX_PI * -0.75f;
				m_rotMove.x = D3DX_PI * -0.75f;
			}
			else if (pKeyboard->GetPress(DIK_D))
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
		else if (pKeyboard->GetPress(DIK_A))
		{// [A]キーが押された時
			// 移動方向の更新
			m_rotMove.y = D3DX_PI * -0.5f;
			m_rotMove.x = D3DX_PI * 0.0f;
		}
		else if (pKeyboard->GetPress(DIK_D))
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


