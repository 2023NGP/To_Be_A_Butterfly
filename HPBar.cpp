#include "stdafx.h"
#include "HPBar.h"

void HPBar::Draw(HDC hdc)
{
    HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(150, 50, 0));
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
    Rectangle(hdc, 50, y + 1, barGauge + 50, y + 29);
    SelectObject(hdc, oldBrush);
    DeleteObject(myBrush);

    myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
    HPEN hPen = CreatePen(PS_DOT, 2, RGB(0, 0, 0));
    HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
    Rectangle(hdc, 50, y, HPBAR_WIDTH + 50, y + 30);
    SelectObject(hdc, oldBrush);
    DeleteObject(myBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hPen);
}
