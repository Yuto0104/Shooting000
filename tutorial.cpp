//=============================================================================
//
// チュートリアルクラス(tutorial.cpp)
// Author : 唐﨑結斗
// 概要 : チュートリアルクラスの管理を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "tutorial.h"

#include "keyboard.h"
#include "sound.h"

#include "application.h"
#include "model_manager.h"
#include "enemy_manager.h"
#include "camera_manager.h"
#include "renderer.h"
#include "object.h"
#include "object3D.h"
#include "model3D.h"
#include "enemy3D.h"
#include "motion_char3D.h"
#include "motion_player3D.h"
#include "mesh.h"
#include "sphere.h"
#include "number.h"
#include "score.h"
#include "life.h"
#include "life_manager.h"
#include "energy_gage.h"
#include "particle.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CMotionPlayer3D *CTutorial::m_MotionPlayer3D = nullptr;		// モーションプレイヤーインスタンス
CScore *CTutorial::m_pScore = nullptr;						// スコアインスタンス
CLifeManager *CTutorial::m_pLifeManager = nullptr;			// ライフマネージャーインスタンス
CEnergyGage *CTutorial::m_pEnergyGage = nullptr;			// エネルギーゲージマネージャー
bool CTutorial::m_bUsePlayer = false;						// プレイヤーを使用しているか

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CTutorial::CTutorial()
{

}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CTutorial::Init()
{
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_BGM003);

	// 背景モデルの設置
	CApplication::GetModelManager()->SetModelBG();

	// カメラマネージャーのインスタンス取得
	CCameraManager *pCameraManager = CApplication::GetCameraManager();
	CApplication::GetCameraBG()->MotionReset();
	CApplication::GetCameraBG()->SetCamera(pCameraManager->GetPosV(), pCameraManager->GetPosR(), pCameraManager->GetRot());
	CApplication::GetCameraBG()->SetNumMotion(0);

	CMotionChar3D *pMotionChar3D = CMotionChar3D::Create("data/MOTION/motion.txt");
	pMotionChar3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pMotionChar3D->SetObjectDrowType(CObject::DROWTYPE_BG);

	m_MotionPlayer3D = CMotionPlayer3D::Create();
	m_MotionPlayer3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, -50.0f));
	m_MotionPlayer3D->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	m_bUsePlayer = true;

	CMesh3D *pMesh3D = CMesh3D::Create();
	pMesh3D->SetRot(D3DXVECTOR3(D3DX_PI * -0.15f, 0.0f, 0.0f));
	pMesh3D->SetSize(D3DXVECTOR3(100000.0f, 0, 100000.0f));
	pMesh3D->SetBlock(CMesh3D::DOUBLE_INT(100, 100));
	pMesh3D->SetSplitTex(true);
	pMesh3D->SetObjectDrowType(CObject::DROWTYPE_BG);
	pMesh3D->LoadTex(13);
	pMesh3D->SetScrollTex(D3DXVECTOR2(-0.08f, -0.06f), true);

	CSphere *pSphere = CSphere::Create();
	pSphere->SetRot(D3DXVECTOR3(D3DX_PI * 0.85f, 0.0f, 0.0f));
	pSphere->SetSize(D3DXVECTOR3(100.0f, 0, 100.0f));
	pSphere->SetBlock(CMesh3D::DOUBLE_INT(100, 100));
	pSphere->SetRadius(50000.0f);
	pSphere->SetSphereRange(D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * -0.5f));
	pSphere->SetObjectDrowType(CObject::DROWTYPE_BG);
	pSphere->SetScrollTex(D3DXVECTOR2(0.005f, 0.0f), true);
	pSphere->LoadTex(12);

	m_pScore = CScore::Create(10, false);
	m_pScore->SetScore(0);
	m_pScore->SetPos(D3DXVECTOR3(1280.0f, m_pScore->GetSize().y / 2.0f, 0.0f));

	m_pLifeManager = CLifeManager::Create();
	m_pLifeManager->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
	m_pLifeManager->SetPos(D3DXVECTOR3(0.0f, m_pLifeManager->GetSize().y / 2.0f, 0.0f));

	m_pEnergyGage = CEnergyGage::Create();
	m_pEnergyGage->SetPos(D3DXVECTOR3(0.0f + m_pEnergyGage->GetSize().x / 2.0f + 20.0f, 450.0f, 0.0f));

	CObject2D *pObject2D = CObject2D::Create();
	pObject2D->SetPos(D3DXVECTOR3(1100.0f, 400.0f, 0.0f));
	pObject2D->SetSize(D3DXVECTOR3(400.0f, 550.0f, 0.0f));
	pObject2D->SetCol(D3DXCOLOR(0.25f, 0.1f, 0.8f, 1.0f));
	pObject2D->LoadTex(22);

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CTutorial::Uninit()
{
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// サウンド終了
	pSound->StopSound();

	m_pEnergyGage->Uninit();

	// スコアの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CTutorial::Update()
{
	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	if (CEnemy3D::GetCntSetEnemy() <= 0)
	{
		CEnemy3D *pEnemy = nullptr;

		pEnemy = CEnemy3D::Create(1);
		pEnemy->SetPos(D3DXVECTOR3(200.0f, 0.0f, 200.0f));
		pEnemy->SetSize(D3DXVECTOR3(2.0f, 2.0f, 2.0f));
		pEnemy->SetLife(100);
		pEnemy->SetShotMode(CEnemy3D::SHOTMODE_NORMAL);
		pEnemy->SetBulletRot(0.0f);
		pEnemy->SetBulletSpeed(10.0f);
		pEnemy->SetMaxShot(60);
		pEnemy->SetColorType(CObject::TYPE_WHITE);

		pEnemy = CEnemy3D::Create(0);
		pEnemy->SetPos(D3DXVECTOR3(-200.0f, 0.0f, 200.0f));
		pEnemy->SetSize(D3DXVECTOR3(2.0f, 2.0f, 2.0f));
		pEnemy->SetLife(100);
		pEnemy->SetShotMode(CEnemy3D::SHOTMODE_NORMAL);
		pEnemy->SetBulletRot(0.0f);
		pEnemy->SetBulletSpeed(10.0f);
		pEnemy->SetMaxShot(60);
		pEnemy->SetColorType(CObject::TYPE_BLACK);
	}

	if (pKeyboard->GetTrigger(DIK_G))
	{
		CApplication::SetNextMode(CApplication::MODE_TITLE);
	}
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CTutorial::Draw()
{

}

