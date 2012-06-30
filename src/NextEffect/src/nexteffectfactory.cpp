/******************************************************************************
	nexteffectfactory.cpp: description
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


#include <time.h>
#include <stdexcept>
#include <assert.h>
#include "nexteffectfactory.h"
#include "nexteffect_blinds.h"
#include "nexteffect_center.h"
#include "nexteffect_cover.h"
#include "nexteffect_enter.h"
#include "nexteffect_fade.h"
#include "nexteffect_leave.h"
#include "nexteffect_move.h"
#include "nexteffect_none.h"
#include "nexteffect_sector.h"
#include "nexteffect_tile.h"

NextEffectFactory* NextEffectFactory::pInstance_ = 0;
bool NextEffectFactory::destroyed_ = false;
NextEffect* NextEffectFactory::current_effect = 0;
EffectId NextEffectFactory::current_effect_id = 0;

#define REGISTER_EFFECT_MAN(ID) \
	extern NextEffect* createEffect##ID(); \
	NextEffectFactory::Instance().registerEffect(ID, createEffect##ID) \
	&& NextEffectFactory::Instance().registerEffectIdName(ID, #ID);

void NextEffectFactory::init()
{

	REGISTER_EFFECT_MAN(BlindsBottomToTop)
	REGISTER_EFFECT_MAN(BlindsLeftToRight)
	REGISTER_EFFECT_MAN(BlindsRightToLeft)
	REGISTER_EFFECT_MAN(BlindsTopToBottom)
	REGISTER_EFFECT_MAN(EllipseOpen)
	REGISTER_EFFECT_MAN(EllipseClose)

	REGISTER_EFFECT_MAN(CoverFromLeft)
	REGISTER_EFFECT_MAN(CoverFromRight)
	REGISTER_EFFECT_MAN(CoverFromBottom)
	REGISTER_EFFECT_MAN(CoverFromBottomLeft)
	REGISTER_EFFECT_MAN(CoverFromBottomRight)
	REGISTER_EFFECT_MAN(CoverFromTop)
	REGISTER_EFFECT_MAN(CoverFromTopLeft)
	REGISTER_EFFECT_MAN(CoverFromTopRight)

	REGISTER_EFFECT_MAN(EnterFromBottom)
	REGISTER_EFFECT_MAN(EnterFromBottomLeft)
	REGISTER_EFFECT_MAN(EnterFromBottomRight)
	REGISTER_EFFECT_MAN(EnterFromLeft)
	REGISTER_EFFECT_MAN(EnterFromRight)
	REGISTER_EFFECT_MAN(EnterFromTop)
	REGISTER_EFFECT_MAN(EnterFromTopLeft)
	REGISTER_EFFECT_MAN(EnterFromTopRight)

	REGISTER_EFFECT_MAN(Fade)

	REGISTER_EFFECT_MAN(LeaveToBottom)
	REGISTER_EFFECT_MAN(LeaveToBottomLeft)
	REGISTER_EFFECT_MAN(LeaveToBottomRight)
	REGISTER_EFFECT_MAN(LeaveToLeft)
	REGISTER_EFFECT_MAN(LeaveToRight)
	REGISTER_EFFECT_MAN(LeaveToTop)
	REGISTER_EFFECT_MAN(LeaveToTopLeft)
	REGISTER_EFFECT_MAN(LeaveToTopRight)

	REGISTER_EFFECT_MAN(MoveFromBottom)
	REGISTER_EFFECT_MAN(MoveFromLeft)
	REGISTER_EFFECT_MAN(MoveFromRight)
	REGISTER_EFFECT_MAN(MoveFromTop)
	REGISTER_EFFECT_MAN(None)
	REGISTER_EFFECT_MAN(SectorClockwise)
	REGISTER_EFFECT_MAN(SectorAntiColckwise)
	REGISTER_EFFECT_MAN(Tile)

}

NextEffectFactory& NextEffectFactory::Instance()
{
	if (!pInstance_) {
		MakeInstance();
	}
	return *pInstance_;
}

NextEffect* NextEffectFactory::createEffect(EffectId effect_id)
{
	EffectCreatorMap::const_iterator it = creators.find(effect_id);
	if (it == creators.end()) {
		QString err_msg("Unknown Effect type id %1");
		throw std::runtime_error(err_msg.arg(effect_id).toStdString());
	}

	if (current_effect_id == it->first) {
		if (!current_effect)
			current_effect = (it->second)();
		return current_effect;
	}

	current_effect_id = it->first;
	NextEffect *ptr = current_effect;
	qDebug("old addr=%p", current_effect);
	current_effect = (it->second)();
	qDebug("created");
	if (ptr) {
		qDebug("new=%p, deleting old %p", current_effect, ptr);
		delete ptr;
		qDebug("deleted");
		ptr = 0;
	}

	return current_effect;
}

bool NextEffectFactory::registerEffect(EffectId effect_id, EffectCreator callback)
{
	qDebug("Effect %d registered", effect_id);
	effect_ids.insert(effect_ids.end(), effect_id);
	return creators.insert(EffectCreatorMap::value_type(effect_id, callback)).second;
}

bool NextEffectFactory::registerEffectIdName(EffectId effect_id, const QString& name)
{
	qDebug("Effect %s registered", qPrintable(name));
	return effect_map.insert(EffectMap::value_type(effect_id, name.toLower())).second;
}

bool NextEffectFactory::unregisterEffect(EffectId effect_id)
{
	qDebug("Effect %d unregistered", effect_id);
	effect_ids.erase(std::remove(effect_ids.begin(), effect_ids.end(), effect_id), effect_ids.end());
	effect_map.erase(effect_id);
	return creators.erase(effect_id) == 1;
}

EffectId NextEffectFactory::effectId(const QString &name) const
{
	for (EffectMap::const_iterator it = effect_map.begin(); it!=effect_map.end(); ++it) {
		if (it->second == name)
			return it->first;
	}
	return Invalid;
}

QString NextEffectFactory::effectName(const EffectId &id) const
{
	return effect_map.find(id)->second;
}

NextEffect* NextEffectFactory::getRandomEffect()
{
	srand(time(0));
	int index = rand() % effect_ids.size();
	qDebug("random %d/%d", index, effect_ids.size());
	int new_eid = effect_ids.at(index);
	qDebug("id %d", new_eid);
	return createEffect(new_eid);
}

NextEffectFactory::NextEffectFactory()
{

}

void NextEffectFactory::MakeInstance()
{
	if (!pInstance_) {
		if (destroyed_) {
			destroyed_ = false;
			throw std::logic_error("Dead Reference Detected");
		}
		pInstance_ = new NextEffectFactory();
		std::atexit(&DestroySingleton);
	}
}

void NextEffectFactory::DestroySingleton()
{
	assert(!destroyed_);
	delete pInstance_;
	pInstance_ = 0;
	destroyed_ = true;
}
