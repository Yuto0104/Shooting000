//=============================================================================
//
// アプリケーションクラス(application.h)
// Author : 唐�ｱ結斗
// 概要 : 各クラスの呼び出しを行うクラス
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "application.h"

#include <assert.h>
#include <stdio.h>

#include "renderer.h"
#include "keyboard.h"
#include "mouse.h"
#include "texture.h"
#include "sound.h"
#include "camera.h"
#include "light.h"

#include "object.h"
#include "object2D.h"
#include "player2D.h"
#include "enemy2D.h"
#include "object3D.h"
#include "model3D.h"
#include "enemy3D.h"
#include "motion_char3D.h"
#include "motion_player3D.h"
#include "mesh.h"
#include "sphere.h"
#include "circle_polygon3D.h"
#include "number.h"
#include "score.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CRenderer *CApplication::m_pRenderer = nullptr;					// レンダラーインスタンス
CKeyboard *CApplication::m_pKeyboard = {};						// キーボードインスタンス
CMouse *CApplication::m_pMouse = {};							// マウスインスタンス
CTexture *CApplication::m_pTexture = nullptr;					// テクスチャインスタンス
CSound *CApplication::m_pSound = nullptr;						// サウンドインスタンス
CCamera *CApplication::m_pCamera = nullptr;						// カメラインスタンス
CCamera *CApplication::m_pCameraBG = nullptr;					// カメラインスタンス
CPlayer2D *CApplication::m_pPlayer2D = nullptr;					// プレイヤーインタンス
CMotionPlayer3D *CApplication::m_MotionPlayer3D = nullptr;		// モーションプレイヤーインスタンス
CScore *CApplication::m_pScore = nullptr;						// スコアインスタンス

//=============================================================================
// コンストラクタ
// Author : 唐�ｱ結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CApplication::CApplication()
{

}

//=============================================================================
// デストラクタ
// Author : 唐�ｱ結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CApplication::~CApplication()
{

}

