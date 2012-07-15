/******************************************************************************
	OptionParser: commandline parser based on my ProgramOptions
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


#ifndef PHOTOKIT_CMDPARSER_H
#define PHOTOKIT_CMDPARSER_H

class QStringList;
namespace PhotoKit {

class OptionParser {
public:
    static void parseCmd(int argc, const char* const* argv);
    static QStringList images;
private:
    OptionParser();
    static void getImagesFromDirs(const QStringList& dirs);
};



}
#endif // PHOTOKIT_CMDPARSER_H
