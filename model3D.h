//=============================================================================
//
// 3Dモデルクラス(model3D.h)
// Author : 唐�ｱ結斗
// 概要 : 3Dモデル生成を行う
//
//=============================================================================
#ifndef _MODEL_H_			// このマクロ定義がされてなかったら
#define _MODEL_H_			// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"
#include "main.h"

//=============================================================================
// 3Dモデルクラス
// Author : 唐�ｱ結斗
// 概要 : 3Dモデル生成を行うクラス
//=============================================================================
class CModel3D : public CObject
{
public:
	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CModel3D *Create(char *pName);			// 3Dモデルの生成

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CModel3D();
	~CModel3D();

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	HRESULT Init() override;														// 初期化
	HRESULT Init(char *pName);														// 初期化
	void Uninit() override;															// 終了
	void Update() override;															// 更新
	void Draw() override;															// 描画
	void SetPos(const D3DXVECTOR3 &pos) override;									// 位置のセッター
	void SetPosOld(const D3DXVECTOR3 &posOld) override { m_posOld = posOld; }		// 過去位置のセッター
	void SetRot(const D3DXVECTOR3 &rot) override;									// 向きのセッター
	void SetSize(const D3DXVECTOR3 &size) override;									// 大きさのセッター
	D3DXVECTOR3 GetPos() override { return m_pos; }									// 位置のゲッター
	D3DXVECTOR3 GetPosOld()  override { return m_posOld; }							// 過去位置のゲッター
	D3DXVECTOR3 GetRot()  override { return m_rot; }								// 向きのゲッター
	D3DXVECTOR3 GetSize()  override { return m_size; }								// 大きさのゲッター
	D3DXMATRIX GetMtxWorld() { return m_mtxWorld; }							// ワールドマトリックスの取得
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; }		// ワールドマトリックスの設定

private:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	LPD3DXMESH		 m_pMesh;			// メッシュ情報へのポインタ
	LPD3DXBUFFER	 m_pBuffer;			// マテリアル情報へのポインタ
	DWORD			 m_nNumMat;			// マテリアル情報の数
	D3DXMATRIX		 m_mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3		 m_pos;				// 位置
	D3DXVECTOR3		 m_posOld;			// 過去位置
	D3DXVECTOR3		 m_rot;				// 向き
	D3DXVECTOR3		 m_size;			// 大きさ
};

#endif