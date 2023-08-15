#pragma once

#include "Modifier.h"
#include "Stat.h"

namespace tpublic
{

	class StatModifiers
	{
	public:
		StatModifiers()
		{

		}

		StatModifiers(
			const Parser::Node*		aSource)
		{
			aSource->ForEachChild([&](
				const Parser::Node*	aChild)
			{
				Stat::Id statId = Stat::StringToId(aChild->m_name.c_str());
				TP_VERIFY(statId != Stat::INVALID_ID, aChild->m_debugInfo, "'%s' is not a valid stat.", aChild->m_name.c_str());
				m_modifiers[statId] = Modifier(aChild);
			});
		}

		void
		ToStream(
			IWriter*				aWriter) const
		{
			uint32_t bits = 0;
			for(uint32_t i = 1; i < (uint32_t)Stat::NUM_IDS; i++)
			{
				if(m_modifiers[i].has_value())
					bits |= 1 << i;
			}

			aWriter->WriteUInt(bits);

			for (uint32_t i = 1; i < (uint32_t)Stat::NUM_IDS; i++)
			{
				if (m_modifiers[i].has_value())
					m_modifiers[i].value().ToStream(aWriter);
			}
		}

		bool
		FromStream(
			IReader*				aReader)
		{
			uint32_t bits;
			if(!aReader->ReadUInt(bits))
				return false;

			for (uint32_t i = 1; i < (uint32_t)Stat::NUM_IDS; i++)
			{
				if((bits & (1 << i)) != 0)
				{
					Modifier modifier;
					if(!modifier.FromStream(aReader))
						return false;

					m_modifiers[i] = modifier;
				}
			}

			return true;
		}

		void
		Apply(
			Stat::Collection&		aStats)
		{
			for (uint32_t i = 1; i < (uint32_t)Stat::NUM_IDS; i++)
			{
				if (m_modifiers[i].has_value())
					aStats.m_stats[i] = m_modifiers[i].value().Calculate(aStats.m_stats[i]);
			}
		}

		void
		Combine(
			const StatModifiers&	aOther)
		{
			for (uint32_t i = 1; i < (uint32_t)Stat::NUM_IDS; i++)
			{
				std::optional<Modifier>& thisModifier = m_modifiers[i];
				const std::optional<Modifier>& otherModifier = aOther.m_modifiers[i];

				if(!thisModifier.has_value() && otherModifier.has_value())
					thisModifier = otherModifier.value();
				else if(thisModifier.has_value() && otherModifier.has_value())
					thisModifier.value().Combine(otherModifier.value());
			}
		}

		// Public data
		std::optional<Modifier>		m_modifiers[Stat::NUM_IDS];
	};

}