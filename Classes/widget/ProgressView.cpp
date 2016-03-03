#include "ProgressView.h"

ProgressView::ProgressView()
{
	m_pProgressBackground = 0;
	m_pProgressForeground = 0;
	m_fTotalProgress = 0;
	m_fCurrentProgress = 0;
	m_fScale = 1.0;
}

void ProgressView::InitProgress(const char* background,const char* foreground,float totalProgress,float currentProgress)
{
	m_pProgressBackground = Sprite::create(background);

	m_pProgressForeground = Sprite::create(foreground);
	m_pProgressForeground->setAnchorPoint(Point(0,0.5));
	m_pProgressForeground->setPosition(Point(-m_pProgressForeground->getContentSize().width * 0.5f, 0));

	

	m_fTotalProgress = totalProgress;
	m_fCurrentProgress = currentProgress;

	m_fScale = m_pProgressForeground->getContentSize().width / totalProgress;


	this->addChild(m_pProgressBackground);
	this->addChild(m_pProgressForeground);

}

float ProgressView::GetCurrentProgress()
{
	return m_fCurrentProgress;
}

float ProgressView::GetTotalProgress()
{
	return m_fTotalProgress;
}

void ProgressView::SetCurrentProgress(float f)
{
	if (f < 0.0)
	{
		m_fCurrentProgress = 0;
	}

	if (f > m_fTotalProgress)
	{
		m_fCurrentProgress = m_fTotalProgress;
	}

	m_fCurrentProgress = f;

	float width = f * m_fScale;

	Point from = m_pProgressForeground->getTextureRect().origin;
	Rect rect = CCRectMake(from.x,from.y,width,m_pProgressForeground->getContentSize().height);
	m_pProgressForeground->setTextureRect(rect);
}

Size ProgressView::GetSize()
{
	return m_pProgressBackground->getContentSize();
}