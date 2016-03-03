#ifndef _PROGRESS_VIEW_H_
#define _PROGRESS_VIEW_H_

#include "cocos2d.h"
USING_NS_CC;

/************************************************************************
*   ������                                                                  
************************************************************************/
class ProgressView:public cocos2d::Node
{
public:
	ProgressView();

	CREATE_FUNC(ProgressView);

	void							InitProgress(const char* background,const char* foreground,float totalProgress,float currentProgress);	// ��ʼ��������
	float							GetTotalProgress();																																	// ��ȡ�ܵĽ���
	float							GetCurrentProgress();																																// ��ȡ��ǰ����
	void							SetCurrentProgress(float f);																														// ���õ�ǰ����
	Size							GetSize();																																					// ��ȡ�������ĳߴ�
private:
	Sprite*						m_pProgressBackground;																															// ����������
	Sprite*						m_pProgressForeground;																															// ������ǰ��
	float							m_fTotalProgress;
	float							m_fCurrentProgress;
	float							m_fScale;																																					// �Ŵ���
};
#endif