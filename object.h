//=============================================================================
//
// �I�u�W�F�N�g�N���X(object.h)
// Author : �������l
// �T�v : �I�u�W�F�N�g�������s��
//
//=============================================================================
#ifndef _OBJECT_H_		// ���̃}�N����`������ĂȂ�������
#define _OBJECT_H_		// ��d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �I�u�W�F�N�g�̍ő吶���\��
#define MAX_OBJECT (0x1000)

//=============================================================================
// �I�u�W�F�N�g�N���X
// Author : �������l
// �T�v : �I�u�W�F�N�g�������s���N���X
//=============================================================================
class CObject
{
public:
	//--------------------------------------------------------------------
	// �F�̎�ʐݒ�
	// Author : �������l
	// �T�v : ���������𕪂�����
	//--------------------------------------------------------------------
	enum COLOR_TYPE
	{
		TYPE_NONE = 0,		// ��ʖ���
		TYPE_WHITE,			// ��
		TYPE_BLACK,			// ��
		MAX_TYPE,			// ��ʂ̍ő�
	};

	//--------------------------------------------------------------------
	// �I�u�W�F�N�g�̎�ʂ̗񋓌^
	// Author : �������l
	// �T�v : �I�u�W�F�N�g�̎�ʏ����i�[
	//--------------------------------------------------------------------
	enum EObjectType
	{
		OBJTYPE_NONE = 0,			// �^�C�v����
		OBJTYPE_2DPOLYGON,			// 2D�|���S��
		OBJTYPE_2DPLAYER,			// �v���C���[
		OBJTYPE_2DBULLET,			// �o���b�g
		OBJTYPE_2DENEMY,			// �G�l�~�[
		OBJTYPE_2DEXPLOSION,		// ����
		MAX_OBJTYPE,				// ��ʂ̍ő吔
	};

	//--------------------------------------------------------------------
	// �I�u�W�F�N�g�̕`����@�̗񋓌^
	// Author : �������l
	// �T�v : �I�u�W�F�N�g�̕`����@�����i�[
	//--------------------------------------------------------------------
	enum EObjectDrowType
	{
		DROWTYPE_GAME = 0,			// �Q�[���`��
		DROWTYPE_BG,				// �w�i�`��
		MAX_DROWTYPE,				// ��ʂ̍ő吔
	};

	//--------------------------------------------------------------------
	// �ÓI�����o�֐�
	//--------------------------------------------------------------------
	static void ReleaseAll(void);									// ���ׂẴI�u�W�F�N�g�̉��
	static void UpdateAll(void);									// ���ׂẴI�u�W�F�N�g�̍X�V
	static void DrawAll(EObjectDrowType objectDrowType);			// ���ׂẴI�u�W�F�N�g�̕`��
	static CObject **GetObjectAll() { return &m_apObject[0]; }		// �I�u�W�F�N�g���̃Q�b�^�[

	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CObject();
	virtual ~CObject();

	//--------------------------------------------------------------------
	// �������z�֐�
	//--------------------------------------------------------------------
	virtual HRESULT Init() = 0;																			// ������
	virtual void Uninit() = 0;																			// �I��
	virtual void Update() = 0;																			// �X�V
	virtual void Draw() = 0;																			// �`��
	virtual void SetPos(const D3DXVECTOR3 &pos) = 0;													// �ʒu�̃Z�b�^�[
	virtual void SetPosOld(const D3DXVECTOR3 &posOld) = 0;												// �ߋ��ʒu�̃Z�b�^
	virtual void SetRot(const D3DXVECTOR3 &rot) = 0;													// �����̃Z�b�^�[
	virtual void SetSize(const D3DXVECTOR3 &size) = 0;													// �傫���̃Z�b�^�[
	virtual D3DXVECTOR3 GetPos() = 0;																	// �ʒu�̃Q�b�^�[
	virtual D3DXVECTOR3 GetPosOld() = 0;																// �ߋ��ʒu�̃Q�b�^
	virtual D3DXVECTOR3 GetRot() = 0;																	// �����̃Q�b�^�[
	virtual D3DXVECTOR3 GetSize() = 0;																	// �傫���̃Q�b�^�[
	void SetObjType(EObjectType objectType) { m_objectType = objectType; }								// �I�u�W�F�N�g�̎�ʐݒ�
	EObjectType GetObjType() { return m_objectType; }													// �I�u�W�F�N�g�̎�ʐݒ�
	void SetObjectDrowType(EObjectDrowType objectDrowType) { m_objectDrowType = objectDrowType; }		// �I�u�W�F�N�g�̕`����@�̐ݒ�
	EObjectDrowType GetObjectDrowType() { return m_objectDrowType; }									// �I�u�W�F�N�g�̕`����@�̐ݒ�
	void SetColorType(COLOR_TYPE colorType) { m_colorType = colorType; }								// �F�^�C�v�̐ݒ�
	COLOR_TYPE GetColorType() { return m_colorType; }													// �F�^�C�v�̐ݒ�

protected:
	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	void Release(void);											// �I�u�W�F�N�g�̉��
	bool ColisonRange2D(CObject *target);						// �͈͂̔���(2D)
	bool ColisonRectangle2D(CObject *target,bool bExtrude);		// ��`�̔���(2D)
	bool ColisonCircle2D(CObject *target, bool bExtrude);		// �~�̔���(2D)

private:
	//--------------------------------------------------------------------
	// �ÓI�����o�ϐ�
	//--------------------------------------------------------------------
	static CObject *m_apObject[MAX_OBJECT];				// �I�u�W�F�N�g���

	//--------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------
	EObjectType			m_objectType;		// �I�u�W�F�N�g�̎��
	EObjectDrowType		m_objectDrowType;	// �I�u�W�F�N�g�̕`����@
	COLOR_TYPE			m_colorType;		// �F�̎��
	int					m_nNumID;			// �i�[��̔ԍ�
};

#endif

