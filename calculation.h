//**************************************************************************************************
//
// �v�Z����(calculation.h)
// Auther�F�������l
//
//**************************************************************************************************
#ifndef _CALCULATION_H_			// ���̃}�N����`������ĂȂ�������
#define _CALCULATION_H_			// ��d�C���N���[�h�h�~�̃}�N����`

//***************************************************************
// �v���g�^�C�v�錾
//***************************************************************
// ����֐�
D3DXVECTOR3	WorldCastVtx(D3DXVECTOR3 vtx, D3DXVECTOR3 FormerPos, D3DXVECTOR3 FormerRot);				// ���[���h���W�ɃL���X�g
D3DXVECTOR3	WorldCastVtx(D3DXVECTOR3 vtx, D3DXVECTOR3 FormerPos, D3DXQUATERNION FormeQuaternion);		// ���[���h���W�ɃL���X�g
float RotNormalization(float fRot);																		// �p�x�̐��K��

#endif
