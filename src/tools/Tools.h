/******************************************************************************
	Tools: tiny but useful tools
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


#ifndef PHTOKIT_TOOLS_H
#define PHTOKIT_TOOLS_H

class QString;
class QStringList;
class QImage;
namespace PhotoKit {

namespace Tools {

//TODO: remove force param
void showTip(const QString& text, bool force = false, int msshow = 8000);
void showTip(const QImage &image, bool force = false, int msshow = 8000);
void showOk(int msshow = 8000);
void showError(int msshow = 8000);

QStringList imageNameFilters();

} //namespace Tools

} //namespace PhotoKit


#endif // PHTOKIT_TOOLS_H
