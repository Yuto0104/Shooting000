//=============================================================================
//
// ���C�t(life.h)
// Author : �������l
// �T�v : ���C�t�������s��
//
//=============================================================================
#ifndef _LIFE_H_		// ���̃}�N����`������ĂȂ�������
#define _LIFE_H_		// ��d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object2D.h"

//=============================================================================
// ���C�t�N���X
// Author : �������l
// �T�v : ���C�t�������s���N���X
//=============================================================================
class CLife : public CObject2D
{
public:
	//--------------------------------------------------------------------
	// ���C�t�̏�Ԃ̗񋓌^
	// Author : �������l
	// �T�v : ���C�t�̏�Ԃ̊Ǘ����s��
	//--------------------------------------------------------------------
	enum LIFE_STATE
	{
		STATE_NORMAL = 0,		// �ʏ�
		STATE_INVALID,			// ����
		STATE_ADDITION,			// �ǉ�
		STATE_MAX				// �ő吔
	};

	//--------------------------------------------------------------------
	// �ÓI�����o�֐�
	//--------------------------------------------------------------------
	// ���C�t�̐���
	static CLife *Create();

	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CLife(int nPriority = CObject::PRIORITY_LEVEL3);
	~CLife() override;

	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	HRESULT Init() override;								// ������
	void Uninit() override;									// �I��
	void Update() override;									// �X�V
	void Draw() override;									// �`��
	void SetLifeState(LIFE_STATE lifeState);				// ���C�t�̏�Ԃ̃Z�b�^�[
	LIFE_STATE GetLifeState() { return m_lifeState; }		// ���C�t�̏�Ԃ̎擾

private:
	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	LIFE_STATE m_lifeState;			// ���C�t�̏��
};

#endif
