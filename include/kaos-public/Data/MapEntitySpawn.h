#pragma once

#include "../DataBase.h"

namespace kaos_public
{

	namespace Data
	{

		struct MapEntitySpawn
			: public DataBase
		{
			static const DataType::Id DATA_TYPE = DataType::ID_MAP_ENTITY_SPAWN;

			struct Entity
			{
				void
				FromSource(
					const Parser::Node*	aNode)
				{
					m_entityId = aNode->m_sourceContext->m_persistentIdTable->GetId(DataType::ID_ENTITY, aNode->m_name.c_str());

					aNode->GetObject()->ForEachChild([&](
						const Parser::Node* aChild)
					{
						if(aChild->m_name == "weight")
							m_weight = aChild->GetUInt32();
						else 
							KP_VERIFY(false, aChild->m_debugInfo, "Invalid 'entity' item.");
					});
				}

				void
				ToStream(
					IWriter*		aStream) const
				{
					aStream->WriteUInt(m_entityId);
					aStream->WriteUInt(m_weight);
				}

				bool
				FromStream(
					IReader*		aStream) 
				{
					if (!aStream->ReadUInt(m_entityId))
						return false;
					if (!aStream->ReadUInt(m_weight))
						return false;
					return true;
				}

				// Public data
				uint32_t		m_entityId;
				uint32_t		m_weight;
			};

			void
			Verify() const
			{
				VerifyBase();
			}

			// Base implementation
			void
			FromSource(
				const Parser::Node*		aNode) override
			{
				aNode->GetObject()->ForEachChild([&](
					const Parser::Node* aChild)
				{
					if(aChild->m_tag == "entity")
					{
						Entity entity;
						entity.FromSource(aChild);
						m_entities.push_back(entity);
					}
					else
					{
						KP_VERIFY(false, aChild->m_debugInfo, "Invalid 'map_entity_spawn' item.");
					}
				});
			}

			void
			ToStream(
				IWriter*				aStream) const override
			{
				ToStreamBase(aStream);
				aStream->WriteObjects(m_entities);
			}

			bool
			FromStream(
				IReader*				aStream) override
			{
				if (!FromStreamBase(aStream))
					return false;
				if (!aStream->ReadObjects(m_entities))
					return false;
				return true;
			}

			// Public data
			std::vector<Entity>		m_entities;
		};

	}

}