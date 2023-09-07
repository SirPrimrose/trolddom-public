#pragma once

#include "IReader.h"
#include "ItemType.h"
#include "IWriter.h"
#include "Parser.h"

namespace tpublic
{

	class ItemMetrics
	{
	public:
		struct Multipliers
		{
			void
			FromSource(
				const Parser::Node* aSource)
			{
				aSource->ForEachChild([&](
					const Parser::Node* aChild)
				{
					if (aChild->m_name == "armor")
						m_armor = aChild->GetFloat();
					else if (aChild->m_name == "cost")
						m_cost = aChild->GetFloat();
					else
						TP_VERIFY(false, aChild->m_debugInfo, "'%s' is not a valid item.", aChild->m_name.c_str());
				});
			}

			void
			ToStream(
				IWriter*		aWriter) const
			{
				aWriter->WriteFloat(m_armor);
				aWriter->WriteFloat(m_cost);
			}

			bool
			FromStream(
				IReader*		aReader) 
			{
				if (!aReader->ReadFloat(m_armor))
					return false;
				if (!aReader->ReadFloat(m_cost))
					return false;
				return true;
			}

			// Public data
			float	m_armor = 1.0f;
			float	m_cost = 1.0f;
		};

		ItemMetrics()
		{

		}

		void
		FromSource(
			const Parser::Node*	aSource)
		{
			aSource->ForEachChild([&](
				const Parser::Node* aChild)
			{
				if(aChild->m_name == "base_armor")
				{
					aChild->GetArray()->ForEachChild([&](
						const Parser::Node* aEntry)
					{
						TP_VERIFY(aEntry->m_type == Parser::Node::TYPE_ARRAY && aEntry->m_children.size() == 2, aEntry->m_debugInfo, "Not a level-armor pair.");
						uint32_t level = aEntry->m_children[0]->GetUInt32();
						uint32_t armor = aEntry->m_children[1]->GetUInt32();

						if(level >= m_levelBaseArmor.size())
							m_levelBaseArmor.resize(level + 1);

						m_levelBaseArmor[level] = armor;
					});
				}
				else if(aChild->m_name == "base_cost")
				{
					aChild->GetArray()->ForEachChild([&](
						const Parser::Node* aEntry)
					{
						TP_VERIFY(aEntry->m_type == Parser::Node::TYPE_ARRAY && aEntry->m_children.size() == 2, aEntry->m_debugInfo, "Not a level-cost pair.");
						uint32_t level = aEntry->m_children[0]->GetUInt32();
						uint32_t cost = aEntry->m_children[1]->GetUInt32();

						if(level >= m_levelBaseCost.size())
							m_levelBaseCost.resize(level + 1);

						m_levelBaseCost[level] = cost;
					});
				}
				else if(aChild->m_tag == "item_type_multipliers")
				{
					ItemType::Id itemType = ItemType::StringToId(aChild->m_name.c_str());
					TP_VERIFY(itemType != ItemType::INVALID_ID, aChild->m_debugInfo, "'%s' is not a valid item type.", aChild->m_name.c_str());
					m_itemTypeMultipliers[itemType].FromSource(aChild);
				}
				else if (aChild->m_tag == "equipment_slot_multipliers")
				{
					EquipmentSlot::Id equipmentSlot = EquipmentSlot::StringToId(aChild->m_name.c_str());
					TP_VERIFY(equipmentSlot != EquipmentSlot::INVALID_ID, aChild->m_debugInfo, "'%s' is not a valid equipment slot.", aChild->m_name.c_str());
					m_equipmentSlotMultipliers[equipmentSlot].FromSource(aChild);
				}
				else
				{
					TP_VERIFY(false, aChild->m_debugInfo, "'%s' is not a valid item.", aChild->m_name.c_str());
				}
			});
		}

		void
		ToStream(
			IWriter*			aStream) const 
		{
			aStream->WriteUInts(m_levelBaseArmor);
			aStream->WriteUInts(m_levelBaseCost);

			for(uint32_t i = 1; i < (uint32_t)ItemType::NUM_IDS; i++)
				m_itemTypeMultipliers[i].ToStream(aStream);

			for (uint32_t i = 1; i < (uint32_t)EquipmentSlot::NUM_IDS; i++)
				m_equipmentSlotMultipliers[i].ToStream(aStream);
		}

		bool
		FromStream(
			IReader*			aStream) 
		{
			if(!aStream->ReadUInts(m_levelBaseArmor))
				return false;
			if (!aStream->ReadUInts(m_levelBaseCost))
				return false;

			for (uint32_t i = 1; i < (uint32_t)ItemType::NUM_IDS; i++)
			{
				if (!m_itemTypeMultipliers[i].FromStream(aStream))
					return false;
			}

			for (uint32_t i = 1; i < (uint32_t)EquipmentSlot::NUM_IDS; i++)
			{
				if (!m_equipmentSlotMultipliers[i].FromStream(aStream))
					return false;
			}

			return true;
		}
		
		uint32_t
		GetLevelBaseArmor(
			uint32_t			aLevel) const
		{
			if(aLevel >= m_levelBaseArmor.size())
				return 1;
			return m_levelBaseArmor[aLevel];
		}

		uint32_t
		GetLevelBaseCost(
			uint32_t			aLevel) const
		{
			if (aLevel >= m_levelBaseCost.size())
				return 1;
			return m_levelBaseCost[aLevel];
		}
		
		// Public data
		std::vector<uint32_t>					m_levelBaseArmor;
		std::vector<uint32_t>					m_levelBaseCost;
		Multipliers								m_itemTypeMultipliers[ItemType::NUM_IDS];
		Multipliers								m_equipmentSlotMultipliers[EquipmentSlot::NUM_IDS];
	};

}
