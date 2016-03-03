#ifndef _PROGRESS_VIEW_H_
#define _PROGRESS_VIEW_H_

#include "cocos2d.h"
USING_NS_CC;

/************************************************************************
*   进度条                                                                  
************************************************************************/
class ProgressView:public cocos2d::Node
{
public:
	ProgressView();

	CREATE_FUNC(ProgressView);

	void							InitProgress(const char* background,const char* foreground,float totalProgress,float currentProgress);	// 初始化进度条
	float							GetTotalProgress();																																	// 获取总的进度
	float							GetCurrentProgress();																																// 获取当前进度
	void							SetCurrentProgress(float f);																														// 设置当前进度
	Size							GetSize();																																					// 获取进度条的尺寸
private:
	Sprite*						m_pProgressBackground;																															// 进度条背景
	Sprite*						m_pProgressForeground;																															// 进度条前景
	float							m_fTotalProgress;
	float							m_fCurrentProgress;
	float							m_fScale;																																					// 放大倍数
};
#endif