//=============================================================================
// 初期化
// Author : 唐�ｱ結斗
// 概要 : メンバ変数を解放し、他クラスのクリエイト関数を呼び出す
//=============================================================================
HRESULT CApplication::Init(HINSTANCE hInstance, HWND hWnd)
{
	// メモリの確保	
	m_pRenderer = new CRenderer;
	m_pTexture = new CTexture;
	m_pSound = new CSound;
	m_pCamera = new CCamera;
	m_pCameraBG = new CCamera;
	// 入力デバイスのメモリ確保
	m_pKeyboard = new CKeyboard;
	m_pMouse = new CMouse;

	// 初期化処理
	assert(m_pRenderer != nullptr);
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)) != 0)
	{//初期化処理が失敗した場合
		return-1;
	}

	// 初期化処理
	assert(m_pTexture != nullptr);
	m_pTexture->Init();

	// 初期化処理
	assert(m_pSound != nullptr);
	m_pSound->Init(hWnd);

	// 初期化処理
	assert(m_pCamera != nullptr);
	m_pCamera->Init();
	m_pCamera->SetViewType(CCamera::TYPE_PARALLEL);

	// 初期化処理
	assert(m_pCameraBG != nullptr);
	m_pCameraBG->Init();

	// 初期化処理
	assert(m_pKeyboard != nullptr);
	if (FAILED(m_pKeyboard->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// 初期化処理
	assert(m_pMouse != nullptr);
	if (FAILED(m_pMouse->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// ライトの作成
	CLight::Create(D3DXVECTOR3(0.2f, -0.8f, -0.4f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CLight::Create(D3DXVECTOR3(0.2f, 0.8f, -0.4f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CLight::Create(D3DXVECTOR3(-0.2f, -0.5f, 0.7f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// オブジェクトの生成
	//CObject2D::Create()->SetPos(D3DXVECTOR3(50.0f, 90.0f, 0.0f));
	//CObject2D::Create()->SetPos(D3DXVECTOR3(700.0f, 600.0f, 0.0f));
	//CEnemy2D::Create(D3DXVECTOR3(800.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f);
	//CEnemy2D::Create(D3DXVECTOR3(700.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f);
	//CEnemy2D::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f);

	/*CObject3D *pObject3D = CObject3D::Create();
	pObject3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pObject3D->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));

	pObject3D = CObject3D::Create();
	pObject3D->SetPos(D3DXVECTOR3(-500.0f, 0.0f, 0.0f));
	pObject3D->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	pObject3D->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
	pObject3D->SetBillboard(true);*/

	/*CModel3D *pModel3D = CModel3D::Create("data/MODEL/airplane000.x");
	pModel3D->SetPos(D3DXVECTOR3(100.0f, 0.0f, 0.0f));
	pModel3D->SetSize(D3DXVECTOR3(5.0f, 5.0f, 5.0f));*/

	CEnemy3D *pEnemy = CEnemy3D::Create("data/MODEL/airplane000.x");
	pEnemy->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pEnemy->SetSize(D3DXVECTOR3(5.0f, 5.0f, 5.0f));
	pEnemy->SetColorType(CObject::TYPE_WHITE);

	CMotionChar3D *pMotionChar3D = CMotionChar3D::Create("data/MOTION/motion.txt");
	pMotionChar3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pMotionChar3D->SetObjectDrowType(CObject::DROWTYPE_BG);

	m_MotionPlayer3D = CMotionPlayer3D::Create();
	m_MotionPlayer3D->SetPos(D3DXVECTOR3(40.0f, 0.0f, -50.0f));
	m_MotionPlayer3D->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

	CMesh3D *pMesh3D = CMesh3D::Create();
	pMesh3D->SetSize(D3DXVECTOR3(2000.0f, 0, 2000.0f));
	pMesh3D->SetBlock(CMesh3D::DOUBLE_INT(100, 100));
	pMesh3D->SetSplitTex(true);
	pMesh3D->SetObjectDrowType(CObject::DROWTYPE_BG);

	CSphere *pSphere = CSphere::Create();
	pSphere->SetRot(D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));
	pSphere->SetSize(D3DXVECTOR3(100.0f, 0, 100.0f));
	pSphere->SetBlock(CMesh3D::DOUBLE_INT(100, 100));
	//pSphere->SetSplitTex(true);
	pSphere->SetRadius(1000.0f);
	pSphere->SetSphereRange(D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * -0.35f));
	pSphere->SetObjectDrowType(CObject::DROWTYPE_BG);

	/*CCirclePolygon3D *pCirclePolygon3D = CCirclePolygon3D::Create();
	pCirclePolygon3D->SetPos(D3DXVECTOR3(0.0f, 10.0f, 0.0f));
	pCirclePolygon3D->SetObjectDrowType(CObject::DROWTYPE_BG);*/

	m_pScore = CScore::Create(10);
	m_pScore->SetScore(0);
	m_pScore->SetPos(D3DXVECTOR3(1280.0f, m_pScore->GetSize().y / 2.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐�ｱ結斗
// 概要 : メモリの解放とリリース関数を呼び出す
//=============================================================================
void CApplication::Uninit()
{
	if (m_pRenderer != nullptr)
	{// 終了処理
		m_pRenderer->Uninit();

		// メモリの解放
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	if (m_pKeyboard != nullptr)
	{// 終了処理
		m_pKeyboard->Uninit();

		// メモリの解放
		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}

	if (m_pMouse != nullptr)
	{// 終了処理
		m_pMouse->Uninit();

		// メモリの解放
		delete m_pMouse;
		m_pMouse = nullptr;
	}

	if (m_pTexture != nullptr)
	{// 終了処理
		m_pTexture->Uninit();

		// メモリの解放
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	if (m_pSound != nullptr)
	{// 終了処理
		m_pSound->Uninit();

		// メモリの解放
		delete m_pSound;
		m_pSound = nullptr;
	}

	if (m_pCamera != nullptr)
	{// 終了処理
		m_pCamera->Uninit();

		// メモリの解放
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if (m_pCameraBG != nullptr)
	{// 終了処理
		m_pCameraBG->Uninit();

		// メモリの解放
		delete m_pCameraBG;
		m_pCameraBG = nullptr;
	}

	// ライトの解放
	CLight::ReleaseAll();

	// オブジェクトの解放
	CObject::ReleaseAll();
}

//=============================================================================
// 更新
// Author : 唐�ｱ結斗
// 概要 : 更新
//=============================================================================
void CApplication::Update()
{
	m_pKeyboard->Update();
	m_pMouse->Update();
	m_pCamera->Update();
	m_pCameraBG->Update();
	m_pRenderer->Update();
}

//=============================================================================
// 描画
// Author : 唐�ｱ結斗
// 概要 : 描画
//=============================================================================
void CApplication::Draw()
{
	m_pRenderer->Draw();
}
