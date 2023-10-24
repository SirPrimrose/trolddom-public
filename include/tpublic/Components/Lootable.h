#pragma once

#include "../Component.h"
#include "../ComponentBase.h"
#include "../ItemInstance.h"
#include "../PlayerTag.h"

namespace tpublic
{

	namespace Components
	{

		struct Lootable
			: public ComponentBase
		{
			static const Component::Id ID = Component::ID_LOOTABLE;
			static const uint8_t FLAGS = 0;
			static const Persistence::Id PERSISTENCE = Persistence::ID_NONE;
			static const Replication REPLICATION = REPLICATION_PUBLIC;

			struct AvailableLoot
			{
				void
				ToStream(
					IWriter*			aWriter) const
				{
					m_playerTag.ToStream(aWriter);
					m_itemInstance.ToStream(aWriter);
				}

				bool
				FromStream(
					IReader*			aReader) 
				{
					if(!m_playerTag.FromStream(aReader))
						return false;
					if (!m_itemInstance.FromStream(aReader))
						return false;
					return true;
				}

				// Public data
				PlayerTag				m_playerTag;
				ItemInstance			m_itemInstance;
			};

			enum Field
			{
				FIELD_LOOT_TABLE_ID,
				FIELD_PLAYER_TAG,
				FIELD_CASH,
				FIELD_AVAILABLE_CASH,
				FIELD_AVAILABLE_LOOT,
				FIELD_VERSION
			};

			static void
			CreateSchema(
				ComponentSchema* aSchema)
			{
				aSchema->Define(ComponentSchema::TYPE_UINT32, FIELD_LOOT_TABLE_ID, "loot_table", offsetof(Lootable, m_lootTableId))->SetDataType(DataType::ID_LOOT_TABLE);
				aSchema->DefineCustomObjectNoSource<PlayerTag>(FIELD_PLAYER_TAG, offsetof(Lootable, m_playerTag));
				aSchema->Define(ComponentSchema::TYPE_BOOL, FIELD_CASH, NULL, offsetof(Lootable, m_cash));
				aSchema->Define(ComponentSchema::TYPE_INT64, FIELD_AVAILABLE_CASH, NULL, offsetof(Lootable, m_availableCash));
				aSchema->DefineCustomObjectsNoSource<AvailableLoot>(FIELD_AVAILABLE_LOOT, offsetof(Lootable, m_availableLoot));
				aSchema->Define(ComponentSchema::TYPE_UINT32, FIELD_VERSION, NULL, offsetof(Lootable, m_version));
			}

			bool
			GetAvailableLootByIndex(
				size_t					aIndex,
				AvailableLoot&			aOut) const
			{
				if(aIndex >= m_availableLoot.size())
					return false;
				aOut = m_availableLoot[aIndex];
				return true;
			}

			// Public data
			uint32_t					m_lootTableId = 0;
			PlayerTag					m_playerTag;
			bool						m_cash = false;
			int64_t						m_availableCash = 0;
			std::vector<AvailableLoot>	m_availableLoot;
			uint32_t					m_version = 0;
		};
	}

}