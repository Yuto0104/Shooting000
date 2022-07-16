//=============================================================================
//
// オブジェクトクラス(object.h)
// Author : 唐﨑結斗
// 概要 : オブジェクト生成を行う
//
//=============================================================================
#ifndef _OBJECT_H_		// このマクロ定義がされてなかったら
#define _OBJECT_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// オブジェクトの最大生成可能数
#define MAX_OBJECT (0x1000)

//=============================================================================
// オブジェクトクラス
// Author : 唐﨑結斗
// 概要 : オブジェクト生成を行うクラス
//=============================================================================
class CObject
{
public:
	//--------------------------------------------------------------------
	// 色の種別設定
	// Author : 唐﨑結斗
	// 概要 : 白か黒かを分ける種別
	//--------------------------------------------------------------------
	enum COLOR_TYPE
	{
		TYPE_NONE = 0,		// 種別無し
		TYPE_WHITE,			// 白
		TYPE_BLACK,			// 黒
		MAX_TYPE,			// 種別の最大
	};

	//--------------------------------------------------------------------
	// オブジェクトの種別の列挙型
	// Author : 唐﨑結斗
	// 概要 : オブジェクトの種別情報を格納
	//--------------------------------------------------------------------
	enum EObjectType
	{
		OBJTYPE_NONE = 0,			// タイプ無し
		OBJTYPE_2DPOLYGON,			// 2Dポリゴン
		OBJTYPE_2DPLAYER,			// プレイヤー
		OBJTYPE_2DBULLET,			// バレット
		OBJTYPE_2DENEMY,			// エネミー
		OBJTYPE_2DEXPLOSION,		// 爆発
		MAX_OBJTYPE,				// 種別の最大数
	};

	//--------------------------------------------------------------------
	// オブジェクトの描画方法の列挙型
	// Author : 唐﨑結斗
	// 概要 : オブジェクトの描画方法情報を格納
	//--------------------------------------------------------------------
	enum EObjectDrowType
	{
		DROWTYPE_GAME = 0,			// ゲーム描画
		DROWTYPE_BG,				// 背景描画
		MAX_DROWTYPE,				// 種別の最大数
	};

	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static void ReleaseAll(void);									// すべてのオブジェクトの解放
	static void UpdateAll(void);									// すべてのオブジェクトの更新
	static void DrawAll(EObjectDrowType objectDrowType);			// すべてのオブジェクトの描画
	static CObject **GetObjectAll() { return &m_apObject[0]; }		// オブジェクト情報のゲッター

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CObject();
	virtual ~CObject();

	//--------------------------------------------------------------------
	// 純粋仮想関数
	//--------------------------------------------------------------------
	virtual HRESULT Init() = 0;																			// 初期化
	virtual void Uninit() = 0;																			// 終了
	virtual void Update() = 0;																			// 更新
	virtual void Draw() = 0;																			// 描画
	virtual void SetPos(const D3DXVECTOR3 &pos) = 0;													// 位置のセッター
	virtual void SetPosOld(const D3DXVECTOR3 &posOld) = 0;												// 過去位置のセッタ
	virtual void SetRot(const D3DXVECTOR3 &rot) = 0;													// 向きのセッター
	virtual void SetSize(const D3DXVECTOR3 &size) = 0;													// 大きさのセッター
	virtual D3DXVECTOR3 GetPos() = 0;																	// 位置のゲッター
	virtual D3DXVECTOR3 GetPosOld() = 0;																// 過去位置のゲッタ
	virtual D3DXVECTOR3 GetRot() = 0;																	// 向きのゲッター
	virtual D3DXVECTOR3 GetSize() = 0;																	// 大きさのゲッター
	void SetObjType(EObjectType objectType) { m_objectType = objectType; }								// オブジェクトの種別設定
	EObjectType GetObjType() { return m_objectType; }													// オブジェクトの種別設定
	void SetObjectDrowType(EObjectDrowType objectDrowType) { m_objectDrowType = objectDrowType; }		// オブジェクトの描画方法の設定
	EObjectDrowType GetObjectDrowType() { return m_objectDrowType; }									// オブジェクトの描画方法の設定
	void SetColorType(COLOR_TYPE colorType) { m_colorType = colorType; }								// 色タイプの設定
	COLOR_TYPE GetColorType() { return m_colorType; }													// 色タイプの設定

protected:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void Release(void);											// オブジェクトの解放
	bool ColisonRange2D(CObject *target);						// 範囲の判定(2D)
	bool ColisonRectangle2D(CObject *target,bool bExtrude);		// 矩形の判定(2D)
	bool ColisonCircle2D(CObject *target, bool bExtrude);		// 円の判定(2D)

private:
	//--------------------------------------------------------------------
	// 静的メンバ変数
	//--------------------------------------------------------------------
	static CObject *m_apObject[MAX_OBJECT];				// オブジェクト情報

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	EObjectType			m_objectType;		// オブジェクトの種別
	EObjectDrowType		m_objectDrowType;	// オブジェクトの描画方法
	COLOR_TYPE			m_colorType;		// 色の種別
	int					m_nNumID;			// 格納先の番号
};

#endif

