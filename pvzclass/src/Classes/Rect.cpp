#include "../../PVZ.h"

using std::max;
using std::min;

int PVZ::GetXOverlap(const PVZ::Rect& rect1, const PVZ::Rect& rect2)
{
	int LeftR, RightR, RightL;
	if (rect1.X >= rect2.X)
	{
		LeftR = rect2.Width + rect2.X;
		RightL = rect1.X;
		RightR = rect1.Width + rect1.X;
	}
	else
	{
		LeftR = rect1.Width + rect1.X;
		RightL = rect2.X;
		RightR = rect2.Width + rect2.X;
	}
	if (LeftR > RightL && LeftR > RightR)
		LeftR = RightR;
	return(LeftR - RightL);
}

bool PVZ::Rect::IsCircleOverlap(const int X, const int Y, const int radius)
{
	bool Xout = false;
	int Xdist = 0;

	if (X < this->X)
		Xout = true, Xdist = this->X - X;
	else if (X > this->X + this->Width)
		Xout = true, Xdist = X - this->X - this->Width;

	if (Y >= this->Y && Y <= this->Y + this->Height)
		return(!Xout || Xdist <= radius);
	else
	{
		int Ydist = this->Y - Y;
		if (Ydist < 0)
			Ydist = Y - this->Y - this->Height;

		return(Xdist * Xdist + Ydist * Ydist <= radius * radius);
	}
}

PVZ::Rect PVZ::Rect::Intersection(const Rect& rect) const
{
	int x1 = max(X, rect.X);
	int x2 = min(X + Width, rect.X + rect.Width);
	int y1 = max(Y, rect.Y);
	int y2 = min(Y + rect.Height, rect.Y + rect.Height);

	if (((x2 - x1) < 0) || ((y2 - y1) < 0))
		return Rect(0, 0, 0, 0);
	else
		return Rect(x1, y1, x2 - x1, y2 - y1);
}
