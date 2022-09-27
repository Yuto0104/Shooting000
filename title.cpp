//=============================================================================
//
// タイトルクラス(title.cpp)
// Author : 唐﨑結斗
// 概要 : タイトルクラスの管理を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "title.h"

#include "application.h"
#include "keyboard.h"
#include "sound.h"
#include "object2D.h"
#include "model_manager.h"
#include "camera_manager.h"
#include "mesh.h"
#include "sphere.h"
#include "bg.h"

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CTitle::CTitle()
{
	m_pTitleLogo = nullptr;				// タイトルロゴオブジェクト
	m_pNewGame = nullptr;				// ニューゲームオブジェクト
	m_pTutorial = nullptr;				// チュートリアルオブジェクト
	m_pExit = nullptr;					// 終了オブジェクト
	m_nCntFrame = 0;					// フレームカウント
	m_bPressEnter = true;				// エンターキーを押せるか
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CTitle::Init()
{
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_BGM000);

	m_pTitleLogo = CObject2D::Create();
	m_pTitleLogo->SetPos(D3DXVECTOR3(640.0f, 280.0f, 0.0f));
	m_pTitleLogo->SetSize(D3DXVECTOR3(200.0f, 400.0f, 0.0f));
	m_pTitleLogo->SetCol(D3DXCOLOR(0.25f, 0.1f, 0.8f, 1.0f));
	m_pTitleLogo->LoadTex(16);

	m_pNewGame = CObject2D::Create();
	m_pNewGame->SetPos(D3DXVECTOR3(640.0f, 550.0f, 0.0f));
	m_pNewGame->SetSize(D3DXVECTOR3(300.0f, 60.0f, 0.0f));
	m_pNewGame->SetCol(D3DXCOLOR(0.25f, 0.1f, 0.8f, 1.0f));
	m_pNewGame->LoadTex(19);
	
	m_pTutorial = CObject2D::Create();
	m_pTutorial->SetPos(D3DXVECTOR3(640.0f, 610.0f, 0.0f));
	m_pTutorial->SetSize(D3DXVECTOR3(280.0f, 50.0f, 0.0f));
	m_pTutorial->SetCol(D3DXCOLOR(0.25f, 0.1f, 0.8f, 1.0f));
	m_pTutorial->LoadTex(23);

	m_pExit = CObject2D::Create();
	m_pExit->SetPos(D3DXVECTOR3(640.0f, 670.0f, 0.0f));
	m_pExit->SetSize(D3DXVECTOR3(170.0f, 50.0f, 0.0f));
	m_pExit->SetCol(D3DXCOLOR(0.25f, 0.1f, 0.8f, 1.0f));
	m_pExit->LoadTex(24);

	// 背景モデルの設置
	CApplication::GetModelManager()->SetModelBG();

	// カメラマネージャーのインスタンス取得
	CCameraManager *pCameraManager = CApplication::GetCameraManager();
	CApplication::GetCameraBG()->MotionReset();
	CApplication::GetCameraBG()->SetCamera(pCameraManager->GetPosV(), pCameraManager->GetPosR(), pCameraManager->GetRot());
	CApplication::GetCameraBG()->SetNumMotion(1);

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

	//CBG *pBG = CBG::Create();
	//pBG->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CTitle::Uninit()
{
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// サウンド終了
	pSound->StopSound();

	// スコアの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CTitle::Update()
{
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	if (m_bPressEnter)
	{
		SelectMode();
	}

	FlashObj();

	if (m_bPressEnter
		&& pKeyboard->GetTrigger(DIK_RETURN))
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
		m_bPressEnter = false;
	}

	if (!m_bPressEnter
		&& m_nCntFrame >= 40)
	{
		switch (m_nextMode)
		{
		case MODE_GAME:
			CApplication::SetNextMode(CApplication::MODE_GAME);
			break;

		case MODE_TUTORIAL:
			CApplication::SetNextMode(CApplication::MODE_TUTORIAL);
			break;

		case MODE_EXIT:
			// ウィンドウを破棄
			DestroyWindow(CApplication::GetWnd());
			break;

		default:
			assert(false);
			break;
		}
	}
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CTitle::Draw()
{

}

//=============================================================================
// オブジェクトの点滅
// Author : 唐﨑結斗
// 概要 : 指定のオブジェクトを点滅させる
//=============================================================================
void CTitle::FlashObj()
{
	CObject2D *pObj = nullptr;

	if (m_bPressEnter)
	{
		m_fAddAlpha += 0.07f;
	}
	else if (!m_bPressEnter)
	{
		m_fAddAlpha += 0.5f;
		m_nCntFrame++;
	}

	switch (m_nextMode)
	{
	case MODE_GAME:
		pObj = m_pNewGame;
		m_pTutorial->SetCol(D3DXCOLOR(0.3f, 0.1f, 1.0f, 1.0f));
		m_pExit->SetCol(D3DXCOLOR(0.3f, 0.1f, 1.0f, 1.0f));
		break;

	case MODE_TUTORIAL:
		pObj = m_pTutorial;
		m_pNewGame->SetCol(D3DXCOLOR(0.3f, 0.1f, 1.0f, 1.0f));
		m_pExit->SetCol(D3DXCOLOR(0.3f, 0.1f, 1.0f, 1.0f));
		break;

	case MODE_EXIT:
		pObj = m_pExit;
		m_pTutorial->SetCol(D3DXCOLOR(0.3f, 0.1f, 1.0f, 1.0f));
		m_pNewGame->SetCol(D3DXCOLOR(0.3f, 0.1f, 1.0f, 1.0f));
		break;

	default:
		assert(false);
		break;
	}

	pObj->SetCol(D3DXCOLOR(0.25f, 0.1f, 0.8f, sinf(m_fAddAlpha) * 3.0f));
}

//=============================================================================
// モードの選択
// Author : 唐﨑結斗
// 概要 : モードの選択する
//=============================================================================
void CTitle::SelectMode()
{
	int nMode = (int)m_nextMode;

	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	if (pKeyboard->GetTrigger(DIK_W))
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
		nMode--;

		if (nMode < 0)
		{
			nMode = 2;
		}
	}
	else if (pKeyboard->GetTrigger(DIK_S))
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
		nMode++;

		if (nMode > 2)
		{
			nMode = 0;
		}
	}

	m_nextMode = (NEXT_MODE)nMode;
}
