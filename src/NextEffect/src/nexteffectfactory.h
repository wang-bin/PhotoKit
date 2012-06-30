/******************************************************************************
	nexteffectfactory.h: description
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


#ifndef NEXTEFFECTFACTORY_H
#define NEXTEFFECTFACTORY_H

#include <map>
#include <vector>
#include "nexteffect.h"

typedef NextEffect* (*EffectCreator)();

#define REGISTER_EFFECT(ID, MAINTYPE) \
	NextEffect* createEffect##ID() { \
		NextEffect *e = new NextEffect_##MAINTYPE(); \
		e->setType(ID); \
		return e; \
	} \
	static bool ok_##ID = NextEffectFactory::Instance().registerEffect(ID, createEffect##ID) \
			&& NextEffectFactory::Instance().registerEffectIdName(ID, #ID);


class Q_EXPORT NextEffectFactory
{
	Q_DISABLE_COPY(NextEffectFactory)
public:
	static NextEffectFactory& Instance();

	static void init(); //don't know why not automatically register. so manual call this is safe;
	NextEffect* createEffect(EffectId effect_id);
	bool registerEffect(EffectId effect_id, EffectCreator callback);
	bool registerEffectIdName(EffectId effect_id, const QString& name);
	bool unregisterEffect(EffectId effect_id);

	EffectId effectId(const QString& name) const;
	QString effectName(const EffectId &id) const;

	NextEffect* getRandomEffect();
private:
	NextEffectFactory();
	static void MakeInstance();
	static void  DestroySingleton();

private:
	static NextEffectFactory* pInstance_;
	static bool destroyed_;

	static EffectId current_effect_id;
	static NextEffect *current_effect;
	typedef std::map<EffectId, EffectCreator> EffectCreatorMap;
	EffectCreatorMap creators;
	//std::vector<int> effect_ids; //init error, why?
	std::vector<EffectId> effect_ids;
	typedef std::map<EffectId, QString> EffectMap;
	EffectMap effect_map;
};

#endif // NEXTEFFECTFACTORY_H
