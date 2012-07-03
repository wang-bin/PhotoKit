/******************************************************************************
    Guide.h: description
    Copyright (C) 2012 Wang Bin <wbsecg1@gmail.com>
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
******************************************************************************/


#ifndef GUIDE_H
#define GUIDE_H

#include <QtCore/QRectF>

namespace PhotoKit {
class BaseAnimationItem;
class Guide
{
public:
    Guide(Guide *follows = 0);
    virtual ~Guide();

    virtual void guide(BaseAnimationItem *item, float moveSpeed) = 0;
    void move(BaseAnimationItem *item, QPointF &dest, float moveSpeed);
    virtual QPointF startPos(){ return QPointF(0, 0); }
    virtual QPointF endPos(){ return QPointF(0, 0); }
    virtual float length(){ return 1; }
    float lengthAll();

    void setScale(float scaleX, float scaleY, bool all = true);
    void setFence(const QRectF &fence, bool all = true);

    int startLength;
    Guide *nextGuide;
    Guide *firstGuide;
    Guide *prevGuide;
    float scaleX;
    float scaleY;
    QRectF fence;
};

} //namespace PhotoKit
#endif // GUIDE_H
